#include "GLHelper.h"


GLHelper::GLHelper(void)
{
	mouseX = 0;
	mouseY = 0;
	WindowHandle = 0;
	w_width = 800;
	w_height = 600;
	frameCount = 0;

	/*camera = new Camera();
    camera->translate(0.f, 0.f, -1.f);*/
}


void GLHelper::InitGL(GLvoid){
	GLenum type;
	glutInitWindowSize(this->w_width, this->w_height);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
	type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	//glutCreateWindow(strcat(DEVELOPER,PROJNAME));
	glutCreateWindow(PROJNAME);

	glClearColor(0.0, 1.0, 0.0, 1.0);

	// enable depth  zbufferi ac
	ToggleDepth();

	glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
    
	this->createNewCamera();

	// obsolete
	// set poly mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLHelper::ToggleDepth(){
	if(glIsEnabled(GL_DEPTH_TEST)){
		glDisable(GL_DEPTH_TEST);
	}else{
		glEnable(GL_DEPTH_TEST);
	}
}

void GLHelper::KeyF(unsigned char key, int x, int y){
	switch (key) {
	case 27: // ESC
		exit(0);
		break;
		//case 'z':
		//case 'Z':
		//	UIHelper::getInstance().speedUp();
		//	break;
		//case 'x':
		//case 'X':
		//	UIHelper::getInstance().speedDown();
		//	break;
	}
}

void GLHelper::SpecialKeyF(int key, int x, int y){
	switch (key) {
		/*case GLUT_KEY_LEFT:
		UIHelper::getInstance().increaseOrbitX();
		break;
		case GLUT_KEY_RIGHT:
		UIHelper::getInstance().decreaseOrbitX();
		break;
		case GLUT_KEY_UP:
		UIHelper::getInstance().decreaseOrbitY();
		break;
		case GLUT_KEY_DOWN:
		UIHelper::getInstance().increaseOrbitY();
		break;
		case GLUT_KEY_PAGE_UP:
		UIHelper::getInstance().zoomIn();
		break;
		case GLUT_KEY_PAGE_DOWN:
		UIHelper::getInstance().zoomOut();
		break;*/
	}
}

void GLHelper::IdleFunction(GLvoid){
	glutPostRedisplay();
}

void GLHelper::ResizeFunction(int Width, int Height){
	//GLHelper that = GLHelper::getInstance();
	GLHelper::getInstance().w_width = Width;
	GLHelper::getInstance().w_height = Height;
	glViewport(0, 0, GLHelper::getInstance().w_width, GLHelper::getInstance().w_height);
}

void GLHelper::Mouse(int x, int y){
	//GLHelper that = GLHelper::getInstance();
	GLfloat deltaX = GLHelper::getInstance().mouseX - x;
	GLfloat deltaY = GLHelper::getInstance().mouseY - y;
	GLHelper::getInstance().currentCamera->rotate(deltaX, 0, 1, 0);
	GLHelper::getInstance().currentCamera->rotate(deltaY, 1, 0, 0);
	GLHelper::getInstance().mouseX = x;
	GLHelper::getInstance().mouseY = y;
	glutPostRedisplay();
}

void GLHelper::BindGUIFunctions(void){
	glutKeyboardFunc(KeyF);
	glutSpecialFunc(SpecialKeyF);
	glutReshapeFunc(ResizeFunction);
	// obsolete
	if(GLHelper::getInstance().DRAW == 0x00){
		throw CustomException("You have to set DRAW method before calling BindGUIFunctions.");
	}
	glutDisplayFunc(GLHelper::getInstance().DRAW);
	glutIdleFunc(IdleFunction);
	glutMotionFunc(Mouse);
}

//void GLHelper::DRAWConvention(void (__cdecl *function)()){
//	(*function)();
//}

void GLHelper::timerCallback(int value)
{
    if (0 != value) {
		char* TempString = (char*)
			malloc(512 + strlen(PROJNAME));
        
		sprintf(
                TempString,
                "%s: %d Frames Per Second @ %d x %d",
				PROJNAME,
                GLHelper::getInstance().frameCount * 4,
				GLHelper::getInstance().w_width,
                GLHelper::getInstance().w_height
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	
	GLHelper::getInstance().frameCount = 0;
	glutTimerFunc(250, GLHelper::timerCallback, 1);
}