/*
 ===================================================================================
	Cygne SDL  v 2.1a      (c) Caz, Niels Wagenaar 2002     niels@nwagenaar.demon.nl
 ===================================================================================

   NEC cpu core  by Bryan McPhail,Oliver Bergmann, Fabrice Frances and David Hedley
   Zlib  by  Jean-loup Gailly and Mark Adler

 ===================================================================================
 SDL Drawing, Scanlines/Double Mode etc
 ===================================================================================
*/

#include <SDL/SDL.h>
#include "include/globals.h"
#include "include/gfx.h"

#define k_m 13
#define k_w 15
#define k_s 29

SDL_Surface *g_pDDSPrimary = NULL;
SDL_Surface *g_pDDSBuffer  = NULL;

DWORD						dx_bits,dx_pitch,cmov,dx_linewidth_blit,dx_buffer_line;
DWORD						dx_r,dx_g,dx_b,dx_sr,dx_sg,dx_sb;
DWORD						dx_r_old,dx_g_old,dx_b_old;
BYTE						dx_buffer[224*144*4];

BYTE header[54]={
66,77,54,122,1,0,0,0,0,0,54,0,
0,0,40,0,0,0,144,0,0,0,224,0,
0,0,1,0,24,0,0,0,0,0,0,122,
1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0};


BYTE BMPTab[224*144*4];


DWORD filecount=0;
int vmodel=0;

void screenshot(void)
{
	BYTE br,bg,bb;
	BYTE d0,d1,d2,d3;
	DWORD add=0;
	DWORD mucho;
	
	unsigned long maxy,maxx;
	if(!vmodel)
	{
		header[0x12]=0xe0;
		header[0x16]=0x90;
		maxx=224;
		maxy=144;
	}
	else
	{
		header[0x12]=0x90;
		header[0x16]=0xe0;
		maxy=224;
		maxx=144;
	}
	char fname[256];
	sprintf(fname,"SCR%.5ld.bmp",filecount++);
	FILE* fil=fopen(fname,"wb");
	fwrite(header,1,54,fil);
	
	unsigned long xx,yy;
	for(yy=0;yy<maxy;yy++)
	for(xx=0;xx<maxx;xx++)
	{
		
	
		if(dx_bits==2)
		{
			d0=dx_buffer[add++];
			d1=dx_buffer[add++];
			mucho=d0|(d1<<8);
		}
		else
		{
			d0=dx_buffer[add++];
			d1=dx_buffer[add++];
			d2=dx_buffer[add++];
			d3=dx_buffer[add++];
			mucho=d0|(d1<<8)|(d2<<16)|(d3<<24);
		}

		br=((mucho&dx_r_old)>>dx_sr)<<(8-dx_r);
		bg=((mucho&dx_g_old)>>dx_sg)<<(8-dx_g);
		bb=((mucho&dx_b_old)>>dx_sb)<<(8-dx_b);

		BMPTab[(xx+(maxy-yy-1)*maxx)*3]=bb;
		BMPTab[(xx+(maxy-yy-1)*maxx)*3+1]=bg;
		BMPTab[(xx+(maxy-yy-1)*maxx)*3+2]=br;
	}
	fwrite(BMPTab,1,224*144*3,fil);

	fclose(fil);
}

void set_shades (void)
{
	  DWORD		red,green,blue;
	  int		i;
	  
  	  switch(wsShades)
	  {
		case 0:
			for (i=0;i<16;i++)
			{
				red=(i*k_w+k_s)>>(8-dx_r);
				green=(i*k_w+k_s)>>(8-dx_g);
				blue=(i*k_w+k_s)>>(8-dx_b);
				colors[i]=(red<<dx_sr)|(green<<dx_sg)|(blue<<dx_sb);
			}
		break;
		case 1:
			for (i=0;i<16;i++)
			{
				red=(i*k_w+k_s)>>(8-dx_r);
				green=(i*k_m+k_s)>>(8-dx_g);
				blue=(i*k_m+k_s)>>(8-dx_b);
				colors[i]=(red<<dx_sr)|(green<<dx_sg)|(blue<<dx_sb);
			}
		break;
		case 2: 
			for (i=0;i<16;i++)
			{
				red=(i*k_m+k_s)>>(8-dx_r);
				green=(i*k_m+k_s)>>(8-dx_g);
				blue=(i*k_w+k_s)>>(8-dx_b);
				colors[i]=(red<<dx_sr)|(green<<dx_sg)|(blue<<dx_sb);
			}
		break;
		case 3:
			for (i=0;i<16;i++)
			{
				red=(i*k_m+k_s)>>(8-dx_r);
				green=(i*k_w+k_s)>>(8-dx_g);
				blue=(i*k_m+k_s)>>(8-dx_b);
				colors[i]=(red<<dx_sr)|(green<<dx_sg)|(blue<<dx_sb);
			}
		break;
	 }		
}


DWORD find1(DWORD data)
{
	DWORD	res=0;
	if(!data)  return 0;
	while(1) {  if(data&1)	return res;  data>>=1;  res++; }
}
				
DWORD count1(DWORD data)
{
	DWORD	res=0;
	for(int i=0;i<32;i++) {  if(data&1)   res++;  data>>=1; }
	return res;
}

void closedx(void)
{
	if(g_pDDSPrimary !=NULL) {
		SDL_FreeSurface(g_pDDSPrimary);
		g_pDDSPrimary = NULL;
	}
}

__inline Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;
    }
}

__inline void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

__inline void scaling( SDL_Surface *src, SDL_Surface *des, int width, int height, int size, int scanline )
{
    register int x=0,y=0, t1 = 0, t2 = 0;
    Uint16 pixel;

    if( (!scanline) && (size >= 2))
    {
	  for(y=0;y< (height); y++)
	  {
	     for(x=0;x<(width);x++)
	     {
			pixel = getpixel( src, x, y );
            for( t1 = 0; t1 < size ; t1++)
	        {
		       for(t2=0; t2 < size ; t2++)
		       {   
                  putpixel( des, ((x*size) + t2) , ((y*size) + t1), pixel);
               }
            }
	     }
	  }
    }
    else if ( scanline )
    {
	  for(y=0;y< (height); y++)
	  {
	     for(x=0;x<(width);x++)
	     {
	     
             pixel = getpixel( src, x, y );
//		if ( Scanline_core == 1 ) 
//		    putpixel( des, (x*size) , ( y*size) , pixel);
//		else {
		    
		    for( t2=0; t2<size; t2++)
		          putpixel( des, (x*size) + t2 , ( y*size) , pixel);
//		}
        	    
	     }
	  }
    }

}

__inline void UpdateFrame_h()
{
    SDL_Rect area;

    memcpy (g_pDDSBuffer->pixels, dx_buffer, g_pDDSBuffer->pitch*g_pDDSBuffer->h);

	switch(fBlitterMode) {
		case 1:
	        SDL_BlitSurface( g_pDDSBuffer, NULL, g_pDDSPrimary, NULL);
	        break;
		case 2:
		    scaling(g_pDDSBuffer, g_pDDSPrimary, 224, 144, 2, 1);
		    break;
		case 3:
		    scaling(g_pDDSBuffer, g_pDDSPrimary, 224, 144, 2, 0 );
		    break;
	}

	SDL_Flip(g_pDDSPrimary);
}

__inline void UpdateFrame_v()
{

    memcpy (g_pDDSBuffer->pixels, dx_buffer, g_pDDSBuffer->pitch*g_pDDSBuffer->h);

	switch(fBlitterMode) {
		case 1:
	 	    SDL_BlitSurface( g_pDDSBuffer, NULL, g_pDDSPrimary, NULL);
	 	    break;
		case 2: // double scanlines
		    scaling(g_pDDSBuffer, g_pDDSPrimary, 144, 224, 2, 1);
		break;
		case 3: // double stretch mode
		    scaling(g_pDDSBuffer, g_pDDSPrimary, 144, 224, 2, 0 );
		break;
	}
	
	SDL_Flip(g_pDDSPrimary);
}

__inline void UpdateFrame_h_asm()
{

	switch(fBlitterMode) {
		case 1:
			__asm__ __volatile__(".align 32
							 cld
							 rep
							 movsw"
							 :
							 :"S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"((g_pDDSPrimary->h * g_pDDSPrimary->pitch)>>1));
		break;
		case 2:
			__asm__ __volatile__("pusha
								 xorl %%eax, %%eax
								 movl $448, %%ebx 
								 emms
								 sum:
								 movq (%0, %%eax, 1), %%mm0
								 movq %%mm0, %%mm1
								 punpcklwd %%mm0, %%mm0
								 addl $8, %%eax
								 punpckhwd %%mm1,%%mm1
								 movq %%mm0, -16(%1, %%eax, 2)
								 cmpl %%ebx, %%eax
								 
								 movq %%mm1, -8(%1, %%eax, 2)
								 jnz sum
								 xorl %%eax, %%eax
								 
								 addl %2, %0
								 decl %%edx
								 
								 leal (%1, %2, 4), %1
								 jnz sum
								 emms
								 popa"
			:
			: "S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"(g_pDDSPrimary->pitch>>1), "d"(144));
		break;
		
		case 3:
			__asm__ __volatile__("pusha
								 xorl %%eax, %%eax
								 movl $896, %%ebx 
								 emms
								 sum3:
								 movq (%0, %%eax, 1), %%mm0
								 movq %%mm0, %%mm1
								 punpcklwd %%mm0, %%mm0
								 addl $8, %%eax
								 punpckhwd %%mm1,%%mm1
								 movq %%mm0, -16(%1, %%eax, 2)
								 cmpl %%ebx, %%eax
								 
								 movq %%mm1, -8(%1, %%eax, 2)
								 jnz sum3
								 xorl %%eax, %%eax
								 
								 addl %2, %0
								 decl %%edx
								 
								 leal (%1, %2, 4), %1
								 jnz sum3
								 emms
								 popa"
			:
			: "S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"(g_pDDSPrimary->pitch>>1), "d"(144));
		break;
	}
	
	SDL_Flip(g_pDDSPrimary);

}


__inline void UpdateFrame_v_asm()
{

	switch(fBlitterMode) {
		case 1:
			__asm__ __volatile__(".align 32
							 cld
							 rep
							 movsw"
							 :
							 :"S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"((g_pDDSPrimary->h * g_pDDSPrimary->pitch)>>1));
		break;
		case 2: // double scanlines
			__asm__ __volatile__("pusha
								 xorl %%eax, %%eax
								 movl $288, %%ebx 
								 emms
								 sumb:
								 movq (%0, %%eax, 1), %%mm0
								 movq %%mm0, %%mm1
								 punpcklwd %%mm0, %%mm0
								 addl $8, %%eax
								 punpckhwd %%mm1,%%mm1
								 movq %%mm0, -16(%1, %%eax, 2)
								 cmpl %%ebx, %%eax
								 
								 movq %%mm1, -8(%1, %%eax, 2)
								 jnz sumb
								 xorl %%eax, %%eax
								 
								 addl %2, %0
								 decl %%edx
								 
								 leal (%1, %2, 4), %1
								 jnz sumb
								 emms
								 popa"
			:
			: "S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"(g_pDDSPrimary->pitch>>1), "d"(224));
		break;
		case 3: // double stretch mode
			__asm__ __volatile__("pusha
								 xorl %%eax, %%eax
								 movl $576, %%ebx 
								 emms
								 sumc:
								 movq (%0, %%eax, 1), %%mm0
								 movq %%mm0, %%mm1
								 punpcklwd %%mm0, %%mm0
								 addl $8, %%eax
								 punpckhwd %%mm1,%%mm1
								 movq %%mm0, -16(%1, %%eax, 2)
								 cmpl %%ebx, %%eax
								 
								 movq %%mm1, -8(%1, %%eax, 2)
								 jnz sumc
								 xorl %%eax, %%eax
								 
								 addl %2, %0
								 decl %%edx
								 
								 leal (%1, %2, 4), %1
								 jnz sumc
								 emms
								 popa"
			:
			: "S"(dx_buffer), "D"(g_pDDSPrimary->pixels), "c"(g_pDDSPrimary->pitch>>1), "d"(224));
		break;
	}
	SDL_Flip(g_pDDSPrimary);
}





int start_dx_h(void)
{
	if(g_pDDSPrimary != NULL) {
		SDL_FreeSurface(g_pDDSPrimary);
		g_pDDSPrimary = NULL;
	}
		
	if(g_pDDSBuffer != NULL) {
		SDL_FreeSurface(g_pDDSBuffer);
		g_pDDSBuffer = NULL;
	}


	g_pDDSPrimary = SDL_SetVideoMode(224 * fScreenSize, 144 * fScreenSize, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);

	g_pDDSBuffer  = SDL_CreateRGBSurface( SDL_SWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA, 	
		    224, 
			144, 
            g_pDDSPrimary->format->BitsPerPixel,
			g_pDDSPrimary->format->Rmask, 
			g_pDDSPrimary->format->Gmask, 
			g_pDDSPrimary->format->Bmask, 
			g_pDDSPrimary->format->Amask );


	memset(g_pDDSPrimary->pixels, 0, g_pDDSPrimary->pitch * g_pDDSPrimary->h);
	memset(g_pDDSBuffer->pixels, 0, g_pDDSBuffer->pitch * g_pDDSBuffer->h);
	
	SDL_WM_SetCaption("Cygne/SDL", NULL);

	if(!g_pDDSPrimary) {
		puts("error");
		return 1;
	}
	
	if(!g_pDDSBuffer) {
		puts("error");
		return 1;
	}


//	dx_bits = g_pDDSPrimary->format->BitsPerPixel;
//	dx_r = g_pDDSPrimary->format->Rmask;
//	dx_g = g_pDDSPrimary->format->Gmask;
//	dx_b = g_pDDSPrimary->format->Bmask;
	dx_bits = g_pDDSBuffer->format->BitsPerPixel;
	dx_r = g_pDDSBuffer->format->Rmask;
	dx_g = g_pDDSBuffer->format->Gmask;
	dx_b = g_pDDSBuffer->format->Bmask;
	
	//if(g_pDDScanner != NULL) {
	//	SDL_FreeSurface(g_pDDScanner);
	//	g_pDDScanner = NULL;
	//}
	
	//if(fBlitterMode)
	//		g_pDDScanner = SDL_CreateRGBSurface(SDL_SWSURFACE, 224, 144,
	//						16, dx_r, dx_g, dx_b, 0);
		
	
//	dx_pitch=g_pDDSPrimary->pitch;
//	cmov=g_pDDSPrimary->pitch/4;
	dx_pitch=g_pDDSBuffer->pitch;
	cmov=g_pDDSBuffer->pitch/4;
	dx_bits/=8;
	dx_sr=find1(dx_r);
	dx_sg=find1(dx_g);
	dx_sb=find1(dx_b);

	dx_r_old=dx_r;
	dx_g_old=dx_g;
	dx_b_old=dx_b;


	dx_r=count1(dx_r);
	dx_g=count1(dx_g);
	dx_b=count1(dx_b);
	
//	dx_linewidth_blit=g_pDDSPrimary->pitch;
	dx_linewidth_blit=g_pDDSBuffer->pitch;
	dx_buffer_line=61*dx_bits;
	set_shades();
	return	0;
}

int start_dx_v(void)
{
	if(g_pDDSPrimary != NULL) {
		SDL_FreeSurface(g_pDDSPrimary);
		g_pDDSPrimary = NULL;
	}
	
	if(g_pDDSBuffer  != NULL) {
		SDL_FreeSurface(g_pDDSBuffer);
		g_pDDSBuffer = NULL;
	}


	g_pDDSPrimary = SDL_SetVideoMode(144*fScreenSize, 224*fScreenSize, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	g_pDDSBuffer  = SDL_CreateRGBSurface( SDL_SWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA, 	
		    144, 
			224, 
            g_pDDSPrimary->format->BitsPerPixel,
			g_pDDSPrimary->format->Rmask, 
			g_pDDSPrimary->format->Gmask, 
			g_pDDSPrimary->format->Bmask, 
			g_pDDSPrimary->format->Amask );
	
	memset(g_pDDSPrimary->pixels, 0, g_pDDSPrimary->pitch * g_pDDSPrimary->h);
	memset(g_pDDSBuffer->pixels, 0, g_pDDSBuffer->pitch * g_pDDSBuffer->h);
	
	SDL_WM_SetCaption("Cygne/SDL", NULL);
	if(!g_pDDSPrimary) {
		puts("Error setting SDL framebuffer\n");
		return 1;
	}

	if(!g_pDDSBuffer) {
		puts("Error setting SDL Buffer\n");
		return 1;
	}

	dx_bits = g_pDDSBuffer->format->BitsPerPixel;
	dx_r = g_pDDSBuffer->format->Rmask;
	dx_g = g_pDDSBuffer->format->Gmask;
	dx_b = g_pDDSBuffer->format->Bmask;
	
	dx_pitch=g_pDDSBuffer->pitch;
	cmov=g_pDDSBuffer->pitch/4;
	dx_bits/=8;
	dx_sr=find1(dx_r);
	dx_sg=find1(dx_g);
	dx_sb=find1(dx_b);
	
	dx_r_old=dx_r;
	dx_g_old=dx_g;
	dx_b_old=dx_b;

	dx_r=count1(dx_r);
	dx_g=count1(dx_g);
	dx_b=count1(dx_b);
	dx_linewidth_blit=g_pDDSBuffer->pitch;
	dx_buffer_line=61*dx_bits;
	set_shades();
	return 0;
}

int start_dx(void)
{
	if(flipd)
	 return start_dx_v();
    else
	 return start_dx_h();
}

void UpdateFrame(int i)
{
	vmodel=i;
	
	if(i)
		UpdateFrame_v();
	else
		UpdateFrame_h();
}

void flip_screen(void)
{
	flipd^=1;
	closedx();
	//set_size(1);
	start_dx();
	fr=0;
}
