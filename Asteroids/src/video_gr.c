#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "vbe.h"
#include "video_gr.h"
#include "graphics.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR			0xF0000000
#define H_RES             	1024
#define V_RES		 					  768
#define BITS_PER_PIXEL		  8

/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *double_buffer;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

unsigned getHorResolution() {
	return h_res;
}

unsigned getVerResolution()
{
	return v_res;
}

char* getGraphicsBuffer()
{
	return video_mem;
}

char* getDoubleBuffer()
{
	return double_buffer;
}

void swapBuffer()
{
	memcpy(video_mem,double_buffer,h_res*v_res * (bits_per_pixel / 8));
}

void* vg_init(unsigned short mode)
{
	struct reg86u r;

	r.u.w.ax = SET_VID; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = BIT(14)|mode; // set bit 14: linear framebuffer
	r.u.b.intno = INT_VBE;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	vbe_mode_info_t vmi_p;

	if(vbe_get_mode_info(mode, &vmi_p) == ERROR)
		return NULL;

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;

	/*Allow memory mapping*/

	int r1;
	struct mem_range mr;
	unsigned int vram_size;
	unsigned int vram_base = vmi_p.PhysBasePtr;

	vram_size = h_res*v_res * (bits_per_pixel / 8);
	double_buffer = malloc(vram_size);

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (r1 = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r1);

	/*Map memory*/

	video_mem = vm_map_phys(SELF, (void	*) mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
	panic("couldn't map video memory");

	return (void*) video_mem;
}

int vg_exit() {
	struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return ERROR;

  }
  return 0;
}

int vg_set_pixel(unsigned short x, unsigned short y, unsigned long color){

        char *video_mem_temp = video_mem;

        if (x < h_res && y < v_res) {

                video_mem_temp += (y * h_res + x) * (bits_per_pixel / 8);

                *video_mem_temp = color;

                return 0;

        }

        return ERROR;

}



