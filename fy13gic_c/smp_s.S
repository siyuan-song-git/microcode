lr .req x30

#define GICR_BASE		0x1080000

/* ReDistributor Registers for Control and Physical LPIs */
#define GICR_CTLR		0x0000
#define GICR_IIDR		0x0004
#define GICR_TYPER		0x0008
#define GICR_STATUSR		0x0010
#define GICR_WAKER		0x0014
#define GICR_PWRR		0x0024
#define GICR_SETLPIR		0x0040
#define GICR_CLRLPIR		0x0048
#define GICR_SEIR		0x0068
#define GICR_PROPBASER		0x0070
#define GICR_PENDBASER		0x0078
#define GICR_INVLPIR		0x00a0
#define GICR_INVALLR		0x00b0
#define GICR_SYNCR		0x00c0
#define GICR_MOVLPIR		0x0100
#define GICR_MOVALLR		0x0110

/* ReDistributor Registers for SGIs and PPIs */
#define GICR_IGROUPRn		0x0080
#define GICR_ISENABLERn		0x0100
#define GICR_ICENABLERn		0x0180
#define GICR_ISPENDRn		0x0200
#define GICR_ICPENDRn		0x0280
#define GICR_ISACTIVERn		0x0300
#define GICR_ICACTIVERn		0x0380
#define GICR_IPRIORITYRn	0x0400
#define GICR_ICFGR0		0x0c00
#define GICR_ICFGR1		0x0c04
#define GICR_IGROUPMODRn	0x0d00
#define GICR_NSACRn		0x0e00

/* Cpu Interface System Registers */
#define ICC_IAR0_EL1		S3_0_C12_C8_0
#define ICC_IAR1_EL1		S3_0_C12_C12_0
#define ICC_EOIR0_EL1		S3_0_C12_C8_1
#define ICC_EOIR1_EL1		S3_0_C12_C12_1
#define ICC_HPPIR0_EL1		S3_0_C12_C8_2
#define ICC_HPPIR1_EL1		S3_0_C12_C12_2
#define ICC_BPR0_EL1		S3_0_C12_C8_3
#define ICC_BPR1_EL1		S3_0_C12_C12_3
#define ICC_DIR_EL1		S3_0_C12_C11_1
#define ICC_PMR_EL1		S3_0_C4_C6_0
#define ICC_RPR_EL1		S3_0_C12_C11_3
#define ICC_CTLR_EL1		S3_0_C12_C12_4
#define ICC_CTLR_EL3		S3_6_C12_C12_4
#define ICC_SRE_EL1		S3_0_C12_C12_5
#define ICC_SRE_EL2		S3_4_C12_C9_5
#define ICC_SRE_EL3		S3_6_C12_C12_5
#define ICC_IGRPEN0_EL1		S3_0_C12_C12_6
#define ICC_IGRPEN1_EL1		S3_0_C12_C12_7
#define ICC_IGRPEN1_EL3		S3_6_C12_C12_7
#define ICC_SEIEN_EL1		S3_0_C12_C13_0
#define ICC_SGI0R_EL1		S3_0_C12_C11_7
#define ICC_SGI1R_EL1		S3_0_C12_C11_5
#define ICC_ASGI1R_EL1		S3_0_C12_C11_6

/*
 * Branch according to exception level
 */
.macro  switch_el, xreg, el3_label, el2_label, el1_label
        mrs     \xreg, CurrentEL
        cmp     \xreg, 0xc
        b.eq    \el3_label
        cmp     \xreg, 0x8
        b.eq    \el2_label
        cmp     \xreg, 0x4
        b.eq    \el1_label
.endm

	.extern vectors
	.global _secondary_start
_secondary_start:
	b	secondary_reset

secondary_reset:
	/*
	 * Could be EL3/EL2/EL1, Initial State:
	 * Little Endian, MMU Disabled, i/dCache Disabled
	 */
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

	bl		_gicr_init

	msr 	daifclr, #0x3
	adr 	x0, __secondary_stack_top
	mrs	x1, mpidr_el1
	lsr	x1,x1,#8
	and 	x1,x1,#0xff
	sub 	x1,x1,#0x1
	lsl	x1,x1,#12
	sub	x0,x0,x1
	mov 	sp, x0
	bl	secondary_entry
whl:
	b whl

_gicr_init:
	ldr	x9, =GICR_BASE
	mrs	x1, mpidr_el1
	lsr	x1,x1,#8
	and x1,x1,#0xff
	lsl x1,x1,#17
	add x9,x9,x1

	/* x9: ReDistributor Base Address of Current CPU */
2:	mov 	w10, #0x0
	str 	w10, [x9, GICR_PWRR]
	nop
	nop
	nop
	mov	w10, #~0x2
	ldr	w11, [x9, GICR_WAKER]
	and	w11, w11, w10		/* Clear ProcessorSleep */
	str	w11, [x9, GICR_WAKER]
	dsb	st
	isb
3:	ldr	w10, [x9, GICR_WAKER]
	tbnz	w10, #2, 3b		/* Wait Children be Alive */

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


	.align 12
	.global __secondary_stack_top
	.fill 28672,1,0xff			/* 7*4096 */
__secondary_stack_top:
