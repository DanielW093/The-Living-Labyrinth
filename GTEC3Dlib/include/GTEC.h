
#ifndef _WIN32
#define sprintf_s snprintf
#include <GL/gl.h>
#include <GL/glu.h>
#include <sys/time.h>
#else
#include "../stdafx.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <SDL.h>
#include "SDL_mixer.h"
#include <string>

using namespace std;

#define RAD2DEG(x)	((double)(x) * 180.0 / 3.14159265359)
#define DEG2RAD(x)  ((double)(x) * 3.14159265359 / 180.0)

