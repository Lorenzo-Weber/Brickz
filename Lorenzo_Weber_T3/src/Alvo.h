#ifndef __ALVO_H__
#define __ALVO_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#include <random>
#include <math.h>

class Alvo {
public:

   int vida;
   int points;
   int tam;
   char * strvida;
   float r, g, b;

   Vector2 pos;
   Vector2 ponto[4];
   Vector2 display[4];


   Alvo(int _vida, float x, float y) {
      vida = _vida;
      points = _vida * 10;
      pos.set(x, y);
      strvida = new char[5];
      sprintf(strvida, "%d", vida);
      tam = 50;

      ponto[0].set(0,0);
      ponto[1].set(0,1);
      ponto[2].set(1,1);
      ponto[3].set(1,0);

      for(int i = 0; i < 4; i++) {
        display[i] = (ponto[i] * tam) + pos;
      }
   }

    void render() {
        calculaCor();
        CV::color(r, g, b);
        CV::line(display[0], display[1]);
        CV::line(display[1], display[2]);
        CV::line(display[2], display[3]);
        CV::line(display[0], display[3]);

        displayVida();

    }

    void calculaCor () {

        if(vida >= 80) {
            r = 255;
            g = 0;
            b = 0;
        }
        else if(vida >= 60) {
            r = 255;
            g = 170;
            b = 0;
        }
        else if(vida >= 40) {
            r = 0;
            g = 255;
            b = 100;
        }
        else {
            r = 0;
            g = 40;
            b = 255;
        }

        r /= 255;
        g /= 255;
        b /= 255;
    }

    void displayVida() {
        CV::color(0,0,0);
        if(vida == 100) {
            CV::text(display[0].x  + tam/2 - tam/3.3,display[2].y - (tam/2), strvida);
        }
        else if(vida >= 10){
            CV::text(display[0].x + tam/2 - tam/5,display[2].y - (tam/2), strvida);
        }
        else {
            CV::text(display[0].x + tam/2 - tam/10,display[2].y - (tam/2), strvida);
        }
    }

    void damage(int dmg) {
      vida -= dmg;
      sprintf(strvida, "%d", vida);
    }
};


#endif
