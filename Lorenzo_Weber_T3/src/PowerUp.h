#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "gl_canvas2d.h"
#include "Vector2.h"

#include <functional>

class PowerUp {
public:

    Vector2 pos;
    float vel = 200;
    float fps;
    int id;
    float r, g, b;
    char name[100];
    Vector2 size;
    std::function<void()> powerup;


    PowerUp (int _id, std::function<void()> func, char * label) {
        id = _id;
        definePwr();
        powerup = func;
        strcpy(name, label);
        size.set(30,30);
    }

    void render(float _fps) {
        fps = _fps;
        CV::color(r, g ,b);
        CV::rectFill(pos, pos + size);
        animate();
    }

    void definePwr() {

        if(id == 1) {
            r = 0.376;
            g = 0.031;
            b = 0.768;
        }
        else if(id == 2) {
            r = 0.709;
            g = 0.121;
            b = 0.078;
        }
        else if(id == 3) {
            r = 0.819;
            g = 0.419;
            b = 0.015;
        }
        else if(id == 4) {
            r = 0.729;
            g = 0.858;
            b = 0;
        }
        else if(id == 5) {
            r = 0.2;
            g = 0.972;
            b = 1;
        }
        else if(id == 6) {
            r = 0;
            g = 0.819;
            b = 0.462;
        }
    }

    void setPos (Vector2 newpos) {
        pos = newpos;
    }

    void animate () {
       Vector2 down;
       down.set(0,-1.3);
        Vector2 vecVel;
        vecVel = down * (vel * 1/fps);
        pos = pos + vecVel;
    }

    void apply () {
        powerup();
    }

};

#endif
