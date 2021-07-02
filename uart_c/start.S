#include "startS.h"

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

	bl gic_init_secure
	bl gic_init_secure_percpu

	msr daifclr, #0x3
	adr x0, _stack_top
	ldr x0, [x0]
	mov sp, x0
	bl	main


gic_init_secure:
	ldr x0,=GICD_BASE
	/*
	 * Initialize Distributor
	 * x0: Distributor Base
	 */
	mov	w9, #0x37		/* EnableGrp0 | EnableGrp1NS */
					/* EnableGrp1S | ARE_S | ARE_NS */
	str	w9, [x0, GICD_CTLR]	/* Secure GICD_CTLR */
	ldr	w9, [x0, GICD_TYPER]
	and	w10, w9, #0x1f		/* ITLinesNumber */
	cbz	w10, 6f			/* No SPIs */
	add	x11, x0, (GICD_IGROUPRn + 4)
	add	x12, x0, (GICD_IGROUPMODRn + 4)
	mov	w9, #~0
5:	str	w9, [x11], #0x4
	str	wzr, [x12], #0x4	/* Config SPIs as Group1NS */
	sub	w10, w10, #0x1
	cbnz	w10, 5b
6:
	ret

gic_init_secure_percpu:
	/*
	 * Initialize ReDistributor
	 * x0: ReDistributor Base
	 */
	ldr 	x0,=GICR_BASE
	mrs	x10, mpidr_el1
	lsr	x9, x10, #32
	bfi	x10, x9, #24, #8	/* w10 is aff3:aff2:aff1:aff0 */
	lsr	x10, x10, #0x8
	mov	x9, x0
7:	ldr	x11, [x9, GICR_TYPER]
	lsr	x11, x11, #32		/* w11 is aff3:aff2:aff1:aff0 */
	cmp	w10, w11
	b.eq	8f
	add	x9, x9, #(2 << 16)
	b	7b

	/* x9: ReDistributor Base Address of Current CPU */
8:	mov	w10, #0x0
	str	w10, [x9, GICR_PWRR]
	nop
	nop
	nop
	mov	w10, #~0x2
	ldr	w11, [x9, GICR_WAKER]
	and	w11, w11, w10		/* Clear ProcessorSleep */
	str	w11, [x9, GICR_WAKER]
	dsb	st
	isb
9:	ldr	w10, [x9, GICR_WAKER]
	tbnz	w10, #2, 9b		/* Wait Children be Alive */
	add	x10, x9, #(1 << 16)	/* SGI_Base */
	mov	w11, #~0
	str	w11, [x10, GICR_IGROUPRn]
	str	wzr, [x10, GICR_IGROUPMODRn]	/* SGIs|PPIs Group1NS */
	mov	w11, #0x1		/* Enable SGI 0 */
	str	w11, [x10, GICR_ISENABLERn]
	/* Initialize Cpu Interface */
	mrs	x10, ICC_SRE_EL3
	orr	x10, x10, #0xf		/* SRE & Disable IRQ/FIQ Bypass & */
					/* Allow EL2 access to ICC_SRE_EL2 */
	msr	ICC_SRE_EL3, x10
	isb
	mrs	x10, ICC_SRE_EL2
	orr	x10, x10, #0xf		/* SRE & Disable IRQ/FIQ Bypass & */
					/* Allow EL1 access to ICC_SRE_EL1 */
	msr	ICC_SRE_EL2, x10
	isb
	mov	x10, #0x3		/* EnableGrp1NS | EnableGrp1S */
	msr	ICC_IGRPEN1_EL3, x10
	isb
	msr	ICC_CTLR_EL3, xzr
	isb
	msr	ICC_CTLR_EL1, xzr	/* NonSecure ICC_CTLR_EL1 */
	isb
	mov	x10, #0x1 << 7		/* Non-Secure access to ICC_PMR_EL1 */
	msr	ICC_PMR_EL1, x10
	isb
	ret

	.global request_irq
request_irq:
	ldr x1,=GICD_BASE
	add x1,x1,#0x100
	lsr x2,x0,#0x5
	lsl x2,x2,#0x2
	add x1,x1,x2
	and x2,x0,#0x1f
	mov x3,#0x1
	lsl x2,x3,x2
	str w2,[x1]

	ldr x1,=GICD_BASE
	add x1,x1,#0x6000
	lsl x2,x0,#0x3
	add x1,x1,x2
	mov x3,#0x0
	str x3,[x1]
	isb
	ret

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
