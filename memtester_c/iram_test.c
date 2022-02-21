#include "mem_types.h"
//#include "mmap.h"
//#include <string.h>

struct mem_test memtests[] = {
	{"Random Value", test_random_value},
	{"Compare XOR", test_xor_comparison},
#if 1 
	{"Compare SUB", test_sub_comparison},
	{"Compare MUL", test_mul_comparison},
	{"Compare DIV", test_div_comparison},
	{"Compare OR", test_or_comparison},
	{"Compare AND", test_and_comparison},
	{"Sequential Increment", test_seqinc_comparison},
	{"Solid Bits", test_solidbits_comparison},
	{"Block Sequential", test_blockseq_comparison},
	{"Checkerboard", test_checkerboard_comparison},
	{"Bit Spread", test_bitspread_comparison},
	{"Bit Flip", test_bitspread_comparison},
	{"Walking Ones", test_walkbits1_comparison},
	{"Walking Zeroes", test_walkbits0_comparison},
#endif
	{0, 0}
};
extern void mprintf(char *line, ...);
struct mem_info meminfo = {
	.magic_start = 0x5a5a5a5a,
	.mem_base = 0x30800000,
	.mem_len  = 0x1000,
	.magic_end = 0x5a5a5a5a
};

void test_entry(unsigned long test_addr, unsigned long len)
{
#if 0
	unsigned long halflen = meminfo.mem_len / 2;
	unsigned long count = halflen / sizeof(unsigned long);
	unsigned long *bufa = (unsigned long*)meminfo.mem_base;
	unsigned long *bufb = (unsigned long*)(meminfo.mem_base + halflen);
	unsigned long i;
#else
	unsigned long halflen, count, *bufa, *bufb, i;
	//unsigned char *tmp;
	meminfo.mem_len = len;
	meminfo.mem_base = test_addr;

	halflen = meminfo.mem_len / 2;
	count = halflen / sizeof(unsigned long);
	bufa = (unsigned long*)meminfo.mem_base;
	bufb = (unsigned long*)(meminfo.mem_base + halflen);

#endif
	while(1){
#if 0
		if (test_stuck_address((unsigned long*)meminfo.mem_base, meminfo.mem_len/sizeof(unsigned long))) {
			mprintf("[IRAM][STUCK TEST][FAIL]\n");
			return;
		}
#endif
		for (i=0;;i++) {
			if (!memtests[i].name) break;

			if (memtests[i].fp(bufa, bufb, count)) {
				//mprintf("%s fail\r\n",memtests[i].name);
				mprintf("[IRAM][%s][FAIL]\n",memtests[i].name);
				break;
			}
			//mprintf("[IRAM][%s][PASS]\n",memtests[i].name);
		}
		//mprintf("[IRAM][%x][PASS]\n",(unsigned long)memtests[i].name);
		//tmp = memtests[i].name;
		
		//mprintf("iram auto test done\n");
	}
	return;

}
