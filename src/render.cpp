#include "render.h"
#include "cheat.h"
#include <cmath>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    uniform vec2 u_resolution;
    void main() {
        vec2 clipSpace = (aPos / u_resolution) * 2.0 - 1.0;
        gl_Position = vec4(clipSpace * vec2(1.0, -1.0), 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 u_color;
    void main() { FragColor = u_color; }
)";

// OpenGL
glCreateShader_t my_glCreateShader = nullptr;
glShaderSource_t my_glShaderSource = nullptr;
glCompileShader_t my_glCompileShader = nullptr;
glCreateProgram_t my_glCreateProgram = nullptr;
glAttachShader_t my_glAttachShader = nullptr;
glLinkProgram_t my_glLinkProgram = nullptr;
glUseProgram_t my_glUseProgram = nullptr;
glGenVertexArrays_t my_glGenVertexArrays = nullptr;
glBindVertexArray_t my_glBindVertexArray = nullptr;
glGenBuffers_t my_glGenBuffers = nullptr;
glBindBuffer_t my_glBindBuffer = nullptr;
glBufferData_t my_glBufferData = nullptr;
glEnableVertexAttribArray_t my_glEnableVertexAttribArray = nullptr;
glVertexAttribPointer_t my_glVertexAttribPointer = nullptr;
glGetUniformLocation_t my_glGetUniformLocation = nullptr;
glUniform2f_t my_glUniform2f = nullptr;
glUniform4f_t my_glUniform4f = nullptr;

GLuint shaderProgram = 0, VAO = 0, VBO = 0;
GLint resUniformLoc = -1, colorUniformLoc = -1;
bool glInitDone = false;

void InitModernGL() {
    if (glInitDone) return;
    my_glCreateShader = (glCreateShader_t)SDL_GL_GetProcAddress("glCreateShader");
    my_glShaderSource = (glShaderSource_t)SDL_GL_GetProcAddress("glShaderSource");
    my_glCompileShader = (glCompileShader_t)SDL_GL_GetProcAddress("glCompileShader");
    my_glCreateProgram = (glCreateProgram_t)SDL_GL_GetProcAddress("glCreateProgram");
    my_glAttachShader = (glAttachShader_t)SDL_GL_GetProcAddress("glAttachShader");
    my_glLinkProgram = (glLinkProgram_t)SDL_GL_GetProcAddress("glLinkProgram");
    my_glUseProgram = (glUseProgram_t)SDL_GL_GetProcAddress("glUseProgram");
    my_glGenVertexArrays = (glGenVertexArrays_t)SDL_GL_GetProcAddress("glGenVertexArrays");
    my_glBindVertexArray = (glBindVertexArray_t)SDL_GL_GetProcAddress("glBindVertexArray");
    my_glGenBuffers = (glGenBuffers_t)SDL_GL_GetProcAddress("glGenBuffers");
    my_glBindBuffer = (glBindBuffer_t)SDL_GL_GetProcAddress("glBindBuffer");
    my_glBufferData = (glBufferData_t)SDL_GL_GetProcAddress("glBufferData");
    my_glEnableVertexAttribArray = (glEnableVertexAttribArray_t)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    my_glVertexAttribPointer = (glVertexAttribPointer_t)SDL_GL_GetProcAddress("glVertexAttribPointer");
    my_glGetUniformLocation = (glGetUniformLocation_t)SDL_GL_GetProcAddress("glGetUniformLocation");
    my_glUniform2f = (glUniform2f_t)SDL_GL_GetProcAddress("glUniform2f");
    my_glUniform4f = (glUniform4f_t)SDL_GL_GetProcAddress("glUniform4f");

    if (!my_glCreateShader) return;
    GLuint vs = my_glCreateShader(GL_VERTEX_SHADER); my_glShaderSource(vs, 1, &vertexShaderSource, NULL); my_glCompileShader(vs);
    GLuint fs = my_glCreateShader(GL_FRAGMENT_SHADER); my_glShaderSource(fs, 1, &fragmentShaderSource, NULL); my_glCompileShader(fs);
    shaderProgram = my_glCreateProgram(); my_glAttachShader(shaderProgram, vs); my_glAttachShader(shaderProgram, fs); my_glLinkProgram(shaderProgram);
    resUniformLoc = my_glGetUniformLocation(shaderProgram, "u_resolution"); colorUniformLoc = my_glGetUniformLocation(shaderProgram, "u_color");
    my_glGenVertexArrays(1, &VAO); my_glGenBuffers(1, &VBO); glInitDone = true;
}

void DrawLineModern(float x1, float y1, float x2, float y2, float r, float g, float b, float w, float h) {
    if (!glInitDone) return;
    float vertices[] = { x1, y1, x2, y2 };
    my_glUseProgram(shaderProgram); my_glUniform2f(resUniformLoc, w, h); my_glUniform4f(colorUniformLoc, r, g, b, 1.0f);
    my_glBindVertexArray(VAO); my_glBindBuffer(GL_ARRAY_BUFFER, VBO); my_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    my_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); my_glEnableVertexAttribArray(0);
    void (*my_glDrawArrays)(GLenum, GLint, GLsizei) = (void (*)(GLenum, GLint, GLsizei))SDL_GL_GetProcAddress("glDrawArrays");
    if(my_glDrawArrays) my_glDrawArrays(GL_LINES, 0, 2);
}

void DrawCircleModern(float cx, float cy, float r, float red, float green, float blue, float w, float h) {
    if (!glInitDone) return;
    const int segments = 64; float vertices[segments * 2];
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        vertices[i * 2] = cx + r * cosf(theta); vertices[i * 2 + 1] = cy + r * sinf(theta);
    }
    my_glUseProgram(shaderProgram); my_glUniform2f(resUniformLoc, w, h); my_glUniform4f(colorUniformLoc, red, green, blue, 1.0f);
    my_glBindVertexArray(VAO); my_glBindBuffer(GL_ARRAY_BUFFER, VBO); my_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    my_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); my_glEnableVertexAttribArray(0);
    void (*my_glDrawArrays)(GLenum, GLint, GLsizei) = (void (*)(GLenum, GLint, GLsizei))SDL_GL_GetProcAddress("glDrawArrays");
    if(my_glDrawArrays) my_glDrawArrays(GL_LINE_LOOP, 0, segments);
}

void DrawFOVOverlays(int screenW, int screenH, float realAimAngle, int aimbotMode) {
    float cx = screenW / 2.0f;
    float cy = screenH / 2.0f;

    if (aimbotMode == 0) {
        if (showClassicFOV) DrawCircleModern(cx, cy, classicFOV, classicFovColor[0], classicFovColor[1], classicFovColor[2], screenW, screenH);
    } else if (aimbotMode == 1) {
        if (showHookFOV) DrawCircleModern(cx, cy, hookFOV, hookFovColor[0], hookFovColor[1], hookFovColor[2], screenW, screenH);
        if (showLaserFOV) DrawCircleModern(cx, cy, laserFOV, laserFovColor[0], laserFovColor[1], laserFovColor[2], screenW, screenH);
    }

    if (coneEnabled && showConeFOV) {
        float halfCone = (coneAngle * PI / 180.0f) / 2.0f;
        float line1X = cx + coneLength * cosf(realAimAngle - halfCone);
        float line1Y = cy + coneLength * sinf(realAimAngle - halfCone);
        float line2X = cx + coneLength * cosf(realAimAngle + halfCone);
        float line2Y = cy + coneLength * sinf(realAimAngle + halfCone);
        DrawLineModern(cx, cy, line1X, line1Y, coneColor[0], coneColor[1], coneColor[2], screenW, screenH);
        DrawLineModern(cx, cy, line2X, line2Y, coneColor[0], coneColor[1], coneColor[2], screenW, screenH);
    }
}
