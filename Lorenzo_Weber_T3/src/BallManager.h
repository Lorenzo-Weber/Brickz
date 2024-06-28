#ifndef __BALLMANAGER_H__
#define __BALLMANAGER_H__

#include "gl_canvas2d.h"
#include "Bola.h"

class BallManager {

public:

   vector<Bola> bolas;
   int qtd = 0;
   int impacts = 0;
   int multiplier = 1;
   float fps;
   int tempo = 0;
   Vector2 opos, odir;

   BallManager() {

   }

    void criaBola(Vector2 pos, Vector2 dir) {
        if(bolas.size() > 0) {
            pos = opos;
            dir = odir;
        }
        else {
            opos = pos;
            odir = dir;
        }
        Bola * b = new Bola(1,0,0);
        b->setlocation(dir - pos, pos);
        bolas.push_back(*b);
        qtd++;
    }

    void setPos (Vector2 pos, Vector2 dir) {
       for(int i = 0; i < bolas.size(); i++) {
            bolas[i].setlocation(dir - pos, pos);
        }
    }

   void animate() {
      for(int i = 0; i < bolas.size(); i++) {
        bolas[i].animate(fps);
      }
   }

   void colision(int sw, int sh) {
      for(int i = 0; i < bolas.size(); i++) {
         if(bolas[i].pos.x < 0 || bolas[i].pos.x > sw) {
            if(bolas[i].pos.x < 0) bolas[i].pos.x = 0;
            else bolas[i].pos.x = sw;
            bolas[i].alterax();
            impacts++;
         }
         if(bolas[i].pos.y > sh - 8) {
            bolas[i].pos.y = sh - 9;
            bolas[i].alteray();
            impacts++;
         }
      }
   }

   void deleteBall () {
        for(int i = 0; i < bolas.size(); i++) {
            if(bolas[i].pos.y < -10) {
                bolas.erase(bolas.begin() + i);
            }
        }
   }

   void velControl (float val) {
        for(int i = 0; i < bolas.size(); i++) {
            bolas[i].multiplier = val;
        }

   }

   void update (int sw, int sh, float _fps, float val) {
      fps = _fps;
      deleteBall();
      animate();
      velControl(val);
      colision(sw, sh);
   }

};

#endif
