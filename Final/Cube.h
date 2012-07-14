//#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

class Cube{

public:
	Cube();
	void SetPos(float x, float y, float z);
	void SetColor(float r, float g, float b);
    void SetBuffers(float x, float y, float z);

	float xbuffer, ybuffer, zbuffer;
	float speed;
	float xpos, ypos, zpos;
	float red, green, blue;

	bool Up;
	bool Right;
	bool erase;
};