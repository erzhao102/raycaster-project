#include <GLFW/glfw3.h>
#include <iostream>

#include "Player.h"

using namespace std;
Player::Player()
{
  x = 0;
  y = 0;
}

Player::Player(float posX, float posY)
{
  x = posX;
  y = posY;
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

void Player::move(float dx, float dy) 
{
  x += dx;
  y += dy;
}

void Player::draw() 
{
  glColor3f(0.0f, 1.0f, 0.0f);  // green
  glPointSize(8);
  glBegin(GL_POINTS);

  // Converting to [-1, 1] range
  // window size is 1024 by 512
  // 1x = 512
  // 1y = 256
  float cx = (x / 512.0f) - 1.0f;  
  float cy = (y / 256.0f) - 1.0f;  

  glVertex2f(cx, cy);
  glEnd();
}
