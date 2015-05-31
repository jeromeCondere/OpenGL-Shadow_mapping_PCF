#pragma once
#include<GL\glew.h>
class FboDepth
{
public:
	FboDepth(int width,int height,bool shadow_map);
	~FboDepth(void);
	void startRender(float near, float far, double fovy);
	void  endRender();
	void bind();
	void unbind();

private :
	GLuint tex,rbo,fbo;
	
	int W,H;
};

