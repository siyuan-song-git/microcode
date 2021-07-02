#include "uart_c.h"
extern void request_irq(unsigned int irq);
static struct uart_dat ud;



unsigned int readl(unsigned long addr)
{
	unsigned int val;
	val = *(volatile unsigned int *)addr;
	return val;
}

unsigned long read1(unsigned long addr)
{
	unsigned long val;
	val = *(volatile unsigned long *)addr;
	return val;
}

void writel(unsigned long addr, unsigned int val)
{
	*(volatile unsigned int *)addr = val;
}

void writeq(unsigned long addr, unsigned long val)
{
	*(volatile unsigned long *)addr = val;
}

void ckg_enable(struct uart_dat *up)
{
	unsigned int val;

	val = readl(up->apb_sys_base);
	val |= up->ckg_en_bit;
	writel(up->apb_sys_base, val);

	val = readl(up->apb_sys_base + 0x0004);
	val |= up->ckg_apb_en_bit;
	writel(up->apb_sys_base + 0x0004, val);
}

void uart_init(struct uart_dat *up)
{
	up->uart_base = UART_BASE(TEST_PORT);
	up->apb_sys_base = APB_BASE;
	up->ckg_en_bit = UART_ENABLE(TEST_PORT);
	up->ckg_apb_en_bit = UART_APB_ENABLE(TEST_PORT);
	up->clk_source = CLK_HZ;

	/* config pin mux */
	*(volatile unsigned int *)0x28200020 = 0x0;
	*(volatile unsigned int *)0x28200024 = 0x0;
}

void set_baudrate(struct uart_dat *up, unsigned int baud)
{
	unsigned int div,val;

	val = readl(up->uart_base + UART_LCR);
	val |= UART_LCR_DLAB;
	writel(up->uart_base + UART_LCR, val);

	div = (up->clk_source / baud + 8) / 16;
	writel(up->uart_base + UART_DLL, div & 0xff);
	writel(up->uart_base + UART_DLH, (div & 0xff00) >> 8);

	val &= ~UART_LCR_DLAB;
	writel(up->uart_base + UART_LCR, val);
}

/*
 *Stick		even parity	parity enable	parity
 *-		-		0		None
 *0		0		1		Odd
 *0		1		1		Even
 *1		0		1		Mark
 *1		1		1		Space
 *
 *data bit:	5/6/7/8
 *sotp bit:	1:1bit,2:1.5bit(only 5 bit data) or 2bit(only 6/7/8 bit data)
 *parity:	NONE/ODD/EVEN/MARK/SPACE
 */
void set_line_ctrl(struct uart_dat *up, unsigned int data,\
		 unsigned int stop, enum PARITY_TYPE par)
{
	unsigned int val, tmp=0;

	switch(data) {
		case 5:
			tmp |= UART_LCR_WLEN5;
			break;
		case 6:
			tmp |= UART_LCR_WLEN6;
			break;
		case 7:
			tmp |= UART_LCR_WLEN7;
			break;
		case 8:
			tmp |= UART_LCR_WLEN8;
			break;
	}

	if(stop == 2)
		tmp |= UART_LCR_STOP;

	switch(par) {
		case NONE:
			break;
		case ODD:
			tmp |= UART_LCR_PARITY;
			break;
		case EVEN:
			tmp |= UART_LCR_PARITY | UART_LCR_EPAR;
			break;
		case MARK:
			tmp |= UART_LCR_PARITY | UART_LCR_SPAR;
			break;
		case SPACE:
			tmp |= UART_LCR_PARITY | UART_LCR_EPAR | UART_LCR_SPAR;
			break;
	}

	val = readl(up->uart_base + UART_LCR);
	val &= ~GENMASK(5, 0);
	val |= tmp;
	writel(up->uart_base + UART_LCR, val);
}


void echo_test(struct uart_dat *up)
{
	unsigned int val;
//	enum PARITY_TYPE par;
	unsigned int lsr;

	/* enable clock */
	ckg_enable(up);

	/* set baudrate in uart1 */
	set_baudrate(up, 115200);


	/* set line: 8bit data, 1bit stop, 0bit parity*/
//	par = EVEN;
	set_line_ctrl(up, 8, 1, NONE);

	while(1)
	{
		lsr = readl(up->uart_base + UART_LSR);
		if(lsr & UART_LSR_DR) {
			if(lsr & UART_LSR_BRK_ERROR_BITS)
				continue;
			val = readl(ud.uart_base + UART_RX);
			writel(ud.uart_base + UART_TX, val);
		}
	}
}

void irq_return(unsigned int irq)
{
	unsigned int iir, val;
	iir = readl(ud.uart_base + UART_IIR);
	iir &= UART_IIR_IID;
	iir += 0x30;
	val = readl(ud.uart_base + UART_RX);
	if(iir != 0x32)
		writel(ud.uart_base + UART_TX, iir);
}

void intr_test(struct uart_dat *up)
{
	/* enable clock */
	ckg_enable(up);

	/* set baudrate in uart1 */
	set_baudrate(up, 115200);

	/* set line: 8bit data, 1bit stop, None parity*/
	set_line_ctrl(up, 8, 1, NONE);

	request_irq(UART_IRQ_NR(TEST_PORT));

	/* interrupt enable */
	writel(ud.uart_base + UART_IER, 0xf);

	/* FIFO enable */
	writel(ud.uart_base + UART_FCR, 0x7);


	/* Auto flow control enable */
	writel(ud.uart_base + UART_MCR, 0x22);
}

int main(void)
{


	struct uart_dat *up = &ud;
	enum TEST_ID tst_id;
	uart_init(up);

	tst_id = TEST_TYPE;
	switch(tst_id) {
		case ECHO_TEST:
			/* echo test */
			echo_test(up);
			break;
		case INTR_TEST:
			/* echo test */
			intr_test(up);
			break;
	}

	while(1);
	return 0;
}
