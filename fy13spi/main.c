#define SPIN_LOCK_BASE		0x10400000UL
#define RELEASE_LOCK		0x10C5A8A8

#define AP_APB_BASE			0x18000000UL
#define CKG_APB_GPIO_EN		20
#define TEST_GPIO_BASE		0x1ae40000UL
#define TEST_GPIO_IRQ		96
//#define TEST_GPIO_BASE		0x1af10000UL
//#define TEST_GPIO_IRQ		224

#define GICD_BASE 		0x1000000UL
#define GICD_IGROUPR 	(GICD_BASE + 0x80)
#define GICR_BASE		0x1080000UL
#define GICR_WAKER		(GICR_BASE + 0x14)
#define GICR_PWRR		(GICR_BASE + 0x24)
#define GICR_IGROUPR	(GICR_BASE + 0x10000 + 0x80)
#define GICR_ISENABLER	(GICR_BASE + 0x10000 + 0x100)
#define IRQ_NR_MAX		239
#define CPU_MAX			0x6

extern void mprintf(char *line, ...);
extern void uart_init(void);
extern void armv8_switch_to_el1(void);
void gpio_interrupt_disable(unsigned long gpio_base);

extern unsigned long _secondary_cpu_start;
unsigned long cpu_wait_flag = 0x0;

unsigned long  get_cpu_id(void)
{
	unsigned long long val;
	asm volatile("mrs %0, mpidr_el1" : "=r"(val));
	val = (val&0xffff) >> 0x8;
	return val;
}

void write_cpu_wait_flag(unsigned long cpu_id)
{
	unsigned int val32 = 1;
	while(val32 == 1)
		val32 = *(volatile unsigned int *)(SPIN_LOCK_BASE + 0x2000);
	asm volatile("isb sy");
	cpu_wait_flag |= 0x1 << cpu_id;
	asm volatile("isb sy");
	*(volatile unsigned int *)(SPIN_LOCK_BASE + 0x2000) = RELEASE_LOCK;
}

unsigned long read_cpu_wait_flag(void)
{
	unsigned int val32 = 1;
	unsigned long val64 = 0;
	while(val32 == 1)
		val32 = *(volatile unsigned int *)(SPIN_LOCK_BASE + 0x2000);
	asm volatile("isb sy");
	val64 = cpu_wait_flag;
	asm volatile("isb sy");
	*(volatile unsigned int *)(SPIN_LOCK_BASE + 0x2000) = RELEASE_LOCK;
	return val64;
}

void interrupt_callback(unsigned int irqnr, unsigned int intr_type)
{
	mprintf(">>>>>irqnr: %d, intr_type: %d\r\n",irqnr,intr_type);
	gpio_interrupt_disable(TEST_GPIO_BASE);
}

void gicd_init(void)
{
	int i;

	*(volatile unsigned int *)GICD_BASE = 0x37;

	for(i=1;i < (IRQ_NR_MAX >> 5);i++) {
		*(volatile unsigned int *)(GICD_IGROUPR+i*4) = 0xffffffff;
	}
	*(volatile unsigned int *)(GICD_IGROUPR+i*4) = (0x1ULL << ((IRQ_NR_MAX&0x1f)+1))-1;

}

void gicr_init(void)
{
	unsigned long cpu_id = 0xff;

	cpu_id = get_cpu_id();
	*(volatile unsigned int *)(GICR_PWRR  + cpu_id*0x20000) = 0;	//rdist power
	*(volatile unsigned int *)(GICR_WAKER + cpu_id*0x20000) = 0;	//rdist wake

	*(volatile unsigned int *)(GICR_IGROUPR + cpu_id*0x20000) = 0xffffffff;	//SGI&PPI group
	*(volatile unsigned int *)(GICR_ISENABLER + cpu_id*0x20000) = 0x1;		//enable SGI0

}
#if 0
static unsigned int read_ICC_SRE_EL3()
{
	unsigned int val;
	asm volatile("mrs %0,s3_6_c12_c12_5": "=r"(val));
	asm volatile("isb sy");
	return val;
}
#endif
static void write_ICC_SRE_EL3(unsigned int val)
{
	asm volatile("msr s3_6_c12_c12_5,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_SRE_EL2(unsigned int val)
{
	asm volatile("msr s3_4_c12_c9_5,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_IGRPEN1_EL3(unsigned int val)
{
	asm volatile("msr s3_6_c12_c12_7,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_CTLR_EL3(unsigned int val)
{
	asm volatile("msr s3_6_c12_c12_4,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_CTLR_EL1(unsigned int val)
{
	asm volatile("msr s3_0_c12_c12_4,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_PMR_EL1(unsigned int val)
{
	asm volatile("msr s3_0_c4_c6_0,%0"::"r"(val));
	asm volatile("isb sy");
}

static void write_ICC_SGI1R_EL1(unsigned long val)
{
	asm volatile("msr s3_0_c12_c11_5,%0"::"r"(val));
	asm volatile("isb sy");
}


void gicc_init(void)
{
	write_ICC_SRE_EL3(0xf);			//SRE & Disable IRQ/FIQ Bypass & Allow EL2 access to ICC_SRE_EL2 
	write_ICC_SRE_EL2(0xf);			//SRE & Disable IRQ/FIQ Bypass & Allow EL1 access to ICC_SRE_EL1
	write_ICC_IGRPEN1_EL3(0x3);		//EnableGrp1NS | EnableGrp1S
	write_ICC_CTLR_EL3(0x0);
	write_ICC_CTLR_EL1(0x0);		//NonSecure ICC_CTLR_EL1
	write_ICC_PMR_EL1(0x80);		//Non-Secure access to ICC_PMR_EL1

}

void gpio_init(void)
{
	unsigned int val32;
	/* enable gpio clock */
	val32 = *(volatile unsigned int *)(AP_APB_BASE + 0x4);
	val32 |= 0x1 << CKG_APB_GPIO_EN;
	*(volatile unsigned int *)AP_APB_BASE = val32;
}

void gpio_config(unsigned long gpio_base)
{
	*(volatile unsigned int *)(gpio_base + 0x4) = 0xff;
	*(volatile unsigned int *)(gpio_base + 0x14) = 0;
}

void gpio_interrupt_enable(unsigned long gpio_base)
{
	*(volatile unsigned int *)(gpio_base + 0x18) = 0xff;
}

void gpio_interrupt_disable(unsigned long gpio_base)
{
	*(volatile unsigned int *)(gpio_base + 0x18) = 0;
}

int main(void)
{
	int i;
	unsigned long cpu_id;

	uart_init();
	mprintf("master cpu online\r\n");

	gicd_init();
	gicr_init();
	gicc_init();

	armv8_switch_to_el1();

	gpio_init();
	gpio_config(TEST_GPIO_BASE);
	*(volatile unsigned int *)(GICD_BASE + 0x100 + (TEST_GPIO_IRQ >> 5)*4) = 0x1 << (TEST_GPIO_IRQ&0x1f);
	gpio_interrupt_enable(TEST_GPIO_BASE);
	

	while(1);
	return 0;
}
