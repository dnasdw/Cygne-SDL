/*
 ===================================================================================
	Cygne SDL  v 2.1a      (c) Caz, Niels Wagenaar 2002     niels@nwagenaar.demon.nl
 ===================================================================================

   NEC cpu core  by Bryan McPhail,Oliver Bergmann, Fabrice Frances and David Hedley
   Zlib  by  Jean-loup Gailly and Mark Adler

 ===================================================================================
*/
#include "include/getparamsdl.h"
#include <SDL.h>
#include "include/Usage.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include "include/globals.h"
#include "include/gfx.h"
#include "include/memory.h"
#include "include/start.h"

// Main include file for Cygne/SDL. Make sure you put the
// #defines and globals in here!
#include "include/mainsdl.h"

// defines for cygne SDL 
static int keydef[12];
static int joydef[4];
static bool exiting = false; /*are we running ?*/
static int fUseJoy = -1;
static int fUseDouble = -1;
static int fUseScanlines = -1;
long fBlitterMode = 0; /*Blit Mode*/
long fScreenSize = 1; /*Screen Scale*/
const char *fArgvFile;		/*argv file for SDL*/


int 		wsc;			/*color/mono*/
BOOL		f_load;			/*rom load request*/	
BOOL		f_stopped;		/*emulation stopped*/
DWORD		flipd;			/*screen flip*/
int		    wsShades;		/*colorset for mono games*/
DWORD		wsCycles;		/*executed cpu cycles(not machine)*/
BYTE		wsLine;			/*current scanline*/
DWORD		wsSkip;			/*if executed cycles > cycles per line (eg big MOVSW) then redraw wsSkip lines( should be wsSkipped ;))*/
DWORD		l_f,c_f;		/*lastframe time , current frame time*/
DWORD 		cycles_line=677; /*670*/
DWORD		rom_size;
BYTE 		vsync;

DWORD k_up1, k_down1, k_left1, k_right1, k_up2, k_down2, k_left2, k_right2, k_a, k_b, k_start, k_up1i, k_down1i, k_left1i, k_right1i;
DWORD k_up2i, k_down2i, k_left2i, k_right2i, k_ai, k_bi, k_starti, k_flipi, link_controls;

void init_param()
{
	int j; 
	for(j = 0; j < 11; j++) {
		keydef[j] = getparam("cygne.cfg", keyparam[j]);
	}
	
	for(j = 0; j < 3; j++) {
		joydef[j] = getparam("cygne.cfg", joyparam[j]);
	}
	
	fUseJoy = getparam("cygne.cfg", "cygne_joy_enabled");
	fUseDouble = getparam("cygne.cfg", "cygne_screen_double");
	fUseScanlines = getparam("cygne.cfg", "cygne_screen_scanlines");
	
	if(fUseDouble == 1 && fUseScanlines == 1) {
		puts("Scanlines and Double mode are enabled in config file, using default of scanline mode");
		fUseScanlines = -1;
	}
	
	if(fUseDouble) {
		fScreenSize = 2;
		fBlitterMode = 3;
	}
	
	if(fUseScanlines) {
		fScreenSize = 2;
		fBlitterMode = 2;
	}
}

void wsReset(void)
{
	int		u0,u1;
	nec_reset(NULL);				/* Reset CPU */
	memset(&wsRAM,0,65536);			/* Clear  RAM  (it's filled with 0 after reset)*/

	for(u0=0;u0<256;u0++)wsIO[u0]=startio[u0];
	for(u0=0;u0<16;u0++)for(u1=0;u1<16;u1++)wsCols[u0][u1]=0;
	for(u0=0;u0<0xc9;u0++)cpu_writeport(u0,startio[u0]);

    wsCycles=0;
	wsLine=0;
	
	wsSetVideo(0,TRUE);
	l_f=0;
	nec_set_reg(NEC_SS,0);
	nec_set_reg(NEC_SP,0x2000);
}

void wsExecuteLine()
{
/* Should be
   Line $00  - <Line  interrupt check>.........<HBL Interrupt check>....	
    ...
   Line $8F  - <Line  interrupt check>.........<HBL Interrupt check>....	
   Line $90  - <VBL Start Interrupt check>.<<Line  interrupt check>.........<HBL Interrupt check>....	
   Line $90  - <<Line  interrupt check>.........<HBL Interrupt check>....	
    ...
   Last Line - <<Line  interrupt check>.........<HBL Interrupt check>....<VBL End Interrupt check>
*/
	unsigned		uI;
	if(f_load){f_load=0;wsROMLoad(fArgvFile);}
	if(!f_stopped)
	{
		wsIO[2]=wsLine;
		wsCycles=nec_execute(cycles_line+(rand()&7));
		if(wsCycles>=cycles_line+cycles_line)
			wsSkip=wsCycles/cycles_line;
		else
			wsSkip=1;
		wsCycles%=cycles_line;
	    for(uI=0;uI<wsSkip;uI++)
		{
		   wsScanline();
		   wsLine++;
		   if(wsLine==144)wsMakeScr=1;

		}
		if(wsLine>158)
		{
			wsLine=0;
			{
				if((wsIO[0xb2]&32))/*VBLANK END INT*/ 
				{
					if(wsIO[0xa7]!=0x35)/*Beatmania Fix*/
					{
						wsIO[0xb6]&=~32;
						nec_int((wsIO[0xb0]+5)*4);
					}
				}
			}
		}
		wsIO[2]=wsLine;
		if(wsMakeScr)
		{
		
			if(wsIO[0xb2]&64) /*VBLANK INT*/
			{
				wsIO[0xb6]&=~64;
				nec_int((wsIO[0xb0]+6)*4);
			}
			fr=(fr+1)%frameskip;
			if(!fr)	
			{
				UpdateFrame(flipd);
			}
			if(vsync)
			{
				if(!l_f)l_f=SDL_GetTicks();
				c_f=SDL_GetTicks();
				while((c_f-l_f)<(12))c_f=SDL_GetTicks();
				l_f=c_f;
			}
			wsMakeScr=0;
		}
       if(wsIO[0xa4]&&(wsIO[0xb2]&128)) /*HBLANK INT*/
	   {
		  
		  if(!wsIO[0xa5])wsIO[0xa5]=wsIO[0xa4];
		  if(wsIO[0xa5]) wsIO[0xa5]--;
		  if((!wsIO[0xa5])&&(wsIO[0xb2]&128))
		  {
		
			  wsIO[0xb6]&=~128;
			  nec_int((wsIO[0xb0]+7)*4);
	
		  }
	  }
	  if((wsIO[0x2]==wsIO[0x3])&&(wsIO[0xb2]&16)) /*SCANLINE INT*/
	  {	
			wsIO[0xb6]&=~16;
			nec_int((wsIO[0xb0]+4)*4);
	  }	 
	}
}


void JoyMotion(unsigned long axis, long value)
{
	switch(axis) {
		case 0:
			value <= -16384 ? k_left1 = 1 : k_left1 = 0;
			value >= 16384 ? k_right1 = 1 : k_right1 = 0;
		break;
		case 1:
			value <= -16384 ? k_up1 = 1 : k_up1 = 0;
			value >= 16384 ? k_down1 = 1 : k_down1 = 0;
		default:
		break;
	}
}

void JoyButtonDown(unsigned long jbutton)
{
	if(jbutton == joydef[JOY_START]) k_start = 1;
	if(jbutton == joydef[JOY_A]) k_a = 1;
	if(jbutton == joydef[JOY_B]) k_b = 1;
}

void JoyButtonUp(unsigned long jbutton)
{
	if(jbutton == joydef[JOY_START]) k_start = 0;
	if(jbutton == joydef[JOY_A]) k_a = 0;
	if(jbutton == joydef[JOY_B]) k_b = 0;
}

// Wonderswan Key press function.
// Additional keys must be set in KeyUp!
void KeyDown(unsigned long key)
{
	char ttt[256];
	if(key == keydef[CYGNE_UP1]) k_up1 = 1;
	if(key == keydef[CYGNE_DOWN1]) k_down1 = 1;
	if(key == keydef[CYGNE_LEFT1]) k_left1 = 1;
	if(key == keydef[CYGNE_RIGHT1]) k_right1 = 1;
		
	if(key == keydef[CYGNE_UP2]) k_up2 = 1;
	if(key == keydef[CYGNE_DOWN2]) k_down2 = 1;
	if(key == keydef[CYGNE_LEFT2]) k_left2 = 1;
	if(key == keydef[CYGNE_RIGHT2]) k_right2 = 1;
		
	if(key == keydef[CYGNE_START]) k_start = 1;
	if(key == keydef[CYGNE_A]) k_a = 1;
	if(key == keydef[CYGNE_B]) k_b = 1;
	switch(key) 
	{
		case SDLK_1:
			cycles_line=391; sprintf(ttt, "Cycles/line -> 391"); SDL_WM_SetCaption(ttt, "Cygne");
		break;
		case SDLK_2:
			cycles_line=574; sprintf(ttt, "Cycles/line -> 574"); SDL_WM_SetCaption(ttt, "Cygne");
		break;
		case SDLK_3:
			cycles_line=837; sprintf(ttt, "Cycles/line -> 837"); SDL_WM_SetCaption(ttt, "Cygne");
		break;
		case SDLK_4:
			cycles_line=677; sprintf(ttt, "Cycles/line -> 677"); SDL_WM_SetCaption(ttt, "Cygne");
		break;
		case SDLK_TAB:
			flip_screen();
		break;
		case SDLK_F12:
			screenshot();
		break;
		case SDLK_ESCAPE:
			exiting = true;
		break;
		default:
		break;
	}
}

// Wonderswan Key release function.
// Additional keys must be set in KeyDown!
void KeyUp(unsigned long key)
{
	if(key == keydef[CYGNE_UP1]) k_up1 = 0;
	if(key == keydef[CYGNE_DOWN1]) k_down1 = 0;
	if(key == keydef[CYGNE_LEFT1]) k_left1 = 0;
	if(key == keydef[CYGNE_RIGHT1]) k_right1 = 0;
		
	if(key == keydef[CYGNE_UP2]) k_up2 = 0;
	if(key == keydef[CYGNE_DOWN2]) k_down2 = 0;
	if(key == keydef[CYGNE_LEFT2]) k_left2 = 0;
	if(key == keydef[CYGNE_RIGHT2]) k_right2 = 0;
		
	if(key == keydef[CYGNE_START]) k_start = 0;
	if(key == keydef[CYGNE_A]) k_a = 0;
	if(key == keydef[CYGNE_B]) k_b = 0;
	
	switch(key)
	{
		case SDLK_1:
			SDL_Delay(500);
			SDL_WM_SetCaption("Cygne", NULL);
		break;
		case SDLK_2:
			SDL_Delay(500);
			SDL_WM_SetCaption("Cygne", NULL);
		break;
		case SDLK_3:
			SDL_Delay(500);
			SDL_WM_SetCaption("Cygne", NULL);
		break;
		case SDLK_4:
			SDL_Delay(500);
			SDL_WM_SetCaption("Cygne", NULL);
		break;
		default:
		break;
	}
}

void CheckKeys()
{
	SDL_Event ev;
	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN:
			KeyDown(ev.key.keysym.sym);
		break;
		case SDL_KEYUP:
			KeyUp(ev.key.keysym.sym);
		break;
		case SDL_JOYBUTTONDOWN:
			JoyButtonDown(ev.jbutton.button);
		break;
		case SDL_JOYBUTTONUP:
			JoyButtonUp(ev.jbutton.button);
		break;
		case SDL_JOYAXISMOTION:
			JoyMotion(ev.jaxis.axis, ev.jaxis.value);
		break;
		case SDL_QUIT:
			exiting = true;
		break;
		}
	}
}




// Our Cygne-main function.
int main(int argc, char *argv[])
{
	if(argc <= 1) {
		Usage();
		return(true);
	}
	int i;
	FILE *in = NULL;
	wsc=1;
	screen_size=2;
	flipd=0;
	wsShades=0;
	wsCycles=0;
	wsLine=0;
	rom_size=128000000;
	wsMakeScr=0;
	fr=0;
	frameskip=1;
	wsVMode=-1;
	vsync=1;
	fScreenSize = 1;
	fBlitterMode = 1;
	static SDL_Joystick *stick;
    int joyport = 0;

    printf("%s\nThe free and OpenSource Bandai Wonderswan Colour emulator.\n", VERSIONINTERNAL);
    printf("Based upon the source tree off %s.\n", VERSION);
    printf("This emulator is released under the GPL license. Read LICENSE for details\n");
	
	init_param();
	
	for (i=0; (i < argc || argv[i] != NULL); i++) {
		
		if(strcmp(argv[i], "-double") == 0) {
			fScreenSize = 2;
			fBlitterMode = 3;
		}
		
		if(strcmp(argv[i], "-scanline") == 0) {
			fScreenSize = 2;
			fBlitterMode = 2;
		}
		
		if(strcmp(argv[i], "-joystick") == 0) {
			fUseJoy = 1;
		}
        
        // Joystick/Gamepad port selection upto 4 ports can be used.
		if(strcmp(argv[i], "-joyport") == 0) {
			fUseJoy = 1;
            joyport = atoi(argv[++i]);
            if (joyport > 3) joyport = 3; // Make sure we don't get above the 4th port.
		}
		
		if (strncmp(argv[i], "-h", 2) == 0) {
			Usage();
			exit(0);
		}
	}
	
	fArgvFile = strdup(argv[1]);
	in = fopen(fArgvFile, "rb");
	
	if(in == NULL) {
		printf("The file %s doesn't exist.\n", argv[1]);
		exit(0);
	}
	fflush(in);
	fclose(in);
	
	wsMakeTiles();
	
	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0 )
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(1);
	}
	
	if(fUseJoy) {
		printf("There are %d joysticks attached\n", SDL_NumJoysticks());
		printf("joystick(%d) is: %s\n", joyport, SDL_JoystickName(joyport)); // Get info from given Joystick at port
		stick = SDL_JoystickOpen(joyport); // Open the joystick port
		if (stick == NULL) {
			printf("Couldn't open joystick %d: %s\n", 0, SDL_GetError());
		}
	}

	start_dx();
	set_shades();
	f_load=TRUE;
	
	while(!exiting) {
		wsExecuteLine();
		CheckKeys();
	}
	saveSRAM();
	SDL_Delay(100);
	closedx();
	
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO | SDL_INIT_TIMER);

	SDL_Quit();
	return(0);
}
