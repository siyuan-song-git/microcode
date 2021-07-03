#include "global.h"



#define PROP_TB_BASE 	0x141008000
#define PEND_TB_BASE	0x141010000
#define DEV_TB_BASE		0x14101a000
#define COL_TB_BASE		0x14101b000
#define IT_TB_BASE		0x14101c000
#define CMDQ_BASE		0x14101f000
#define CMDQ_SIZE		0x0


#define GITS_BASE_OFF	(0x00040000 + 0x20000 * TEST_ITS_NUM)
#define GITS_BASE	(GICD_BASE + GITS_BASE_OFF)

#define CMD_MAPD	0x08
#define CMD_MAPC	0x09
#define CMD_MAPI	0x0B
#define CMD_INV		0x0C
#define CMD_INT		0x03

#define ITT_ENTRY_SIZE	0xD


typedef unsigned int 	u32;
typedef unsigned long long 	u64;
typedef union mpdir_reg {
	struct mpidr_fields {
		unsigned int aff0:	8;
		unsigned int aff1:	8;
		unsigned int aff2:	8;
		unsigned int mt:	1;
		unsigned int res1:	5;
		unsigned int u:		1;
		unsigned int res2:	1;
		unsigned int aff3:	8;
		unsigned int res3:	24;
	} fields;
	unsigned long long word;
} core_mpidr;

typedef struct interrupt_struct {
	unsigned int id;
	unsigned int group;
	unsigned int priority;
	char*	security;
} interrupt;

typedef struct test_control_struct {
	unsigned int core_num;
	unsigned int lpiSup;
	unsigned int are0Sup;
	unsigned int spiSup;
} test_control;


typedef struct {
	volatile unsigned int	*GICD_CTLR;
	volatile unsigned int	*GICD_TYPER;
	volatile unsigned int	*GICD_IIDR;
	volatile unsigned int	*GICD_SETSPI_SR;
	volatile unsigned int	*GICD_IGROUPR;
	volatile unsigned int	*GICD_ISENABLER;
	volatile unsigned int	*GICD_IGRPMODR;
	volatile unsigned char	*GICD_IPRIORITYR;
	volatile unsigned int	*GICD_ITARGETSR0;
	volatile unsigned int	*GICD_PIDR2;
	volatile unsigned long	*GICD_IROUTER;
} dist_regs;

typedef struct {	
	volatile unsigned int	*GICR_CTLR;
	volatile unsigned int	*GICR_TYPER;
	volatile unsigned int	*GICR_WAKER;
	volatile unsigned int	*GICR_PWRR;
	volatile unsigned int	*GICR_SETLPIR;
	volatile unsigned int	*GICR_PROPBASER;
	volatile unsigned int	*GICR_PENDBASER;
	volatile unsigned int	*GICR_IGROUPR;
	volatile unsigned int	*GICR_IGRPMODR;
	volatile unsigned char	*GICR_IPRIORITYR;
	volatile unsigned int	*GICR_ISENABLER;
	volatile unsigned int	*GICR_ISPENDR;
} redist_regs;

typedef struct {
	volatile unsigned int	*GITS_CTLR;
	volatile unsigned int	*GITS_CBASER;
	volatile unsigned long	*GITS_CWRITER;	
	volatile unsigned int	*GITS_BASER;
} its_regs;

