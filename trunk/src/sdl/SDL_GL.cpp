#include "SDL_GL.h"
#include <stdlib.h>
// GBA screensize.
#define GBA_WIDTH   224
#define GBA_HEIGHT  144

#define GBA_HEIGHT_P2 256
#define GBA_WIDTH_P2  256

//Uint8 *BoycaGL::Delta = NULL;
static GLuint texture = 0;
static GLfloat texcoord[4];
static GLfloat texMinX, texMinY;
static GLfloat texMaxX, texMaxY;
static int x = 0;
static int y = 0;
static int w;
static int h;

BoycaGL::BoycaGL()
	: image(NULL), display(NULL)
{
}

void BoycaGL::InitGL(SDL_Surface *surface, int drawmethod, int scale, int gltexture)
{
        SDL_Rect area;
        int		width, height;
        
		this->display = surface;

		size = scale;

		printf("\nOpenGL driver information :\n");
		printf("\n");
		printf( "Vendor            : %s\n", glGetString( GL_VENDOR ) );
		printf( "Renderer          : %s\n", glGetString( GL_RENDERER ) );
		printf( "Version           : %s\n", glGetString( GL_VERSION ) );
		printf( "OpenGL drawmethod : ");

		switch( drawmethod ) {
			case 1 :
				printf("GL_QUAD rendering\n\n"); break;
            case 2 : 
				printf("SDL_OPENGLBLIT (only for TV-mode and Dot Matrox mode)\n\n"); break;
			default :
				printf("GL_TRIANGLE rendering\n\n"); break;
		}

        image = SDL_CreateRGBSurface(
			SDL_HWSURFACE, // | SDL_SRCCOLORKEY | SDL_SRCALPHA,
			power_of_two(GBA_WIDTH), power_of_two(GBA_HEIGHT),
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN 
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000);
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF);

#endif

        glViewport(0, 0, surface->w*size, surface->h*size);

        glGenTextures(1, &texture);
        
        /* Copy the surface into the GL texture image */
        area.x = 0;
        area.y = 0;
        area.w = surface->w;
        area.h = surface->h;
        SDL_BlitSurface(surface, &area, image, &area);        

        glBindTexture(GL_TEXTURE_2D, texture);

        if ( gltexture == 0 ) { 
		/* Standard pixel enlargement */
		    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);// Set Texture Max Filter
		    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);// Set Texture Min Filter
	    } else {
    		/* OpenGL blur effect */
    		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    	}

    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     256, 256,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);

		/* Use the surface width and height expanded to powers of 2 */
		width  = (power_of_two( GBA_WIDTH)  ) ;
		height = (power_of_two( GBA_HEIGHT)  ) ;

		texcoord[0] = 0.0f;			/* Min X */
		texcoord[1] = 0.0f;			/* Min Y */
		texcoord[2] = (GLfloat)surface->w / width;	/* Max X */
		texcoord[3] = (GLfloat)surface->h / height;	/* Max Y */

		/* Make texture coordinates easy to understand */
		texMinX = texcoord[0];
		texMinY = texcoord[1];
		texMaxX = texcoord[2];
		texMaxY = texcoord[3];

		w = surface->w*size;
 	    h = surface->h*size;
}

int BoycaGL::power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

void BoycaGL::DrawLogoTexture(SDL_Surface *gbascreen, int drawmethod, int gltexture)
{

    SDL_BlitSurface(display, NULL, image, NULL);

    glTexSubImage2D(GL_TEXTURE_2D,
		     0,
		     0,
		     0,
		     256, 256,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);

	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, gbascreen->w, gbascreen->h);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)gbascreen->w, (GLdouble)gbascreen->h, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture);
    
	// Some videocards seem to benefit more with the TRIANGLE_STRIP then
	// with the QUAD_STRIP drawing. People can select there own drawing
	// method with OpenGL ;)
	switch( drawmethod ) {
		case 1 :
            glBegin(GL_QUADS);
                glTexCoord2f(texMinX, texMinY);
                glVertex2f(x, y);
                glTexCoord2f(texMaxX, texMinY);
                glVertex2f(x + w , y);
                glTexCoord2f(texMaxX, texMaxY);
                glVertex2f(x + w , y + h );
                glTexCoord2f(texMinX, texMaxY);
                glVertex2f(x, y + h );
            glEnd();      

			break;
		default :
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
				glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
				glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
				glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
			glEnd();
			break;
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

    glDisable(GL_TEXTURE_2D);

    /* display and swap the buffers */
	glFlush();
	SDL_GL_SwapBuffers( );

}

void BoycaGL::FreeGL(void)
{

     SDL_FreeSurface(image);

}
