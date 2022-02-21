
#define CLUSTERPARTCR_EL1		s3_0_c15_c4_3
#define CLUSTERACPSID_EL1		s3_0_c15_c4_1
#define CLUSTERSTASHSID_EL1		s3_0_c15_c4_2
#define CLUSTERTHREADSIDOVR_EL1		s3_0_c15_c4_7
#define CLUSTERTHREADSID_EL1 		s3_0_c15_c4_0
#define CPU1_WRITE_ADDR		0x440040000

	.global secondary_sart
secondary_sart:
/*	b secondary_sart */
	mrs	x10,CLUSTERTHREADSID_EL1
	mov	x11,#0x2
	orr	x10,x10,x11
	msr	CLUSTERTHREADSID_EL1,x10

	mov x0,#0x41000000
	bl mmu_enable
whil1:	b whil1
	ldr x0,=CPU1_WRITE_ADDR
	mov x1,#0x4000
	mov x2,#0x5a5a
	mov x3,x2
	lsl x3,x3,#0x10
	add x2,x2,x3
	lsl x3,x3,#0x10
	add x2,x2,x3
	lsl x3,x3,#0x10
	add x2,x2,x3

lp1:	str x2, [x0]
	sub x1,x1,#0x8
	add x0,x0,#0x8
	cmp x1,#0x0
	bne lp1

	.global third_start
third_start:
	b third_start





	.global mmu_start
mmu_start:
	mov x0,#0x0
/*	mov x0,#0x10000
	lsl x0,x0,#0x10
*/	mov x1,#0x4100
	lsl x1,x1,#0x10
	add x0,x1,x0
	mov x1,#0x0
	mov x2,#0x3000
	lsr x3,x2,#0x3
cloop:
	str x1,[x0]
	add x0,x0,#0x8
	sub x3,x3,#0x1
	cmp x3,#0
	bne cloop

set_tbl0:
	mov x0,#0x0
/*	mov x0,#0x10000
	lsl x0,x0,#0x10
*/	mov x1,#0x4100
	lsl x1,x1,#0x10
	add x0,x1,x0		/* x0=0x41000000 */

	mov x1,#0x1002
	add x1,x1,x0		/* x1=0x41001003 */
	str x1,[x0]

	mov x2,#0x08
	add x2,x2,x0		/* x2=0x41000008 */

	mov x3,#0x1001
	add x3,x3,x1		/* x3=0x41002003 */
	str x3,[x2]

set_tbl1:
	mov x4,#0x2000
	add x4,x4,x0		/* x4=0x41002000 */

	mov x5,#0x0
/*	mov x5,#0x10000
	lsl x5,x5,#0x10
*/	mov x6,#0x40000000
	add x5,x5,x6
	mov x6,#0x411
	add x5,x5,x6		/* x5=0x40000411 */
	mov x6,#0x200000
	mov x7,#0x400
	lsr x8,x7,#0x3
tl1_lp:
	str x5,[x4]
	add x4,x4,#0x8
	add x5,x5,x6
	sub x8,x8,#0x1
	cmp x8,#0
	bne tl1_lp
set_tbl2:
	mov x6,#0x0
/*	mov x6,#0x100000000
*/	mov x7,#0x41000000
	add x6,x6,x7		/* x6=0x41000000 */
	mov x7,x6
	add x7,x7,#0x1000
	add x7,x7,#0x3		/* x7=0x41001003 */
	str x7,[x6]

	mov x6,#0x0
/*	mov x6,#0x100000000
*/	mov x7,#0x41000000
	add x6,x6,x7
	add x6,x6,#0x1000	/* x6=0x4101000 */
	mov x7,#0x401		/* x7=0x411 */
	mov x8,#0x200000
	mov x9,#0x1000
	lsr x10,x9,#0x3
tl2_lp:
	str x7,[x6]
	add x6,x6,#0x8
	add x7,x7,x8
	sub x10,x10,#0x1
	cmp x10,#0
	bne tl2_lp
.global mmu_enable
mmu_enable:
    ic	ialluis         /* invalidate all dcache */
	isb	sy
	msr ttbr0_el3,x0 
	mov x0,#0x80800000
	mov x1,#0x3020
	add x0,x0,x1
	msr tcr_el3,x0 
	mov x0,#0xff44
	lsl x0,x0,#0x18
	mov x1,#0xc040
	lsl x1,x1,#0x4
	add x0,x0,x1
	msr mair_el3,x0
	mov x0,#0xc518
	lsl x0,x0,#0x8
	add x0,x0,#0x3d
	msr sctlr_el3,x0 
	ret
/* www:	b www */
