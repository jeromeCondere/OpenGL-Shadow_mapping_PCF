#include "FboDepth.h"


FboDepth::FboDepth(int width,int height,bool shadow_map):W(width),H(height)
{
 glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    if(!shadow_map)
	{glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); }

   if( shadow_map)//pour faire la shadow map prise en compte par samper2dshadow dans le shader
   {
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); 
		
   }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, W, H, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
      glBindTexture(GL_TEXTURE_2D, 0);
	
	  
	 glGenFramebuffersEXT(1, &fbo);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

      
        glGenRenderbuffersEXT(1, &rbo);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, W, H);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, tex, 0);

      //specifie la fait que l'on rend seulement le depth buffer dans la depth texture!
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}


FboDepth::~FboDepth(void)
{

	 glDeleteFramebuffersEXT(1, &fbo);
        glDeleteRenderbuffersEXT(1, &rbo);
 		glDeleteTextures(1, &tex);
}
void FboDepth::startRender(float near, float far, double fovy)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, W, H);//met le viewport a la taille de la texture
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity();
    gluPerspective(fovy, (float)(W)/H, near, far);
    glMatrixMode(GL_MODELVIEW);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

		glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT); //pas besoin de liquider le color buffer!!
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 



}

void FboDepth::endRender()
{
	glMatrixMode(GL_MODELVIEW);
glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

      
 glBindTexture(GL_TEXTURE_2D, tex);
 glGenerateMipmapEXT(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0);

glLoadIdentity();
glPopMatrix();

glMatrixMode(GL_PROJECTION);
glPopMatrix();
  //remise de la matrice de projection initiale
	

glMatrixMode(GL_MODELVIEW);

glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //reactive le color buffer
glLoadIdentity();
}

void FboDepth::bind()
{glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, tex);



	
	



}
void FboDepth::unbind()
{glBindTexture(GL_TEXTURE_2D, 0);
}