#include <stdio.h>
//#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "Text.h"

Text::Text()
{
    // default to white text
    SetColor(1.0, 1.0, 1.0);
    SetFont(GLUT_BITMAP_TIMES_ROMAN_10);
}

void
Text::SetPos(float a, float b, float c)
{
    x = a;
    y = b;
    z = c;
}

void
Text::SetFont(void *f)
{
    font = f;
}

void
Text::SetColor(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

void
Text::DisplayText(char *string)
{
    // save color and set new color
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glColor3f(r, g, b);
    glRasterPos3f(x, y, z);

    char *c;
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);

    // restore old color
    glPopAttrib();
}