/*
 ===================================================================================
		Cygne WIN  v 2.1a      (c) Dox 2002     dox@space.pl
 ===================================================================================

   NEC cpu core  by Bryan McPhail,Oliver Bergmann, Fabrice Frances and David Hedley
   Zlib  by  Jean-loup Gailly and Mark Adler

 ===================================================================================
*/

#ifdef __SDL__
#ifdef __QNX__
#define __GCC_BUILTIN
#endif
#include <string.h> // memset
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef bool BOOL;
#define TRUE true
#define FALSE false
#endif

extern BYTE		wsTCache[512*64];		  //tiles cache
extern BYTE		wsTCacheFlipped[512*64];  	  //tiles cache (H flip)
extern BYTE		wsTileRow[8];		  //extracted 8 pixels (tile row)
extern BYTE		wsTCacheUpdate[512];	  //tiles cache flags
extern BYTE		wsTCache2[512*64];		  //tiles cache
extern BYTE		wsTCacheFlipped2[512*64];  	  //tiles cache (H flip)
extern BYTE		wsTCacheUpdate2[512];	  //tiles cache flags
extern int		wsVMode;			  //Video Mode	
extern DWORD	wsMonoPal[16][4];
extern DWORD	wsColors[8];
extern DWORD	wsCols[16][16];

void wsSetVideo(int,int);
void wsMakeTiles(void);
void wsGetTile(DWORD,DWORD,int,int,int);
#ifndef __SDL__
void wsSetVideo(int, BOOL);
#endif
void wsScanline(void);
void screenshot(void);

extern DWORD		dx_r,dx_g,dx_b,dx_sr,dx_sg,dx_sb;
extern DWORD		dx_bits,dx_pitch,cmov,dx_linewidth_blit,dx_buffer_line;


