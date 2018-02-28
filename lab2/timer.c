#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8254.h"
#include "timer.h"

unsigned long master_counter = 0;
int hook_id = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq) {
    unsigned char conf;
    int works = timer_get_conf(timer, &conf);
    if (works == ERROR)
    	return ERROR;
    conf = conf | BIT(5) | BIT (4);    // bit 5 and  4 to 1, LSB followed by MSB
	if(timer==0) // 0 0
		{
		if(conf & BIT(7))
		conf = conf & 0x7F;
		if(conf & BIT(6))
		conf = conf & 0xBF;
		}
	if(timer==1) // 0 1
		{
		if(conf & BIT(7))
		conf = conf & 0x7F;
		if(!(conf & BIT(6)))
		conf = conf | 0x40;
		}
	if(timer==2) // 1 0
		{
		if(!(conf & BIT(7)))
		conf = conf | 0x80;
		if(conf & BIT(6))
		conf = conf & 0xBF;
		}
	if (sys_outb(TIMER_CTRL,conf) == ERROR)
		return ERROR;
	unsigned long Frequence = TIMER_FREQ/freq;
        //Writes LSB
		works = sys_outb(TIMER_0 + timer,Frequence);
		//Writes MSB
		works = sys_outb(TIMER_0 + timer,(Frequence >> 8));

	if(works == ERROR)
	return ERROR;
	return 0;
}

int timer_subscribe_int(void) {
	int hook_id_copy = hook_id;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)
		if (sys_irqenable(&hook_id) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int timer_unsubscribe_int() {
	if (sys_irqdisable(&hook_id) == OK)
		if (sys_irqrmpolicy(&hook_id) == OK)
			return 0;
	return ERROR;
}

void timer_int_handler() {
	master_counter++;
}

int timer_get_conf(unsigned char timer, unsigned char *st) {
	unsigned long conf;
	conf = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); //Sets conf to the correct readback command
	if(sys_outb(TIMER_CTRL,conf) == ERROR)
		return ERROR;
	sys_inb(TIMER_0 + timer,&conf);
	*st = (unsigned char) conf;
	return 0;
}

int timer_display_conf(unsigned char conf) {
	unsigned char compare; //This will be reused in order to simplify the code
	//Checks Output bit (7)
	printf("Out pin is ");
	if(BIT(7)==(conf & BIT(7)))
		printf("1\n");
	else
		printf("0\n");
	//No use on returning error because it can only be 1 or 0

	//Checks Count bit (6)
	printf("Count: ");
	if (BIT(6)==(conf & BIT(6)))
		printf("Null count \n");
	else
		printf("Count available for reading \n");
	//Checks the type of access bits (5 , 4)
	compare = conf & (BIT(5) | BIT(4));
	printf("Type of access: ");
	if ((BIT(5) | BIT(4)) == compare)
			printf("LSB followed by MSB \n");
	else if (BIT(5) == compare)
		printf("MSB \n");
	else if (BIT(4) == compare)
	    printf("LSB \n");
	else if (compare == 0)
		printf("Counter Latch Command \n");
	//Checks Operating mode bits (3,2,1)
    printf("Operating mode: ");
    compare = conf & (BIT(3) | BIT(2) | BIT(1));
    compare >>= 1;

    if(compare == 0)
    	printf("0 \n");
    else if(compare==1)
    	printf("1 \n");
    else if(compare==2 || compare ==6)
    	printf("2 \n");
    else if(compare==3 || compare == 7)
    	printf("3 \n");
    else if (compare == 4)
    	printf("4 \n");
    else if (compare == 5)
    	printf(" 5 \n");
    else return ERROR;

	//Checks Counting mode
	printf("Counting mode: ");
	compare = conf & BIT(0);
	if (compare == TIMER_BIN)
		printf("Binary \n \n");
	else
		printf("BCD \n \n");
	return 0;
}

int timer_test_time_base(unsigned long freq) {
	timer_set_frequency(0,freq);
	return 1;
}

int timer_test_int(unsigned long time) {
	int ipc_status, r, second;
	message msg;

	int irq_set = timer_subscribe_int();
	if(irq_set == ERROR)
		return ERROR;
	int time_iterations = time * TIMER_FREQUENCE;
	int iterator = 0;
	while(iterator < time_iterations)
	{
		iterator++;
		if((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
			{
				switch (_ENDPOINT_P(msg.m_source))
				{
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set)
					{
						if(iterator % TIMER_FREQUENCE == 0)
						{
						second = iterator / TIMER_FREQUENCE;
						printf("%d \n",second);
						timer_int_handler();
						}
					}
					break;
				default:
					break;
				}
			}
	}
	if(timer_unsubscribe_int() == ERROR)
		return ERROR;
	return 0;
}

int timer_test_config(unsigned char timer) {
	if(timer < 0 || timer > 2)
	{
		printf("Timer does not exist \n");
		return ERROR;
	}
	else
	{
		unsigned char st;
		timer_get_conf(timer,&st);
		timer_display_conf(st);
		return 0;
	}
}



