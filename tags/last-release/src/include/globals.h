/*
 ===================================================================================
		Cygne WIN  v 2.0      (c) Dox 2001     dox@space.pl
 ===================================================================================

   NEC cpu core  by Bryan McPhail,Oliver Bergmann, Fabrice Frances and David Hedley
   Zlib  by  Jean-loup Gailly and Mark Adler

 ===================================================================================
*/
#ifdef __SDL__
#include <string.h> // memset

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#ifndef TARGET_WIN32 // Win32/Cygwin and Win32/Mingw allready has bool defined.
	typedef bool BOOL;
#endif

#endif

#define		cpu_readop cpu_readmem20	
#define		cpu_readop_arg cpu_readmem20	
enum {
	NEC_IP=1, NEC_AW, NEC_CW, NEC_DW, NEC_BW, NEC_SP, NEC_BP, NEC_IX, NEC_IY,
	NEC_FLAGS, NEC_ES, NEC_CS, NEC_SS, NEC_DS,
	NEC_VECTOR, NEC_PENDING, NEC_NMI_STATE, NEC_IRQ_STATE};


extern		DWORD k_up1;
extern		DWORD k_down1;
extern		DWORD k_left1;
extern		DWORD k_right1;
extern		DWORD k_up2;
extern		DWORD k_down2;
extern		DWORD k_left2;
extern		DWORD k_right2;
extern		DWORD k_a;
extern		DWORD k_b;
extern		DWORD k_start;
extern		DWORD k_up1i;
extern		DWORD k_down1i;
extern		DWORD k_left1i;
extern		DWORD k_right1i;
extern		DWORD k_up2i;
extern		DWORD k_down2i;
extern		DWORD k_left2i;
extern		DWORD k_right2i;
extern		DWORD k_ai;
extern		DWORD k_bi;
extern		DWORD k_starti;
extern		DWORD k_flipi;
extern		DWORD link_controls;
extern		DWORD colors[16];
extern		DWORD fr;
extern		DWORD frameskip;
extern		char path_app[1024];
extern		char path_save[1024];
extern		char path_sram[1024];
extern		char path_rom[1024];
extern		DWORD l_f,c_f;
extern		DWORD wsMakeScr;
extern		BYTE vsync;
#ifndef __SDL__
extern		HINSTANCE instance;
#endif
#ifndef TARGET_WIN32
     extern		BOOL f_load;
     extern		BOOL f_stopped;
#else // Needed since bool and BOOL are defined on Cygwin/Mingw32
     extern		bool f_load;
     extern		bool f_stopped;
#endif
extern		DWORD flipd;
extern		int	wsShades;
extern		BYTE dx_buffer[224*144*4];
#ifndef __SDL__
extern		HWND window;
#endif
extern		char dirname[2048];
extern		DWORD wsCycles;
extern		BYTE wsLine;
extern		DWORD rom_size;
extern		int screen_size;
extern		DWORD crc;
extern		int wsc;
extern		const DWORD tabkey[47];
	

void nec_set_reg(int,unsigned);
void wsReset(void);
void wsSaveState(void);
void wsLoadState(void);
#ifdef __SDL__
void wsROMLoad(const char *filenameTo);
#else
void wsROMLoad(HWND);
#endif
void saveSRAM(void);
#ifndef __SDL__
BOOL CALLBACK cproc(HWND,UINT,WPARAM,LPARAM);
#endif
void WriteRegistry(void);
void ReadRegistry(void);
void closedx(void);
BYTE  cpu_readmem20(DWORD);
void init_mem(void);
void deinit_mem(void);
void wsDefaultKeys();
void refresh_menu_colors(void);
void set_size(int);
void flip_screen(void);
void WriteRegistry(void);
int nec_execute(int cycles);	
unsigned nec_get_reg(int regnum);
void nec_reset (void *param);
void cpu_writemem20(DWORD adr,BYTE co);
BYTE cpu_readport(BYTE adr);
void cpu_writeport(DWORD adr,BYTE co);
#ifdef __SDL__
void UpdateFrame(int i);
#else
void UpdateFrame(HWND,int);
#endif
int start_dx(void);
void set_shades(void);
void nec_int(DWORD wektor);
void make_screen(void);
#ifdef __SDL__
void UpdateFrame(int i);
void KeyDown(unsigned long key);
void KeyUp(unsigned long key);
extern long fBlitterMode;
extern long fScreenSize;
#else
void UpdateFrame(HWND,int);
#endif
int start_dx(void);
void set_shades(void);
void nec_int(DWORD wektor);
void make_screen(void);
