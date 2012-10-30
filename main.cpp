#ifndef _INCLUDES_H_
#include "includes.h"
#endif

bool draw = false;

//Uses opengl's built in vertex methods ( glVertex2i makes a vertex that takes 2 ints, 2f takes 2 floats, 3 for 3d, etc...)
void draw_something( void ){


	glEnable(GL_BLEND);

	glEnable(GL_POLYGON_SMOOTH);

	//rgb colors for the polygon, using floats 0 - 1 instead of 0 - 255
	glColor3f(0.4, 0.8, 0.4);
	//denotes the start of a polygon, will then create a polygon using the vertices you give it
	glBegin(GL_POLYGON);
		glVertex2i( center_x - 100, center_y - 100 );
		glVertex2i( center_x + 100, center_y - 100 );
		glVertex2i( center_x + 100, center_y + 100 );
		glVertex2i( center_x - 100, center_y + 100 );
	glEnd();

	//How big the points are
	glPointSize( 4.0 );
	//smooth lines
	glEnable(GL_POINT_SMOOTH);


	glColor3f(0.8, 0.4, 0.4);
	//starts points instead of a polygon
	glBegin(GL_POINTS);
	glVertex2i( center_x, center_y );
	glEnd();

	glDisable(GL_BLEND);

	//flushes changes to the screen, really importante
	glFlush();
}

//continuously looping
void display( void )
{
	glClearColor (0.0, 0.0, 0.0, 0.0);	// set the clear buffer color to be black
    glClear(GL_COLOR_BUFFER_BIT);		// clear color buffer

    glMatrixMode (GL_PROJECTION);		// don't edit this line
	glLoadIdentity ();					// don't edit this line
	glOrtho (0, win_width,				// left, right
			 0, win_height,				// bottom, top
			 0, 1);						// near and far plane
	glMatrixMode (GL_MODELVIEW);		// don't edit this line

	glDisable(GL_DEPTH_TEST);			// since this is a 2d example
	if( draw) draw_something();

	glFlush();
}

void keyboard( unsigned char c, int x, int y  )
{
	switch (c) {
		case 27:
			exit(0);
			break;
		case 'a':
			draw = true;
			break;
		default:
			break;
	}


	glutPostRedisplay();
}

int main( int argc, char** argv )
{
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	// consts declared in include.h
	glutInitWindowSize( win_width, win_height );
	glutInitWindowPosition( win_pos_x, win_pos_y );
	glutCreateWindow("Start");

	//methods I was talking about
	glutDisplayFunc(display);
	glutKeyboardFunc( keyboard );

	//never leaves glutmainloop after it starts
	glutMainLoop();
	return 0;
}
