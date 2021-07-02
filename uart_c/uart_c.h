#ifndef __UART_C_H__
#define __UART_C_H__

#define BITS_PER_LONG		64
#define BIT(X)			(0x1UL << (X))
#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))


#define APB_BASE		0x18000000
#define UART0_BASE		0x18200000
#define UART1_BASE		0x18300000
#define UART2_BASE		0x18400000
#define UART3_BASE		0x18500000
#define UART0_CKG_EN_BIT	(0X1 << 0)
#define UART1_CKG_EN_BIT	(0X1 << 1)
#define UART2_CKG_EN_BIT	(0X1 << 2)
#define UART3_CKG_EN_BIT	(0X1 << 3)
#define UART0_CKG_APB_EN_BIT	(0x1 << 0)
#define UART1_CKG_APB_EN_BIT	(0x1 << 1)
#define UART2_CKG_APB_EN_BIT	(0x1 << 2)
#define UART3_CKG_APB_EN_BIT	(0x1 << 3)

#define UART0_RX_PIN_MUX_ADDR 	0x28200000
#define UART0_TX_PIN_MUX_ADDR 	0x28200000
#define UART0_CTS_PIN_MUX_ADDR 	0x28200000
#define UART0_RTS_PIN_MUX_ADDR 	0x28200000

#define UART0_RX_PIN_MUX_VAL 	0x28200000
#define UART0_TX_PIN_MUX_VAL 	0x28200000
#define UART0_CTS_PIN_MUX_VAL 	0x28200000
#define UART0_RTS_PIN_MUX_VAL 	0x28200000

#define UART0_RX_PIN_MUX_ADDR 	0x28200000
#define UART0_TX_PIN_MUX_ADDR 	0x28200000
#define UART0_CTS_PIN_MUX_ADDR 	0x28200000
#define UART0_RTS_PIN_MUX_ADDR 	0x28200000

#define UART0_RX_PIN_MUX_VAL 	0x28200000
#define UART0_TX_PIN_MUX_VAL 	0x28200000
#define UART0_CTS_PIN_MUX_VAL 	0x28200000
#define UART0_RTS_PIN_MUX_VAL 	0x28200000

#define UART0_RX_PIN_MUX_ADDR 	0x28200000
#define UART0_TX_PIN_MUX_ADDR 	0x28200000
#define UART0_CTS_PIN_MUX_ADDR 	0x28200000
#define UART0_RTS_PIN_MUX_ADDR 	0x28200000

#define UART0_RX_PIN_MUX_VAL 	0x28200000
#define UART0_TX_PIN_MUX_VAL 	0x28200000
#define UART0_CTS_PIN_MUX_VAL 	0x28200000
#define UART0_RTS_PIN_MUX_VAL 	0x28200000

#define UART0_RX_PIN_MUX_ADDR 	0x28200000
#define UART0_TX_PIN_MUX_ADDR 	0x28200000
#define UART0_CTS_PIN_MUX_ADDR 	0x28200000
#define UART0_RTS_PIN_MUX_ADDR 	0x28200000

#define UART0_RX_PIN_MUX_VAL 	0x28200000
#define UART0_TX_PIN_MUX_VAL 	0x28200000
#define UART0_CTS_PIN_MUX_VAL 	0x28200000
#define UART0_RTS_PIN_MUX_VAL 	0x28200000



#define TEST_PORT		0
#define TEST_TYPE		INTR_TEST
#define CLK_HZ			24000000	/* 24M */

#define UART0_IRQ_NR		51
#define UART1_IRQ_NR		52
#define UART2_IRQ_NR		53
#define UART3_IRQ_NR		54







/*
 * DLAB=0
 */
#define UART_RX			0x0000	/* In:  Receive buffer */
#define UART_TX			0x0000	/* Out: Transmit buffer */

#define UART_IER		0x0004	/* Out: Interrupt Enable Register */
#define UART_IER_MSI		BIT(3) /* Enable Modem status interrupt */
#define UART_IER_RLSI		BIT(2) /* Enable receiver line status interrupt */
#define UART_IER_THRI		BIT(1) /* Enable Transmitter holding register int. */
#define UART_IER_RDI		BIT(0) /* Enable receiver data interrupt */

/*
 * DLAB=1
 */
#define UART_DLL		0x0000	/* Out: Divisor Latch Low */
#define UART_DLH		0x0004	/* Out: Divisor Latch High */

/*
 * Read
 */
#define UART_IIR		0x0008	/* In:  Interrupt ID Register */
#define UART_IIR_IID		GENMASK(3,0)
#define UART_IIR_MSI		(0x0 << 0) /* Modem status interrupt */
#define UART_IIR_NO_INT		(0x1 << 0) /* No interrupts pending */
#define UART_IIR_THRI		(0x2 << 0) /* Transmitter holding register empty */
#define UART_IIR_RDI		(0x4 << 0) /* Receiver data interrupt */
#define UART_IIR_RLSI		(0x6 << 0) /* Receiver line status interrupt */
#define UART_IIR_BUSY		(0x7 << 0) /* DesignWare APB Busy Detect */
#define UART_IIR_RX_TIMEOUT	(0xc << 0) /* OMAP RX Timeout interrupt */
#define RESERVED		GENMASK(5,4)
#define UART_IIR_FIFO_EN_STATUS	GENMASK(7,6) /* Indicate FIFOs enable */
#define UART_IIR_FIFO_NO_EN	(0x0 << 6) /* FIFO disable */
#define UART_IIR_FIFO_EN	(0x3 << 6) /* FIFO enable */

/*
 * Write
 */
#define UART_FCR		0x0008	/* Out: FIFO Control Register */
#define UART_FCR_ENABLE_FIFO	BIT(0) /* Enable the FIFO */
#define UART_FCR_CLEAR_RCVR	BIT(1) /* Clear the RCVR FIFO */
#define UART_FCR_CLEAR_XMIT	BIT(2) /* Clear the XMIT FIFO */
#define UART_FCR_DMA_SELECT	BIT(3) /* For DMA applications */
#define UART_FCR_T_TRIG 	GENMASK(5,4) /* TX Empty Trigger */
#define UART_FCR_T_TRIG_00	(0x0 << 4) /* FIFO empty */
#define UART_FCR_T_TRIG_01	(0x1 << 4) /* 2 characters in the FIFO */
#define UART_FCR_T_TRIG_10	(0x2 << 4) /* FIFO 1/4 full */
#define UART_FCR_T_TRIG_11	(0x3 << 4) /* FIFO 1/2 full */
#define UART_FCR_R_TRIG		GENMASK(7,6) /* RCVR trigger */
#define UART_FCR_R_TRIG_00	(0x0 << 6) /* 1 character in the FIFO */
#define UART_FCR_R_TRIG_01	(0x1 << 6) /* FIFO 1/4 full */
#define UART_FCR_R_TRIG_10	(0x2 << 6) /* FIFO 1/2 full */
#define UART_FCR_R_TRIG_11	(0x3 << 6) /* FIFO 2 less than full */

/*
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 */
#define UART_LCR		0x000c /* Out: Line Control Register */
#define UART_LCR_DLAB		BIT(7) /* Divisor latch access bit */
#define UART_LCR_SBC		BIT(6) /* Set break control */
#define UART_LCR_SPAR		BIT(5) /* Stick parity (?) */
#define UART_LCR_EPAR		BIT(4) /* Even parity select */
#define UART_LCR_PARITY		BIT(3) /* Parity Enable */
#define UART_LCR_STOP		BIT(2) /* Stop bits: 0=1 bit, 1=2 bits */
#define UART_LCR_DLS		GENMASK(1,0)
#define UART_LCR_WLEN5		(0x0 << 0) /* Wordlength: 5 bits */
#define UART_LCR_WLEN6		(0x1 << 0) /* Wordlength: 6 bits */
#define UART_LCR_WLEN7		(0x2 << 0) /* Wordlength: 7 bits */
#define UART_LCR_WLEN8		(0x3 << 0) /* Wordlength: 8 bits */


#define UART_MCR 		0x0010 /* Out: Modem Control Register */
#define UART_MCR_SIRE		BIT(6) /* SIR Mode Enable */
#define UART_MCR_AFE		BIT(5) /* Enable auto-RTS/CTS (TI16C550C/TI16C750) */
#define UART_MCR_LOOP		BIT(4) /* Enable loopback test mode */
#define UART_MCR_OUT2		BIT(3) /* Out2 complement */
#define UART_MCR_OUT1		BIT(2) /* Out1 complement */
#define UART_MCR_RTS		BIT(1) /* RTS complement */
#define UART_MCR_DTR		BIT(0) /* DTR complement */


#define UART_LSR		0x0014 /* In:  Line Status Register */
#define UART_LSR_ADDR_RCVD	BIT(8) /* Indicates the character is an address or data */
#define UART_LSR_RFIFOE		BIT(7) /* Receiver Fifo error */
#define UART_LSR_TEMT		BIT(6) /* Transmitter empty */
#define UART_LSR_THRE		BIT(5) /* Transmit-hold-register empty */
#define UART_LSR_BI		BIT(4) /* Break interrupt indicator */
#define UART_LSR_FE		BIT(3) /* Frame error indicator */
#define UART_LSR_PE		BIT(2) /* Parity error indicator */
#define UART_LSR_OE		BIT(1) /* Overrun error indicator */
#define UART_LSR_DR		BIT(0) /* Receiver data ready */
#define UART_LSR_BRK_ERROR_BITS	0x1E /* BI, FE, PE, OE bits */
/*
 * Access to some registers depends on register access / configuration
 * mode.
 */
#define UART_LCR_CONF_MODE_A	UART_LCR_DLAB	/* Configutation mode A */
#define UART_LCR_CONF_MODE_B	0xBF		/* Configutation mode B */

struct uart_dat {
	unsigned long 	uart_base;
	unsigned long 	apb_sys_base;
	unsigned int 	ckg_en_bit;
	unsigned int 	ckg_apb_en_bit;
	unsigned long 	clk_source;

};

enum TEST_ID
{
	ECHO_TEST = 1,
	INTR_TEST,
};
enum PARITY_TYPE
{
	NONE = 1,
	ODD,
	EVEN,
	MARK,
	SPACE
};


#define UART_BASE_(X)		(UART##X##_BASE)
#define UART_ENABLE_(X)		(UART##X##_CKG_EN_BIT)
#define UART_APB_ENABLE_(X)	(UART##X##_CKG_APB_EN_BIT)
#define UART_BASE(X)		UART_BASE_(X)
#define UART_ENABLE(X)		UART_ENABLE_(X)
#define UART_APB_ENABLE(X)	UART_APB_ENABLE_(X)


#define UART_RX_PIN_MUX_ADDR_(X) 	(UART##X##_RX_PIN_MUX_ADDR)
#define UART_TX_PIN_MUX_ADDR_(X) 	(UART##X##_TX_PIN_MUX_ADDR)
#define UART_CTS_PIN_MUX_ADDR_(X) 	(UART##X##_CTS_PIN_MUX_ADDR)
#define UART_RTS_PIN_MUX_ADDR_(X) 	(UART##X##_RTS_PIN_MUX_ADDR)
#define UART_RX_PIN_MUX_ADDR(X) 	UART_RX_PIN_MUX_ADDR_(X)
#define UART_TX_PIN_MUX_ADDR(X) 	UART_TX_PIN_MUX_ADDR_(X)
#define UART_CTS_PIN_MUX_ADDR(X) 	UART_CTS_PIN_MUX_ADDR_(X)
#define UART_RTS_PIN_MUX_ADDR(X) 	UART_RTS_PIN_MUX_ADDR_(X)

#define UART_RX_PIN_MUX_VAL_(X) 	(UART##X##_RX_PIN_MUX_VAL)
#define UART_TX_PIN_MUX_VAL_(X) 	(UART##X##_TX_PIN_MUX_VAL)
#define UART_CTS_PIN_MUX_VAL_(X) 	(UART##X##_CTS_PIN_MUX_VAL)
#define UART_RTS_PIN_MUX_VAL_(X) 	(UART##X##_RTS_PIN_MUX_VAL)
#define UART_RX_PIN_MUX_VAL(X) 		UART_RX_PIN_MUX_VAL_(X)
#define UART_TX_PIN_MUX_VAL(X) 		UART_TX_PIN_MUX_VAL_(X)
#define UART_CTS_PIN_MUX_VAL(X) 	UART_CTS_PIN_MUX_VAL_(X)
#define UART_RTS_PIN_MUX_VAL(X) 	UART_RTS_PIN_MUX_VAL_(X)

#define UART_IRQ_NR_(X)		(UART##X##_IRQ_NR)
#define UART_IRQ_NR(X)		UART_IRQ_NR_(X)



#endif