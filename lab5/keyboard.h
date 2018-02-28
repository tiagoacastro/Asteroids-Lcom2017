int kbd_subscribe_int(void);

int kbd_unsubscribe_int();

int handler();

int handler_poll();

int kbd_read_buffer_c();

int kbd_read_buffer_asm();

int kbd_issue_cmd(unsigned long cmd);

unsigned long kbd_read_poll();
unsigned long kbd_read_out_buff();
int kbd_write_in_buff(unsigned long cmd);

typedef int bool;
#define true 1
#define false 0

unsigned long key;
int master_counter;
