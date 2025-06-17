#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Player.h"

#define _USE_MATH_DEFINES
using namespace std;

const int mapLength = 8, mapWidth = 8, mapArea = 64;
int map[mapLength][mapWidth] =
{
  {1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,1,0,0,1,1,1},
  {1,0,1,0,0,0,0,1},
  {1,0,1,0,1,0,0,1},
  {1,1,1,1,1,1,1,1}
};
Player player(300, 300, 0);

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void draw2Dmap() 
{
  const float tileSizeX = 1.0f / mapWidth;    // width of 1 tile in NDC
  const float tileSizeY = 2.0f / mapLength;   // height of 1 tile in NDC

  for(int row = 0; row < mapLength; row++) 
  {
    for(int col = 0; col < mapWidth; col++) 
    {
      if(map[row][col] == 1) 
      {
          glColor3f(1.0f, 1.0f, 1.0f);  // wall
      } 
      else 
      {
        glColor3f(0.0f, 0.0f, 0.0f);  // floor
      }

      // Convert grid position to NDC
      float x = -1.0f + col * tileSizeX;
      float y = -1.0f + (mapLength - 1 - row) * tileSizeY;

      // Drawing Tile with 4 points
      glBegin(GL_QUADS);
      glVertex2f(x, y); // bot left
      glVertex2f(x + tileSizeX, y); // top left
      glVertex2f(x + tileSizeX, y + tileSizeY); // top right
      glVertex2f(x, y + tileSizeY); // bot right
      glEnd();

      // Drawing border for tile
      glColor3f(0.5f, 0.5f, 0.5f); // gray
      glLineWidth(1.0f);
      glBegin(GL_LINE_LOOP);
      glVertex2f(x, y);
      glVertex2f(x + tileSizeX, y);
      glVertex2f(x + tileSizeX, y + tileSizeY);
      glVertex2f(x, y + tileSizeY);
      glEnd();


      
    }
  }
}

void drawRays()
{
  float rayX, rayY;

}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  int forward = 0, sideway = 0;
  if(action == GLFW_PRESS || action == GLFW_REPEAT) 
  {
    if(key == GLFW_KEY_W) 
    {
      forward = 5;
    }
    if(key == GLFW_KEY_A) 
    {
      sideway = -5;
    }
    if(key == GLFW_KEY_S)
    { 
      forward - 5;
    }
    if(key == GLFW_KEY_D) 
    {
      sideway = 5;
    }
    player.move(forward,sideway);
  }
}

void mouseCallback(GLFWwindow* window, double posX, double posY)
{
    // first value when program is ran
    static double currentX = posX;
    static bool firstMovement = true; 

    if(firstMovement)
    {
        currentX = posX;
        firstMovement = false;
        return;
    }

    // takes the small change which is proportional to a value on the unit circle
    double dx = currentX - posX;
    currentX = posX;

    // adjusts with sens
    float sensitivity = 0.03f; 
    float angleChange = dx * sensitivity;
    player.setPosAngle(player.getPosAngle() + angleChange);
}

int display()
{
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(1024, 512, "Map", NULL, NULL);
  
  // Check for window 
  if(!window)
  {
    glfwTerminate();
    return -1;
  }
  
  // Setting as current window
  glfwMakeContextCurrent(window);

  // centering window
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwSetWindowPos(window, (mode->width - 1024) / 2, (mode->height - 512) / 2);

  // turns off mouse acceleration and hides cursor
  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Loops while window is active
  while(!glfwWindowShouldClose(window))
  {
        glClear(GL_COLOR_BUFFER_BIT); // Clearing background
        glClearColor(0.5f,0.5f,0.5f,1.0f);
        draw2Dmap();
        player.draw();

        // Movement Events
        glfwSetKeyCallback(window, keyCallBack);
        glfwSetCursorPosCallback(window, mouseCallback);


        glfwSwapBuffers(window); // Refreshes to current frame
        glfwPollEvents(); // Handles events
  }
  glfwTerminate();
  return 0;
}

int main()
{

  // Initializing GLFW
  if(!glfwInit())
  {
    return -1;
  }

  // Error callback
  glfwSetErrorCallback(error_callback);

  display();

  glfwTerminate();
  return 0;
}