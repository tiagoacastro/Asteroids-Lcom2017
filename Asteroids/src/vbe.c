#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)
#define ERROR -1

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  mmap_t temp_map;
  struct reg86u r;

  if(lm_init() == NULL)
    return ERROR;

  if (lm_alloc(sizeof(vbe_mode_info_t), &temp_map) == NULL)
		return ERROR;

  r.u.w.ax = GET_MODE_INFO;               /*VBE get mode info	*/
	/*translate the buffer linear address to a far pointer	*/
	r.u.w.es = PB2BASE(temp_map.phys);    /*set a segment base*/
	r.u.w.di = PB2OFF(temp_map.phys);     /*set the offset accordingly*/
	r.u.w.cx = mode;
	r.u.b.intno = INT_VBE;
	if( sys_int86(&r) != OK ) { /*call BIOS	*/
		printf("get_mode_info: sys_int86() failed \n");
		return ERROR;
  }

  *vmi_p = *(vbe_mode_info_t *)temp_map.virtual;

  lm_free(&temp_map);

  return 0;
}
