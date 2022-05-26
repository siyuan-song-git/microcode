#define TIMER0_BASE			0x19400000
#define TIMER0_IRQ			32

#define GPIO4_BASE			0x1ae40000
#define	GPIO4_IRQ			96
#define GPIO17_BASE			0x1af10000
#define	GPIO17_IRQ			224

extern void release_secondary_cpu(void);

extern void uart_init(void);
void mprintf(char *line, ...);

#define CPU_NR                  6
enum {
        CPU_OFFLINE = 0,
        CPU_ONLINE,
		CPU_IDLE,
		CPU_ALIVE,
};

extern unsigned char cpu_status[CPU_NR];

typedef void(*interrupt_func_t)(void *data);
extern int request_irq(int irq, interrupt_func_t func, void *data);
extern int gic_set_affinity(unsigned int irq_num, unsigned int cpu);


int check_all_cpu_status(unsigned int cpu_s)
{
	unsigned int i;

	do {
		for(i = 0; i < CPU_NR; i++)
		{
			if(cpu_status[i] != cpu_s)
				return -1;
		}
	}while(i < CPU_NR);

	return 0;
}

int interrupt_callback_count = 0;
void timer_callback(void *data)
{
	*(volatile unsigned int *)(TIMER0_BASE + 0x0014)= 0;
	if(interrupt_callback_count != 0)
		cpu_status[interrupt_callback_count] = CPU_ALIVE;
	
	mprintf("timer callback\r\n");

	gic_set_affinity(TIMER0_IRQ, ++interrupt_callback_count);
	if(interrupt_callback_count < CPU_NR)
		*(volatile unsigned int *)(TIMER0_BASE + 0x0014) = 1;
	if(interrupt_callback_count != 1)
		while(cpu_status[0] != CPU_IDLE);
	

}

void spi_route_to_multcore_test()
{
	int ret = 0;

	ret = request_irq(TIMER0_IRQ, &timer_callback, (void *)0);
	gic_set_affinity(TIMER0_IRQ, 0);

	*(volatile unsigned int *)(TIMER0_BASE + 0x0000) = 0xffff;
	*(volatile unsigned int *)(TIMER0_BASE + 0x0010) = 0xffff;
	*(volatile unsigned int *)(TIMER0_BASE + 0x0014) = 1;	
}

void gpio_callback(void *data)
{
	mprintf("gpio_callback \r\n");
	*(volatile unsigned int *)(data + 0x0018) = 0;
}

static void write_icc_asgi1r_el1(unsigned long long val)
{
	asm volatile("msr S3_0_C12_C11_6,%0"::"r"(val));
	asm volatile("isb sy");	 
}

static void write_icc_sgi1r_el1(unsigned long long val)
{
	asm volatile("msr S3_0_C12_C11_5,%0"::"r"(val));
	asm volatile("isb sy");	 
}

static void write_icc_sgi0r_el1(unsigned long long val)
{
	asm volatile("msr S3_0_C12_C11_7,%0"::"r"(val));
	asm volatile("isb sy");	 
}




int main(void)
{
	unsigned int i=0;
	int ret = 0;

	uart_init();
	mprintf("mprintf() function init finished \r\n");
	cpu_status[0] = CPU_ONLINE;
	release_secondary_cpu();

	while(check_all_cpu_status(CPU_ONLINE));
	cpu_status[0] = CPU_ALIVE;

	mprintf("all cpu online \r\n");

	mprintf("spi_route_to_multcore_test start\r\n");
	spi_route_to_multcore_test();

	while(check_all_cpu_status(CPU_ALIVE));
	cpu_status[0] = CPU_IDLE;

	mprintf("spi_route_to_multcore_test end\r\n");
	

	mprintf("spi_>192_bit_test start\r\n");
	ret = request_irq(GPIO4_IRQ, &gpio_callback, (void *)GPIO4_BASE);
	gic_set_affinity(96, 0);

	*(volatile unsigned int *)(GPIO4_BASE + 0x0008) = 0x1;
	*(volatile unsigned int *)(GPIO4_BASE + 0x0014) = 0x0;
	*(volatile unsigned int *)(GPIO4_BASE + 0x0018) = 0x1;

	ret = request_irq(GPIO17_IRQ, &gpio_callback, (void *)GPIO17_BASE);
	gic_set_affinity(224, 0);

	*(volatile unsigned int *)(GPIO17_BASE + 0x0008) = 0x1;
	*(volatile unsigned int *)(GPIO17_BASE + 0x0014) = 0x0;
	*(volatile unsigned int *)(GPIO17_BASE + 0x0018) = 0x1;
	mprintf("spi_>192_bit_test end\r\n");
	
	//mprintf("ASGI1R:\r\n");
	//write_icc_asgi1r_el1(0xffULL);
	
	mprintf("SGI1R:\r\n");
	write_icc_sgi1r_el1(0xffULL);
	
	//mprintf("SGI0R:\r\n");
	//write_icc_sgi0r_el1(0xffULL);

	while(1);

	return 0;
}
