#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_h


#define MAX_44008_ADDR					0x82

#define REG_INT_STATUS					0x00
#define REG_MAIN_CFG					0x01
#define REG_AMB_CFG						0x02
#define REG_AMB_CLEAR_HIGH_BYTE			0x04
#define REG_AMB_CLEAR_LOW_BYTE			0x05
#define REG_AMB_RED_HIGH_BYTE			0x06
#define REG_AMB_RED_LOW_BYTE			0x07
#define REG_AMB_GREEN_HIGH_BYTE			0x08
#define REG_AMB_GREEN_LOW_BYTE			0x09
#define REG_AMB_BLUE_HIGH_BYTE			0x0A
#define REG_AMB_BLUE_LOW_BYTE			0x0B
#define REG_AMB_IR_HIGH_BYTE			0x0C
#define REG_AMB_IR_LOW_BYTE				0x0D
#define REG_AMB_IRCOMP_HIGH_BYTE		0x0E
#define REG_AMB_IRCOMP_LOW_BYTE			0x0F
#define REG_AMB_TEMP_HIGH_BYTE			0x12
#define REG_AMB_TEMP_LOW_BYTE			0x13

/* INT status register */


/* Main configuration register parameters */
#define MAIN_CFG_AMB_INT_EN_MASK		0x01
#define MAIN_CFG_AMB_INT_EN				0x01

#define MAIN_CFG_AMBSEL_MASK			0x0C
#define MAIN_CFG_AMBSEL_CLEAR_ACTIVE	0x00
#define MAIN_CFG_AMBSEL_GREEN_ACTIVE	0x04
#define MAIN_CFG_AMBSEL_IR_ACTIVE		0x08
#define MAIN_CFG_AMBSEL_TEMP_ACTIVE		0x0C

#define MAIN_CFG_MODE_MASK				0x30
#define MAIN_CFG_MODE_CLEAR_ONLY		0x00
#define MAIN_CFG_MODE_CLEAR_IR			0x10
#define MAIN_CFG_MODE_CLEAR_IR_RGB		0x20

/* Ambient Configuration Register Parameters */
#define AMB_CFG_AMBPGA_MASK				0x03
#define AMB_CFG_AMBPGA__0				0x00
#define AMB_CFG_AMBPGA__1				0x01
#define AMB_CFG_AMBPGA__2				0x02
#define AMB_CFG_AMBPGA__3				0x03

#define AMB_CFG_AMBTIM_MASK				0x1C
#define AMB_CFG_AMBTIM_100_MS_14_BIT	0x00	/* 1 x RSB for fixed */
#define AMB_CFG_AMBTIM_25_MS_12_BIT		0x04	/* 4 x RSB for fixed */
#define AMB_CFG_AMBTIM_6_25_MS_10_BIT	0x80	/* 16 x RSB for fixed */
#define AMB_CFG_AMBTIM_1_5625_MS_8_BIT	0x0C	/* 64 x RSB for fixed */
#define AMB_CFG_AMBTIM_400_MS_14_BIT_H	0x10	/* 1/4 x RSB for fixed */

#define AMB_CFG_TEMPEN_MASK				0x20
#define ABM_CFG_TEMPEN					0x20

#define AMB_CFG_COMPEN_MASK				0x40
#define AMB_CFG_COMPEN					0x40

#define AMB_CFG_TRIM_MASK				0x80
#define AMB_CFG_TRIM_FACTORY			0x00
#define AMB_CFG_TRIM_USER_DEF			0x80

typedef enum MAX44008_val_grab{
	GRAB_HIGH_BYTE,
	GRAB_LOW_BYTE,
	STORE_AND_INCREMENT
}MAX44008_val_grab_t;

typedef struct MAX44008_indexer{
	MAX44008_val_grab_t curval;
	int index;
}MAX44008_indexer_t;



void init_light_sensor(void);

void read_int_register(void);

void configure_MAX44008(void);

void read_R_val(void);
void read_G_val(void);
void read_B_val(void);
void read_T_val(void);
void read_IR_val(void);




#endif
