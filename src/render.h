#pragma once
#include <GL/gl.h>
#include <SDL2/SDL.h>

typedef GLuint (*glCreateShader_t)(GLenum type);
typedef void (*glShaderSource_t)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (*glCompileShader_t)(GLuint shader);
typedef GLuint (*glCreateProgram_t)(void);
typedef void (*glAttachShader_t)(GLuint program, GLuint shader);
typedef void (*glLinkProgram_t)(GLuint program);
typedef void (*glUseProgram_t)(GLuint program);
typedef void (*glGenVertexArrays_t)(GLsizei n, GLuint* arrays);
typedef void (*glBindVertexArray_t)(GLuint array);
typedef void (*glGenBuffers_t)(GLsizei n, GLuint* buffers);
typedef void (*glBindBuffer_t)(GLenum target, GLuint buffer);
typedef void (*glBufferData_t)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (*glEnableVertexAttribArray_t)(GLuint index);
typedef void (*glVertexAttribPointer_t)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef GLint (*glGetUniformLocation_t)(GLuint program, const GLchar* name);
typedef void (*glUniform2f_t)(GLint location, GLfloat v0, GLfloat v1);
typedef void (*glUniform4f_t)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

extern glCreateShader_t my_glCreateShader;
extern glShaderSource_t my_glShaderSource;
extern glCompileShader_t my_glCompileShader;
extern glCreateProgram_t my_glCreateProgram;
extern glAttachShader_t my_glAttachShader;
extern glLinkProgram_t my_glLinkProgram;
extern glUseProgram_t my_glUseProgram;
extern glGenVertexArrays_t my_glGenVertexArrays;
extern glBindVertexArray_t my_glBindVertexArray;
extern glGenBuffers_t my_glGenBuffers;
extern glBindBuffer_t my_glBindBuffer;
extern glBufferData_t my_glBufferData;
extern glEnableVertexAttribArray_t my_glEnableVertexAttribArray;
extern glVertexAttribPointer_t my_glVertexAttribPointer;
extern glGetUniformLocation_t my_glGetUniformLocation;
extern glUniform2f_t my_glUniform2f;
extern glUniform4f_t my_glUniform4f;

extern GLuint shaderProgram, VAO, VBO;
extern GLint resUniformLoc, colorUniformLoc;
extern bool glInitDone;

void InitModernGL();
void DrawLineModern(float x1, float y1, float x2, float y2, float r, float g, float b, float w, float h);
void DrawCircleModern(float cx, float cy, float rad, float r, float g, float b, float w, float h);
void DrawFOVOverlays(int screenW, int screenH, float realAimAngle, int aimbotMode);
