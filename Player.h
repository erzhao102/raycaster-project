#ifndef _PLAYER
#define _PLAYER

class Player {
  private:
    float x,y;
  
  public:
    Player();
    Player(float posX, float posY);
    float getPosX();
    void setPosX(float newX);
    float getPosY();
    void setPosY(float newY);
    void move(float dx, float dy);
    void draw();

};

#endif