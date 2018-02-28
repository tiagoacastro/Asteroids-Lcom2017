#include "test3.h"
#include "i8042.h"
#include "keyboard.h"

int kbd_test_scan(unsigned short ass) {
	int ipc_status, r;
	master_counter = 0;

	message msg;

	int irq_set = kbd_subscribe_int();

	if(irq_set == ERROR)
	{
		printf("Error subscribing to keyboard \n");
		return ERROR;
	}
	while(key != ESC_KEY)
	{
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
						if(ass==0)
							kbd_read_buffer_c();
						else
							kbd_read_buffer_asm();
						handler();
					}
				default:
					break;
				}
			}
	}
	#ifdef LAB3
	printf("Sys_inb kernel calls %d \n", master_counter);
	#endif
	if(kbd_unsubscribe_int() == ERROR)
	{
	  printf("Error subscribing to keyboard \n");
		return ERROR;
	}
	return 0;
}

int kbd_test_poll() {
	master_counter = 0;
	unsigned long read = READ_CMD_BYTE;
	unsigned long write = WRITE_CMD_BYTE;
	unsigned long temp;


	while(key!= ESC_KEY)
    {
			key = kbd_read_poll();
			if (key == ERROR)
			return ERROR;
	    handler_poll();
    }

	//Read command byte
	kbd_issue_cmd(read);
	//Read Out buffer
	temp=kbd_read_out_buff();
	//Write command byte
	kbd_issue_cmd(write);
	//Enable keyboard
	kbd_write_in_buff(temp | KBC_EN_INT);

  #ifdef LAB3
	printf("Sys_inb kernel calls %d \n", master_counter);
	#endif
	return ERROR;
}

int kbd_test_timed_scan(unsigned short n) {
	int ipc_status, r;
	master_counter = 0;
	unsigned short timer_counter = 0;
	bool time_over = false;

	message msg;

	int irq_set = kbd_subscribe_int();
	if(irq_set == ERROR)
	{
		printf("Error subscribing to keyboard \n");
		return ERROR;
	}

	int irq_set_timer = timer_subscribe_int();
	if(irq_set_timer == ERROR)
	{
		printf("Error subscribing to timer \n");
		return ERROR;
	}

	while(key != ESC_KEY && !time_over)
	{
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
					if (msg.NOTIFY_ARG & irq_set_timer)
					{
						timer_counter++;
						if(timer_counter == 60 * n)
						{
							printf("%d seconds have passed, the program has been stopped \n", n);
							time_over = true;
							break;
						}
					}
					if (msg.NOTIFY_ARG & irq_set)
					{
						timer_counter = 0;
						kbd_read_buffer_c();
						handler();
					}
				default:
					break;
				}
			}
		}
	if(kbd_unsubscribe_int() == ERROR)
	{
		printf("Error unsubscribing to keyboard \n");
		return ERROR;
	}
	if(timer_unsubscribe_int() == ERROR)
	{
		printf("Error subscribing to timer \n");
		return ERROR;
	}
	return 0;
}
