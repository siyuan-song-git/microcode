
#define RELEASE_CPU_ADDR	0x02400008
#define CPU0_START_ADDR		0x02400010
#define CPU_NR                  6
enum {
        CPU_OFFLINE = 0,
        CPU_ONLINE,
        CPU_IDLE,
        CPU_ALIVE
};

unsigned char cpu_status[CPU_NR] = {0};

extern int  _secondary_start;

extern void mprintf(char *line, ...);

void release_secondary_cpu()
{
        unsigned long long secondary_cpu_start_addr = 0;
	unsigned int i = 0;
	secondary_cpu_start_addr = (unsigned long long )&_secondary_start;
	secondary_cpu_start_addr = secondary_cpu_start_addr >> 2;

	for (i = 1; i < 8; i++)
	{
		*(volatile unsigned int *)(unsigned long long)(CPU0_START_ADDR + i * 8) = secondary_cpu_start_addr;
	}

	*(volatile unsigned int *)(unsigned long long)(RELEASE_CPU_ADDR) = 0x0;//0xFC;

	return;
}

unsigned int get_cpu_id()
{
        unsigned long val;

        asm volatile("mrs %0, mpidr_el1" : "=r" (val));
        val = val >> 8;
        val = val & 0xff;

        return (unsigned int)val;
}

void test_code()
{
        unsigned int cpu_id, lsr_temt;
        
        cpu_id = get_cpu_id();
        mprintf("cpu%d online\r\n",cpu_id);
        cpu_status[cpu_id] = CPU_ONLINE;

}

void secondary_entry()
{
        unsigned int cpu_id;

        cpu_id = get_cpu_id();
        mprintf("cpu%d online\r\n",cpu_id);
        cpu_status[cpu_id] = CPU_ONLINE;

        while(cpu_status[0] != CPU_ALIVE);
        
        while(1){cpu_status[cpu_id] = CPU_IDLE;}
}