#ifndef __PWRMANAGER_H__
#define __PWRMANAGER_H__

#include "gl_canvas2d.h"
#include "PowerUp.h"
#include "Vector2.h"

#include <vector>
#include <ctime>

class PwrManager {
public:

    vector<PowerUp> pwr;
    clock_t timer;
    float dist = 2500;
    bool display;
    int sw, sh;
    char label[100];

    PwrManager() {
        display = false;
    }

    void render(Vector2 pos, int _sw, int _sh, float fps) {
        for (int i = 0; i < pwr.size(); i++) {
            pwr[i].render(fps);
        }
        colision(pos);
        deletepwr();
        if(display) displayPwr();
        sw = _sw;
        sh = _sh;
    }

    void insertPwr (int id, Vector2 pos ,std::function<void()> func, char * label) {
        PowerUp * power = new PowerUp(id, func, label);
        power->setPos(pos);

        pwr.push_back(*power);
    }

    void colision (Vector2 pos) {
        for(int i = 0; i < pwr.size(); i++) {
            if(pwr[i].pos.x <= pos.x && pwr[i].pos.x + pwr[i].size.x >= pos.x && pwr[i].pos.y <= pos.y) {
                display = true;
                timer = clock();
                strcpy(label, pwr[i].name);
                pwr[i].apply();
                pwr.erase(pwr.begin() + i);
            }
        }
    }

    void deletepwr () {
        for(int i = 0; i < pwr.size(); i++) {
            if(pwr[i].pos.y + pwr[i].size.y < -1) {
                pwr.erase(pwr.begin() + i);
            }
        }
    }

    void displayPwr() {
        if(clock() - timer < dist) {
            CV::color(0, 0, 0);
            CV::text(sw/2 - 60, sh/2, label);
        }
    }

};

#endif
