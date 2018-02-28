#include "i8042.h"

.data
.global _data
_data:
.long

.data
.global _reg
_reg:
.long

.text
.global _asm_sys
_asm_sys:

  xor %eax, %eax
  mov _reg, %al
  outb %al, $RTC_ADDR_REG

  inb $RTC_DATA_REG, %al
  mov %al, _data

  ret

.end _asm_sys
