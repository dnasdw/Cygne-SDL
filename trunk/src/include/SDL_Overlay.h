#ifndef _BOYCA_OVERLAY_H_
#define _BOYCA_OVERLAY_H_

#include <SDL/SDL.h>

void Init_Overlay(SDL_Surface *surface, int overlaytype);
void Quit_Overlay(void);
void Draw_Overlay(SDL_Surface *surface, int size);
SDL_Overlay	*overlay;
SDL_Rect overlay_rect;

#endif
