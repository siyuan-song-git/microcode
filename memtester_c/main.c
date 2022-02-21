typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_arg(v,l)   __builtin_va_arg(v,l)


#define AP_AHB_BASE			0x10000000
#define SPIN_LOCK_BASE		0x10400000
#define RELEASE_LOCK		0x10C5A8A8
#define AP_APB_BASE			0x18000000
#define CKG_UART0_EN		0x0
#define CKG_APB_UART0_EN	0x0
#define UART0_SOFT_RST		0x0
#define UART_BASE 			0x18200000
#define UART_RBR 			(*(unsigned int *)UART_BASE)
#define UART_THR 			(*(unsigned int *)UART_BASE)
#define UART_LSR 			(*(unsigned int *)(UART_BASE + 0x14))

#define IRAM_BASE			0x0
#define VEU_SRAM_BASE		0x30800000

extern unsigned long mmu_tbl_addr;
extern unsigned long _start;

extern void mmu_enable(unsigned long);
extern void test_entry(unsigned long test_addr, unsigned long len);

unsigned long tbl0_addr = (unsigned long)&mmu_tbl_addr;
unsigned long tbl1_addr = (unsigned long)&mmu_tbl_addr + 0x1000;
unsigned long tbl2_addr = (unsigned long)&mmu_tbl_addr + 0x2000;

static unsigned long  get_cpu_id(void)
{
	unsigned long long val;
	asm volatile("mrs %0, mpidr_el1" : "=r"(val));
	val = (val&0xffff) >> 0x8;
	return val;
}

void uart_init(void)
{
	unsigned int clock = 24000000;
	unsigned int baudrate = (clock/115200 + 8)/16;
	unsigned int val32 = 0;

	/* enable uart clock */
	val32 = *(volatile unsigned int *)AP_APB_BASE;
	val32 |= 0x1 << CKG_UART0_EN;
	*(volatile unsigned int *)AP_APB_BASE = val32;

	val32 = *(volatile unsigned int *)(AP_APB_BASE + 0x4);
	val32 |= 0x1 << CKG_APB_UART0_EN;
	*(volatile unsigned int *)(AP_APB_BASE + 0x4) = val32;

	/* release uart */
	val32 = *(volatile unsigned int *)(AP_APB_BASE + 0xc);
	val32 &= ~(0x0 << UART0_SOFT_RST);
	*(volatile unsigned int *)(AP_APB_BASE + 0xc) = val32;

	/* reset fifo */
	*(volatile unsigned int *)(UART_BASE + 0x8) = 0 << 6;

	/* set DLAB for setting baudrate */
	val32 = *(volatile unsigned int *)(UART_BASE + 0xc);
	val32 |= 0x1 << 7;
	*(volatile unsigned int *)(UART_BASE + 0xc) = val32;

	/* set baudrate */
	*(volatile unsigned int *)(UART_BASE + 0x0) = baudrate & 0xff;
	*(volatile unsigned int *)(UART_BASE + 0x4) = (baudrate & 0xff00) >> 8;

	/* set DLAB for normal config */
	val32 = *(volatile unsigned int *)(UART_BASE + 0xc);
	val32 &= ~(0x1 << 7);
	*(volatile unsigned int *)(UART_BASE + 0xc) = val32;

	/* set line status 8 bit 1 stop no parity no flow control */
	*(volatile unsigned int *)(UART_BASE + 0xc) = 0x3;

	/* set fifo control rx fifo 1/4 full & fifo enable */
	*(volatile unsigned int *)(UART_BASE + 0x8) = 0x41;

	val32 = *(volatile unsigned int *)AP_AHB_BASE;
	val32 |= (1 << 10);
	*(volatile unsigned int *)AP_AHB_BASE = val32;
}

void m_putchar(int val)
{
	while (!(UART_LSR & (1 << 6)));
	UART_THR = val;
	if ('\n' == val)
		m_putchar('\r');
}

void m_puts(char *line)
{
	while (*line)
		m_putchar(*line++);
}

void print_int_nonzero(unsigned long val)
{
	if (val <= 0)
		return;
	print_int_nonzero(val/10);
	m_putchar('0' + val%10);
}

void print_int(unsigned long val)
{
	if (val == 0)
		m_putchar('0');
	else
		print_int_nonzero(val);
}

void print_hex_nonzero(unsigned long val)
{
	if (val <= 0)
		return;
	print_hex_nonzero(val/0x10);
	if ((val%0x10) >= 10)
		m_putchar('a' + val%0x10 - 10);
	else
		m_putchar('0' + val%0x10);
}

void print_hex(unsigned long val)
{
	if (val == 0)
		m_putchar('0');
	else
		print_hex_nonzero(val);
}

void mprintf(char *line, ...)
{
	va_list arg_p;
	unsigned int val32 = 1;
	unsigned long arg_val, id;
	unsigned char *l = line;
	char *ps_val;

	while(val32 == 1)
		val32 = *(volatile unsigned int *)(SPIN_LOCK_BASE + 0x1000);

	id = get_cpu_id();
	m_putchar('-');
	print_int(id);
	m_putchar('-');

	va_start(arg_p, line);
	while(*l)
	{
		while ((*l) && (*l != '%'))
			m_putchar(*l++);
		if (0 == *l)
			break;
		l++;
		switch (*l)
		{
			case 'd':
				arg_val = va_arg(arg_p, int);
				print_int(arg_val);
				break;
			case 'x':
				arg_val = va_arg(arg_p, int);
				print_hex(arg_val);
				break;
			case 's':
				ps_val = va_arg(arg_p, char *);
				m_puts(ps_val);
				/*
				for (ps_val = va_arg(arg_p, char *); *ps_val; ps_val++)
					m_putchar(*ps_val);
				*/
				break;
		}
		l++;
	}
	*(volatile unsigned int *)(SPIN_LOCK_BASE + 0x1000) = RELEASE_LOCK;
}

static void clr_addr(unsigned long addr , unsigned int size)
{
	unsigned int i = 0;
	for(i = 0; i < size; i += 8)
	{
		*(volatile unsigned long long *)(addr + i) = 0;
	}
}

void mmu_table_map(void)
{
	unsigned int i;

	clr_addr(tbl0_addr, 0x1000);
	clr_addr(tbl1_addr, 0x1000);
	clr_addr(tbl2_addr, 0x1000);

	for ( i=0; i<0x1000/8; i++)
	{
		if(i*0x200000 == IRAM_BASE || i*0x200000 == VEU_SRAM_BASE)
			*(volatile unsigned long *)(tbl1_addr+ i*8) = 0x00000411 + 0x200000 * i;
		else
			*(volatile unsigned long *)(tbl1_addr+ i*8) = 0x60000000000401 + 0x200000 * i;
	}
	*(volatile unsigned long *)tbl0_addr = tbl1_addr + 3;
}

#define TEST_SIZE 0x8000

int main(void)
{
	int i;
	unsigned long id = get_cpu_id();
	mprintf("cpu%d alive!\r\n",id);
	uart_init();
	mmu_table_map();
	mmu_enable(tbl0_addr);
	mprintf("cpu%d mmu enable!\r\n",id);

	for (i=0; i<7; i++)
		*(volatile unsigned long *)(0x2400018UL+i*8) = (unsigned long)&_start >> 2;
	*(volatile unsigned int *)0x2400008 = 0x0;

	test_entry(0x30800000,TEST_SIZE);

	while(1);
	return 0;
}


void secondary_cpu_entry(void)
{
	unsigned long id = get_cpu_id();
	mprintf("cpu%d alive!\r\n",id);
	mmu_enable(tbl0_addr);
	mprintf("cpu%d mmu enable!\r\n",id);

	test_entry(0x30800000 + TEST_SIZE*id,TEST_SIZE);

}