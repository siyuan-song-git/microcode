#define NUM_CORES	8
#define NUM_ITS		3
#define GICR0_BASE_OFF	((2 * NUM_ITS + 4) << 16)
#define GICD_BASE 	0X1000000
#define GICR_BASE	(GICD_BASE + GICR0_BASE_OFF)
#define TEST_ITS_NUM	0
