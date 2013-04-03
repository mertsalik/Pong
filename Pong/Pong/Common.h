// FOR THE OPENGL3x ****
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include "gl_driver.h"
#include <GL\glut.h>


#if defined(_DEBUG)
#include <crtdbg.h>
#endif
// FOR THE OPENGL3x ****

// Assimp imports
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

// GLM imports

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <string>

// defines

#define PROJNAME "PONG"

