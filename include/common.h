#pragma once

// some common includes
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <cstdint>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream> // TODO: remove this dependency by adding a logger class
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// math includes
//#define GLM_FORCE_CXX98
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtx/norm.hpp> //for squared length: length2

// xml includes
#include <pugixml.hpp>

#if defined(WIN32)
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#if defined(WIN32) && defined(_MSC_VER)
#include <windows.h>

#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif
#endif

#if defined(WIN32) && defined(__GNUC__)
// prevent some warnings when using MinGW
#define GLUT_NO_LIB_PRAGMA
#define GLUT_NO_WARNING_DISABLE
#define GLUT_DISABLE_ATEXIT_HACK
#endif

#if defined(__GNUC__)
#define GLX_GLXEXT_LEGACY
#define GL_GLEXT_PROTOTYPES
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>  // The GL Header File
#include <GLUT/freeglut.h>  // The GL Utility Toolkit (GLUT) Header
#include <OpenAL/alut.h>
#else
#include <GL/gl.h>      // The GL Header File
#include <GL/freeglut.h>    // The GL Utility Toolkit (GLUT) Header
#include <AL/alut.h>
#endif

// foreach iteration
#define foreach(type, var, list) for(type::iterator var = (list).begin(); var != (list).end(); ++var)
#define foreach_const(type, var, list) for(type::const_iterator var = (list).begin(); var != (list).end(); ++var)
#define foreachr(type, var, list) for(type::reverse_iterator var = (list).rbegin(); var != (list).rend(); ++var)
#define foreachr_const(type, var, list) for(type::const_reverse_iterator var = (list).rbegin(); var != (list).rend(); ++var)
