#ifndef __GET_PARAM_H__
#define __GET_PARAM_H__

#include <stdio.h>
extern "C"  {
int getparam(const char *file, const char *param);
void freeparam(void);

static char *keyparam[] = {
	"cygne_up1",
	"cygne_down1",
	"cygne_left1",
	"cygne_right1",
	"cygne_up2",
	"cygne_down2",
	"cygne_left2",
	"cygne_right2",
	"cygne_start",
	"cygne_a",
	"cygne_b",
	NULL
};
}
static char *joyparam[] = {
	"cygne_joy_start",
	"cygne_joy_a",
	"cygne_joy_b",
	NULL
};

enum { 
	CYGNE_UP1,
	CYGNE_DOWN1,
	CYGNE_LEFT1,
	CYGNE_RIGHT1,
	CYGNE_UP2,
	CYGNE_DOWN2,
	CYGNE_LEFT2,
	CYGNE_RIGHT2,
	CYGNE_START,
	CYGNE_A,
	CYGNE_B
};

enum {
	JOY_START,
	JOY_A,
	JOY_B
};

#endif