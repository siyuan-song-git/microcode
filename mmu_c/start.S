.macro	switch_el, xreg, el3_label, el2_label, el1_label
	 mrs	 \xreg, CurrentEL
	 cmp	 \xreg, 0xc
	 b.eq	 \el3_label
	 cmp	 \xreg, 0x8
	 b.eq	 \el2_label
	 cmp	 \xreg, 0x4
	 b.eq	 \el1_label
.endm

#define CLUSTERPARTCR_EL1		s3_0_c15_c4_3
#define CLUSTERACPSID_EL1		s3_0_c15_c4_1
#define CLUSTERSTASHSID_EL1		s3_0_c15_c4_2
#define CLUSTERTHREADSIDOVR_EL1	s3_0_c15_c4_7


	.extern vectors
	.extern __bss_start
	.extern __bss_end
	.section .text.first
	.global _start
_start:
	adr	x0, vectors
	switch_el x1, 3f, 2f, 1f
3:	msr	vbar_el3, x0
	mrs	x0, scr_el3
	orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr	scr_el3, x0
	msr	cptr_el3, xzr			/* Enable FP/SIMD */
	b	0f
2:	msr	vbar_el2, x0
	mov	x0, #0x33ff
	msr	cptr_el2, x0			/* Enable FP/SIMD */
	b	0f
1:	msr	vbar_el1, x0
	mov	x0, #3 << 20
	msr	cpacr_el1, x0			/* Enable FP/SIMD */
0:

	adr x0, _b_start
	ldr x0, [x0]
	adr x1, _b_end
	ldr x1, [x1]
	mov x2, #0x0
clbss_l:
	str x2, [x0]
	add x0, x0, #8
	cmp x0, x1
	ble clbss_l

	mrs	x10,ACTLR_EL3
	mov	x11,#0x3
	lsl	x11,x11,#0xa
	orr	x10,x10,x11
	msr	ACTLR_EL3,x10

	mrs	x10,CLUSTERPARTCR_EL1
	mov	x11,#0x8601
	orr	x10,x10,x11
	msr	CLUSTERPARTCR_EL1,x10

	mrs	x10,CLUSTERACPSID_EL1
	mov	x11,#0x0
	orr	x10,x10,x11
	msr	CLUSTERACPSID_EL1,x10

	mrs	x10,CLUSTERSTASHSID_EL1
	mov	x11,#0x0
	orr	x10,x10,x11
	msr	CLUSTERSTASHSID_EL1,x10

	mrs	x10,CLUSTERTHREADSIDOVR_EL1
	mov	x11,#0x60000
	add	x11,x11,#0x2
	orr	x10,x10,x11
	msr	CLUSTERTHREADSIDOVR_EL1,x10


	msr daifclr, #0x3
	adr x0, _stack_top
	ldr x0, [x0]
	mov sp, x0
	bl	main


.align 8
	.global _b_start
_b_start:
	.quad _bss_start

	.align 8
	.global _b_end
_b_end:
	.quad _bss_end


	.align 8
	.global _stack_top
_stack_top:
	.quad __stack_top

	.align 8
	.fill 4096,1,0xff
__stack_top:
