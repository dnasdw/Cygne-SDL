#ifndef _BOYCA_GL_H_
#define _BOYCA_GL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef	long int32;
typedef unsigned char bool8;

class BoycaGL 
{
public:
	BoycaGL();
	void InitGL(SDL_Surface *surface, int drawmethod, int scale, int gltexture);
	inline void SDL_GL_Enter2DMode(SDL_Surface *gbascreen);
	inline void SDL_GL_Leave2DMode();
	inline static int power_of_two(int input);
	inline GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord, int gltexture);
	void DrawLogoTexture(SDL_Surface *surface, int drawmethod, int gltexture);
    int  OpenGLScreenshot(char *filename);
	void FreeGL(void);
private:
	SDL_Surface *image;
	SDL_Surface *glbuffer;
	SDL_Surface *display;
	Uint16 pixel;
	int	size;
    GLuint scanlinetexture[4];
	static Uint8 *Delta;
};

#endif
