//
//  main.c
//  IDK
//
//  Created by Jacob Gordon on 3/18/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#include "GLBase.h"


#include <stdlib.h>
#include <stdio.h>

#include "codename.h"
#include "PlaceToDrawStuff.h"

static void WhyMe( int status ) ;
static void SetWindowSize(int width, int height) ;
static void GetWindowSize(int *width, int *height) ;
static void MouseFunc(float x, float y ) ;
static cn_point windowSize ;

static GLFWwindow* window = NULL ;

void initScene(int x, int y) {
    
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
    
    printf("%s\n",glfwGetVersionString()) ;
    
    //Window Init.
    JHGPixels_createwindow(512,512 , GetWindowSize, SetWindowSize) ;
    
    resize(x,y) ;
    
    //renderer Init.
    startup();
    
}

static void WhyMe( int status ) {
    
    //pirates_shutdown() ;
    
    JHGPixels_destroywindow();
    
    cleanup() ;
    
    exit(status);
}

void changeSize(int w, int h) {
    
    resize(w, h);
}

void changewindowSize(int w, int h) {

    windowSize.x = w ;
    windowSize.y = h ;
}

void SetWindowSize(int width, int height) {
    
     glfwSetWindowSize(window, width, height);
    
}

void GetWindowSize(int *width, int *height) {
    
    *width = windowSize.x ;
    
    *height = windowSize.y ;

}

void mouseMovement(int x, int y) {
    
    int WinH, WinW ;
    
    JHGPixels_GetWindowSize(&WinW,&WinH);
    
    y = (y < 1 ) ? WinH : WinH - y ;
    
    MouseFunc((float)x,(float)y) ;
    
}

void MouseFunc(float x, float y ) {
    
    int WinWidth ;
    int WinHeight ;
    
    JHGPixels_GetWindowSize(&WinWidth, &WinHeight) ;
    
    x = x/WinWidth ;
    
    y = y/WinHeight ;
    
    IDK_theMouse(x,y) ;
}

void renderScene(void) {
    
    drawstuff();
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE) ;
    }
   
    switch (key) {
            
        case GLFW_KEY_W:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, w_key) ;
            
            break;
            
        case GLFW_KEY_A:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, a_key) ;
            
            break;
            
        case GLFW_KEY_S:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, s_key) ;
            
            break;
            
        case GLFW_KEY_D:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, d_key) ;
            
            break;
            
        case GLFW_KEY_RIGHT:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, right_key) ;
            
            break;
            
        case GLFW_KEY_LEFT:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, left_key) ;
            
            break;
            
        case GLFW_KEY_UP:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, up_key) ;
            
            break;
            
        case GLFW_KEY_DOWN:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, down_key) ;
            
            break;
            
        default:
            break;
    }
    
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    changewindowSize(width,height) ;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    changeSize(width,height) ;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    mouseMovement(xpos, ypos) ;
    
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
        switch (button) {
                
            case GLFW_MOUSE_BUTTON_RIGHT:
                IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Right) ;
                break;
                
            case GLFW_MOUSE_BUTTON_LEFT:
                IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Left) ;
                break;
                
            case GLFW_MOUSE_BUTTON_MIDDLE:
                IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Middle) ;
                break;
                
            default:
                break;
        }
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        IDK_setMouseActive() ;
    }
    else
    {
        IDK_setMouseInactive() ;
    }
}

int main(int argc, char **argv) {
    
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3) ;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2) ;
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1) ;
    
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE) ;
    
    window = glfwCreateWindow(861, 580, "IDK", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    
    glfwSetWindowSizeCallback(window, window_size_callback) ;
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback) ;
    
    int width, height;
    
    glfwGetWindowSize(window, &width, &height);
    
    changewindowSize(width, height) ;
    
    glfwGetFramebufferSize(window, &width, &height);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    initScene(width,height) ;
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        renderScene();
        glfwSwapBuffers(window);
        
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    WhyMe(0);
}
