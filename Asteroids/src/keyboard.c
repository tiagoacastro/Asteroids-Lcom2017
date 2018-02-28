#include "keyboard.h"
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>

int hook_id_kbd = 6;
extern unsigned long asmKey;
unsigned long asm_handler();

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

void kbd_read_buffer_asm()
{
  asm_handler();
  key = asmKey;
}
