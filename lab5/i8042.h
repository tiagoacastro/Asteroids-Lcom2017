#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Macros for Lab3

#define DELAY_US				20000
#define ESC_KEY					0x81
#define KBC_IRQ					1
#define OUT_BUF					0x60
#define IN_BUF					0x60
#define MSB_2BYTE				0xE0
#define BIT(n)					(0x01<<(n))
#define ERROR					-1
#define STAT_REG				0x64
#define KBC_CMD_REG			    0x64
#define READ_CMD_BYTE		    0x20
#define WRITE_CMD_BYTE			0x60
#define PAR_ERR					BIT(7)
#define TO_ERR					BIT(6)
#define IBF						BIT(1)
#define OBF						BIT(0)
#define AUX						BIT(5)
#define DELAY_US    		    20000
#define POLL_HANDLER		    1
#define SCAN_HANDLER		    0
#define KBC_EN_INT				BIT(0)

//macros from LAB2 needed to program the Timer

#define TIMER0_IRQ	        0 

#endif /* _LCOM_I8254_H */
