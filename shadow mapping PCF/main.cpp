#include<iostream>
#include<GL\glew.h>
#include<GL\glut.h>

#include"glsl.h"


#include"FboDepth.h"
using namespace std;
using namespace cwc;



float rot;
bool enable=true;



static void resize(int w,int h);









void keyboard(unsigned char key,int x, int y);
static void display()
{
	static int n;
static glShaderManager SM;
static glShader *shader;
if(!n)
{
shader=	SM.loadfromFile("vert.txt","frag.txt");

}
FboDepth d(1260,960,true);

	static double modelView[16];
		static double projection[16];
		
		// Moving from unit cube [-1,1] to [0,1]  
		const GLdouble bias[16] = {	
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};
		
		

	float l[3]={10,10,3};






	

	glMatrixMode(GL_MODELVIEW);
	d.startRender(1,40,75);
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); 
		glLoadIdentity();
	  gluLookAt(2,2,2,0,0,0,0,0,1);//light position
	 glPushMatrix();
	 glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);//capture de la matrice avant sinon la rotation affecte la matrice
		glCullFace(GL_FRONT);
	   glColor4f(1, 1, 1, 1);
	  glFrontFace(GL_CCW );
	   
	   glBegin(GL_QUADS);
	  glVertex2f(-10,-10);
	  glVertex2f(10,-10);
	  glVertex2f(10,10); 
	  glVertex2f(-10,10);
	   
		
		  glEnd();
		   glFrontFace(GL_CW );//methode eviter le self shadowing
		  glPopMatrix();
	  glColor4f(0, 1, 1, 1);
	glTranslatef(0,0,1); 
	glRotatef(rot,0,0,1);
	    glRotatef(90,1,0,0);
		
	glutSolidTeapot(1);
	  glColor4f(1, 1, 1, 1);

	
	
		
		d.endRender();
		glViewport(0,0,800,600);//remise viewport a la taille de la fenetre
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); 
		 glFrontFace(GL_CCW );
		

	 gluLookAt(3,4,4,0,0,0,0,0,1);
	 glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	glLoadIdentity();	
	glLoadMatrixd(bias);
	
	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
	glActiveTextureARB(GL_TEXTURE0);
	 d.bind();

	 shader->begin();
	 shader->setUniform1i("ShadowMap",0,shader->GetUniformLocation("ShadowMap"));
	 shader->setUniform3f("LightPosition",2,2,2,shader->GetUniformLocation("LightPosition"));
	 glCullFace(GL_BACK);
	  glColor4f(1, 1, 1, 1);
	   glFrontFace(GL_CCW );//methode eviter le self shadowing
	 glBegin(GL_QUADS);
	  glVertex2f(-10,-10);
	  glVertex2f(10,-10);
	  glVertex2f(10,10); 
	  glVertex2f(-10,10);
	   
		
		  glEnd();
		  glFrontFace(GL_CW );
              
		  glPopMatrix();
	  glColor4f(0, 1, 1, 1);
	glTranslatef(0,0,1); 
	glRotatef(rot,0,0,1);
	    glRotatef(90,1,0,0);
		
	glutSolidTeapot(1);
	  glColor4f(1, 1, 1, 1);





	d.unbind();

	shader->end();





n++;
	if(n>3)
		n=2;
	glLoadIdentity();
glutSwapBuffers();
}
static void resize(int w,int h)
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75.,(GLfloat)w/(GLfloat)h,0.2,100.0);
  


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


}






void init();
int main(int argc,char **argv)
{

glutInit(&argc,argv);
glutInitWindowSize(800,600);
glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);



glutCreateWindow(" shadow mapping");
GLenum err=glewInit();
if(err!=GLEW_OK)
{return 0;}
init();
glutKeyboardFunc(keyboard);
glutDisplayFunc(display);
glutReshapeFunc(resize);

glutMainLoop();// ne jamais oublier sinon rien
return 0;
}

void init()
{
     glShadeModel(GL_SMOOTH);
   

  glClearColor(0.0,0.0,0.0,0.0);
glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL); 
glEnable(GL_TEXTURE_2D);
glEnable(GL_CULL_FACE);





}

void keyboard(unsigned char key,int x, int y)
{

    switch(key) {
    case 27  :
    exit(0);
    break;
	case 'r':
		rot+=3;
		break;
case 'e':
		enable=!enable;
		break;
default:
break;

break;
    }
    glutPostRedisplay();

}
