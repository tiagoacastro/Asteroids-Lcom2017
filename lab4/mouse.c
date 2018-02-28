#include "mouse.h"
#include <minix/com.h>
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/sysutil.h>

//global variables
int hook_id = 0;
int hook_id_kbd =3;
int hook_id_timer = 10;
int byte_index = 0;
int packet[3];
short size_pos = 0;
short size_neg = 0;

// functions

int mouse_subscribe_int(void){
  int hook_id_copy = hook_id;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == OK)
		if (sys_irqenable(&hook_id) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int mouse_unsubscribe_int(){
  if (sys_irqdisable(&hook_id) == OK)
		if (sys_irqrmpolicy(&hook_id) == OK)
			return 0;
	return ERROR;
}

int enable_stream_mode(){
  unsigned long ret;
  if(writeToMouse(EN_STREAM) == ERROR){
		return ERROR;
  }
	if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    enable_stream_mode();
  }
  else
    if(ret != ACK)
      return ERROR;
}
int enable_packets(){
	if (disable_data_reporting() == ERROR){
		return ERROR;
  }
  if (enable_data_reporting() == ERROR){
    return ERROR;
  }
  if (enable_stream_mode() == ERROR){
		return ERROR;
  }
  return 0;
}

int disable_data_reporting(){
  unsigned long ret;
	if(writeToMouse(DIS_DATA_REPORTING) == ERROR){
		return ERROR;
  }
  if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    disable_data_reporting();
  }
  else
    if(ret != ACK)
      return ERROR;
}
int enable_data_reporting(){
  unsigned long ret;
  if (writeToMouse(EN_DATA_REPORTING) == ERROR){
		return ERROR;
  }
  if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    enable_data_reporting();
  }
  else
    if(ret != ACK)
      return ERROR;
}

unsigned long read_OUTBUF() {
	int try = 0;
	unsigned long stat, ret;
	while (try < 3) {

		if (sys_inb(STAT_REG, &stat) != OK)
			return ERROR;

		if (stat & OBF)
		{
			if(sys_inb(OUT_BUF, &ret) != OK)
        return ERROR;

			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return ret;
			else
				return ERROR;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		try++;
	}
	return ERROR;
}

int writeToMouse(unsigned long cmd){
  if(writeToKBC(STAT_REG, WRITE_TO_MOUSE) == ERROR)
	 return ERROR;

  if(writeToKBC(OUT_BUF, cmd) == ERROR)
	 return ERROR;
return 0;
}

int writeToKBC(unsigned long reg, unsigned long cmd){
  unsigned long stat;
  int try = 0;

  while(try < 3)
  {
	  if(sys_inb(STAT_REG, &stat) != OK)
	 	  return ERROR;

	  if((stat & IBF) == 0)
	  {
		  if(sys_outb(reg, cmd) != OK)
			  return ERROR;
      else
		  return 0;
	  }
    tickdelay(micros_to_ticks(DELAY_US));
    try++;
  }
  return ERROR;
}

int packet_handler(){
  unsigned long byte = read_OUTBUF();
	if (byte == ERROR){
		return ERROR;
	}
	if (byte_index == 0){
		if (byte & BIT(3))
			 packet[byte_index] = byte;
		byte_index++;
    return 0;
	}
  if (byte_index == 1){
    packet[byte_index] = byte;
    byte_index++;
		return 0;
  }
	if (byte_index == 2){
    packet[byte_index] = byte;
    byte_index = 0;
	  return 1;
	}
}

void packet_print(){

  printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);

	if (packet[0] & LB)
		printf("LB=1 ");
	else
		printf("LB=0 ");

  if (packet[0] & MB)
  	printf("MB=1 ");
  else
  	printf("MB=0 ");

	if (packet[0] & RB)
		printf("RB=1 ");
	else
		printf("RB=0 ");

	if (packet[0] & XOVF)
		printf("XOV=1 ");
	else
		printf("XOV=0 ");

	if (packet[0] & YOVF)
		printf("YOV=1 ");
	else
		printf("YOV=0 ");

    int byte2 = packet[1];
  	int byte3 = packet[2];

  //passing from 2's complement if needed
	printf("X=");
	if (byte2 & BIT(7)) {
    byte2 = byte2 ^ 0xFF;
    byte2++;
    byte2 = -byte2;
	}
	printf("%d ", byte2);

  //passing from 2's complement if needed
	printf("Y=");
	if (byte3 & BIT(7)) {
    byte3 = byte3 ^ 0xFF;
    byte3++;
    byte3 = -byte3;
	}
  printf("%d \n\n", byte3);
}


int empty_OUTBUF() {
	unsigned long stat;
  if (sys_inb(STAT_REG, &stat) != OK)
    return ERROR;
	while ((stat & OBF) != 0) {
		if (sys_inb(OUT_BUF, &stat) != OK)
			return ERROR;
		tickdelay(micros_to_ticks(DELAY_US));
    if (sys_inb(STAT_REG, &stat) != OK)
      return ERROR;
	}
  return 0;
}

unsigned long disable_mouse_interrupts()
{
	unsigned long cmdb, cmdb_copy;
	//Subscribe to keyboard to prevent MINIX keyboard IH from stealing outbuffer contents

	if(kbd_subscribe_int()==ERROR)
		return ERROR;

	if(writeToKBC(KBC_CMD_REG,READ_CMD)==ERROR)
		return ERROR;

	if(sys_inb(OUT_BUF,&cmdb) != OK)
		return ERROR;

  cmdb_copy = cmdb;

	if(writeToKBC(KBC_CMD_REG,WRITE_CMD)==ERROR)
		return ERROR;

	cmdb=(cmdb & ~BIT(1));

	if(writeToKBC(IN_BUF,cmdb)==ERROR)
			return ERROR;

	if(kbd_unsubscribe_int()==ERROR)
				return ERROR;

	return cmdb_copy;
}

int enable_mouse_interrupts(unsigned long cmdb)
{
	//Subscribe to keyboard to prevent MINIX keyboard IH from stealing outbuffer contents

	if(kbd_subscribe_int()==ERROR)
		return ERROR;

	if(writeToKBC(KBC_CMD_REG,WRITE_CMD)==ERROR)
		return ERROR;

	if(writeToKBC(IN_BUF,cmdb)==ERROR)
			return ERROR;

	if(kbd_unsubscribe_int()==ERROR)
				return ERROR;

	return 0;
}

int wait_output(int waitTime)
{
	unsigned long stat;
	while(waitTime>0)
	{
		if (sys_inb(STAT_REG, &stat) != OK)
			return ERROR;

		if(stat & OBF)
			return 1;
		else
		{
			waitTime--;
			tickdelay(micros_to_ticks(10000));
		}
	}

	return 0;
}

int set_remote_mode()
{
	unsigned long response = 0;
	while (1)
		{
			if (writeToMouse(SET_REMOTE) == ERROR)
				return ERROR;
			if(wait_output(20))
				if(read_OUTBUF()==ACK)
					break;
				else
					return ERROR;
		}

	return 0;
}

int load_packets()
{
	packet[0]=read_OUTBUF();

	if(packet[0]==ERROR)
		return ERROR;

	packet[1]=read_OUTBUF();

	if(packet[1]==ERROR)
		return ERROR;

	packet[2]=read_OUTBUF();

	if(packet[2]==ERROR)
		return ERROR;
}

short line_scaner(){
  int byte2 = packet[1];
  int byte3 = packet[2];
  unsigned long byte2_sign = packet[1] & BIT(7);
  unsigned long byte3_sign = packet[2] & BIT(7);
  if (packet[0] & RB){
    if (byte2 & BIT(7)) {
      byte2 = byte2 ^ 0xFF;
      byte2++;
	  }
    if (byte3 & BIT(7)) {
      byte3 = byte3 ^ 0xFF;
      byte3++;
	  }
    if((packet[1] & BIT(7)) && (packet[2] & BIT(7))){
      size_pos = 0;
      size_neg += byte2;
      return size_neg;
    }else
    if(!(packet[1] & BIT(7)) && !(packet[2] & BIT(7))){
      size_pos += byte2;
      size_neg = 0;
      return size_pos;
    }
      size_pos = 0;
      size_neg = 0;
      return 0;
  }
  size_pos = 0;
  size_neg = 0;
  return 0;
}

//timer functions

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

int timer_int_handler(int counter) {
	counter++;
  return counter;
}

//keyboard functions

int kbd_subscribe_int(void)
{
  int hook_id_copy = hook_id_kbd;
	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) == OK)
		if (sys_irqenable(&hook_id_kbd) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int kbd_unsubscribe_int()
{
  if (sys_irqdisable(&hook_id_kbd) == OK)
		if (sys_irqrmpolicy(&hook_id_kbd) == OK)
			return 0;
	return ERROR;
}
