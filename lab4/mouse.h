#include "i8042.h"

//function declarations
int mouse_subscribe_int(void);
int mouse_unsubscribe_int();
int enable_packets();
int disable_data_reporting();
int enable_stream_mode();
unsigned long read_OUTBUF();
int writeToMouse(unsigned long cmd);
int writeToKBC(unsigned long reg, unsigned long cmd);
int packet_handler();
void packet_print();
int empty_OUTBUFF();
int timer_int_handler(int counter);
int timer_subscribe_int(void);
int timer_unsubscribe_int();
short line_scaner();
unsigned long disable_mouse_interrupts();
int enable_mouse_interrupts(unsigned long cmdb);
int wait_output(int waitTime);
int set_remote_mode();
int load_packets();
int kbd_subscribe_int(void);
int kbd_unsubscribe_int();
