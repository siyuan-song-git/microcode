#include "gic600.h"

extern void armv8_switch_to_el1(void);
static u64 read_mpidr_el1(void)
{
        u64 mpidr_el1;
        asm volatile("mrs %0,s3_0_c0_c0_5": "=r"(mpidr_el1));
        asm volatile("dsb sy");
        return mpidr_el1;
}

static int setDistAddr(dist_regs* dist)
{
	dist->GICD_CTLR		= (unsigned int 	*)(GICD_BASE);
	dist->GICD_TYPER	= (unsigned int 	*)(GICD_BASE + 0x0004);
	dist->GICD_IIDR		= (unsigned int 	*)(GICD_BASE + 0x0008);
	dist->GICD_SETSPI_SR	= (unsigned int 	*)(GICD_BASE + 0x0050);
	dist->GICD_IGROUPR	= (unsigned int 	*)(GICD_BASE + 0x0080);
	dist->GICD_ISENABLER	= (unsigned int 	*)(GICD_BASE + 0x0100);
	dist->GICD_IPRIORITYR	= (unsigned char	*)(GICD_BASE + 0x0400);
	dist->GICD_ITARGETSR0	= (unsigned int 	*)(GICD_BASE + 0x0800);
	dist->GICD_IGRPMODR	= (unsigned int 	*)(GICD_BASE + 0x0D00);
	dist->GICD_IROUTER	= (unsigned long	*)(GICD_BASE + 0x6000);
	dist->GICD_PIDR2 	= (unsigned int 	*)(GICD_BASE + 0xFFE8);

	if(dist->GICD_PIDR2[0] == 0x3B) {
		return 0;
	} else {
		return -1;
	}
}


static int getReDistAddr(core_mpidr mpidr, unsigned** gicr_base)
{
	unsigned *gicr_typer;
	unsigned *gicr_pidr;
	int re_dist_cnt = 0;

	do {
		gicr_typer = ((unsigned *)GICR_BASE + (re_dist_cnt << 15) + 2);
		gicr_pidr = ((unsigned *)gicr_typer + 0x3FF8);
		if(gicr_pidr[0] == 0x3B) {
			re_dist_cnt++;
			if(((gicr_typer[1] & 0xffffff) == ((mpidr.fields.aff2 << 16) \
				+ (mpidr.fields.aff1 << 8) + mpidr.fields.aff0))) {
				*gicr_base = gicr_typer - 2;
			}
		} else {
			return -1;
		}

	} while(!(gicr_typer[0] & 0x10));

	if(re_dist_cnt != NUM_CORES) {
		return -1;
	} else {
		return 0;
	}
}

static int setReDistAddr(redist_regs* redist, unsigned** gicr_base)
{
	if(gicr_base != 0) {
		redist->GICR_CTLR	= (unsigned int 	*)((char*)*gicr_base + 0x00000);
		redist->GICR_TYPER	= (unsigned int 	*)((char*)*gicr_base + 0x00008);
		redist->GICR_WAKER	= (unsigned int 	*)((char*)*gicr_base + 0x00014);
		redist->GICR_PWRR	= (unsigned int 	*)((char*)*gicr_base + 0x00024);
		redist->GICR_SETLPIR	= (unsigned int 	*)((char*)*gicr_base + 0x00040);			
		redist->GICR_PROPBASER	= (unsigned int 	*)((char*)*gicr_base + 0x00070);			
		redist->GICR_PENDBASER	= (unsigned int 	*)((char*)*gicr_base + 0x00078);			
		redist->GICR_IGROUPR	= (unsigned int 	*)((char*)*gicr_base + 0x10080);			
		redist->GICR_IGRPMODR	= (unsigned int 	*)((char*)*gicr_base + 0x10D00);	
		redist->GICR_IPRIORITYR	= (unsigned char 	*)((char*)*gicr_base + 0x10400);
		redist->GICR_ISENABLER	= (unsigned int 	*)((char*)*gicr_base + 0x10100);
		redist->GICR_ISPENDR	= (unsigned int 	*)((char*)*gicr_base + 0x10200);
		return 0;	
	} else {
		return -1;
	}
}

static inline void write_icc_sre_el1(u32 val)
{
        asm volatile("msr s3_0_c12_c12_5,%0"::"r"((u32)val));// #0x3,#0x0,c12,c12,#0x5
}
static inline void write_icc_sre_el3(u32 val)
{
        asm volatile("msr s3_6_c12_c12_5,%0"::"r"((u32)val));//  #0x3,#0x6,c12,c12,#0x5,x0
}

static void setSre(dist_regs* dist, int sre)
{
	dist->GICD_CTLR[0] = 0x0;
	while(dist->GICD_CTLR[0] & 0x80000000){}
	dist->GICD_CTLR[0] = dist->GICD_CTLR[0] | (sre << 5) + (sre << 4) & 0x30;

	write_icc_sre_el3(sre);	
	write_icc_sre_el1(sre);
	asm volatile("isb sy");
}

static void setupDist(dist_regs* dist, core_mpidr mpidr)
{
	dist->GICD_CTLR[0] = dist->GICD_CTLR[0] | 0xF;
	dist->GICD_IROUTER[32] = 0;
}

static int setupReDist(test_control* test_ctrl, redist_regs* redist, core_mpidr mpidr)
{
	int i;

	redist->GICR_WAKER[0] = redist->GICR_WAKER[0];
	redist->GICR_WAKER[0] = 0;
	for(i=0;(redist->GICR_WAKER[0] & 0x4); i++) {
		if(i>=100)
			return -1;
	}

	redist->GICR_PROPBASER[0] = (u32)PROP_TB_BASE + 13;
	//redist->GICR_PROPBASER[0] = PROP_TB_BASE | 0x8f;
	redist->GICR_PROPBASER[1] = 0 | (u32)(PROP_TB_BASE >> 32);

	redist->GICR_PENDBASER[0] = PEND_TB_BASE + (0x1000 * test_ctrl->core_num);
	//redist->GICR_PENDBASER[0] = PEND_TB_BASE + (0x1000 * test_ctrl->core_num) | 0x80;
	//redist->GICR_PENDBASER[0] = (PEND_TB_BASE + (0x1000 * test_ctrl->core_num)) | 0x800;
	redist->GICR_PENDBASER[1] = 1 << 30;

	redist->GICR_CTLR[0] = 0x1;
	return 0;
}

static void setupDep(test_control* test_ctrl, dist_regs* dist)
{
	test_ctrl->lpiSup = dist->GICD_TYPER[0] & 0x20000;
	test_ctrl->spiSup = dist->GICD_TYPER[0] & 0x10000;
	test_ctrl->are0Sup = (dist->GICD_CTLR[0] & 0x30) == 0;
}

static inline void write_scr_el3(u32 val)
{
        asm volatile("msr s3_6_c1_c1_0,%0"::"r"((u32)val));
}

static inline void write_icc_ctlr_el3(u32 val)
{
        asm volatile("msr s3_6_c12_c12_4,%0"::"r"((u32)val));
}

static inline void write_icc_igrpen0_el1(u32 val)
{
        asm volatile("msr s3_0_c12_c12_6,%0"::"r"((u32)val));
}

static inline void write_icc_igrpen1_el3(u32 val)
{
        asm volatile("msr s3_6_c12_c12_7,%0"::"r"((u32)val));
}

static inline void write_icc_pmr_el1(u32 val)
{
        asm volatile("msr s3_0_c4_c6_0,%0"::"r"((u32)val));
}

static void write_icc_bpr0_el1(u32 val)
{
        asm volatile("msr s3_0_c12_c8_3,%0"::"r"((u32)val));
}

static void setupCPUIF(void)
{
	write_scr_el3(0x6);
        asm volatile("isb sy");

	write_icc_ctlr_el3(0x1c);
	write_icc_igrpen0_el1(0x1);
	write_icc_igrpen1_el3(0x3);
	write_icc_pmr_el1(0xff);
	write_icc_bpr0_el1(0x3);
	asm volatile("isb sy");
}

static void setItsAddr(its_regs* its)
{
	its->GITS_CTLR  	= (unsigned int 	*)(GITS_BASE);
	its->GITS_CBASER  	= (unsigned int 	*)(GITS_BASE + 0x80);
	its->GITS_CWRITER	= (unsigned long 	*)(GITS_BASE + 0x88);
	its->GITS_BASER	  	= (unsigned int 	*)(GITS_BASE + 0x100);
}

static void setupIts(its_regs* its)
{
	setItsAddr(its);

	its->GITS_BASER[0] = (u32)DEV_TB_BASE;
	its->GITS_BASER[1] = 0x80000000 | (u32)(DEV_TB_BASE >> 32);
	its->GITS_BASER[2] = (u32)COL_TB_BASE; 
	its->GITS_BASER[3] = 0x80000000 | (u32)(COL_TB_BASE >> 32);

	its->GITS_CWRITER[0] = 0;

	its->GITS_CBASER[0] = (u32)CMDQ_BASE + CMDQ_SIZE;
	its->GITS_CBASER[1] = 0x80000000 | (u32)(CMDQ_BASE >> 32);

	its->GITS_CTLR[0] = 0x1;
}

static inline void write_icc_sgi0r_el1(u64 val)
{
        //asm volatile("msr_s " __stringify(ICC_SGI1R_EL1) ", %0" : : "r" (val));
        asm volatile("msr s3_0_c12_c11_7,%0"::"r"(val));
}


static int genSgi(redist_regs* redist, core_mpidr mpidr, interrupt intr)
{
	if(intr.id < 16) {
		redist->GICR_IPRIORITYR[intr.id] = intr.priority;
		redist->GICR_IGROUPR[0] = 0;
		redist->GICR_IGRPMODR[0] = 0;
		redist->GICR_ISENABLER[0] |= 1<<intr.id;
		asm volatile("dsb sy");
		write_icc_sgi0r_el1(((unsigned long)mpidr.fields.aff2 << 32) + (intr.id << 24) +\
			(mpidr.fields.aff1 << 16) + (1 << mpidr.fields.aff0));
		return 0;
	} else {
		return -1;
	}
}


static int axisChk(test_control* test_ctrl, redist_regs* redist, core_mpidr mpidr)
{
	interrupt intr;
	int ret;
	intr.id = test_ctrl->core_num & 0x0f;
	intr.group = 0;
	intr.priority = 0x50 + ((test_ctrl->core_num & 0x70) >> 4);

	ret = genSgi(redist, mpidr, intr);
	if(ret < 0)
		return -1;	
	return 0;
}

static void c_mapd(unsigned long* dword, int cmd, int dev_id, int size, u64 itt_addr, int valid)
{
	dword[0] = ((unsigned long)dev_id << 32) | (cmd);
	dword[1] = size;
	dword[2] = ((unsigned long)valid << 63) | (itt_addr & 0x7ffffffffff00);
	dword[3] = 0;
}

static void c_mapc(unsigned long* dword, int cmd, int collection, int target_addr, int valid)
{
	dword[0] = cmd;
	dword[1] = 0;
	dword[2] = ((unsigned long)valid << 63) | ((unsigned long)target_addr << 16) | (collection);
	dword[3] = 0;
}

static void c_mapi(unsigned long* dword, int cmd, int dev_id, int id, int collection)
{
	dword[0] = ((unsigned long)dev_id << 32) | (cmd);
	dword[1] = id;
	dword[2] = collection;
	dword[3] = 0;
}

static void c_mapti(unsigned long* dword, int cmd, int dev_id, int eve_id, int int_id, int collection)
{
	dword[0] = ((unsigned long)dev_id << 32) | (cmd);
	dword[1] = ((unsigned long)int_id << 32) | eve_id;
	dword[2] = collection;
	dword[3] = 0;
}

static void c_inv(unsigned long* dword, int cmd, int dev_id, int id)
{
	dword[0] = ((unsigned long)dev_id << 32) | (cmd);
	dword[1] = id;
	dword[2] = 0;
	dword[3] = 0;
}

static void c_int(unsigned long* dword, int cmd, int dev_id, int id)
{
	dword[0] = ((unsigned long)dev_id << 32) | (cmd);
	dword[1] = id;
	dword[2] = 0;
	dword[3] = 0;
}

static void itsCmdWrite(unsigned long* dword, unsigned long** cwrite_ptr)
{
	int i;

	for(i = 0; i < 4; i++) {
		(*cwrite_ptr)[i] = dword[i];
	}
	asm volatile("dsb sy");
	*cwrite_ptr = (*cwrite_ptr == (unsigned long*)(CMDQ_BASE + 512*(CMDQ_SIZE + 1))) ? (unsigned long*)CMDQ_BASE : *cwrite_ptr + 4;
}

static void genLpi(redist_regs* redist, its_regs* its, core_mpidr mpidr, interrupt intr, unsigned int collection)
{
	unsigned long* cwrite_ptr;
	unsigned int dev_id;
	unsigned long dword[4];

	((unsigned char*)PROP_TB_BASE)[(intr.id - 8192)] = intr.priority | 3;

#if NUM_ITS != 0
	cwrite_ptr = (unsigned long*)(its->GITS_CWRITER[0] + CMDQ_BASE);
	dev_id = 0x1;
	c_mapd(dword, CMD_MAPD, dev_id, ITT_ENTRY_SIZE, IT_TB_BASE, 1);
	itsCmdWrite(dword, &cwrite_ptr);

	c_mapc(dword, CMD_MAPC, collection, 0, 1);
	itsCmdWrite(dword, &cwrite_ptr);

	//c_mapi(dword, CMD_MAPI, dev_id, intr.id, collection);
	//itsCmdWrite(dword, &cwrite_ptr);

	c_mapti(dword, CMD_MAPTI, dev_id, 10, intr.id, collection);
	itsCmdWrite(dword, &cwrite_ptr);

	c_inv(dword, CMD_INV, dev_id, intr.id);
	itsCmdWrite(dword, &cwrite_ptr);

	//c_int(dword, CMD_INT, dev_id, intr.id);
	//itsCmdWrite(dword, &cwrite_ptr);

	its->GITS_CWRITER[0] = (unsigned long)((char*)cwrite_ptr - CMDQ_BASE);
#else
	redist->GICR_SETLPIR[0] = intr.id;
#endif
}

#define INT_NUM 4
static int aximChk(test_control* test_ctrl, redist_regs* redist, its_regs* its, core_mpidr mpidr)
{
	unsigned int collection = 0;
	//interrupt intr;

	int i;
	interrupt intr[INT_NUM];
	for(i=0;i<INT_NUM;i++)
	{
		intr[i].id = test_ctrl->core_num + 8192 + i;
		intr[i].group = 1;
		intr[i].priority = 0xc0;
	}

	//intr.id = test_ctrl->core_num + 8192;
	//intr.group = 1;
	//intr.priority = 0xc0;
	if(test_ctrl->lpiSup) {
		if(test_ctrl->core_num)
			return -1;
		for(i=0;i<INT_NUM;i++)
		{
			genLpi(redist, its, mpidr, intr[i], collection);	
		}
		//genLpi(redist, its, mpidr, intr, collection);
		return 0;
	} else {
		return -1;
	}
}

static int test_entry(void)
{
	int sre, ret;
	u32 val;
	dist_regs dist;
	redist_regs redist;
	its_regs its;
	core_mpidr mpidr;
	unsigned *gicr_base;

	test_control test_ctrl_obj = {0};
	test_control* test_ctrl = &test_ctrl_obj;

	//*(volatile unsigned int *)0x2400068 = 0x01050100;
	*(volatile unsigned int *)0x24000c4 = 0x1 << (16 + TEST_ITS_NUM);

	if(TEST_ITS_NUM == 2){
		val = *(volatile unsigned int *)0x24000c4;
		val |= 0x1;
		*(volatile unsigned int *)0x24000c4 = val;		
	} else if(TEST_ITS_NUM == 1){
		*(volatile unsigned int *)0x24000c8 = 0x1 << 16;
	} else {
		*(volatile unsigned int *)0x24000c8 = 0x1;
	}

	mpidr.word = read_mpidr_el1();
	ret = setDistAddr(&dist);
	if(ret < 0)
		return -1;
	
	ret = getReDistAddr(mpidr, &gicr_base);
	if(ret < 0)
		return -1;

	ret = setReDistAddr(&redist, &gicr_base);
	if(ret < 0)
		return -1;

	//setupTest
	test_ctrl->core_num = ((redist.GICR_TYPER[0] & 0x00ffff00) >> 8);

	setupDep(test_ctrl, &dist);

	redist.GICR_PWRR[0] = 0x2;
	sre = 1;
#if 1	//setup
	setSre(&dist, sre);
	setupDist(&dist, mpidr);
	ret = setupReDist(test_ctrl, &redist, mpidr);
	if(ret < 0)
		return -1;
	setupCPUIF();
	setupIts(&its);
#endif
#if 0	// check AXI Stream Interfaces (cpu interface) by LPI
	ret = axisChk(test_ctrl, &redist, mpidr);
	if(ret < 0)
		return -1;
#endif
#if 1	// check AXI Master Interface by LPI
	armv8_switch_to_el1();
	ret = aximChk(test_ctrl, &redist, &its, mpidr);
	if(ret < 0)
		return -1;
#endif

	return 0;
}

static void clr_addr(unsigned long addr , unsigned int size)
{
	unsigned int i = 0;
	for(i = 0; i < size; i += 8)
	{
		*(volatile unsigned long long *)(addr + i) = 0;
	}
}

void main (void)
{
	int ret,i;	
	clr_addr(0x141008000, 0x141020000 - 0x141008000);
	ret = test_entry();
	
	//while(1);
	while(1)
	{
		for(i = 0; i < INT_NUM; i++)
		{
			for(ret = 0; ret < 1000; ret++) {}
			//*(volatile long long *)0x1050040 = 0x100002000ULL + (u64)i;
			*(volatile unsigned int *)(0x3000040 + 1000000 * TEST_ITS_NUM) = 0xaULL + (u32)i;
		}
	}
	while(1);
	return;
}
