#include <stdio.h>
//#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "Cube.h"



Cube::Cube()
{
	SetColor(1.0,0.0,0.0);
	SetPos(-4.0, 0.0, -6.0);
	Up =false;
	speed=0.005;
	xbuffer=16.0;
    ybuffer=3.0;
    zbuffer=2.0;
	erase=false;
}

void Cube::SetColor(float r, float g, float b)
{
	red=r;
	green=g;
	blue=b;
}

void Cube::SetPos(float x, float y, float z)
{
	xpos=x;
	ypos=y;
	zpos=z;
}

void Cube::SetBuffers(float x, float y, float z)
{
    xbuffer = x;
    ybuffer = y;
    zbuffer = z;
}