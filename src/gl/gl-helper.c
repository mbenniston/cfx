#include "gl-helper.h"
#include "window.h"
#include <string.h>

#include "../../modules/stb/stb_image.h"
#include "../../modules/font8x8/font8x8_basic.h"

GLuint colorProg, textureProg, rectVao, rectVbo, lineVao, lineVbo;
GLint trans_loc[2], scale_loc[2], text_loc[2], color_loc[2], tex_scale_loc, tex_trans_loc;
GLuint textAtlasTexture;

GLuint currentProg = 0;
GLuint currentModel = 0;
GLuint currentTexture = 0;

static const float rect_verts[] = {
    0,0, 0,0, 
    1,0, 1,0, 
    1,1, 1,1, 
    1,1, 1,1, 
    0,1, 0,1, 
    0,0, 0,0
};

static const float line_verts[] = {
    1,1,  
    0,0  
};

static const char* VERTEX_SOURCE = ""
"#version 330 core\n"
"\n"
"layout(location = 0) in vec2 position;\n"
"layout(location = 1) in vec2 texCoords;\n"
"\n"
"out vec2 v_texCoords;\n"
"\n"
"uniform vec2 u_trans, u_scale;\n"
"uniform vec2 u_tex_trans, u_tex_scale;\n"
"\n"
"void main()\n"
"{\n"
"    v_texCoords = u_tex_trans + (u_tex_scale * texCoords);\n"
"    gl_Position = vec4(u_trans + u_scale * position, 0.0f, 1.0f);\n"
"    gl_Position.y = - gl_Position.y;\n"
"}\n";


static const char* TEXTURE_FRAGMENT_SOURCE = ""
"#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"in vec2 v_texCoords;\n"
"\n"
"uniform sampler2D u_texture;\n"
"uniform vec3 u_color;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = texture(u_texture, v_texCoords) * vec4(u_color, 1.0);\n"
"    //FragColor = vec4(v_texCoords,0, 1.0);\n"
"    if(FragColor.a < 0.5f) {\n"
"        discard;\n"
"        \n"
"    }\n"
"}\n";

static const char* COLOR_FRAGMENT_SOURCE = ""
"#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"in vec2 v_texCoords;\n"
"\n"
"uniform vec3 u_color;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(u_color,1);\n"
"}\n";

unsigned int glhLoadTexture(const char* path) 
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int w, h, channels;
    unsigned char* imageData = stbi_load(path, &w, &h, &channels, 0);

    if(!imageData) {
        printf("Could not load texture: %s\n", path);
        exit(1);
    }

    switch (channels)
    {
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        break;
    
    default:
        printf("Textures with %d channels are not supported\n", channels);
        exit(1);
        break;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);

    return texture;
}

void glhBindTexture(unsigned int t) {
    if(currentTexture != t){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t);
        currentTexture = t;
    }
}

void glhUnbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    currentTexture = 0;
}

void glhUnloadTexture(unsigned int texture) {
    glhUnbindTexture();
    glDeleteTextures(1, &texture);
}

void makeTextAtlasTexture()
{
    const int ATLAS_WIDTH = 128 * 8;
    
    unsigned char* textureData = malloc(ATLAS_WIDTH * 8 * 4);
    memset(textureData, 0x00, ATLAS_WIDTH * 8 * 4);

    for(int c = 0; c < 128; c++) {
        char* character = font8x8_basic[(int)c];

        for (int i=0; i < 8; i++) {
            for (int j=0; j < 8; j++) {
                unsigned char* pixel = &textureData[((c*8 + i) + j * ATLAS_WIDTH) * 4]; 
                bool s = character[j] & 1 << i;
                if(s) {
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    pixel[3] = 255;
                } 
            }
        }   
    }

    glGenTextures(1, &textAtlasTexture);
    glBindTexture(GL_TEXTURE_2D, textAtlasTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ATLAS_WIDTH, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(textureData);
}

GLuint loadShader(const char* path, GLenum type) {
    GLuint shader = glCreateShader(type);

    FILE* file = fopen(path, "r");
    if(!file) {
        printf("Could not open file %s\n", path);
        exit(1);
    }

    fseek(file, 0L, SEEK_END);
    int sz = ftell(file);
    rewind(file);

    char* source = malloc(sz);
    fread(source, 1, sz, file);

    glShaderSource(shader, 1, (const char * const*)&source, &sz);
    glCompileShader(shader);

    free(source);

    return shader;
}

GLuint loadShaderSource(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, (const char * const*)&source, 0);
    glCompileShader(shader);

    return shader;
}

GLuint makeProgram(const char* vsource, const char* fsource) {
    GLuint vProg = loadShaderSource(vsource, GL_VERTEX_SHADER);
    GLuint fProg = loadShaderSource(fsource, GL_FRAGMENT_SHADER);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vProg);
    glAttachShader(shaderProg, fProg);
    glLinkProgram(shaderProg);
    glValidateProgram(shaderProg);

    glDeleteShader(vProg);
    glDeleteShader(fProg);

    return shaderProg;
}

void makeQuad() {
    glGenVertexArrays(1, &rectVao);
    glBindVertexArray(rectVao);

    glGenBuffers(1, &rectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_verts), rect_verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0); 
}

void bindQuad() {
    glBindVertexArray(rectVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    currentModel = rectVao;
}

void drawQuad() {
    if(currentModel != rectVao){
        bindQuad();
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void unbindQuad() {
    currentModel = 0;
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}


void makeLine() {
    glGenVertexArrays(1, &lineVao);
    glBindVertexArray(lineVao);

    glGenBuffers(1, &lineVbo);
    glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_verts), line_verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(float), (void*)0);

    glBindVertexArray(0); 
}

void bindLine() {
    glBindVertexArray(lineVao);
    glEnableVertexAttribArray(0);
    currentModel = lineVao;
}

void drawLine() {
    if(currentModel != lineVao){
        bindLine();
    }

    glDrawArrays(GL_LINES, 0, 2);
}

void unbindLine() {
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
    currentModel = 0;
}

void glhInit() {
    makeTextAtlasTexture();

    colorProg = makeProgram(VERTEX_SOURCE, COLOR_FRAGMENT_SOURCE);
    textureProg = makeProgram(VERTEX_SOURCE, TEXTURE_FRAGMENT_SOURCE);

    trans_loc[0] = glGetUniformLocation(colorProg, "u_trans");
    scale_loc[0] = glGetUniformLocation(colorProg, "u_scale");
    text_loc [0] = glGetUniformLocation(colorProg, "u_texture");
    color_loc[0] = glGetUniformLocation(colorProg, "u_color");

    trans_loc[1] = glGetUniformLocation(textureProg, "u_trans");
    scale_loc[1] = glGetUniformLocation(textureProg, "u_scale");
    text_loc [1] = glGetUniformLocation(textureProg, "u_texture");
    color_loc[1] = glGetUniformLocation(textureProg, "u_color");

    tex_scale_loc = glGetUniformLocation(textureProg, "u_tex_scale");
    tex_trans_loc = glGetUniformLocation(textureProg, "u_tex_trans");

    glUseProgram(textureProg);
    glUniform1i(text_loc[1], 0);

    makeLine();
    makeQuad();
}

void glhRect(int x, int y, int w, int h) {
    if(currentProg != textureProg) {
        glUseProgram(textureProg);
        currentProg = textureProg;
    }

    glUniform1i(text_loc[1], 0);
    glUniform2f(trans_loc[1], -1 + 2 * ((float)x / winGetWidth()),-1 + 2 * ((float)y / winGetHeight()));
    glUniform2f(scale_loc[1], 2 * (float)w / winGetWidth(),2 * (float)h / winGetHeight());
    glUniform3f(color_loc[1], 1,1,1);
    glUniform2f(tex_scale_loc, 1.0f,1.0f);
    glUniform2f(tex_trans_loc, 0, 0);
    drawQuad();
}

void glhRectColor(int x, int y, int w, int h, Color col) {
    if(currentProg != colorProg) {
        glUseProgram(colorProg);
        currentProg = colorProg;
    }

    glUniform1i(text_loc[0], 0);
    glUniform2f(trans_loc[0], -1 + 2 * ((float)x / winGetWidth()),-1 + 2 * ((float)y / winGetHeight()));
    glUniform2f(scale_loc[0], 2 * (float)w / winGetWidth(),2 * (float)h / winGetHeight());
    glUniform3f(color_loc[0], col.r / 255.0, col.g / 255.0, col.b / 255.0);

    drawQuad();
}

void glhLineColor(int startX, int startY, int endX, int endY, Color col) {
    if(currentProg != colorProg) {
        glUseProgram(colorProg);
        currentProg = colorProg;
    }
    
    int dx = endX - startX;
    int dy = endY - startY;

    glUniform1i(text_loc[0], 0);
    glUniform2f(trans_loc[0], -1 + 2 * ((float)startX / winGetWidth()),-1 + 2 * ((float)startY / winGetHeight()));
    glUniform2f(scale_loc[0], 2 * (float)dx / winGetWidth(),2 * (float)dy / winGetHeight());

    glUniform3f(color_loc[0], col.r / 255.0, col.g / 255.0, col.b / 255.0);

    drawLine();
}

void glhCharColor(int x, int y, int w, int h, char c, Color col) {
   if(currentProg != textureProg) {
        glUseProgram(textureProg);
        currentProg = textureProg;
    }

    glhBindTexture(textAtlasTexture);

    glUniform1i(text_loc[1], 0);
    glUniform2f(trans_loc[1], -1 + 2 * ((float)x / winGetWidth()),-1 + 2 * ((float)y / winGetHeight()));
    glUniform2f(scale_loc[1], 2 * (float)w / winGetWidth(),2 * (float)h / winGetHeight());
    glUniform3f(color_loc[1], col.r / 255.0, col.g / 255.0, col.b / 255.0);

    glUniform2f(tex_scale_loc, 1.0f/(128),1.0f);
    glUniform2f(tex_trans_loc,  c / 128.0, 0);

    drawQuad();
}

void glhCleanup() {
    glDeleteProgram(colorProg);
    glDeleteProgram(textureProg);

    glDeleteBuffers(1, &rectVbo);
    glDeleteVertexArrays(1, &rectVao);
    glDeleteBuffers(1, &lineVbo);
    glDeleteVertexArrays(1, &lineVao);
    glhUnloadTexture(textAtlasTexture);
}