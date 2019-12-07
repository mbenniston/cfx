#include "./glad/glad.h"
// #include "../../modules/glfw/include/GLFW/glfw3.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "window.h"
#include "../dw_cmds.h"
#include "gl-helper.h"

DrawMode window_DrawMode = DM_IMMEDIATE;
void* window_CmdBuf;
size_t window_CmdBufSize, window_CmdBufMaxSize;
size_t window_CmdCount;

Texture window_Fb;

int keyMap[400];
int mouseMap[12];

static struct GLFWwindow* s_Window = NULL;
static bool shouldClose = false;
static int s_mouseX = 0, s_mouseY = 0, s_WindowWidth = 0, s_WindowHeight = 0;

void process_CmdBuf();
void optimise_CmdBuf();

void setDrawMode(DrawMode dm)
{
    window_DrawMode = dm;
}

void pushCmd(const Cmd* const cmd)
{
    int size = 0;
    switch (cmd->type)
    {
    case POINT_CMD:
        size += sizeof(Cmd_Point);
        break;
    case RECT_CMD:
        size += sizeof(Cmd_Rect);
        break;
    case BLIT_IMAGE_CMD:
        size += sizeof(Cmd_Image);
        break;
    case LINE_CMD:
        size += sizeof(Cmd_Line);
        break;
    case CHAR_CMD:
        size += sizeof(Cmd_Char);
        break;
    default:
        break;
    }

    if(window_CmdBufSize + size > window_CmdBufMaxSize) {
        window_CmdBuf = realloc(window_CmdBuf, window_CmdBufMaxSize + size * 10);
        window_CmdBufMaxSize += size * 10;
    }

    memcpy((char*)window_CmdBuf + window_CmdBufSize, cmd, size);
    window_CmdBufSize += size;
}

static double lerp(double a, double b, double c)
{
    return a + (b - a) * c;
}

static void win_kb_func(GLFWwindow* window,int key,int mod,int isPressed,int y)
{
    keyMap[key] = (isPressed == GLFW_PRESS);
}

static void win_mouse_move_func(GLFWwindow *window, double x, double y)
{
    s_mouseX = (int)x;
    s_mouseY = (int)y;
}

static void win_mouse_button_func(GLFWwindow* window, int button, int isPressed, int mods)
{
    mouseMap[button] = (isPressed == GLFW_PRESS);
}

void winOpen(int width, int height, const char* title)
{
    s_WindowWidth = width;
    s_WindowHeight = height;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    s_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(s_Window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string
	const GLubyte* shaderVer = glGetString (GL_SHADING_LANGUAGE_VERSION); // version as a string

	printf("Renderer: %s\n",renderer);
	printf("OpenGL version supported %s\n",version);
	printf("OpenGL shader version: %s\n",shaderVer);

    glfwSetKeyCallback(s_Window, win_kb_func);
    glfwSetMouseButtonCallback(s_Window, win_mouse_button_func);
    glfwSetCursorPosCallback(s_Window, win_mouse_move_func);

    window_CmdBufSize = 0;
    window_CmdBufMaxSize = 4086 * 8;
    window_CmdBuf = malloc(window_CmdBufMaxSize);

    glhInit();
}

void winClose()
{
    glhCleanup();

    free(window_CmdBuf);
    glfwTerminate();
}

void winClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void winBlit(const Texture* const readbuf)
{
}

bool winShouldClose()
{
    return shouldClose;
}

double winGetTime()
{
    return glfwGetTime();
}

int winGetKey(int keyCode)
{
    return keyMap[keyCode];
}

void winUpdate()
{
    if(window_DrawMode == DM_BUFFERED) { 
        process_CmdBuf(); 
        // optimise_CmdBuf(); 
    }

    glfwSwapInterval(1);
    glfwPollEvents();
    glfwSwapBuffers(s_Window);
    shouldClose = glfwWindowShouldClose(s_Window);    
}

int winGetMouseX()
{
    return s_mouseX;
}

int winGetMouseY()
{
    return s_mouseY;
}

int winGetWidth() 
{
    int width;
    glfwGetWindowSize(s_Window, &width, NULL);
    return width;
}
int winGetHeight() 
{
    int height;
    glfwGetWindowSize(s_Window, NULL, &height);
    return height;
}

int winGetMouseButton(int button)
{
    return mouseMap[button];
}

void process_CmdBuf() 
{
    window_CmdCount = 0;

    int position = 0;
    while(position < window_CmdBufSize)
    {
        Cmd* cmd = (Cmd*)(window_CmdBuf + position);

        switch (cmd->type)
        {
        case POINT_CMD:
        {
            Cmd_Point* point = (Cmd_Point*)cmd;
            process_point(*point);
            position += sizeof(Cmd_Point);   
            window_CmdCount++;
        }
            break;
        case RECT_CMD:
        {
            Cmd_Rect* rect = (Cmd_Rect*)cmd;
            process_rect(*rect);
            position += sizeof(Cmd_Rect);   
            window_CmdCount++;
        }
        break;        
        case LINE_CMD:
        {
            Cmd_Line* line = (Cmd_Line*)cmd;
            process_line(*line);
            position += sizeof(Cmd_Line);   
            window_CmdCount++;
        }
        break;            
        case BLIT_IMAGE_CMD:
        {
            Cmd_Image* image = (Cmd_Image*)cmd;
            process_image(*image);
            position += sizeof(Cmd_Image);   
            window_CmdCount++;
        }
        break;
        case CHAR_CMD:
        {
            Cmd_Char* c = (Cmd_Char*)cmd;
            process_char(*c);
            position += sizeof(Cmd_Char);   
            window_CmdCount++;
        }
        break;
        default:
            break;
        }
    }
    window_CmdBufSize = 0;
}

void optimise_CmdBuf()
{
}


void process_point(Cmd_Point point)
{
   if(point.texture.pixels != window_Fb.pixels) {
       fprintf(stderr, "Cannot draw to texture in OpenGL mode\n");
       exit(1);
   }

    glhRectColor(point.x, point.y, 1, 1, point.color);
}

void process_rect(Cmd_Rect rect)
{
    if(rect.texture.pixels != window_Fb.pixels) {
        fprintf(stderr, "Cannot draw to texture in OpenGL mode\n");
        exit(1);
    }

    glhRectColor(rect.x, rect.y, rect.w, rect.h, rect.color);
}

void process_line(Cmd_Line line)
{
    if(line.texture.pixels != window_Fb.pixels) {
        fprintf(stderr, "Cannot draw to texture in OpenGL mode\n");
        exit(1);
    }

    glhLineColor(line.startX, line.startY, line.endX, line.endY, line.color);
}

void process_image(Cmd_Image image)
{
    if(image.destTexture.pixels != window_Fb.pixels) {
        fprintf(stderr, "Cannot draw to texture in OpenGL mode\n");
        exit(1);
    }

    glhBindTexture(image.srcTexture.width);
    glhRect(image.x, image.y, image.w, image.h);
}

void process_char(Cmd_Char cmd) {
   if(cmd.destTexture.pixels != window_Fb.pixels) {
        fprintf(stderr, "Cannot draw to texture in OpenGL mode\n");
        exit(1);
    }

    glhCharColor(cmd.x, cmd.y, 8 * cmd.w, 8 *cmd.h, cmd.character, cmd.color);
}