#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <cfloat>
#include "Player.h"
#include "Level.h"

#define _USE_MATH_DEFINES
using namespace std;

Level map;
Player player(300, 300, 0);



void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float findDistance(float ax, float ay, float bx, float by)
{
  return sqrt( (bx - ax) * (bx - ax) + (by - ay) * (by - ay) );
}

float DegreesToRadians(float degrees) {
    return degrees * (M_PI / 180);
}

void draw3DWalls(float theta, int index, float distance)
{
  float fov = DegreesToRadians(60.0f);  // 60 degrees FOV
  float screenWidth = 512.0f;
  float screenHeight = 512.0f; // size of screen in pixels
  float projectionOffset = (screenWidth / 2.0f) / tan(fov / 2.0f);
  float omega = theta - player.getPosAngle();
  if(omega < -M_PI) 
  {
    omega += 2 * M_PI;
  }
  if(omega > M_PI)
  {
    omega -= 2 * M_PI;
  } 
  float xDistance = distance * cos(omega); // removing fish eye effect by using the x component of the distance
  float lineHeight = (tileSize * projectionOffset) / xDistance;
  if (lineHeight > screenHeight)  // limiting the height of the line to the screen height
  {
      lineHeight = screenHeight;
  }
  float centerY = 0.0f; 
  float halfLine = (lineHeight / 256.0f) / 2.0f;
  float hx = (((index * 8.0f) + 530.0f) / 512.0f) - 1.0f; // converting to opengl units from pixels
  float topY = centerY + halfLine;
  float bottomY = centerY - halfLine;
  glLineWidth(8.0f);
  glBegin(GL_LINES);
  glVertex2f(hx, bottomY);
  glVertex2f(hx, topY);
  glEnd();
}

void drawRays()
{
  int rays = 60; 
  int viewDistance = 8;
  float x = player.getPosX();
  float y = player.getPosY();
  float vX,vY,dX,dY;
  int mX,mY;
  float distance;

  
  for(int i = 0; i < rays; i++)
  {
    float theta = player.getPosAngle() + DegreesToRadians(30.0f - i); 
    if(theta > 2 * M_PI)
    {
      theta -= 2 * M_PI;
    }
    if(theta < 0)
    {
      theta += 2 * M_PI;
    }

    // horizontal 
    float horDistance = FLT_MAX;
    float horX, horY; 
    float negArcTan = -1/tan(theta); // for finding the x value since we have both an angle and y
    if(theta > M_PI)
    {
      vY = (((int)y >> 6) << 6) - 0.0001;  // divides by 64 by bit shifting 6 down then shifting up 6 to floor the value
      vX = (y - vY) * negArcTan + x;
      dY = -64;
      dX = -dY * negArcTan;
    }
    else if(theta < M_PI)
    {
      vY = (((int)y >> 6) << 6) + 64;
      vX = (y - vY) * negArcTan + x;
      dY = 64;
      dX = -dY * negArcTan;
    }
    if (fabs(sin(theta)) < 0.0001f)
    {
      horDistance = FLT_MAX;
    }
    for(int j = 0; j < viewDistance; j++)
    {
      mX = ((int)(vX)>>6);
      mY = 7 - ((int)(vY)>>6); // 7 is to adjust from the pixel starting point to 2d array
      if(mX >= 0 && mX < mapWidth && mY >= 0 && mY < mapHeight)
      {
        if(map.getWalls()[mY][mX] == 1)
        {
          horDistance = findDistance(x,y,vX,vY);
          horX = vX;
          horY = vY;
          break;
        }
        
          vY += dY;
          vX += dX;
        
      }
    }

    //vertical
    float verDistance = FLT_MAX;
    viewDistance = 8; 
    float negTan = -tan(theta); // for finding the y value since we have both an angle and x
    if(theta > M_PI_2 && theta < 3 * M_PI_2)
    {
      vX = (((int)x >> 6) << 6) - 0.0001;  // divides by 64 by bit shifting 6 down then shifting up 6 to floor the value
      vY = (x - vX) * negTan + y;
      dX = -64;
      dY = -dX * negTan;
    }
    else if(theta < M_PI_2 || theta > 3 * M_PI_2)
    {
      vX = (((int)x >> 6) << 6) + 64;
      vY = (x - vX) * negTan + y;
      dX = 64;
      dY = -dX * negTan;
    }
    if (fabs(cos(theta)) < 0.0001f)
    {
      verDistance = FLT_MAX;
    }

    for(int j = 0; j < viewDistance; j++)
    {
      mX = ((int)(vX)>>6);
      mY = 7 - ((int)(vY)>>6); // 7 is to adjust from the pixel starting point to 2d array
      if(mX >= 0 && mX < mapWidth && mY >= 0 && mY < mapHeight)
      {
        if(map.getWalls()[mY][mX] == 1)
        {
          verDistance = findDistance(x,y,vX,vY);
          break;
        }
        
          vY += dY;
          vX += dX;
        
      }
    }

    // creating rays
    if(verDistance > horDistance)
    {
      distance = horDistance;
      vX = horX;
      vY = horY;
      glColor3f(0.7f,0.0f,0.0f);
    }
    else
    {
      distance = verDistance;
      glColor3f(1.0f,0.0f,0.0f);
    }
    
    float cx = (player.getPosX() / 512.0f) - 1.0f;  
    float cy = (player.getPosY() / 256.0f) - 1.0f;
    float endX = (vX / 512.0f) - 1.0f;
    float endY = (vY / 256.0f) - 1.0f;
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(cx,cy);
    glVertex2f(endX, endY);
    glEnd();
    
    // creating 3D model
    draw3DWalls(theta,i, distance);
  }
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
      sideway = 5;
    }
    if(key == GLFW_KEY_S)
    { 
      forward = -5;
    }
    if(key == GLFW_KEY_D) 
    {
      sideway = -5;
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
    float sensitivity = 0.001f; 
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
        map.generate2DLevel();
        player.draw();

        // Movement Events
        glfwSetKeyCallback(window, keyCallBack);
        glfwSetCursorPosCallback(window, mouseCallback);

        drawRays();
        glfwSwapBuffers(window); // Refreshes to current frame
        glfwPollEvents(); // Handles events
  }
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