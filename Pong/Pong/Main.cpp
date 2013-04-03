#include "Common.h"
#include "GLHelper.h"
#include "Model.h"

Model *ball;

Model *capsule;

//Camera *camera;

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	ball->draw(GLHelper::getInstance().currentCamera);

	glutSwapBuffers();
	GLHelper::getInstance().increaseFrameCount();
}

int main(int arg, char*argv[]){
	try{
		GLHelper::getInstance().InitGL();

		ball = new Model("Ball.obj",
			"vert_shader.vert",
			"frag_shader.frag");

		GLHelper::getInstance().DRAW = Draw;
		GLHelper::getInstance().BindGUIFunctions();
		GLHelper::getInstance().timerCallback(0);
		glutMainLoop();
		return 0;
	}catch(CustomException& cex){
		cout << "ERROR!!" << endl;
		cout<< cex.what() << endl;
		exit(0);
	}catch(exception& ex){
		cout << "ERROR!!" << endl;
		cout<< ex.what() << endl;
		exit(0);
	}
}