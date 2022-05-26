#define GICD_BASE		0x1000000
#define GICR_BASE		0x1080000

/* Distributor Registers */
#define GICD_CTLR		0x0000
#define GICD_TYPER		0x0004
#define GICD_IIDR		0x0008
#define GICD_STATUSR		0x0010
#define GICD_SETSPI_NSR		0x0040
#define GICD_CLRSPI_NSR		0x0048
#define GICD_SETSPI_SR		0x0050
#define GICD_CLRSPI_SR		0x0058
#define GICD_SEIR		0x0068
#define GICD_IGROUPRn		0x0080
#define GICD_ISENABLERn		0x0100
#define GICD_ICENABLERn		0x0180
#define GICD_ISPENDRn		0x0200
#define GICD_ICPENDRn		0x0280
#define GICD_ISACTIVERn		0x0300
#define GICD_ICACTIVERn		0x0380
#define GICD_IPRIORITYRn	0x0400
#define GICD_ITARGETSRn		0x0800
#define GICD_ICFGR		0x0c00
#define GICD_IGROUPMODRn	0x0d00
#define GICD_NSACRn		0x0e00
#define GICD_SGIR		0x0f00
#define GICD_CPENDSGIRn		0x0f10
#define GICD_SPENDSGIRn		0x0f20
#define GICD_IROUTERn		0x6000

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

#define ES_TO_AARCH64		1
#define ES_TO_AARCH32		0

#define CPTR_EL2_RES1		(3 << 12 | 0x3ff)

/*
 * SCR_EL3 bits definitions
 */
#define SCR_EL3_RW_AARCH64	(1 << 10) /* Next lower level is AArch64     */
#define SCR_EL3_RW_AARCH32	(0 << 10) /* Lower lowers level are AArch32  */
#define SCR_EL3_HCE_EN		(1 << 8)  /* Hypervisor Call enable          */
#define SCR_EL3_SMD_DIS		(1 << 7)  /* Secure Monitor Call disable     */
#define SCR_EL3_RES1		(3 << 4)  /* Reserved, RES1                  */
#define SCR_EL3_NS_EN		(1 << 0)  /* EL0 and EL1 in Non-scure state  */

/*
 * SCTLR_EL2 bits definitions
 */
#define SCTLR_EL2_RES1		(3 << 28 | 3 << 22 | 1 << 18 | 1 << 16 |\
				 1 << 11 | 3 << 4)	    /* Reserved, RES1 */
#define SCTLR_EL2_EE_LE		(0 << 25) /* Exception Little-endian          */
#define SCTLR_EL2_WXN_DIS	(0 << 19) /* Write permission is not XN       */
#define SCTLR_EL2_ICACHE_DIS	(0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL2_SA_DIS	(0 << 3)  /* Stack Alignment Check disabled   */
#define SCTLR_EL2_DCACHE_DIS	(0 << 2)  /* Data cache disabled              */
#define SCTLR_EL2_ALIGN_DIS	(0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL2_MMU_DIS	(0)       /* MMU disabled                     */
/*
 * SPSR_EL3/SPSR_EL2 bits definitions
 */
#define SPSR_EL_END_LE		(0 << 9)  /* Exception Little-endian          */
#define SPSR_EL_DEBUG_MASK	(1 << 9)  /* Debug exception masked           */
#define SPSR_EL_ASYN_MASK	(1 << 8)  /* Asynchronous data abort masked   */
#define SPSR_EL_SERR_MASK	(1 << 8)  /* System Error exception masked    */
#define SPSR_EL_IRQ_MASK	(1 << 7)  /* IRQ exception masked             */
#define SPSR_EL_FIQ_MASK	(1 << 6)  /* FIQ exception masked             */
#define SPSR_EL_T_A32		(0 << 5)  /* AArch32 instruction set A32      */
#define SPSR_EL_M_AARCH64	(0 << 4)  /* Exception taken from AArch64     */
#define SPSR_EL_M_AARCH32	(1 << 4)  /* Exception taken from AArch32     */
#define SPSR_EL_M_SVC		(0x3)     /* Exception taken from SVC mode    */
#define SPSR_EL_M_HYP		(0xa)     /* Exception taken from HYP mode    */
#define SPSR_EL_M_EL1H		(5)       /* Exception taken from EL1h mode   */
#define SPSR_EL_M_EL2H		(9)       /* Exception taken from EL2h mode   */

/*
 * CPTR_EL2 bits definitions
 */
#define CPTR_EL2_RES1		(3 << 12 | 0x3ff)           /* Reserved, RES1 */

/*
 * SCTLR_EL2 bits definitions
 */
#define SCTLR_EL2_RES1		(3 << 28 | 3 << 22 | 1 << 18 | 1 << 16 |\
				 1 << 11 | 3 << 4)	    /* Reserved, RES1 */
#define SCTLR_EL2_EE_LE		(0 << 25) /* Exception Little-endian          */
#define SCTLR_EL2_WXN_DIS	(0 << 19) /* Write permission is not XN       */
#define SCTLR_EL2_ICACHE_DIS	(0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL2_SA_DIS	(0 << 3)  /* Stack Alignment Check disabled   */
#define SCTLR_EL2_DCACHE_DIS	(0 << 2)  /* Data cache disabled              */
#define SCTLR_EL2_ALIGN_DIS	(0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL2_MMU_DIS	(0)       /* MMU disabled                     */

/*
 * CNTHCTL_EL2 bits definitions
 */
#define CNTHCTL_EL2_EL1PCEN_EN	(1 << 1)  /* Physical timer regs accessible   */
#define CNTHCTL_EL2_EL1PCTEN_EN	(1 << 0)  /* Physical counter accessible      */

/*
 * HCR_EL2 bits definitions
 */
#define HCR_EL2_RW_AARCH64	(1 << 31) /* EL1 is AArch64                   */
#define HCR_EL2_RW_AARCH32	(0 << 31) /* Lower levels are AArch32         */
#define HCR_EL2_HCD_DIS		(1 << 29) /* Hypervisor Call disabled         */

/*
 * CPACR_EL1 bits definitions
 */
#define CPACR_EL1_FPEN_EN	(3 << 20) /* SIMD and FP instruction enabled  */

/*
 * SCTLR_EL1 bits definitions
 */
#define SCTLR_EL1_RES1		(3 << 28 | 3 << 22 | 1 << 20 |\
				 1 << 11) /* Reserved, RES1                   */
#define SCTLR_EL1_UCI_DIS	(0 << 26) /* Cache instruction disabled       */
#define SCTLR_EL1_EE_LE		(0 << 25) /* Exception Little-endian          */
#define SCTLR_EL1_WXN_DIS	(0 << 19) /* Write permission is not XN       */
#define SCTLR_EL1_NTWE_DIS	(0 << 18) /* WFE instruction disabled         */
#define SCTLR_EL1_NTWI_DIS	(0 << 16) /* WFI instruction disabled         */
#define SCTLR_EL1_UCT_DIS	(0 << 15) /* CTR_EL0 access disabled          */
#define SCTLR_EL1_DZE_DIS	(0 << 14) /* DC ZVA instruction disabled      */
#define SCTLR_EL1_ICACHE_DIS	(0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL1_UMA_DIS	(0 << 9)  /* User Mask Access disabled        */
#define SCTLR_EL1_SED_EN	(0 << 8)  /* SETEND instruction enabled       */
#define SCTLR_EL1_ITD_EN	(0 << 7)  /* IT instruction enabled           */
#define SCTLR_EL1_CP15BEN_DIS	(0 << 5)  /* CP15 barrier operation disabled  */
#define SCTLR_EL1_SA0_DIS	(0 << 4)  /* Stack Alignment EL0 disabled     */
#define SCTLR_EL1_SA_DIS	(0 << 3)  /* Stack Alignment EL1 disabled     */
#define SCTLR_EL1_DCACHE_DIS	(0 << 2)  /* Data cache disabled              */
#define SCTLR_EL1_ALIGN_DIS	(0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL1_MMU_DIS	(0)       /* MMU disabled                     */


.macro armv8_switch_to_el2_m, ep, flag, tmp
	msr	cptr_el3, xzr		/* Disable coprocessor traps to EL3 */
	mov	\tmp, #CPTR_EL2_RES1
	msr	cptr_el2, \tmp		/* Disable coprocessor traps to EL2 */

	/* Initialize Generic Timers */
	msr	cntvoff_el2, xzr

	/* Initialize SCTLR_EL2
	 *
	 * setting RES1 bits (29,28,23,22,18,16,11,5,4) to 1
	 * and RES0 bits (31,30,27,26,24,21,20,17,15-13,10-6) +
	 * EE,WXN,I,SA,C,A,M to 0
	 */
	ldr	\tmp, =(SCTLR_EL2_RES1 | SCTLR_EL2_EE_LE |\
			SCTLR_EL2_WXN_DIS | SCTLR_EL2_ICACHE_DIS |\
			SCTLR_EL2_SA_DIS | SCTLR_EL2_DCACHE_DIS |\
			SCTLR_EL2_ALIGN_DIS | SCTLR_EL2_MMU_DIS)
	msr	sctlr_el2, \tmp

	mov	\tmp, sp
	msr	sp_el2, \tmp		/* Migrate SP */
	mrs	\tmp, vbar_el3
	msr	vbar_el2, \tmp		/* Migrate VBAR */

	/*
	 * The next lower exception level is AArch64, 64bit EL2 | HCE |
	 * RES1 (Bits[5:4]) | Non-secure EL0/EL1.
	 * and the SMD depends on requirements.
	 */

	ldr	\tmp, =(SCR_EL3_RW_AARCH64 | SCR_EL3_HCE_EN |\
			SCR_EL3_SMD_DIS | SCR_EL3_RES1 |\
			SCR_EL3_NS_EN)
	msr	scr_el3, \tmp

	/* Return to the EL2_SP2 mode from EL3 */
	ldr	\tmp, =(SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_M_AARCH64 | SPSR_EL_M_EL2H)
	msr	spsr_el3, \tmp
	msr	elr_el3, \ep
	eret

.endm

.macro armv8_switch_to_el1_m, ep, flag, tmp
	/* Initialize Generic Timers */
	mrs	\tmp, cnthctl_el2
	/* Enable EL1 access to timers */
	orr	\tmp, \tmp, #(CNTHCTL_EL2_EL1PCEN_EN |\
		CNTHCTL_EL2_EL1PCTEN_EN)
	msr	cnthctl_el2, \tmp
	msr	cntvoff_el2, xzr

	/* Initilize MPID/MPIDR registers */
	mrs	\tmp, midr_el1
	msr	vpidr_el2, \tmp
	mrs	\tmp, mpidr_el1
	msr	vmpidr_el2, \tmp

	/* Disable coprocessor traps */
	mov	\tmp, #CPTR_EL2_RES1
	msr	cptr_el2, \tmp		/* Disable coprocessor traps to EL2 */
	msr	hstr_el2, xzr		/* Disable coprocessor traps to EL2 */
	mov	\tmp, #CPACR_EL1_FPEN_EN
	msr	cpacr_el1, \tmp		/* Enable FP/SIMD at EL1 */

	/* SCTLR_EL1 initialization
	 *
	 * setting RES1 bits (29,28,23,22,20,11) to 1
	 * and RES0 bits (31,30,27,21,17,13,10,6) +
	 * UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	 * CP15BEN,SA0,SA,C,A,M to 0
	 */
	ldr	\tmp, =(SCTLR_EL1_RES1 | SCTLR_EL1_UCI_DIS |\
			SCTLR_EL1_EE_LE | SCTLR_EL1_WXN_DIS |\
			SCTLR_EL1_NTWE_DIS | SCTLR_EL1_NTWI_DIS |\
			SCTLR_EL1_UCT_DIS | SCTLR_EL1_DZE_DIS |\
			SCTLR_EL1_ICACHE_DIS | SCTLR_EL1_UMA_DIS |\
			SCTLR_EL1_SED_EN | SCTLR_EL1_ITD_EN |\
			SCTLR_EL1_CP15BEN_DIS | SCTLR_EL1_SA0_DIS |\
			SCTLR_EL1_SA_DIS | SCTLR_EL1_DCACHE_DIS |\
			SCTLR_EL1_ALIGN_DIS | SCTLR_EL1_MMU_DIS)
	msr	sctlr_el1, \tmp

	mov	\tmp, sp
	msr	sp_el1, \tmp		/* Migrate SP */
	mrs	\tmp, vbar_el2
	msr	vbar_el1, \tmp		/* Migrate VBAR */

	/* Check switch to AArch64 EL1 or AArch32 Supervisor mode */
	cmp	\flag, #ES_TO_AARCH32
	b.eq	31f

	/* Initialize HCR_EL2 */
	ldr	\tmp, =(HCR_EL2_RW_AARCH64 | HCR_EL2_HCD_DIS)
	msr	hcr_el2, \tmp

	/* Return to the EL1_SP1 mode from EL2 */
	ldr	\tmp, =(SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_M_AARCH64 | SPSR_EL_M_EL1H)
	msr	spsr_el2, \tmp
	msr     elr_el2, \ep
	eret

31:
	/* Initialize HCR_EL2 */
	ldr	\tmp, =(HCR_EL2_RW_AARCH32 | HCR_EL2_HCD_DIS)
	msr	hcr_el2, \tmp

	/* Return to AArch32 Supervisor mode from EL2 */
	ldr	\tmp, =(SPSR_EL_END_LE | SPSR_EL_ASYN_MASK |\
			SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_T_A32 | SPSR_EL_M_AARCH32 |\
			SPSR_EL_M_SVC)
	msr     spsr_el2, \tmp
	msr     elr_el2, \ep
	msr daifclr, #0xF
	eret
.endm



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

	ldr	x0, =GICD_BASE
	bl	gic_init_secure

	ldr	x0, =GICR_BASE
	bl	gic_init_secure_percpu

	bl	armv8_switch_to_el1

	msr daifclr, #0x3
	adr x0, _stack_top
	ldr x0, [x0]
	mov sp, x0
	bl	main

	.align 4
	.global gic_init_secure
gic_init_secure:
	/*
	 * Initialize Distributor
	 * x0: Distributor Base
	 */
	mov	w9, #0x37		/* EnableGrp0 | EnableGrp1NS */
					/* EnableGrp1S | ARE_S | ARE_NS */
	str	w9, [x0, GICD_CTLR]	/* Secure GICD_CTLR */
	ldr	w9, [x0, GICD_TYPER]
	and	w10, w9, #0x1f		/* ITLinesNumber */
	cbz	w10, 11f			/* No SPIs */
	add	x11, x0, (GICD_IGROUPRn + 4)
	add	x12, x0, (GICD_IGROUPMODRn + 4)
	mov	w9, #~0
10:	str	w9, [x11], #0x4
	str	wzr, [x12], #0x4	/* Config SPIs as Group1NS */
	sub	w10, w10, #0x1
	cbnz	w10, 10b
11:
	ret

/*************************************************************************
 * For Gicv2:
 * void gic_init_secure_percpu(DistributorBase, CpuInterfaceBase);
 * For Gicv3:
 * void gic_init_secure_percpu(ReDistributorBase);
 *
 * Initialize secure copy of GIC at EL3.
 *
 *************************************************************************/
 	.align 4
	.global gic_init_secure_percpu
gic_init_secure_percpu:
	/*
	 * Initialize ReDistributor
	 * x0: ReDistributor Base
	 */
	mrs	x10, mpidr_el1
	lsr	x9, x10, #32
	bfi	x10, x9, #24, #8	/* w10 is aff3:aff2:aff1:aff0 */
	lsr	x10, x10, #0x8
	mov	x9, x0
21:	ldr	x11, [x9, GICR_TYPER]
	lsr	x11, x11, #32		/* w11 is aff3:aff2:aff1:aff0 */
	cmp	w10, w11
	b.eq	22f
	add	x9, x9, #(2 << 16)
	b	21b

	/* x9: ReDistributor Base Address of Current CPU */
22:	mov 	w10, #0x0
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
23:	ldr	w10, [x9, GICR_WAKER]
	tbnz	w10, #2, 23b		/* Wait Children be Alive */

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


 	.align 4
	.global armv8_switch_to_el1
armv8_switch_to_el1:
	/*
	 * All cpu will enter EL2 and optionally EL1.
	 */
	adr	x4, lowlevel_in_el2
	ldr	x5, =ES_TO_AARCH64
	armv8_switch_to_el2_m x4, x5, x6
lowlevel_in_el2:

	adr	x4, lowlevel_in_el1
	ldr	x5, =ES_TO_AARCH64
	armv8_switch_to_el1_m x4, x5, x6
lowlevel_in_el1:
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
