#include "test5.h"
#include <string.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>

#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "lmlib.h"
#include "vbe.h"
#include "video_gr.h"
#include "video_test.h"
#include <machine/int86.h>
#include "sprite.h"



int video_test_init(unsigned short mode, unsigned short delay) {

	if(vg_init(mode) == NULL)
		return ERROR;

	sleep(delay);

	if(vg_exit() == ERROR)
		return ERROR;

	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	//inicialize graphics mode

	if(vg_init(V1024x768) == NULL)
		return ERROR;

	//create square

	vg_create_square(x, y, size, color);

	video_dump_fb();

	//exit condition

	int ipc_status, r;
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
	  printf("Error subscribing to keyboard \n");
		return ERROR;
	}

	//go back to text mode

	if(vg_exit() == ERROR)
		return ERROR;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi,
		           unsigned short xf, unsigned short yf, unsigned long color) {

	//inicialize graphics mode

		if(vg_init(V1024x768) == NULL)
			return ERROR;

		//create line

		vg_create_line(xi, yi, xf, yf,color);

		video_dump_fb();

		//exit condition

		int ipc_status, r;
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
		  printf("Error subscribing to keyboard \n");
			return ERROR;
		}

		//go back to text mode

		if(vg_exit() == ERROR)
			return ERROR;

		return 0;

}

int video_test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	//inicialize graphics mode

	if (vg_init(V1024x768) == NULL)
		return ERROR;

	//create pixmap

	vg_create_pixmap(xpm, xi, yi);

	video_dump_fb();

	//exit condition

	int ipc_status, r;
	message msg;

	int irq_set = kbd_subscribe_int();

	if (irq_set == ERROR) {
		printf("Error subscribing to keyboard \n");
		return ERROR;
	}

	while (key != ESC_KEY) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_read_buffer_c();
					handler();
				}
			default:
				break;
			}
		}
	}
	if (kbd_unsubscribe_int() == ERROR) {
		printf("Error subscribing to keyboard \n");
		return ERROR;
	}

	//go back to text mode

	if (vg_exit() == ERROR)
		return ERROR;

	return 0;


}

int video_test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	//inicialize graphics mode
	unsigned int fps = 60 / f;
	unsigned int interr = 0;
	short xspeed, yspeed;
	if (vg_init(V1024x768) == NULL)
		return ERROR;

	unsigned int negSpeedCounter = 0;
	unsigned int negSpeedInterr;
	bool nSpeed = false;
	if (s < 0) {
		nSpeed = true;
		negSpeedInterr = 0-s;
		s=1;
		xspeed = 1;
		yspeed = 1;
	}

	//It will move only on X axis unless there isnt a change in X
	if (xi != xf) {
		if (xi < xf) {
			yf = yi;
			yspeed = 0;
			xspeed = s;
		} else {
			yf = yi;
			yspeed = 0;
			xspeed = 0 - s;
		}

	} else {
		if (yi < yf) {
			xspeed = 0;
			yspeed = s;
		} else {
			xspeed = 0;
			yspeed = 0 - s;
		}
	}

	Sprite* sp = create_sprite(xpm, xi, yi, xspeed, yspeed);

	vg_drawSprite(sp);

	video_dump_fb();

	//exit condition

	int ipc_status, r;
	message msg;

	int irq_set = kbd_subscribe_int();

	int irq_set_timer = timer_subscribe_int();
	if (irq_set_timer == ERROR) {
		printf("Error subscribing to timer \n");
		return ERROR;
	}

	if (irq_set == ERROR) {
		printf("Error subscribing to keyboard \n");
		return ERROR;
	}

	while (key != ESC_KEY) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_read_buffer_c();
					handler();
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					interr++;
					if (interr % fps == 0) {
						if (nSpeed) {
							negSpeedCounter++;
							if (negSpeedCounter % negSpeedInterr == 0) {
								if (xi < xf || yi < yf) {
									if (sp->x < xf || sp->y < yf)
									{
										move_xpm(sp);
										video_dump_fb();
									}
								} else {
									if (xf < xi || yf < yi)
										if (sp->y > yf || sp->x > xf)
										{
											move_xpm(sp);
											video_dump_fb();
										}
								}
							}
						}
						if (xi < xf || yi < yf) {
							if (sp->x < xf || sp->y < yf)
							{
								move_xpm(sp);
								video_dump_fb();
							}
						} else {
							if (xf < xi || yf < yi)
								if (sp->y > yf || sp->x > xf)
								{
									move_xpm(sp);
									video_dump_fb();
								}
						}
					}
				}

			default:
				break;
			}
		}
	}
if (kbd_unsubscribe_int() == ERROR) {
	printf("Error subscribing to keyboard \n");
	return ERROR;
}
if(timer_unsubscribe_int() == ERROR)
{
	printf("Error subscribing to timer \n");
	return ERROR;
}

//go back to text mode
destroy_sprite( sp);

if (vg_exit() == ERROR)
return ERROR;

return 0;


}

int test_controller() {

	vbe_info_block_t vib_p;

	if (vbe_get_info_block(&vib_p) == ERROR) {
		printf("Error getting info block \n");
		return ERROR;
	}

	uint16_t *pntr = (uint16_t*) vib_p.Reserved;

	//print information

	printf("%d.%d\n", vib_p.VbeVersion >> 8, vib_p.VbeVersion & 0x00FF);

	while (*pntr != 0xFFFF) {
		printf("0x%x:", *pntr);
		pntr++;
	}

	printf("\n%d\n", vib_p.TotalMemory * 64);

	return 0;
}
