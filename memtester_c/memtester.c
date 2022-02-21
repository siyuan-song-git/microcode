//#include <arch-api.h>
#include "mem_types.h"

//#include <string.h>

extern void mprintf(char *line, ...);

static unsigned long rand(void)
{
	static unsigned long uirandom[] = {
		#include "random_data.h"
	};
	static unsigned long sz = sizeof(uirandom)/sizeof(uirandom[0]);
	static unsigned long it = 0,mod;
	mod = sz & it;
	it++;
	return uirandom[mod];
}

unsigned long compare_regions(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long p3,p4;
	unsigned long i,res = 0;

	for(i = 0; i < count; i++, p1++, p2++) {
		p3 = *p1;
		p4 = *p2;
		if (p3 != p4) {
			mprintf("fail val *p1:%x,*p2:%x\r\n",p3,p4);
			mprintf("fail add  p1:%x, p2:%x\r\n",p1,p2);
			res = -1;
		}
	}

	return res;
}

unsigned long test_stuck_address(unsigned long *bufa, unsigned long count)
{   
    unsigned long *p1 = bufa;
    unsigned long j;
    long i;
    for(j = 0; j < 2; j++){
        p1 = (unsigned long *)bufa;
        for(i = 0; i < count; i++){
            *p1 = ((j + i) % 2) == 0 ? (unsigned long)p1 : (~(unsigned long)p1);
            p1++;
        }
        p1 = (unsigned long *)bufa;
        for(i = 0; i < count; i++, p1++){
            if (*p1 != (((j + i) % 2) == 0 ? (unsigned long) p1 : ~((unsigned long) p1))){
            if(((j + i) % 2) == 0)
                return (((unsigned long) p1)^(*p1));
            else
                return ((~((unsigned long) p1))^(*p1));
            }
        }
    }
    return 0;
}

unsigned long test_random_value(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i;

	for (i = 0; i < count; i++) {
		*p1++ = *p2++ = rand();
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_xor_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}

	return compare_regions(bufa, bufb, count);
}
#if 1
unsigned long test_sub_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_mul_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ *= q;
		*p2++ *= q;
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_div_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++, p1++, p2++) {
		if(!q)
			q++;
		//*p1 = rt_udiv(q, *p1, &mod);
		//*p2 = rt_udiv(q, *p2, &mod);
		*p1 /= q;
		*p2 /= q;
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_or_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ |= q;
		*p2++ |= q;
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_and_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ &= q;
		*p2++ &= q;
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_seqinc_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,q = rand();

	for (i = 0; i < count; i++) {
		*p1++ = *p2++ = (i + q);
	}

	return compare_regions(bufa, bufb, count);
}

unsigned long test_solidbits_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j,q;

	for (j = 0; j < 64; j++) {
		q = (j & 0x1) == 0 ? UL_ONEBITS : 0;
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i & 0x1) == 0 ? q : ~q;
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}

	return 0;
}

unsigned long test_checkerboard_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j,q;

	for (j = 0; j < 64; j++) {
		q = (j & 0x1) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i & 0x1) == 0 ? q : ~q;
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}

	return 0;
}

unsigned long test_blockseq_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j;

	for (j = 0; j < 256; j++) {
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (unsigned long)UL_BYTE(j);
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}

	return 0;
}

unsigned long test_walkbits0_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j;

	for (j = 0; j < (UL_LEN << 1); j++) {
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = ONE << j;
			} else { /* Walk it back down */
				*p1++ = *p2++ = ONE << ((UL_LEN << 1) - j - 1);
			}
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}
	return 0;
}

unsigned long test_walkbits1_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j;

	for (j = 0; j < (UL_LEN << 1); j++) {
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = UL_ONEBITS ^ (ONE << j);
			} else { /* Walk it back down */
				*p1++ = *p2++ = UL_ONEBITS ^ (ONE << ((UL_LEN << 1) - j - 1));
			}
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}
	return 0;
}

unsigned long test_bitspread_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j;

	for (j = 0; j < (UL_LEN << 1); j++) {
		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			if (j < UL_LEN) { /* Walk it up. */
				*p1++ = *p2++ = ((i & 0x1) == 0)
					? (ONE << j) | (ONE << (j + 2))
					: UL_ONEBITS ^ ((ONE << j)
					                | (ONE << (j + 2)));
			} else { /* Walk it back down */
				*p1++ = *p2++ = ((i & 0x1) == 0)
					? (ONE << ((UL_LEN << 1) - 1 - j)) | (ONE << ((UL_LEN << 1) + 1 -j))
					: UL_ONEBITS ^ (ONE << ((UL_LEN << 1) - 1 - j) 
					                | (ONE << ((UL_LEN << 1) + 1 - j)));
			}
		}

		if(compare_regions(bufa, bufb, count))
			return -1;
	}
	return 0;
}

unsigned long test_bitflip_comparison(unsigned long *bufa, unsigned long *bufb, unsigned long count)
{
	unsigned long *p1 = bufa;
	unsigned long *p2 = bufb;
	unsigned long i,j,k,q;

	for (k = 0; k < UL_LEN; k++) {
		q = ONE << k;
		for (j=0; j < 8; j++) {
			q = ~q;
			p1 = bufa;
			p2 = bufb;
			for (i = 0; i < count; i++)
				*p1++ = *p2++ = (i & 0x1) == 0 ? q : ~q;

			if (compare_regions(bufa, bufb, count))
				return -1;
		}
	}
	return 0;
}
#endif