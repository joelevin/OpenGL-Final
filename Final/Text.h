//#include <gl/freeglut.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

class Text {
    public:
        Text();
        void DisplayText(char *);
        void SetPos(float, float, float);
        void SetFont(void *);
        void SetColor(float r, float g, float b);

    private:
        float x, y, z; // raster pos of text
        float r, g, b; // color values
        void *font;    // font (GLUT_BITMAP_*)
};