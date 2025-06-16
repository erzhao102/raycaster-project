#include <GLFW/glfw3.h>
#include <iostream>
#include "Player.h"
using namespace std;

const int mapLength = 8, mapWidth = 8, mapArea = 64;
int map[mapLength][mapWidth] =
{
  {1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1}
};
Player player(300, 300);

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void draw2Dmap() 
{
  const float tileSizeX = 2.0f / mapWidth;    // width of 1 tile in NDC
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

      glBegin(GL_QUADS); // drawing a tile with the 4 points
      glVertex2f(x, y);
      glVertex2f(x + tileSizeX, y);
      glVertex2f(x + tileSizeX, y + tileSizeY);
      glVertex2f(x, y + tileSizeY);
      glEnd();
    }
  }
}


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  if(action == GLFW_PRESS || action == GLFW_REPEAT) 
  {
    if(key == GLFW_KEY_W) 
    {
      player.move(0, 5);
    }
    if(key == GLFW_KEY_A) 
    {
      player.move(-5, 0);
    }
    if(key == GLFW_KEY_S)
    { 
      player.move(0, -5);
    }
    if(key == GLFW_KEY_D) 
    {
      player.move(5, 0);
    }
  }
}

int display()
{
  GLFWwindow* window = glfwCreateWindow(1024, 1024, "Map", NULL, NULL);
  
  // Check for window 
  if(!window)
  {
    glfwTerminate();
    return -1;
  }
  
  // Setting as current window
  glfwMakeContextCurrent(window);

  // Loops while window is active
  while(!glfwWindowShouldClose(window))
  {
        glClear(GL_COLOR_BUFFER_BIT); // Clearing background
        draw2Dmap();
        player.draw();
        glfwSetKeyCallback(window, keyCallBack);


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