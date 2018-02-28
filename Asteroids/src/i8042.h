#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n)					      (0x01<<(n))
#define ERROR					      -1
#define DELAY_US                      20000

// keyboard

#define ESC_KEY						  0x81
#define OUT_BUF						  0x60
#define IN_BUF						  0x60
#define MSB_2BYTE					  0xE0
#define KBC_CMD_REG			   		  0x64
#define READ_CMD_BYTE		    	  0x20
#define WRITE_CMD_BYTE				  0x60
#define DELAY_US    		    	  20000
#define POLL_HANDLER		   		  1
#define SCAN_HANDLER		          0
#define KBC_EN_INT					  BIT(0)

#define W_MAKE                 0x11
#define W_BREAK                0x91
#define A_MAKE                 0x1E
#define A_BREAK                0x9E
#define S_MAKE                 0x1F
#define S_BREAK                0x9F
#define D_MAKE                 0x20
#define D_BREAK                0xA0

#define OBF 					      BIT(0)
#define IBF 					      BIT(1)
#define AUX 					      BIT(5)
#define TO_ERR 				          BIT(6)
#define PAR_ERR                       BIT(7)

#define KBC_IRQ						  1

// mouse

#define DIS_DATA_REPORTING            0xF5
#define EN_DATA_REPORTING             0xF4
#define READ_CMD                      0x20
#define WRITE_CMD                     0x60
#define DIS_MOUSE                     0xA7
#define EN_MOUSE                      0xA8
#define CHECK_MOUSE_INTERFACE         0xA9
#define WRITE_TO_MOUSE                0xD4
#define STAT_REG	                  0x64
#define ACK                           0xFA
#define NACK                          0xFE
#define EN_STREAM                     0xEA
#define SET_REMOTE					  0xF0
#define	READ_DATA					  0xEB
#define KBC_CMD_REG					  0x64

#define LB 			                  BIT(0)
#define RB 				          	  BIT(1)
#define MB 				              BIT(2)
#define XSIGN 			              BIT(4)
#define YSIGN                          BIT(5)
#define XOVF 			              BIT(6)
#define YOVF                          BIT(7)

#define MOUSE_IRQ                     12

//rtc

#define RTC_IRQ	           8
#define RTC_ADDR_REG	     0x70
#define RTC_DATA_REG	     0x71
#define REG_A			         10
#define REG_B  			       11
#define REG_C              12
#define RTC_CONF_24_12     BIT(1)
#define RTC_CONF_DM        BIT(2)
#define RTC_CONF_UIE       BIT(4)
#define RTC_CONF_UIE_A     BIT(7)
#define RTC_UF             BIT(4)
#define SECONDS     	     0
#define MINUTES 		       2
#define HOURS	             4
#define DAY	               7
#define MONTH	             8
#define YEAR               9

//key codes

#define Akey 0x1E
#define Bkey 0x30
#define Ckey 0x2E
#define Dkey 0x20
#define Ekey 0x12
#define Fkey 0x21
#define Gkey 0x22
#define Hkey 0x23
#define Ikey 0x17
#define Jkey 0x24
#define Kkey 0x25
#define Lkey 0x26
#define Mkey 0x32
#define Nkey 0x31
#define Okey 0x18
#define Pkey 0x19
#define Qkey 0x10
#define Rkey 0x13
#define Skey 0x1F
#define Tkey 0x14
#define Ukey 0x16
#define Vkey 0x2F
#define Wkey 0x11
#define Xkey 0x2D
#define Ykey 0x15
#define Zkey 0x2C

#define ENTER       0x1C
#define BACK_SPACE  0x0E

#endif /* _LCOM_I8254_H */
