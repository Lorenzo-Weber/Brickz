#ifndef __CANHAO_H__
#define __CANHAO_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Bola.h"
#include "BallManager.h"

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class Canhao {

public:

   bool a, d;
   bool shooting;
   float vel = 200;

   int sw, sh;

   float fps;
   clock_t timer;
   float dist = 150;

   int qtd;
   int restante;
   int wave = 0;
   int stamin;

   bool gameOver = false;
   bool win = false;

   float val = 1;

   BallManager * bm;
   Vector2 o, pos;

   Canhao(int _qtd) {
      o.set(250, 0);
      qtd = _qtd;
      restante = _qtd;
      stamin = 1;
      a = d = shooting = false;
      bm = new BallManager();
   }

   void clickListener (int button, int state) {
      if(button == 0 && state) {
         if(restante == qtd && !gameOver && !win) {
            shooting = true;
            instanciaBola();
         }
      }
       if(button == 2 && state == 0) {
            val = 1.3;
        }
        if(button == 2 && state == 1) {
            val = 1;
        }
   }

   void instanciaBola () {
        bm->criaBola(o, pos);
        restante--;
        timer = clock();
   }

   void keyboardListener(int key, int type) {
      if((key == 97 || key == 65) && type) a = true;
      if((key == 100 || key == 68) && type) d = true;
      if((key == 97 || key == 65) && !type) a = false;
      if((key == 100 || key == 68) && !type) d = false;
   }

   void move() {
      if(a) {
         if(o.x > 0) {
               Vector2 left;
               left.set(-0.5, 0);
               Vector2 vecVel;
               vecVel = left * (vel * stamin/fps);
               o = o + vecVel;
         }
      }
      if(d) {
         if(o.x < sw) {
               Vector2 right;
                right.set(0.5, 0);
               Vector2 vecVel;
               vecVel = right * (vel * stamin/fps);
               o = o + vecVel;
         }
      }
   }

   void empty() {
     if(bm->bolas.size() == 0 && shooting) {
        shooting = false;
        reload();
        bm->impacts = 0;
        wave++;
     }
   }

   void shoot () {
      if(shooting) {
         if(restante > 0 && clock() - timer > dist) {
            instanciaBola();
         }
         bm->update(sw, sh, fps, val);
      }
   }

   void reload () {
        if(!restante) {
            restante = qtd;
        }
   }

   void render(int mx, int my, int _sw, int _sh, float _fps) {
      fps = _fps;
      update();
      sw = _sw;
      sh = _sh;
      CV::color(0,0,0);
      pos.set(mx, my);
      pos = pos - o;
      pos.normalize();
      pos = pos * 80;
      pos = pos + o;
      CV::line(o, pos);
    }

   void update() {
      move();
      shoot();
      empty();
   }
};


#endif
