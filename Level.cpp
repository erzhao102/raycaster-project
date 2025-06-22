#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Level.h"

using namespace std;

Level::Level()
{
  // Using a static 2D array for now, will replace with a random level generator later
  // This is used to generate a 2D walls
  int tempWalls[mapHeight][mapWidth] =
  {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,0,0,1},  
    {1,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,0,1},
    {1,1,1,1,1,1,1,1}
  };

  // This is used to generate a 2D floor
  // 0 represents floor type 1, 1 represents floor type 2
  int tempFloor[mapHeight][mapWidth] = 
  {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
  };

  // This is used to generate a 2D ceiling
  // 0 represents ceiling, 1 represents traps
  int tempCeiling[mapHeight][mapWidth] =
  {
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  };

  for (int i = 0; i < mapHeight; ++i)
  {
        for (int j = 0; j < mapWidth; ++j) 
        {
            walls[i][j] = tempWalls[i][j];
            floor[i][j] = tempFloor[i][j];
            ceiling[i][j] = tempCeiling[i][j];
        }
  }
}

const int (&Level::getWalls() const)[mapHeight][mapWidth] 
{
    return walls;
}

const int (&Level::getFloor() const)[mapHeight][mapWidth] 
{
    return floor;
}

const int (&Level::getCeiling() const)[mapHeight][mapWidth] 
{
    return ceiling;
}

void Level::setWalls(int newWalls[mapHeight][mapWidth]) 
{
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            walls[i][j] = newWalls[i][j];
        }
    }
}

void Level::setFloor(int newFloor[mapHeight][mapWidth]) 
{
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            floor[i][j] = newFloor[i][j];
        }
    }
}

void Level::setCeiling(int newCeiling[mapHeight][mapWidth]) 
{
    for (int i = 0; i < mapHeight; ++i)
    {
        for (int j = 0; j < mapWidth; ++j)
        {
            ceiling[i][j] = newCeiling[i][j];
        }
    }
  
}



void Level::generate2DLevel()
{
  // for later if it is floor check both floor and ceiling for different types and traps and color that tile accordingly
  for(int row = 0; row < mapHeight; row++) 
  {
    for(int col = 0; col < mapWidth; col++) 
    {
      // Wall
      if(walls[row][col] == 1) 
      {
          glColor3f(1.0f, 1.0f, 1.0f);  // wall (white)
      } 
      else 
      {
        // Floor coloring: 0 = black, 1 = blue
        if (floor[row][col] == 0)
          glColor3f(0.0f, 0.0f, 0.0f); // black
        else
          glColor3f(0.0f, 0.0f, 1.0f); // blue
      }

      // Convert grid position to NDC
      float x = -1.0f + col * tileSizeX;
      float y = -1.0f + (mapHeight - 1 - row) * tileSizeY;

      // Drawing Tile with 4 points
      glBegin(GL_QUADS);
      glVertex2f(x, y); // bot left
      glVertex2f(x + tileSizeX, y); // top left
      glVertex2f(x + tileSizeX, y + tileSizeY); // top right
      glVertex2f(x, y + tileSizeY); // bot right
      glEnd();

      // Draw yellow dot if ceiling[row][col] == 1 (trap) and not a wall
      if (walls[row][col] == 0 && ceiling[row][col] == 1) {
        float centerX = x + tileSizeX / 2.0f;
        float centerY = y + tileSizeY / 2.0f;
        glColor3f(1.0f, 1.0f, 0.0f); // yellow
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(centerX, centerY);
        glEnd();
      }

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

