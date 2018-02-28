#include "keyboard.h"
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>

int hook_id = 0;
int hook_id_timer = 10;
extern unsigned long asmKey;
unsigned long asm_handler();

int kbd_subscribe_int(void)
{
  int hook_id_copy = hook_id;
	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == OK)
		if (sys_irqenable(&hook_id) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int kbd_unsubscribe_int()
{
  if (sys_irqdisable(&hook_id) == OK)
		if (sys_irqrmpolicy(&hook_id) == OK)
			return 0;
	return ERROR;
}


int handler(){
	int nBytes;
	static bool doubleByte = false;
  if (MSB_2BYTE == key)
      nBytes= 2; //2 bytes
    else
      nBytes = 1; //1 byte
	if(nBytes == 1 && !doubleByte) //1 byte
	{
		if (BIT(7) == (BIT(7) & key))
		{
			printf("Breakcode: 0x%02x\n", key);
		}
		else
		{
			printf("Makecode:  0x%02x\n", key);
		}
	}
	if(nBytes == 2 || doubleByte) //2 bytes
	{
		//If it is a scanCode with 2 Bytes it will do another iteration and in the second one write the 2Byte code
		if(!doubleByte)
			{
				doubleByte = true;
		    }
		else
		{
			if (BIT(7) == (BIT(7) & key))
			{
				printf("Breakcode: 0xE0 0x%02x\n", key);
			}
		    else
			{
		    	printf("Makecode:  0xE0 0x%02x\n", key);
			}
		    doubleByte = false;
		}
	}
		if(nBytes == ERROR)
			return ERROR;
		return 0;
}

int handler_poll(){
	int nBytes;
	static bool doubleByte = false;

	if (MSB_2BYTE == key)
		  nBytes= 2; //2 bytes
	  else
		  nBytes = 1; //1 byte

	if(nBytes == 1 && !doubleByte) //1 byte
	{
		if (BIT(7) == (BIT(7) & key))
		{
			printf("Breakcode: 0x%02x\n", key);
		}
		else
		{
			printf("Makecode:  0x%02x\n", key);
		}
	}
	if(nBytes == 2 || doubleByte) //2 bytes
	{
		//If it is a scanCode with 2 Bytes it will do another iteration and in the second one write the 2Byte code
		if(!doubleByte)
			{
				doubleByte = true;
		    }
		else
		{
			if (BIT(7) == (BIT(7) & key))
			{
				printf("Breakcode: 0xE0 0x%02x\n", key);
			}
		    else
			{
		    	printf("Makecode:  0xE0 0x%02x\n", key);
			}
		    doubleByte = false;
		}
	}
		if(nBytes == ERROR)
			return ERROR;
		return 0;
}

int kbd_read_buffer_c()
{
  unsigned long errorValue;
  errorValue = sys_inb(OUT_BUF, &key);
  if (errorValue != OK)
  {
    printf("Sys_inb failed with: %d", errorValue);
    return ERROR;
  }
#ifdef LAB3
  //Incrementing sys_inb kernel calls counter
  master_counter++;
#endif


  tickdelay(micros_to_ticks(DELAY_US));
return 0;
}

int kbd_read_buffer_asm()
{
  asm_handler();
  key = asmKey;
#ifdef LAB3
//Incrementing sys_inb kernel calls counter
  master_counter++;
#endif

  tickdelay(micros_to_ticks(DELAY_US));
return 0;
}


int kbd_issue_cmd(unsigned long cmd)
{
	unsigned long stat;
	while(true)
	{
		if(sys_inb(STAT_REG, &stat)!=OK)
			return ERROR;
#ifdef LAB3
		master_counter++;
#endif
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 )
		{
			if(sys_outb(KBC_CMD_REG, cmd)!=OK)
				return ERROR;
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

unsigned long kbd_read_poll()
{
	unsigned long stat, data;
	while(true)
	{
		if(sys_inb(STAT_REG, &stat)!=OK)
			return ERROR;
#ifdef LAB3
		master_counter++;
#endif

		/* If data is available for reading*/
		if( stat & OBF )
		{
			if(sys_inb(OUT_BUF, &data)!=OK)
				return ERROR;
#ifdef LAB3
			master_counter++;
#endif

			if ( (stat & (PAR_ERR | TO_ERR)) == 0 )
				return data;
			else
				return ERROR;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

unsigned long kbd_read_out_buff()
{
	unsigned long data, stat;

	if(sys_inb(STAT_REG, &stat)!=OK)
		return ERROR;
#ifdef LAB3
	master_counter++;
#endif

	if( stat & OBF )
	{
	if(sys_inb(OUT_BUF, &data)!= OK)
		return ERROR;
#ifdef LAB3
	master_counter++;
#endif

	if ( (stat & (PAR_ERR | TO_ERR)) == 0 )
		return data;
	else
		return ERROR;
	}
	return ERROR;
}

int kbd_write_in_buff(unsigned long cmd)
{
	unsigned long stat;

	if(sys_inb(STAT_REG, &stat)!=OK)
		return ERROR;
#ifdef LAB3
	master_counter++;
#endif

	/* loop while 8042 input buffer is not empty */
	if( (stat & IBF) == 0 )
	{
		if(sys_outb(IN_BUF, cmd)!=OK)
			return ERROR;
		return 0;
	}
	return ERROR;
}

int timer_subscribe_int(void) {
	int hook_id_copy = hook_id_timer;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) == OK)
		if (sys_irqenable(&hook_id_timer) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int timer_unsubscribe_int() {
	if (sys_irqdisable(&hook_id_timer) == OK)
		if (sys_irqrmpolicy(&hook_id_timer) == OK)
			return 0;
	return ERROR;
}
