#ifndef __BOLA_H__
#define __BOLA_H__

#include "gl_canvas2d.h"
#include "Alvo.h"
#include <vector>
#include <math.h>

class Bola{

public:
  float r, g, b;
  Vector2 dir, pos;
  float vel;
  float raio;
  bool del = false;
  float multiplier = 1;

  Bola(int _r, int _g, int _b)
  {
    r = _r;
    g = _g;
    b = _b;
    raio = 8;
    vel = 300;
  }

  void setlocation(Vector2 _dir, Vector2 _pos) {
    dir = _dir;
    dir.normalize();
    pos = _pos;
  }

  void animate (float fps) {
    Vector2 vecVel;
    vecVel = dir * (vel * multiplier/fps) ;
    pos = pos + vecVel;
    CV::color(0.2,0.6,1);
    CV::circleFill(pos, raio, 20);
  }

  void alterax () {
    dir.set(dir.x * -1, dir.y);
  }
  void alteray() {
    dir.set(dir.x, dir.y * -1);
  }

};

#endif
