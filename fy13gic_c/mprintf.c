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

extern unsigned long  get_cpu_id(void);

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
