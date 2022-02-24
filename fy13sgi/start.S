#include "elx.h"
.macro	switch_el, xreg, el3_label, el2_label, el1_label
	 mrs	 \xreg, CurrentEL
	 cmp	 \xreg, 0xc
	 b.eq	 \el3_label
	 cmp	 \xreg, 0x8
	 b.eq	 \el2_label
	 cmp	 \xreg, 0x4
	 b.eq	 \el1_label
.endm

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

	msr daifclr, #0x3
	adr x0, _stack_top
	mov sp, x0
	bl	main
_master_cpu_end:
	b _master_cpu_end

	.global armv8_switch_to_el1
armv8_switch_to_el1:
	adr	x4, lowlevel_in_el2
	mov	x5,#0x1
	armv8_switch_to_el2_m x4, x5, x6
lowlevel_in_el2:
	adr	x4, lowlevel_in_el1
	mov	x5,#0x1
	armv8_switch_to_el1_m x4, x5, x6
lowlevel_in_el1:
	msr daifclr, #0x3 
	ret

	.align 8
	.global _secondary_cpu_start
_secondary_cpu_start:
	adr	x0, vectors
	switch_el x1, 13f, 12f, 11f
13:	msr	vbar_el3, x0
	mrs	x0, scr_el3
	orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr	scr_el3, x0
	msr	cptr_el3, xzr			/* Enable FP/SIMD */
	b	10f
12:	msr	vbar_el2, x0
	mov	x0, #0x33ff
	msr	cptr_el2, x0			/* Enable FP/SIMD */
	b	10f
11:	msr	vbar_el1, x0
	mov	x0, #3 << 20
	msr	cpacr_el1, x0			/* Enable FP/SIMD */
10:

	mrs x0, mpidr_el1
	mov x1,#0xffff
	and x1,x0,x1
	lsl x1,x1,#0x4
	adr x0, _stack_top
	sub x0,x0,x1
	mov sp, x0
	bl secondary_cpu_entry
slave_cpu_end:
	b slave_cpu_end

	.align 8
	.global _b_start
_b_start:
	.quad _bss_start

	.align 8
	.global _b_end
_b_end:
	.quad _bss_end

	.align 12
	.global _stack_top
	.fill 32768,1,0xff
_stack_top:


