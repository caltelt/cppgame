

#ifndef _INLCUDES_H_
#define _INCLUDES_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/
#endif


#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include <iostream>
#include <vector>
#include <stdlib.h>

// for debugging
//#define DEBUG

using namespace std;

//---------------------------------------------
// variables that i need for the entire span of
// my program
//---------------------------------------------
static const int win_width  = 800;
static const int win_height = 600;
static const int win_pos_x  = 150;
static const int win_pos_y  = 150;
static const int center_x	= win_width/2.0;
static const int center_y	= win_height/2.0;

// forward declaration (don't really need it)
//class Pt;




#endif
