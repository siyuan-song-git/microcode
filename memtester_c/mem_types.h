#ifndef MEM_TYPES_H_
#define MEM_TYPES_H_

#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define CHECKERBOARD1 0x55555555
#define CHECKERBOARD2 0xaaaaaaaa
#define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))
#define ONE 0x00000001L

struct mem_test {
	char *name;
	unsigned long (*fp)(unsigned long*, unsigned long*, unsigned long);
};

struct mem_info {
	unsigned long magic_start;
	unsigned long mem_base;
	unsigned long mem_len;
	unsigned long magic_end;
};

unsigned long test_stuck_address(unsigned long *bufa, unsigned long count);
unsigned long test_random_value(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_xor_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
#if 1
unsigned long test_sub_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_mul_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_div_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_or_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_and_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_seqinc_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_solidbits_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_checkerboard_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_blockseq_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_walkbits0_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_walkbits1_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_bitspread_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
unsigned long test_bitflip_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count);
#endif
#endif