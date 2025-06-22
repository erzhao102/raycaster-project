#ifndef LEVEL_H
#define LEVEL_H

constexpr int mapHeight = 8;
constexpr int mapWidth = 8;
constexpr int tileSize = 64;
constexpr float tileSizeX = 1.0f / mapWidth;    // width of 1 tile in NDC
constexpr float tileSizeY = 2.0f / mapHeight;   // height of 1 tile in NDC

class Level
{
  private:
    int walls[mapHeight][mapWidth];
    int floor[mapHeight][mapWidth];
    int ceiling[mapHeight][mapWidth];
    const int tileSize = 64; // size of each tile in pixels

  public:
    Level();
    const int (&getWalls() const)[mapHeight][mapWidth];
    const int (&getFloor() const)[mapHeight][mapWidth];
    const int (&getCeiling() const)[mapHeight][mapWidth];

    void setWalls(int newWalls[mapHeight][mapWidth]);
    void setFloor(int newFloor[mapHeight][mapWidth]);
    void setCeiling(int newCeiling[mapHeight][mapWidth]);
    bool isWall(int x, int y) const;
    void generate2DLevel();
    void generate3DLevel();

};


#endif