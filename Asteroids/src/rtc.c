#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "rtc.h"
#include "i8042.h"

static int hook_id_rtc = 8;
extern unsigned long data;
extern unsigned long reg;
unsigned long asm_sys();

int rtc_subscribe_int(void) {
	int tmp_hook_id = hook_id_rtc;

	if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_rtc) == OK)
		if(sys_irqenable(&hook_id_rtc) == OK)
    	return BIT(tmp_hook_id);

	return ERROR;
}


int rtc_enable(){
	unsigned long var;

	if(sys_outb(RTC_ADDR_REG, REG_B) != OK)
			return ERROR;

	if(sys_inb(RTC_DATA_REG, &var) != OK)
			return ERROR;

	var = var | RTC_CONF_UIE;

	if(sys_outb(RTC_ADDR_REG, REG_B) != OK)
			return ERROR;

	if(sys_outb(RTC_DATA_REG, var) != OK)
			return ERROR;

	return 0;
}

int rtc_disable(){
	unsigned long var;

	if(sys_outb(RTC_ADDR_REG, REG_B) != OK)
			return ERROR;

	if(sys_inb(RTC_DATA_REG, &var) != OK)
			return ERROR;

	var = var & ~RTC_CONF_UIE;

	if(sys_outb(RTC_ADDR_REG, REG_B) != OK)
			return ERROR;

	if(sys_outb(RTC_DATA_REG, var) != OK)
			return ERROR;

	return 0;
}

int rtc_unsubscribe_int() {
  if (sys_irqdisable(&hook_id_rtc) == OK)
		if(sys_irqrmpolicy(&hook_id_rtc) == OK)
    	return 0;

	return ERROR;
}

Date* newDate(){
    Date* date;
    date = (Date*) malloc(sizeof(Date));
		date->year = -1;
		date->month = -1;
		date->day = -1;
		date->hour = -1;
		date->minute = -1;
		date->second = -1;
    return date;
}

void deleteDate(Date* date){
  free(date);
}

void updateDate(Date* date){

		reg = REG_C;
		asm_sys();

		reg = REG_B;
		asm_sys();

		int bcd = ((data & BIT(2)) == 0);

		reg = SECONDS;
		asm_sys();
		if (bcd)
			date->second = bcd_to_int(data);
		else
			date->second = data;

		reg = MINUTES;
		asm_sys();
		if (bcd)
			date->minute = bcd_to_int(data);
		else
			date->minute = data;

		reg = HOURS;
		asm_sys();
		if (bcd)
			date->hour = bcd_to_int(data);
		else
			date->hour = data;

		reg = DAY;
		asm_sys();
		if (bcd)
			date->day = bcd_to_int(data);
		else
			date->day = data;

		reg = MONTH;
		asm_sys();
		if (bcd)
			date->month = bcd_to_int(data);
		else
			date->month = data;

		reg = YEAR;
		asm_sys();
		if (bcd)
			date->year = bcd_to_int(data);
		else
			date->year = data;
}

int bcd_to_int(unsigned long x) {
    return x - 6 * (x >> 4);
}
