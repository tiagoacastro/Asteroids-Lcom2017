#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//Macros for Lab4

#define BIT(n)					              (0x01<<(n))
#define ERROR					                -1
#define DELAY_US                      20000

#define MOUSE_IRQ                     12
#define DIS_DATA_REPORTING            0xF5
#define EN_DATA_REPORTING             0xF4
#define READ_CMD                      0x20
#define WRITE_CMD                     0x60
#define DIS_MOUSE                     0xA7
#define EN_MOUSE                      0xA8
#define CHECK_MOUSE_INTERFACE         0xA9
#define WRITE_TO_MOUSE                0xD4
#define STAT_REG	                    0x64
#define OUT_BUF                       0x60
#define IN_BUF						            0x60
#define ACK                           0xFA
#define NACK                          0xFE
#define EN_STREAM                     0xEA
#define SET_REMOTE					          0xF0
#define	READ_DATA					            0xEB
#define KBC_CMD_REG					          0x64

//packet

#define LB 			                     	BIT(0)
#define RB 				                    BIT(1)
#define MB 				                    BIT(2)
#define XOVF 			                    BIT(6)
#define YOVF                          BIT(7)

//stat_reg

#define OBF 					                BIT(0)
#define IBF 					                BIT(1)
#define AUX 					                BIT(5)
#define TO_ERR 				                BIT(6)
#define PAR_ERR                       BIT(7)

//macros from LAB2 needed to program the Timer

#define TIMER0_IRQ	                  0

//macros from LAB3 needed to program the keyboard

#define KBC_IRQ				                1

#endif /* _LCOM_I8254_H */
