
.macro  switch_el, xreg, el3_label, el2_label, el1_label
         mrs     \xreg, CurrentEL
         cmp     \xreg, 0xc
         b.eq    \el3_label
         cmp     \xreg, 0x8
         b.eq    \el2_label
         cmp     \xreg, 0x4
         b.eq    \el1_label
.endm

/*
 * Enter Exception.
 * This will save the processor state that is ELR/X0~X30
 * to the stack frame.
 */
.macro	exception_entry
	stp	x29, x30, [sp, #-16]!
	stp	x27, x28, [sp, #-16]!
	stp	x25, x26, [sp, #-16]!
	stp	x23, x24, [sp, #-16]!
	stp	x21, x22, [sp, #-16]!
	stp	x19, x20, [sp, #-16]!
	stp	x17, x18, [sp, #-16]!
	stp	x15, x16, [sp, #-16]!
	stp	x13, x14, [sp, #-16]!
	stp	x11, x12, [sp, #-16]!
	stp	x9, x10, [sp, #-16]!
	stp	x7, x8, [sp, #-16]!
	stp	x5, x6, [sp, #-16]!
	stp	x3, x4, [sp, #-16]!
	stp	x1, x2, [sp, #-16]!

	/* Could be running at EL3/EL2/EL1 */
	switch_el x11, 3f, 2f, 1f
3:	mrs	x1, esr_el3
	mrs	x2, elr_el3
	b	0f
2:	mrs	x1, esr_el2
	mrs	x2, elr_el2
	b	0f
1:	mrs	x1, esr_el1
	mrs	x2, elr_el1
0:
	stp	x2, x0, [sp, #-16]!
	mov	x0, sp
.endm

/*
 * Exception vectors.
 */
	.align	11
	.globl	vectors
vectors:
	.align	7
	b	_do_bad_sync	/* Current EL Synchronous Thread */

	.align	7
	b	_do_bad_irq	/* Current EL IRQ Thread */

	.align	7
	b	_do_bad_fiq	/* Current EL FIQ Thread */

	.align	7
	b	_do_bad_error	/* Current EL Error Thread */

	.align	7
	b	_do_sync	/* Current EL Synchronous Handler */

	.align	7
	b	_do_irq		/* Current EL IRQ Handler */

	.align	7
	b	_do_fiq		/* Current EL FIQ Handler */

	.align	7
	b	_do_error	/* Current EL Error Handler */


_do_bad_sync:
	exception_entry
	mov	x1,#0x1
	b	whl
	b	exception_exit

_do_bad_irq:
	exception_entry
	mov	x1,#0x2
	b	whl
	b	exception_exit

_do_bad_fiq:
	exception_entry
	mov	x1,#0x3
	b	whl
	b	exception_exit

_do_bad_error:
	exception_entry
	mov	x1,#0x4
	b	whl
	b	exception_exit

_do_sync:
	exception_entry
	mov	x1,#0x5
	b	whl
	b	exception_exit

_do_irq:
	exception_entry
	mov	x1,#0x6
	mrs 	x0,s3_0_c12_c12_0 
	bl	clr_intr
	b	exception_exit

_do_fiq:
	exception_entry
	mov	x1,#0x7
	b	whl
	b	exception_exit

_do_error:
	exception_entry
	mov	x1,#0x8
	b	whl
	b	exception_exit
whl:
	b whl

exception_exit:
	ldp	x2, x0, [sp],#16
	switch_el x11, 3f, 2f, 1f
3:	msr	elr_el3, x2
	b	0f
2:	msr	elr_el2, x2
	b	0f
1:	msr	elr_el1, x2
0:
	ldp	x1, x2, [sp],#16
	ldp	x3, x4, [sp],#16
	ldp	x5, x6, [sp],#16
	ldp	x7, x8, [sp],#16
	ldp	x9, x10, [sp],#16
	ldp	x11, x12, [sp],#16
	ldp	x13, x14, [sp],#16
	ldp	x15, x16, [sp],#16
	ldp	x17, x18, [sp],#16
	ldp	x19, x20, [sp],#16
	ldp	x21, x22, [sp],#16
	ldp	x23, x24, [sp],#16
	ldp	x25, x26, [sp],#16
	ldp	x27, x28, [sp],#16
	ldp	x29, x30, [sp],#16
	eret

clr_intr:
	msr s3_0_c12_c12_1,x0
	msr s3_0_c12_c11_1,x0
	and x0,x0,#0xfff
	add x2,x0,#0x66

/*	mov x2,#0x66
*/	mov x0,#0x18200000
	str x2,[x0]
	ret

