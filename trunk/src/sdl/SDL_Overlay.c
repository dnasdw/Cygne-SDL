#include "BoycaOverlay.h"
#include <stdio.h>
#include <stdlib.h>

// GBA screensize.
#define GBA_WIDTH   224
#define GBA_HEIGHT  144

void Init_Overlay(SDL_Surface *gbascreen, int overlaytype)
{

	overlay = SDL_CreateYUVOverlay( GBA_WIDTH, GBA_HEIGHT, overlaytype, gbascreen);
	printf("Created %dx%dx%d %s %s overlay\n",overlay->w,overlay->h,overlay->planes,
			overlay->hw_overlay?"hardware":"software",
			overlay->format==SDL_YV12_OVERLAY?"YV12":
			overlay->format==SDL_IYUV_OVERLAY?"IYUV":
			overlay->format==SDL_YUY2_OVERLAY?"YUY2":
			overlay->format==SDL_UYVY_OVERLAY?"UYVY":
			overlay->format==SDL_YVYU_OVERLAY?"YVYU":
			"Unknown");


}

void Quit_Overlay(void)
{

    SDL_FreeYUVOverlay(overlay);

}

/* NOTE: These RGB conversion functions are not intended for speed,
         only as examples.
*/
inline void RGBtoYUV(Uint8 *rgb, int *yuv)
{
	int i;
#if 1 /* these are the two formulas that I found on the FourCC site... */
	yuv[0] = 0.299*rgb[0] + 0.587*rgb[1] + 0.114*rgb[2];
	yuv[1] = (rgb[2]-yuv[0])*0.565 + 128;
	yuv[2] = (rgb[0]-yuv[0])*0.713 + 128;
#else
	yuv[0] = (0.257 * rgb[0]) + (0.504 * rgb[1]) + (0.098 * rgb[2]) + 16;
	yuv[1] = 128 - (0.148 * rgb[0]) - (0.291 * rgb[1]) + (0.439 * rgb[2]);
	yuv[2] = 128 + (0.439 * rgb[0]) - (0.368 * rgb[1]) - (0.071 * rgb[2]);
#endif
	/* clamp values...if you need to, we don't seem to have a need */
	/*
	for(i=0;i<3;i++)
	{
		if(yuv[i]<0)
			yuv[i]=0;
		if(yuv[i]>255)
			yuv[i]=255;
	}
	*/
}

inline ConvertRGBtoYV12(SDL_Surface *s, SDL_Overlay *o)
{
	int x,y;
	int yuv[3];
	Uint8 *p,*op[3];

	SDL_LockSurface(s);
	SDL_LockYUVOverlay(o);

	/* Black initialization */
	/*
	memset(o->pixels[0],0,o->pitches[0]*o->h);
	memset(o->pixels[1],128,o->pitches[1]*((o->h+1)/2));
	memset(o->pixels[2],128,o->pitches[2]*((o->h+1)/2));
	*/

	/* Convert */
	for(y=0; y<s->h && y<o->h; y++)
	{
		p=s->pixels+s->pitch*y;
		op[0]=o->pixels[0]+o->pitches[0]*y;
		op[1]=o->pixels[1]+o->pitches[1]*(y/2);
		op[2]=o->pixels[2]+o->pitches[2]*(y/2);
		for(x=0; x<s->w && x<o->w; x++)
		{
			RGBtoYUV(p,yuv);
			*(op[0]++)=yuv[0];
			if(x%2==0 && y%2==0)
			{
				*(op[1]++)=yuv[2];
				*(op[2]++)=yuv[1];
			}
			p+=s->format->BytesPerPixel;
		}
	}

	SDL_UnlockYUVOverlay(o);
	SDL_UnlockSurface(s);
}

inline ConvertRGBtoIYUV(SDL_Surface *s, SDL_Overlay *o)
{
	int x,y;
	int yuv[3];
	Uint8 *p,*op[3];

	SDL_LockSurface(s);
	SDL_LockYUVOverlay(o);

	/* Black initialization */
	/*
	memset(o->pixels[0],0,o->pitches[0]*o->h);
	memset(o->pixels[1],128,o->pitches[1]*((o->h+1)/2));
	memset(o->pixels[2],128,o->pitches[2]*((o->h+1)/2));
	*/

	/* Convert */
	for(y=0; y<s->h && y<o->h; y++)
	{
		p=s->pixels+s->pitch*y;
		op[0]=o->pixels[0]+o->pitches[0]*y;
		op[1]=o->pixels[1]+o->pitches[1]*(y/2);
		op[2]=o->pixels[2]+o->pitches[2]*(y/2);
		for(x=0; x<s->w && x<o->w; x++)
		{
			RGBtoYUV(p,yuv);
			*(op[0]++)=yuv[0];
			if(x%2==0 && y%2==0)
			{
				*(op[1]++)=yuv[1];
				*(op[2]++)=yuv[2];
			}
			p+=s->format->BytesPerPixel;
		}
	}

	SDL_UnlockYUVOverlay(o);
	SDL_UnlockSurface(s);
}

inline ConvertRGBtoUYVY(SDL_Surface *s, SDL_Overlay *o)
{
	int x,y;
	int yuv[3];
	Uint8 *p,*op;

	SDL_LockSurface(s);
	SDL_LockYUVOverlay(o);

	for(y=0; y<s->h && y<o->h; y++)
	{
		p=s->pixels+s->pitch*y;
		op=o->pixels[0]+o->pitches[0]*y;
		for(x=0; x<s->w && x<o->w; x++)
		{
			RGBtoYUV(p,yuv);
			if(x%2==0)
			{
				*(op++)=yuv[1];
				*(op++)=yuv[0];
				*(op++)=yuv[2];
			}
			else
				*(op++)=yuv[0];

			p+=s->format->BytesPerPixel;
		}
	}

	SDL_UnlockYUVOverlay(o);
	SDL_UnlockSurface(s);
}

inline ConvertRGBtoYVYU(SDL_Surface *s, SDL_Overlay *o)
{
	int x,y;
	int yuv[3];
	Uint8 *p,*op;

	SDL_LockSurface(s);
	SDL_LockYUVOverlay(o);

	for(y=0; y<s->h && y<o->h; y++)
	{
		p=s->pixels+s->pitch*y;
		op=o->pixels[0]+o->pitches[0]*y;
		for(x=0; x<s->w && x<o->w; x++)
		{
			RGBtoYUV(p,yuv);
			if(x%2==0)
			{
				*(op++)=yuv[0];
				*(op++)=yuv[2];
				op[1]=yuv[1];
			}
			else
			{
				*op=yuv[0];
				op+=2;
			}

			p+=s->format->BytesPerPixel;
		}
	}

	SDL_UnlockYUVOverlay(o);
	SDL_UnlockSurface(s);
}

inline ConvertRGBtoYUY2(SDL_Surface *s, SDL_Overlay *o)
{
	int x,y;
	int yuv[3];
	Uint8 *p,*op;

	SDL_LockSurface(s);
	SDL_LockYUVOverlay(o);

	for(y=0; y<s->h && y<o->h; y++)
	{
		p=s->pixels+s->pitch*y;
		op=o->pixels[0]+o->pitches[0]*y;
		for(x=0; x<s->w && x<o->w; x++)
		{
			RGBtoYUV(p,yuv);
			if(x%2==0)
			{
				*(op++)=yuv[0];
				*(op++)=yuv[1];
				op[1]=yuv[2];
			}
			else
			{
				*op=yuv[0];
				op+=2;
			}

			p+=s->format->BytesPerPixel;
		}
	}

	SDL_UnlockYUVOverlay(o);
	SDL_UnlockSurface(s);
}

inline void Convert32bit(SDL_Surface *display)
{

		SDL_Surface *newsurf;
		SDL_PixelFormat format;

		format.palette=NULL;
		format.BitsPerPixel=32;
		format.BytesPerPixel=4;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		format.Rshift=0;
		format.Gshift=8;
		format.Bshift=16;
#else
		format.Rshift=24;
		format.Gshift=16;
		format.Bshift=8;
#endif
		format.Ashift=0;
		format.Rmask=0xff<<format.Rshift;
		format.Gmask=0xff<<format.Gshift;
		format.Bmask=0xff<<format.Bshift;
		format.Amask=0;
		format.Rloss=0;
		format.Gloss=0;
		format.Bloss=0;
		format.Aloss=8;
		format.colorkey=0;
		format.alpha=0;

		newsurf=SDL_ConvertSurface(display, &format, SDL_SWSURFACE);
		if(!newsurf)
		{
			fprintf(stderr, "Couldn't convert picture to 32bits RGB: %s\n",
							SDL_GetError());
			exit(1);
		}

        switch(overlay->format)
        {
		case SDL_YV12_OVERLAY:
			ConvertRGBtoYV12(newsurf,overlay);
			break;
		case SDL_UYVY_OVERLAY:
			ConvertRGBtoUYVY(newsurf,overlay);
			break;
		case SDL_YVYU_OVERLAY:
			ConvertRGBtoYVYU(newsurf,overlay);
			break;
		case SDL_YUY2_OVERLAY:
			ConvertRGBtoYUY2(newsurf,overlay);
			break;
		case SDL_IYUV_OVERLAY:
			ConvertRGBtoIYUV(newsurf,overlay);
			break;
		default:
			printf("cannot convert RGB picture to obtained YUV format!\n");
			exit(1);
			break;
	    }

}


inline void Draw_Overlay(SDL_Surface *display, int size)
{
        static SDL_Rect		  src;
        static SDL_Rect		  dest;

        src.x = 0;
        src.y = 0;
        src.w = GBA_WIDTH;
        src.h = GBA_HEIGHT;
        dest.x = 0;
        dest.y = 0;
        dest.w = GBA_WIDTH;
        dest.h = GBA_HEIGHT;
        
		SDL_LockYUVOverlay(overlay);
  
        Convert32bit(display);
        
		overlay_rect.x = 0;
		overlay_rect.y = 0;
		overlay_rect.w = GBA_WIDTH  * size;
		overlay_rect.h = GBA_HEIGHT * size;

		SDL_DisplayYUVOverlay( overlay, &overlay_rect);
		SDL_UnlockYUVOverlay(overlay);
}
