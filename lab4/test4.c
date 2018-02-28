#include "mouse.h"
#include <minix/com.h>
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>

int mouse_test_packet(unsigned short cnt){
  message msg;
  int ipc_status, r, ret, irq_set = mouse_subscribe_int();
  unsigned short i = 0;

  if(irq_set == ERROR){
    printf("Error subscribing to mouse \n");
    return ERROR;
  }

  if(enable_packets() == ERROR){
    printf("Error enabling packets \n");
    return ERROR;
  }

  while (i != cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.NOTIFY_ARG & irq_set) {
          if(packet_handler() == 1){
            packet_print();
            i++;
          }
        }
        break;
      default:
        break;
      }
    }
  }
  if(disable_data_reporting() == ERROR){
    printf("Error disabling data reporting \n");
    return ERROR;
  }
  if (empty_OUTBUF() == ERROR){
		printf("Error emptying OUTBUF\n");
		return ERROR;
	}
  if (mouse_unsubscribe_int() == ERROR){
		printf("Error unsubscribing to mouse\n");
		return ERROR;
	}
  return 0;
}

int mouse_test_remote(unsigned long period, unsigned short cnt){
	unsigned short i = 0;
		unsigned long response = 0;
		message msg;
		int ipc_status, r, ret ;
    unsigned long inicial_cmd;

    inicial_cmd = disable_mouse_interrupts();

    if(inicial_cmd == ERROR){
      printf("Error disabling mouse interrupts\n");
      return ERROR;
    }

		if (enable_data_reporting() == ERROR){
      printf("Error enabling data reporting\n");
			return ERROR;
    }

		if (disable_data_reporting() == ERROR){
      printf("Error disabling data reporting\n");
			return ERROR;
    }

		if (set_remote_mode() == ERROR){
      printf("Error seting remote mode\n");
			return ERROR;
    }

		while (i < cnt) {
			while(1)
			{
				if(writeToMouse(READ_DATA) == ERROR)
					return ERROR;
				if(wait_output(20))
					if(read_OUTBUF()==ACK)
						break;
				else
					return ERROR;
			}

			if (load_packets() == ERROR)
				return ERROR;

			packet_print();
			i++;
			tickdelay(micros_to_ticks(period * 1000));
		}

		if (enable_stream_mode() == ERROR){
      printf("Error enabling stream mode\n");
			return ERROR;
    }

		if(enable_mouse_interrupts(inicial_cmd)==ERROR){
      printf("Error enabling mouse interrupts\n");
			return ERROR;
    }

		if (empty_OUTBUF() == ERROR){
      printf("Error emptying OUTBUF\n");
			return ERROR;
    }

}

int mouse_test_async(unsigned short idle_time){
  int time_end = 0;
  message msg;
  unsigned short counter;
  int ipc_status, r, ret, irq_set = mouse_subscribe_int(), irq_set_timer = timer_subscribe_int();

  if(irq_set == ERROR){
    printf("Error subscribing to mouse \n");
    return ERROR;
  }

  if(irq_set_timer == ERROR){
    printf("Error subscribing to timer \n");
    return ERROR;
  }

  if(enable_packets() == ERROR){
    printf("Error enabling packets \n");
    return ERROR;
  }
  while (!time_end) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.NOTIFY_ARG & irq_set) {
          if(packet_handler()==1){
            packet_print();
          }
          counter = 0;
        }
        if (msg.NOTIFY_ARG & irq_set_timer) {
          counter = timer_int_handler(counter);
          if(counter % 60 == 0)
            if(counter == idle_time * 60)
            {
              time_end = 1;
              printf("%d seconds have passed - time has ended \n", idle_time);
            }
        }
        break;
      default:
        break;
      }
    }
  }
  if(disable_data_reporting() == ERROR){
    printf("Error disabling stream mode \n");
    return ERROR;
  }
  if (empty_OUTBUF() == ERROR){
    printf("Error emptying OUTBUF\n");
    return ERROR;
  }
  if (mouse_unsubscribe_int() == ERROR){
    printf("Error unsubscribing to mouse\n");
    return ERROR;
  }
  if (timer_unsubscribe_int() == ERROR){
    printf("Error unsubscribing to timer\n");
    return ERROR;
  }
  return 0;
}

int mouse_test_gesture(short length){
  message msg;
  int ipc_status, r, ret, irq_set = mouse_subscribe_int();
  short size = 0;

  if(irq_set == ERROR){
    printf("Error subscribing to mouse \n");
    return ERROR;
  }

  if(enable_packets() == ERROR){
    printf("Error enabling packets \n");
    return ERROR;
  }

  while (size < length) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.NOTIFY_ARG & irq_set) {
          if(packet_handler()==1){
            size = line_scaner();
            packet_print();
          }
        }
        break;
      default:
        break;
      }
    }
  }

  printf("exit condition has been met\n");

  if(disable_data_reporting() == ERROR){
    printf("Error disabling stream mode \n");
    return ERROR;
  }

  if (empty_OUTBUF() == ERROR){
    printf("Error emptying OUTBUF\n");
    return ERROR;
  }

  if (mouse_unsubscribe_int() == ERROR){
    printf("Error unsubscribing to mouse\n");
    return ERROR;
  }
  return 0;
}
