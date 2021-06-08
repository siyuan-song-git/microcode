	.section .text.first
	.global _start
_start:
	mov x0,#0x10000
	lsl x0,x0,#0x10
	mov x1,#0x4100
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
	mov x0,#0x10000
	lsl x0,x0,#0x10
	mov x1,#0x4100
	lsl x1,x1,#0x10
	add x0,x1,x0		/* x0=0x141000000 */

	mov x1,#0x1002
	add x1,x1,x0		/* x1=0x141001002 */
	str x1,[x0]

	mov x2,#0x28
	add x2,x2,x0		/* x2=0x141000028 */

	mov x3,#0x1001
	add x3,x3,x1		/* x3=0x141002003 */
	str x3,[x2]

set_tbl1:
	mov x4,#0x2000
	add x4,x4,x0		/* x4=0x141002000 */

	mov x5,#0x10000
	lsl x5,x5,#0x10
	mov x6,#0x40000000
	add x5,x5,x6
	mov x6,#0x411
	add x5,x5,x6		/* x5=0x140000411 */
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
	mov x6,#0x100000000
	mov x7,#0x41000000
	add x6,x6,x7		/* x6=0x141000000 */
	mov x7,x6
	add x7,x7,#0x1000
	add x7,x7,#0x3		/* x7=0x141001003 */
	str x7,[x6]

	mov x6,#0x100000000
	mov x7,#0x41000000
	add x6,x6,x7
	add x6,x6,#0x1000	/* x6=0x14101000 */
	mov x7,#0x411		/* x7=0x411 */
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

mmu_enable:
	msr ttbr0_el3,x0 
	mov x0,#0x80810000
	mov x1,#0x301c
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
	
/*	mov x0,#0x10000
	lsl x0,x0,#0x10
	mov x1,#0x40100000
	add x0,x0,x1
*/
	mov x0,#0x100
	mov x1,#0x1234
	str x1,[x0]
	
www:	b www

	
