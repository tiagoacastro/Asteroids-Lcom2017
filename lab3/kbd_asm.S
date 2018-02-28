#include "i8042.h"

.data
.global _asmKey
_asmKey:
.long

.text
.global _asm_handler
_asm_handler:
  inb $STAT_REG, %al
	testb $OBF, %al
	loopz _asm_handler
  xor %eax, %eax
  inb $OUT_BUF, %al
  mov %eax, _asmKey
  ret
.end _asm_handler
