extern void mmu_start(void);

struct reg_info_t {
	unsigned int reg_addr;
	unsigned int regv;
	unsigned int mask;
};

unsigned int ddr_cfg_info[] = {
	#include "soc_init_reg_list.h"
};

void dram_init(void)
{
	unsigned int i, reg_val;
	unsigned int len = ddr_cfg_info[0];
	struct reg_info_t *reginfo = (struct reg_info_t *)(&ddr_cfg_info[1]);
	for(i = 0; i < len; i++)
	{
		if((reginfo[i].reg_addr & 0x3) == 0x0)
		{
			reg_val = *(volatile unsigned int *)(unsigned long)reginfo[i].reg_addr;
			reg_val &= ~reginfo[i].mask;
			reg_val |= reginfo[i].regv;
			*(volatile unsigned int *)(unsigned long)reginfo[i].reg_addr = reg_val;
		} else {
			reginfo[i].reg_addr &= ~0x3;
			while((*(volatile unsigned int *)(unsigned long)reginfo[i].reg_addr & reginfo[i].mask) != reginfo[i].regv)
			{
				;
			}
		}
	}
}
#if 1
void mc_memcpy(void *dest, void *src, unsigned int size)
{
	unsigned int i = 0;
	for(i = 0; i < size; i += 8)
		*(unsigned long *)(dest + i) = *(unsigned long *)(src + i);
}
#endif
int main(void)
{
	void *dest = (void *)0x43000000;
	void *src  = (void *)0x42000000;
	unsigned int size, count;
	int i = 0;

	dram_init();
	mmu_start();

	size = 300*1024;
	count = 1000;
	mc_memcpy(dest,src,size);

	for (i = 0; i < count; i++)
	{
		mc_memcpy(dest,src,300*1024);
	}

	while(1);

	return 0;
}
