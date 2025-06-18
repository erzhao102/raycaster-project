#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Player.h"
#define _USE_MATH_DEFINES

using namespace std;

Player::Player()
{
  x = 0;
  y = 0;
  angle = 0;
}

Player::Player(float posX, float posY, float posAngle)
{
  x = posX;
  y = posY;
  angle = posAngle;
}

float Player::getPosX()
{
  return x;
}

void Player::setPosX(float newX)
{
  x = newX;
}

float Player::getPosY()
{
  return y;
}

void Player::setPosY(float newY)
{
  y = newY;
}

float Player::getPosAngle()
{
  return angle;
}

void Player::setPosAngle(float newAngle)
{
  if (newAngle < 0)
  {
    newAngle += 2 * M_PI;
  }
  if (newAngle > 2 * M_PI)
  {
    newAngle -= 2 * M_PI;
  }
  angle = newAngle;
}

void Player::move(int forward, int sideway)
{
    // takes the movement from keyboard and adjusts with the angle from the mouse
    x += cos(angle) * forward + cos(angle + M_PI_2) * sideway;
    y += sin(angle) * forward + sin(angle + M_PI_2) * sideway;
}


void Player::draw() 
{
  // Converting to [-1, 1] range
  // window size is 1024 by 512
  // 1x = 512
  // 1y = 256
  float cx = (x / 512.0f) - 1.0f;  
  float cy = (y / 256.0f) - 1.0f;
  
  //Creating Player
  glColor3f(0.0f, 1.0f, 0.0f);  // green
  glPointSize(8); // size of Player
  glBegin(GL_POINTS); // starting opengl to draw single point
  glVertex2f(cx, cy); // drawing vertex
  glEnd();

// was for testing not needed anymore
/*  float lineLength = 0.1f; // Controls how long the direction line is
  float dx = cos(angle) * lineLength;
  float dy = sin(angle) * lineLength;


  //Creating a vector which points at what the player is facing
  glColor3f(0.0f, 1.0f, 0.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(cx, cy); // player coords
  glVertex2f(cx + dx, cy + dy);  // facing angle
  glEnd();
*/

  
}
