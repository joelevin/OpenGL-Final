//#include <gl/freeglut.h>        
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
//#include <OpenAL/alut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
using std::vector;
#include "Window.h"
#include "Cube.h"
#include "Text.h"
//#include "Camera.h"
#import <QuartzCore/QuartzCore.h>
#import <CoreAudio/CoreAudio.h>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")





bool keys[256]={false};

bool    fullscreen;   
float	rquad = 0.0; 
float scale = 2.0f;
int cube;
int cubeCounter = 0;
int beam;

std::vector<Cube> beamsLeft;
std::vector<Cube> beamsRight;
std::vector<Cube> jackpots;

int mainWindow;
int subWindow;

GLboolean fogFlag;
GLboolean lightingFlag;
GLboolean blendFlag;

float lastX, lastY;

Text *infoText = new Text();
char cubeText[25];

Window w;
void mouse_movement();





//ALCdevice* device;
//ALCcontext* context;
//ALuint Buffer;
//ALuint Source;
//// Position of the source sound.
//ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
//
//// Velocity of the source sound.
//ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
//
//
//// Position of the listener.
//ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
//
//// Velocity of the listener.
//ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
//
//// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
//ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
//
//
//void buildSound()
//{
////    ALCubyte DeviceName[] = "DirectSound3D";
////    device = alcOpenDevice(DeviceName);
//    
//    device = alcOpenDevice(NULL);
//    context = alcCreateContext(device, NULL);
//    alcMakeContextCurrent(context);
//    
//    alGetError();
//    
//}
//
//
//ALboolean loadALData()
//{
//    //Variables to load into
//    
//    ALenum format;
//    ALsizei size;
//    ALvoid* data;
//    ALsizei freq;
//    ALboolean loop;
//    
//    alGenBuffers(1, &Buffer);
//    if (alGetError() != AL_NO_ERROR)
//        return AL_FALSE;
//    
//    //ALUTloadWav
//    alBufferData(Buffer, format, data, size, freq);
//    //alutUnloadWAV
//    
//    
//    //Bind buffer with a source
//    alGenSources(1, &Source);
//    if (alGetError() != AL_NO_ERROR)
//        return AL_FALSE;
//    
//    alSourcei(Source, AL_BUFFER, Buffer);
//    alSourcef(Source, AL_PITCH, 1.0f);
//    alSourcef(Source, AL_GAIN, 1.0f);
//    alSourcefv(Source, AL_POSITION, SourcePos);
//    alSourcefv(Source, AL_VELOCITY, SourceVel);
//    alSourcei(Source, AL_LOOPING, AL_TRUE);
//    
//    if(alGetError() != AL_NO_ERROR)
//        return AL_FALSE;
//    
//    return AL_TRUE;
//}
//
//void setListenerValues()
//{
//    alListenerfv(AL_POSITION, ListenerPos);
//    alListenerfv(AL_VELOCITY, ListenerVel);
//    alListenerfv(AL_ORIENTATION, ListenerOri);
//}
//
//void destroySound()
//{
//    //Delete
//    alDeleteBuffers(1, &Buffer);
//    alDeleteSources(1, &Source);
//    alcMakeContextCurrent(NULL);
//    alcDestroyContext(context);
//    alcCloseDevice(device);
//
//}

void checkCollision()
{
    //Left beams
    for (int i = 0; i < beamsLeft.size(); i++)
    {
        if ((w.cam->eye_x >= (beamsLeft[i].xpos - beamsLeft[i].xbuffer))
            && 
            (w.cam->eye_x <= (beamsLeft[i].xpos + beamsLeft[i].xbuffer)))
        {
            if ((w.cam->eye_z >= (beamsLeft[i].zpos - beamsLeft[i].zbuffer))
                && 
                (w.cam->eye_z <= (beamsLeft[i].zpos + beamsLeft[i].zbuffer)))
            {
                w.cam->eye_x -= 0.1;
                w.cam->eye_z -= 0.1;
                
                w.cam->center_x -= 0.1;
                w.cam->center_z -= 0.1;
            }
        }
    }
    
    //Right beams
    for (int i = 0; i < beamsRight.size(); i++)
    {
        if ((w.cam->eye_x >= (beamsRight[i].xpos - beamsRight[i].xbuffer))
            && 
            (w.cam->eye_x <= (beamsRight[i].xpos + beamsRight[i].xbuffer)))
        {
            if ((w.cam->eye_z >= (beamsRight[i].zpos - beamsRight[i].zbuffer))
                && 
                (w.cam->eye_z <= (beamsRight[i].zpos + beamsRight[i].zbuffer)))
            {
                w.cam->eye_x -= 0.1;
                w.cam->eye_z -= 0.1;
                
                w.cam->center_x -= 0.1;
                w.cam->center_z -= 0.1;
            }
        }
    }
    
    //Jackpots
    if ((w.cam->eye_x >= (jackpots[0].xpos - jackpots[0].xbuffer))
        && 
        (w.cam->eye_x <= (jackpots[0].xpos + jackpots[0].xbuffer)))
    {
        if ((w.cam->eye_z >= (jackpots[0].zpos - jackpots[0].zbuffer))
            && 
            (w.cam->eye_z <= (jackpots[0].zpos + jackpots[0].zbuffer)))
        {
            w.cam->eye_x -= 0.1;
            w.cam->eye_z -= 0.1;
            
            w.cam->center_x -= 0.1;
            w.cam->center_z -= 0.1;
            jackpots[0].erase = TRUE;
            cubeCounter++;
        }
    }
    
    for(int i=0; i < jackpots.size(); i++)
	{
		if(jackpots[i].erase)
		{
			jackpots.erase(jackpots.begin()+i);
		}
	}

    

}

void buildLists()
{
	cube = glGenLists(1);
	glNewList(cube,GL_COMPILE);
    glPushMatrix();
    glScalef(scale, 1.0, scale);
	
	glBegin(GL_QUADS);								
        //Top
        glColor4f(0.84, 0.0, 0.84, 1.0);
		glVertex3f( 1.0, 1.0,-2.0);		
		glVertex3f(-1.0, 1.0,-2.0);		
		glVertex3f(-1.0, 1.0, 2.0);		
		glVertex3f( 1.0, 1.0, 2.0);
        //Bottom
		glVertex3f( 1.0,-1.0, 2.0);		
		glVertex3f(-1.0,-1.0, 2.0);		
		glVertex3f(-1.0,-1.0,-2.0);		
		glVertex3f( 1.0,-1.0,-2.0);	
        //Back
		glVertex3f( 1.0, 1.0, 2.0);			
		glVertex3f(-1.0, 1.0, 2.0);			
		glVertex3f(-1.0,-1.0, 2.0);			
		glVertex3f( 1.0,-1.0, 2.0);
        //Front
		glVertex3f( 1.0,-1.0,-2.0);		
		glVertex3f(-1.0,-1.0,-2.0);		
		glVertex3f(-1.0, 1.0,-2.0);		
		glVertex3f( 1.0, 1.0,-2.0);
        //Right
		glVertex3f(-1.0, 1.0, 2.0);		
		glVertex3f(-1.0, 1.0,-2.0);		
		glVertex3f(-1.0,-1.0,-2.0);		
		glVertex3f(-1.0,-1.0, 2.0);
        //Left
		glVertex3f( 1.0, 1.0,-2.0);			
		glVertex3f( 1.0, 1.0, 2.0);			
		glVertex3f( 1.0,-1.0, 2.0);			
		glVertex3f( 1.0,-1.0,-2.0);			
	glEnd();
    glPopMatrix();
	glEndList();
    
    beam = glGenLists(1);
    glNewList(beam, GL_COMPILE);
    glPushMatrix();
        glScalef(8.0, 3.0, 1.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glColor4f(0.84, 0.84, 0.84, 1.0);
    
            glColor3f(0.0, 1.0, 0.0);
            //Top
            glVertex3f( 1.0, 1.0,-2.0);		
            glVertex3f(-1.0, 1.0,-2.0);		
            glVertex3f(-1.0, 1.0, 2.0);		
            glVertex3f( 1.0, 1.0, 2.0);
            //Bottom
            glVertex3f( 1.0,-1.0, 2.0);		
            glVertex3f(-1.0,-1.0, 2.0);		
            glVertex3f(-1.0,-1.0,-2.0);		
            glVertex3f( 1.0,-1.0,-2.0);	
            //Back
            glVertex3f( 1.0, 1.0, 2.0);			
            glVertex3f(-1.0, 1.0, 2.0);			
            glVertex3f(-1.0,-1.0, 2.0);			
            glVertex3f( 1.0,-1.0, 2.0);
            //Front
            glVertex3f( 1.0,-1.0,-2.0);		
            glVertex3f(-1.0,-1.0,-2.0);		
            glVertex3f(-1.0, 1.0,-2.0);		
            glVertex3f( 1.0, 1.0,-2.0);
            //Right
            glVertex3f(-1.0, 1.0, 2.0);		
            glVertex3f(-1.0, 1.0,-2.0);		
            glVertex3f(-1.0,-1.0,-2.0);		
            glVertex3f(-1.0,-1.0, 2.0);
            //Left
            glVertex3f( 1.0, 1.0,-2.0);			
            glVertex3f( 1.0, 1.0, 2.0);			
            glVertex3f( 1.0,-1.0, 2.0);			
            glVertex3f( 1.0,-1.0,-2.0);		
        glEnd();
    glPopMatrix();
    glEndList();
}

void buildFog()
{
	GLfloat fogColor[4] = {0.84, 0.84, 0.84, 1.0};


	glEnable(GL_FOG);

	
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, 0.005);
    glHint (GL_FOG_HINT, GL_NICEST);
	glFogf (GL_FOG_START, -100.0);
    glFogf (GL_FOG_END, 25.0);

	glClearColor(0.84, 0.84, 0.84, 1.0);

}

void buildBeams()
{
    //5 right, offset by 10
    for (int i = 0; i <= 1; i++)
    {
        Cube beam;
        beam.SetPos(-14, -1.0, -60 + (i * 25));
        beamsRight.push_back(beam);
    }
    
    Cube beam;
    beam.SetPos(-14, -1.0, 0.0);
    beamsRight.push_back(beam);
    
    for (int i = 3; i <= 4; i++)
    {
        Cube beam;
        beam.SetPos(-14, -1.0, -40 + (i * 25));
        beamsRight.push_back(beam);
    }

    
    //4 left
    for (int i = 0; i <= 1; i++)
    {
        Cube beam;
        beam.SetPos(14, -1.0, -50 + (i * 25));
        beamsLeft.push_back(beam);
    }
    for (int i = 3; i <= 4; i++)
    {
        Cube beam;
        beam.SetPos(14, -1.0, -50 + (i * 25));
        beamsLeft.push_back(beam);
    }
}

void init()
{
    glShadeModel(GL_SMOOTH);	

	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

    glClearColor(0.0, 0.0, 0.0, 0.5);	

    glClearDepth(1.0);									
    glEnable(GL_DEPTH_TEST);	
	
    glDepthFunc(GL_LEQUAL);								
    glEnable(GL_COLOR_MATERIAL);  
    
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    w.cam->SetPosition(0.0, 0, -70,  0, 0, 0,  0, 1, 0);

	buildLists();	
    buildBeams();
	buildFog();
    
    Cube jackpot;
    jackpot.SetBuffers(1, 1, 1);
    jackpot.SetPos(0, -1, 80);
    jackpots.push_back(jackpot);
}




void keyhandler()
{
	if(keys[27])
		exit(0);

	if(keys['w'])
		w.cam->MoveForward();

	if(keys['a'])
		w.cam->RotateLeft();

	if(keys['d'])
		w.cam->RotateRight();

	if(keys['q'])
		 w.cam->StrafeLeft();

	if(keys['e'])
		 w.cam->StrafeRight();

	if(keys['s'])
		 w.cam->MoveBackward();
    if(keys['f'])
    {
        glGetBooleanv(GL_FOG, &fogFlag);
        if (fogFlag)
        {
            glDisable(GL_FOG);
        }
        else
        {
            glEnable(GL_FOG);
        }
    }
    if(keys['l'])
    {
        glGetBooleanv(GL_LIGHTING, &lightingFlag);
        if (lightingFlag)
        {
            glDisable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_LIGHTING);
        }
    }
    if(keys['b'])
    {
        glGetBooleanv(GL_BLEND, &blendFlag);
        if (blendFlag)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
        }
    }
}

void subWindowRender()
{
    glutSetWindow(subWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,1);
	glLoadIdentity();
	glPushMatrix();
    infoText->SetColor(1,0,0);
    infoText->SetFont(GLUT_BITMAP_HELVETICA_18);
    
    infoText->SetPos(-10,-1,-6);
    sprintf(cubeText,"Teapots hit: %d", cubeCounter);
    infoText->DisplayText(cubeText);
	glPopMatrix();
	glutSwapBuffers();
}


void render(void)   
{
    GLfloat ambient_and_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat spotlight[] = { 1.0, 1.0, 0.0, 0.0 };
    GLfloat position[] = { w.cam->eye_x, w.cam->eye_y, -1*(w.cam->eye_z), 1.0 };
    GLfloat positionFlashlight[] = { 0.0, 0.0, -1.0, 1.0};


	//mouse_movement();
	keyhandler();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);

    glLoadIdentity();	
    
    //Floor
	glPushMatrix();
        w.cam->Update();
        
        glBegin(GL_QUADS);	
            glColor3f( 0.84, 0.84, 0.84);	
            glVertex3f( 25, -2.0,-100);	

            glVertex3f( 25, -2.0, 100);			

            glVertex3f( -25,-2.0, 100);	

            glVertex3f( -25,-2.0,-100);		
        glEnd();
                
    glPopMatrix();
    
    
    //Trench
    glPushMatrix();
    
        w.cam->Update();
    
//        for (int i = 1; i < 5; i++)
//        {
//
//            glPushMatrix();
//            
//                glTranslatef(0.0, 0.0, -3.0*float(i));
//                glEnable(GL_POLYGON_OFFSET_FILL);
//                glPolygonOffset(-1.0, 1.0);
//                glRotatef(rquad,1.0,1.0,0.0);
//                glCallList(cube);
//                glRotatef(rquad,0.0,1.0,0.0);
//                glDisable(GL_POLYGON_OFFSET_FILL);
//                glColor3f(0.0, 0.0, 0.0);
//                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//                glCallList(cube);
//                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//                
//            glPopMatrix();
//        }
//
//        glTranslatef(-6.0, 0.0, 12.1);
    
        //Left beams
        for (int i = 1; i < 3; i++)
        {
            glPushMatrix();
                glTranslatef(14, -1.0, -25.0 * float(i));
                glCallList(beam);
            glPopMatrix();
        }

        for (int i = 1; i < 3; i++)
        {
            glPushMatrix();
                glTranslatef(14, -1.0, 25.0 * float(i));
                glCallList(beam);
            glPopMatrix();
        }

        
        
//        //Right beams
        for (int i = 1; i < 3; i++)
        {
            glPushMatrix();
                glTranslatef(0.0, 0.0, -10);
                glTranslatef(-14, -1.0, -25.0 * float(i));
                glCallList(beam);
            glPopMatrix();
        }
    
        glPushMatrix();
            glTranslatef(-14, -1.0, 0);
            glCallList(beam);
        glPopMatrix();
        
        for (int i = 1; i < 3; i++)
        {
            glPushMatrix();
                glTranslatef(0.0, 0.0, 10);
                glTranslatef(-14, -1.0, 25.0 * float(i));
                glCallList(beam);
            glPopMatrix();
        }
        
        //Left wall
        for (int i = 1; i < 17; i++)
        {
            glPushMatrix();
                glTranslatef(24.0, -1.0, -6.0 * float(i));
                glCallList(cube);
            glPopMatrix();
        }	
    
        glPushMatrix();
            glTranslatef(24.0, -1.0, 0.0);
            glCallList(cube);
        glPopMatrix();
    
        for (int i = 1; i < 17; i++)
        {
            glPushMatrix();
                glTranslatef(24.0, -1.0, 6.0 * float(i));
                glCallList(cube);
            glPopMatrix();
        }	
    
        //Right wall
        for (int i = 1; i < 17; i++)
        {
            glPushMatrix();
            glTranslatef(-24.0, -1.0, -6.0 * float(i));
            glCallList(cube);
            glPopMatrix();
        }	
    
        glPushMatrix();
            glTranslatef(-24.0, -1.0, 0.0);
            glCallList(cube);
        glPopMatrix();
        
        for (int i = 1; i < 17; i++)
        {
            glPushMatrix();
                glTranslatef(-24.0, -1.0, 6.0 * float(i));
                glCallList(cube);
            glPopMatrix();
        }	

    glPopMatrix();
    
    //Jackpot
    glPushMatrix();
        w.cam->Update();
        glTranslatef(0, -1, 80);
        for (int i = 0; i < jackpots.size(); i++)
        {
            glColor3f(1.0, 0.8, 0);
            glRotatef(rquad, 0, 1, 0);
            glutSolidTeapot(1);
        }
    glPopMatrix();
 
	rquad -= 0.6;
    
    checkCollision();
    
    glutSwapBuffers ( );
    //subWindowRender();
}


void subInit()
{
    glShadeModel(GL_SMOOTH);
	glClearColor(1,1,1,0.5);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h)
{   
    glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);     
	glLoadIdentity();               
	
	if (h == 0) h = 1;
	gluPerspective(80, (float)w/(float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);      
	glLoadIdentity();        
    
    glutSetWindow(subWindow);
	glutPositionWindow(0,(h-50));
	glutReshapeWindow(w+10,60);
	float ratio=1.0*(w+10)/60;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w+10,60);
	gluPerspective(45,ratio, 1.0,1000);
	glMatrixMode(GL_MODELVIEW);
}




void keyboardUp(unsigned char key, int x, int y)
{
	 keys[key]=false;
}

void keyboardDown(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		default:
			 keys[key]=true;
	}
}

void special_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
		case GLUT_KEY_F1:			
			fullscreen = !fullscreen;       
			if (fullscreen) glutFullScreen(); 
			else glutReshapeWindow(500, 500);   
			break;
		default:
			break;
	}
}

void mouse_movement(int x, int y)
{
    int diffX = x - lastX;
    int diffY = y - lastY;
    
    lastX = x;
    lastY = y;
    
    
    w.cam->Rotate(x, y);
}

int main(int argc, char** argv)
{
    w.InitWindow(&argc, argv,1280, 800,0,0);
    w.MakeWindow("Trench Run");
    mainWindow = w.GetWindowID();
	w.DisplayFunction(render);
	w.ReshapeFunction(reshape);
    w.ShowCursor(false);
	w.IdleFunction (render);

	init(); 
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(special_keys);   
	glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouse_movement);
    
//    w.MakeSubWindow(mainWindow, 1280-500, 0, 500, 50);
    w.MakeSubWindow(mainWindow, 0, 450, 510, 60);
    subWindow = w.GetWindowID();
    w.DisplayFunction(subWindowRender);
    subInit();
    

	glutMainLoop();
	return 0;
}