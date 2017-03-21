#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shaderLoader.h"

/// http://angusforbes.com/blog/openglglsl-render-to-texture/
/// http://nullprogram.com/blog/2014/06/10/

static const GLfloat vertices[] = { 
					-1.0f, -1.0f, 0.0f,
					 1.0f, -1.0f, 0.0f,
					-1.0f,  1.0f, 0.0f,
					-1.0f,  1.0f, 0.0f,
					 1.0f, -1.0f, 0.0f,
					 1.0f,  1.0f, 0.0f,
    };

GLuint vbo;
GLuint vao;

GLuint programID = 0;
GLuint texProgramID = 0;

GLuint textureA;
GLuint textureB;
GLuint fboA;
GLuint fboB;

GLubyte* data;
GLubyte val;

int counter = 0;

GLuint ww = 768;
GLuint hh = 700;

GLuint texID;

void drawToScreen(GLuint, GLuint);

bool resetTexture = false;
bool stop = true;
//---------------------------------------------

void klawisz(GLubyte key, int x, int y)
{
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'r':
		resetTexture = !resetTexture;
		break;
	case ' ':
		stop = !stop;
		break;
	}
}

void idle()
{
	if(stop)
		glutPostRedisplay();
}
//int num=0;
void display(void) 
{
	if(resetTexture)
	{
		data = (GLubyte *) malloc(ww*hh*4*sizeof(GLubyte));
	
		for (int i = 0; i < ww * hh; i++) 
		{   
			val = 0; 
			data[i*4] = data[i*4+1] = data[i*4+2] = val;
			data[i*4+3] = 255;
		}

		//z na 1		niebieska
		data[54594*4] = data[54594*4+1] = data[54594*4+2] = data[54594*4+3] = 255;
		data[54593*4] = data[54593*4+1] = 153;
		data[54593*4+2] = data[54593*4+3] = 255; 
	
		// z na 0		//zielona
		data[36168*4] = data[36168*4+1] = 153;	///r,g
		data[36168*4+3] = 255;		//alpha
		data[36168*4+2] = 0;		//b
		data[36169*4] = data[36169*4+1] = data[36169*4+3] = 255;		//rg alpha
		data[36169*4+2] = 0;	//b

		/// z na 0.5	//szara
		data[36141*4] = data[36141*4+1] = 153;	///r,g
		data[36141*4+3] = 255;		//alpha
		data[36141*4+2] = 122;		//b
		data[36142*4] = data[36142*4+1] = data[36142*4+3] = 255;		//rg alpha
		data[36142*4+2] = 122;

		glBindTexture(GL_TEXTURE_2D, textureA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ww, hh, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glBindTexture(GL_TEXTURE_2D, textureB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ww, hh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	/*	glBindFramebuffer(GL_FRAMEBUFFER, fboA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureA, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, fboB);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureB, 0);
	*/	
		resetTexture = false;
		counter = 0;
	}
	if((counter++ % 2) == 0)
	{
		//printf("texA\n");
		glBindFramebuffer(GL_FRAMEBUFFER, fboA);
		glViewport(0,0,ww,hh);
		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureB);
		glUniform1i(texID,0);
			
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glDisableVertexAttribArray(0);

		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}	
	else
	{
		//printf("texB\n");
		glBindFramebuffer(GL_FRAMEBUFFER, fboB);
		glViewport(0,0,ww,hh);
		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureA);
		glUniform1i(texID,0);
			
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);

		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//printf("counter %d\n", counter);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,ww,hh);
	
	if((counter % 2) == 0)
		drawToScreen(texProgramID, textureA);
	else
		drawToScreen(texProgramID, textureB);

	glutSwapBuffers();

}

void drawToScreen(GLuint program, GLuint texture)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glUniform1i(texID,0);
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(ww, hh);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Langton's Ant");
   
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glewInit();

	glutKeyboardFunc(klawisz);
	
	programID = loadShaders("antVertexShader.glsl", "antFragmentShader.glsl");
	texProgramID = loadShaders("textureVertexShader.glsl", "textureFragmentShader.glsl");
	
	texID = glGetUniformLocation(programID, "tex");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
  
	data = (GLubyte *) malloc(ww*hh*4*sizeof(GLubyte));
	
	for (int i = 0; i < ww * hh; i++) 
	{   
		val = 0; 
		data[i*4] = data[i*4+1] = data[i*4+2] = val;
		data[i*4+3] = 255;
	}

	data[54594*4] = data[54594*4+1] = data[54594*4+2] = data[54594*4+3] = 255;
	data[54593*4] = data[54593*4+1] = 153;
	data[54593*4+2] = data[54593*4+3] = 255; 
	
	// z na 0		//zielona
	data[36168*4] = data[36168*4+1] = 153;	///r,g
	data[36168*4+3] = 255;		//alpha
	data[36168*4+2] = 0;		//b
	data[36169*4] = data[36169*4+1] = data[36169*4+3] = 255;		//rg alpha
	data[36169*4+2] = 0;	//b

	/// z na 0.5	//szara
	data[36141*4] = data[36141*4+1] = 153;	///r,g
	data[36141*4+3] = 255;		//alpha
	data[36141*4+2] = 122;		//b
	data[36142*4] = data[36142*4+1] = data[36142*4+3] = 255;		//rg alpha
	data[36142*4+2] = 122;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureA);
	glBindTexture(GL_TEXTURE_2D, textureA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ww, hh, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureB);
	glBindTexture(GL_TEXTURE_2D, textureB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ww, hh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenFramebuffers(1, &fboA);
    glBindFramebuffer(GL_FRAMEBUFFER, fboA);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureA, 0);

	glGenFramebuffers(1, &fboB);
    glBindFramebuffer(GL_FRAMEBUFFER, fboB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureB, 0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glutMainLoop();
    return 0;
}