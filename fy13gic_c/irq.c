#define ICC_IAR1_EL1_SPURIOUS   0x3ff
#define NR_CPUS                 6

#define GICD_BASE               0x1000000
#define GICR_BASE               0x1080000

#define GICD_CTLR					0x0000
#define GICD_TYPER					0x0004
#define GICD_IIDR					0x0008
#define GICD_STATUSR				0x0010
#define GICD_IGROUPR				0x0080
#define GICD_ISENABLER				0x0100
#define GICD_ICENABLER				0x0180
#define GICD_ISPENDR				0x0200
#define GICD_ICPENDR				0x0280
#define GICD_ISACTIVER				0x0300
#define GICD_ICACTIVER				0x0380
#define GICD_IPRIORITYR				0x0400
#define GICD_ITARGETSR				0x0800
#define GICD_ICFGR					0x0C00
#define GICD_IGRPMODR				0x0D00
#define GICD_NSACR					0x0E00
#define GICD_SGIR					0x0F00
#define GICD_IROUTER				0x6000
#define GICD_PIDR2					0xFFE8

#define GICD_CTLR_RWP				(1U << 31)

enum {
	IRQ_TYPE_NONE			= 0x00000000,
	IRQ_TYPE_EDGE_RISING	= 0x00000001,
	IRQ_TYPE_EDGE_FALLING	= 0x00000002,
	IRQ_TYPE_EDGE_BOTH		= (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
	IRQ_TYPE_LEVEL_HIGH		= 0x00000004,
	IRQ_TYPE_LEVEL_LOW		= 0x00000008,
	IRQ_TYPE_LEVEL_MASK		= (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
	IRQ_TYPE_SENSE_MASK		= 0x0000000f,
	IRQ_TYPE_DEFAULT		= IRQ_TYPE_SENSE_MASK,

	IRQ_TYPE_PROBE		= 0x00000010,

	IRQ_LEVEL			= (1 <<  8),
	IRQ_PER_CPU			= (1 <<  9),
	IRQ_NOPROBE			= (1 << 10),
	IRQ_NOREQUEST		= (1 << 11),
	IRQ_NOAUTOEN		= (1 << 12),
	IRQ_NO_BALANCING	= (1 << 13),
	IRQ_MOVE_PCNTXT		= (1 << 14),
	IRQ_NESTED_THREAD	= (1 << 15),
	IRQ_NOTHREAD		= (1 << 16),
	IRQ_PER_CPU_DEVID	= (1 << 17),
	IRQ_IS_POLLED		= (1 << 18),
	IRQ_DISABLE_UNLAZY	= (1 << 19),
};

typedef struct irq_hand{
	int irq;
	void(*func)(void *data);
	void *data;
}irq_handler_t;

struct pt_regs {
	unsigned int uregs[18];
};






static irq_handler_t irq_handler[256];
typedef void(*interrupt_func_t)(void *data);

void mprintf(char *line, ...);


/************************************************ handle irq start *************************************/
static unsigned long long gic_read_iar1_el1(void)
{
	unsigned long long  irqstat;
	asm volatile("mrs %0, s3_0_c12_c12_0": "=r"(irqstat));
	asm volatile("dsb sy");
	return irqstat;
}

static void gic_write_eoir1_el1(unsigned int irq)
{
	asm volatile("msr s3_0_c12_c12_1,%0"::"r"((unsigned long long)irq));
	asm volatile("isb sy");	 
}

static unsigned int gic_read_icc_ctlr_el1(void)
{
	unsigned int val;

	asm volatile("mrs %0, s3_0_c12_c12_4":"=r"(val));
	asm volatile("dsb sy");
	return val;
}

static int check_EOImode(void)
{
	unsigned int val;
	val = gic_read_icc_ctlr_el1();
	val &= 1 <<1;
	return val;
}

static  void gic_write_dir(unsigned int irq)
{
	asm volatile("msr s3_0_c12_c11_1,%0"::"r"((unsigned long long)irq));
	asm volatile("isb sy");
}

unsigned int irq_get_cpu_id()
{
        unsigned long long val;

        asm volatile("mrs %0, mpidr_el1" : "=r" (val));
        val = val >> 8;
        val = val & 0xff;

        return (unsigned int)val;
}

void handle_IPI(unsigned int irq, struct pt_regs *regs)
{
	mprintf("cpu%d get sgi%d interrupt and wake up\r\n",irq_get_cpu_id(),irq);
}

void do_irqs(unsigned int irq)
{
    mprintf("irq number: %d \t",irq);
	if (irq_handler[irq].irq == irq) {
		irq_handler[irq].func(irq_handler[irq].data);
	}
}


void gic_handle_irq(struct pt_regs *regs)
{
	//printf("gicv3_handle_irq start\r\n");
	unsigned int irqnr, i =0;
	do {
		irqnr = gic_read_iar1_el1();

		if ((irqnr > 15 && irqnr < 1020) || irqnr >= 8192) {
			gic_write_eoir1_el1(irqnr);
			if(check_EOImode())
				gic_write_dir(irqnr);
			do_irqs(irqnr);
			continue;
		}
		if (irqnr < 16) {
			gic_write_eoir1_el1(irqnr);
			if(check_EOImode())
				gic_write_dir(irqnr);
			handle_IPI(irqnr, regs);
			continue;
		}

	} while (irqnr != ICC_IAR1_EL1_SPURIOUS);
}

void do_irq(struct pt_regs *pt_regs, unsigned int esr)
{
	esr = esr;
    mprintf("irq interrupt \r\n");
	gic_handle_irq(pt_regs);
}

void do_fiq(struct pt_regs *pt_regs, unsigned int esr)
{
	esr = esr;
    mprintf("fiq interrupt \r\n");
	gic_handle_irq(pt_regs);
}
/************************************************ handle irq end *************************************/


/************************************************ request irq start *************************************/

static inline unsigned int readl_relaxed(volatile void *addr)
{
	return *((volatile unsigned int *)addr);
}

static int gic_peek_irq(unsigned int irq_num, unsigned int offset)
{
	unsigned int mask = 1 << (irq_num % 32);
	void  *base;

	if (irq_num < 32)
		base = (void *)(GICR_BASE + 0x10000); /* GICR_SGI_base */
	else
		base = (void *)(GICD_BASE);

	return !!(readl_relaxed(base + offset + (irq_num / 32) * 4) & mask);
}

void gic_mask_irq(unsigned int irq_num)
{
	unsigned int mask = 1 << (irq_num % 32);

    *(volatile unsigned int *)(unsigned long long)(GICD_BASE + GICD_ICENABLER + (irq_num / 32) * 4) = mask;

	//writel_relaxed(mask, g_gic->dist_base + GIC_DIST_ENABLE_CLEAR + (irq_num / 32) * 4);
}

 void gic_unmask_irq(unsigned int irq_num)
 {
	 unsigned int mask = 1 << (irq_num % 32);
 
	 //writel_relaxed(mask, g_gic->dist_base + GIC_DIST_ENABLE_SET + (irq_num / 32) * 4);
     *(volatile unsigned int *)(unsigned long long)(GICD_BASE + GICD_ISENABLER + (irq_num / 32) * 4) = mask;
 }

 void udelay(unsigned int ns)
 {
     unsigned int i,j;

    for(i = 0; i < ns; i++)
    {
        for(j = 0; j < 10000; j++){;}
    }
 }

 void gic_do_wait_for_rwp(void *base)
{
	unsigned int count = 1000000;	/* 1s! */

	while (readl_relaxed(base + GICD_CTLR) & GICD_CTLR_RWP) {
		count--;
		if (!count) {
			//mprintf("RWP timeout, gone fishing\r\n");
			return;
		}
		//udelay(1);
	}
}


int gic_set_affinity(unsigned int irq_num, unsigned int cpu)
{
	void  *reg;
	int enabled;
	//unsigned long long val;

	if (cpu >= NR_CPUS)
		return -1;

	if (irq_num < 32)
		return -1;

	/* If interrupt was enabled, disable it first */
	enabled = gic_peek_irq(irq_num, GICD_ISENABLER);
	if (enabled)
		gic_mask_irq(irq_num);

	reg = (void *)(unsigned long long)(GICD_BASE + GICD_IROUTER + (irq_num * 8));
//	val = gic_mpidr_to_affinity(cpu_logical_map(cpu));
//	gic_write_irouter(val, reg);
	*(volatile unsigned long long *)reg = (unsigned long long)cpu; 

	/*
	 * If the interrupt was enabled, enabled it again. Otherwise,
	 * just wait for the distributor to have digested our changes.
	 */
	if (enabled)
		gic_unmask_irq(irq_num);
	else
		gic_do_wait_for_rwp((void *)GICD_BASE);

	return 0;
}

 static int gic_configure_irq(unsigned int irq, unsigned int type, void *base)
 {
	 unsigned int confmask = 0x2 << ((irq % 16) * 2);
	 unsigned int confoff = (irq / 16) * 4;
	 unsigned int val, oldval;
	 int ret = 0;
	 unsigned long flags;
 
	 /*
	  * Read current configuration register, and insert the config
	  * for "irq", depending on "type".
	  */
	 val = oldval = readl_relaxed(base + GICD_ICFGR + confoff);
	 if (type & IRQ_TYPE_LEVEL_MASK)
		 val &= ~confmask;
	 else if (type & IRQ_TYPE_EDGE_BOTH)
		 val |= confmask;
 
	 /* If the current configuration is the same, then we are done */
	 if (val == oldval) {
		 return 0;
	 }
 
	 /*
	  * Write back the new configuration, and possibly re-enable
	  * the interrupt. If we fail to write a new configuration for
	  * an SPI then WARN and return an error. If we fail to write the
	  * configuration for a PPI this is most likely because the GIC
	  * does not allow us to set the configuration or we are in a
	  * non-secure mode, and hence it may not be catastrophic.
	  */
	 //writel_relaxed(val, base + GICD_ICFGR + confoff);
     *(volatile unsigned int *)(base + GICD_ICFGR + confoff) = val;
	 if (readl_relaxed(base + GICD_ICFGR + confoff) != val) {
		 if (irq >= 32)
			 ret = -1;
		 else
			 mprintf("GIC: PPI%d is secure or misconfigured\n", irq - 16);
	 }
 
	 gic_do_wait_for_rwp(base);
 
	 return ret;
 }


int gic_set_type(unsigned int irq_num, unsigned int type)
{
	void (*rwp_wait)(void);
	void  *base;

	/* Interrupt configuration for SGIs can't be changed */
	if (irq_num < 16)
		return -1;

	/* SPIs have restrictions on the supported types */
	if (irq_num >= 32 && type != IRQ_TYPE_LEVEL_HIGH &&
			 type != IRQ_TYPE_EDGE_RISING)
		return -1;

	if (irq_num < 32) {
		base = (void *)(unsigned long long)GICR_BASE + 0x10000; /* GICR_SGI_base */
	} else {
		base = (void *)(unsigned long long)GICD_BASE;
	}

	return gic_configure_irq(irq_num, type, base);
}

int request_irq(int irq, interrupt_func_t func, void *data)
{
	if (irq_handler[irq].irq != 0)
		return -1;
	irq_handler[irq].irq = irq;
	irq_handler[irq].func = func;
	irq_handler[irq].data = data;
	gic_set_affinity(irq, 0);
	gic_set_type(irq, IRQ_TYPE_LEVEL_HIGH);
	gic_unmask_irq(irq);
	
	return 0;
}

/************************************************ request irq end *************************************/




