#ifndef _PLAYER
#define _PLAYER

class Player {
  private:
    float x,y,angle;
  
  public:
    Player();
    Player(float posX, float posY, float posAngle);
    float getPosX();
    void setPosX(float newX);
    float getPosY();
    void setPosY(float newY);
    float getPosAngle();
    void setPosAngle(float newAngle);
    void move(int foward, int sideway);
    void draw();

};

#endif