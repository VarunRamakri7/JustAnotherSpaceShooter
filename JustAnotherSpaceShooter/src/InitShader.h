#pragma once

#include <windows.h>
#include <GL/GL.h>

GLuint InitShader( const char* vertexShaderFile, const char* fragmentShaderFile );
GLuint InitShader( const char* vertexShaderFile, const char* geometryShader, const char* fragmentShaderFile );