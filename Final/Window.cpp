//#include <GL/freeglut.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <math.h>
#import <QuartzCore/QuartzCore.h>
#import <ApplicationServices/ApplicationServices.h>
#include "Window.h"


Window::~Window()
{
    glutDestroyWindow(wndid);
}


void
Window::InitWindow(int *argc, char **argv, int w, int h, int px, int py)
{
	cam = new Camera();
    glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE); 
    glutInitWindowSize(w, h);
    glutInitWindowPosition(px, py);

}

void
Window::MakeWindow(char* title)
{
    wndid = glutCreateWindow(title);
    SetActive();
}

void
Window::MakeSubWindow(int parent, int x, int y, int width, int height)
{
    wndid = glutCreateSubWindow(parent, x, y, width, height);
    SetActive();
}

void
Window::DestroyWindow()
{
    glutDestroyWindow(wndid);
}

void
Window::SetActive()
{
    glutSetWindow(wndid);
}

int
Window::GetWindowID()
{
    return wndid;
}

void
Window::ShowCursor(bool b)
{
    if(b)
		glutSetCursor (GLUT_CURSOR_LEFT_ARROW);
	else
		glutSetCursor (GLUT_CURSOR_NONE);

}


void
Window::DisplayFunction(void (*display)() )
{
    glutDisplayFunc(display);    
}

void
Window::WindowInit(void (*init)() )
{
    init();    
}

void
Window::ReshapeFunction(void (*reshape)(int, int) )
{
    glutReshapeFunc(reshape);    
}

void
Window::KeyBoardFunction(void (*keyboard)(unsigned char ,int , int ))
{
    glutKeyboardFunc(keyboard);     
}

void
Window::SpecialKeyBoardFunction(void (*special_keys)(int ,int , int ))
{
    glutSpecialFunc(special_keys);     
}

void
Window::IdleFunction(void (*render)(void ))
{
    glutIdleFunc(render);   
}


Window::Camera::Camera()
{
    // set default speeds
//    up = 0.01;
//    down = -0.003;
//    left = right = 0.006;
    up = 0.1;
    down = -0.1;
    left = right = 0.006;
    strafe_left = -0.1;
    strafe_right = 0.1;
//    strafe_left = -0.003;
//    strafe_right = 0.003;

    // set position defaults
    eye_x, eye_y, eye_z = 0;
    center_x, center_y, center_z = 0;
    up_x, up_y, up_z = 0;
    xstrafe, ystrafe, zstrafe = 0;
}


void Window::Camera::SetPosition(float x, float y, float z, float xv, float yv, float zv,
                    float x_up, float y_up, float z_up)
{
    eye_x = x;
    eye_y = y;
    eye_z = z;

    center_x = xv;
    center_y = yv;
    center_z = zv;

    up_x = x_up;
    up_y = y_up;
    up_z = z_up;
}

void Window::Camera::Move(float direction)
{
    // look direction is view minus our position
    float xlookdirection = center_x - eye_x;
    float ylookdirection = center_y - eye_y;
    float zlookdirection = center_z - eye_z;

    float dp = 1 / (float)sqrt((xlookdirection * xlookdirection) + 
                               (ylookdirection * ylookdirection) +
                               (zlookdirection * zlookdirection));

    xlookdirection *= dp;
    ylookdirection *= dp;
    zlookdirection *= dp;

    UpdatePosition(xlookdirection, zlookdirection, direction);
}

void Window::Camera::CalculateStrafe()
{
    float xdir = center_x - eye_x;
    float ydir = center_y - eye_y;
    float zdir = center_z - eye_z;

    // normalize direction
    float dp = 1 / (float)sqrt((xdir * xdir) + (ydir * ydir) + (zdir * zdir));
    xdir *= dp;
    ydir *= dp;
    zdir *= dp;

    // get cross product of direction
    float xcross = (ydir * up_z) - (zdir * up_y);
    float ycross = (zdir * up_x) - (xdir * up_z);
    float zcross = (xdir * up_y) - (ydir * up_x);

    xstrafe = xcross;
    ystrafe = ycross;
    zstrafe = zcross;
}

void Window::Camera::Strafe(float direction)
{
    CalculateStrafe();
    UpdatePosition(xstrafe, zstrafe, direction);
}

void Window::Camera::UpdatePosition(float xdir, float zdir, float direction)
{
    GLfloat zUpperLimit = 80;
    GLfloat xUpperLimit = 21;
    GLfloat zLowerLimit = -2*50;
    GLfloat xLowerLimit = -21;
    // move on x and z axis
    if ((eye_x <= xUpperLimit && eye_x >= xLowerLimit) && (eye_z <= zUpperLimit && eye_z >= zLowerLimit))
    {
        eye_x += xdir * direction;
        eye_z += zdir * direction;
    }
    else
    {
        eye_x -= xdir * direction;
        eye_z -= zdir * direction;
    }

    // move view
    if ((eye_x <= xUpperLimit && eye_x >= xLowerLimit) && (eye_z <= zUpperLimit && eye_z >= zLowerLimit))
    {
        center_x += xdir * direction;
        center_z += zdir * direction;
    }
    else
    {
        eye_x -= xdir * direction;
        eye_z -= zdir * direction;
    }
}

void Window::Camera::RotateView(float angledir, float xspeed, float yspeed, float zspeed)
{
    float cosangle = (float)cos(angledir);
    float sinangle = (float)sin(angledir);

    float xlookdir = center_x - eye_x;
    float ylookdir = center_y - eye_y;
    float zlookdir = center_z - eye_z;

    float dp = 1 / (float)sqrt((xlookdir * xlookdir) + (ylookdir * ylookdir) +
                               (zlookdir * zlookdir));
    xlookdir *= dp;
    ylookdir *= dp;
    zlookdir *= dp;

    float xnewdir, ynewdir, znewdir;

    // calculate new x position
    xnewdir = (cosangle + (1 - cosangle) * xspeed) * xlookdir;
    xnewdir += ((1 - cosangle) * xspeed * yspeed - zspeed * sinangle) * ylookdir;
    xnewdir += ((1 - cosangle) * xspeed * zspeed + yspeed * sinangle) * zlookdir;

    // Calculate the new Y position
    ynewdir = ((1 - cosangle) * xspeed * yspeed + zspeed * sinangle) * xlookdir;
    ynewdir += (cosangle + (1 - cosangle) * yspeed) * ylookdir;
    ynewdir += ((1 - cosangle) * yspeed * zspeed - xspeed * sinangle) * zlookdir;

    // Calculate the new Z position
    znewdir = ((1 - cosangle) * xspeed * zspeed - yspeed * sinangle) * xlookdir;
    znewdir += ((1 - cosangle) * yspeed * zspeed + xspeed * sinangle) * ylookdir;
    znewdir += (cosangle + (1 - cosangle) * zspeed) * zlookdir;

    center_x = eye_x + xnewdir;
    center_y = eye_y + ynewdir;
    center_z = eye_z + znewdir;
}


void Window::Camera::Update()
{
    gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, up_x, up_y, up_z);
}



void Window::Camera::MoveForward()
{
    Move(up);
}

void Window::Camera::MoveBackward()
{
    Move(down);
}

void Window::Camera::StrafeLeft()
{
    Strafe(strafe_left);
}

void Window::Camera::StrafeRight()
{
    Strafe(strafe_right);
}

void Window::Camera::Rotate(int x, int y)
{
    int mid_x = 640 >> 1;
    int mid_y = 480 >> 1;

    // do nothing if we are in the middle
    if ((x == mid_x) && (y == mid_y))
        return;

    float ydirection = (float)(mid_x - x) / 1000.0f;
    float yrotation = (float)(mid_y - y) / 1000.0f;

    rotangle -= yrotation;
   /* if (rotangle > 1.0f) {
        rotangle = 1.0f;
        return;
    } else if (rotangle < -1.0f) {
        rotangle = -1.0f;
        return;
    }*/

    float xdir = center_x - eye_x;
    float ydir = center_y - eye_y;
    float zdir = center_z - eye_z;

    // get cross product of direction
    float xaxis = (ydir * up_z) - (zdir * up_y);
    float yaxis = (zdir * up_x) - (xdir * up_z);
    float zaxis = (xdir * up_y) - (ydir * up_x);

    // normalize direction
    float dp = 1 / (float)sqrt((xaxis * xaxis) + (yaxis * yaxis) + (zaxis * zaxis));
    xaxis *= dp;
    yaxis *= dp;
    zaxis *= dp;

    RotateView(yrotation, xaxis, yaxis, zaxis);
    RotateView(ydirection, 0, 1, 0);

    //SetCursorPos(mid_x, mid_y);
    
//    CGPoint p = CGPointMake(mid_x, mid_y);
//    CGWarpMouseCursorPosition(p);
    glutWarpPointer(mid_x, mid_y);
}

void Window::Camera::RotateLeft()
{
    RotateView(left, 0.0, 1.0, 0.0);
}

void Window::Camera::RotateRight()
{
    RotateView(right, 0.0, -1.0, 0.0);
}
