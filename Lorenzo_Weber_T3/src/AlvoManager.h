#ifndef __ALVOMANAGER_H__
#define __ALVOMANAGER_H__

#include "gl_canvas2d.h"
#include "Alvo.h"

#include <vector>
#include <iostream>
#include <random>
#include <ctime>

int generateRandomNumber(int min, int max) {

    static std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(gen);
}

class AlvoManager {
    public:
        int qtd;
        int offset = 50;
        float points = 0;
        vector<Alvo> alvos;
        bool gameOver = false;
        bool win = false;
        int sw, sh;
        int multiDmg = 1;
        Vector2 last;

        AlvoManager() {
            last.set(-1, -1);
        }

        ~ AlvoManager() {
        }

        void insereAlvo(int x, int y, int life) {
            Alvo * a = new Alvo(life, x, y);
            alvos.push_back(*a);
        }

        void render(int _sw, int _sh) {
            sw = _sw;
            sh = _sh;
            for(int i = 0; i < alvos.size(); i++) {
                alvos[i].render();
                deleteAlvo();
                gameover();
                won();
            }
        }

        void deleteAlvo() {
            for(int i = 0; i < alvos.size(); i++) {
                if(alvos[i].vida <= 0) {
                    points += alvos[i].points;
                    last = alvos[i].display[1];
                    alvos.erase(alvos.begin() + i);
                }
            }
        }

        void down() {
            for(int i = 0; i < alvos.size(); i++) {
                for(int j = 0; j < 4; j++) {
                    alvos[i].display[j].y -= offset;
                }
            }
        }

        void up(int tiles) {
            for(int i = 0; i < alvos.size(); i++) {
                for(int j = 0; j < 4; j++) {
                    alvos[i].display[j].y += offset * tiles;
                }
            }
        }

        void gameover() {
            for(int i = 0; i < alvos.size(); i++) {
                for(int j = 0; j < 4; j++) {
                    if(alvos[i].display[j].y <= 1) gameOver = true;
                }
            }
        }

        void won () {
            if(alvos.size() == 0) win = true;
        }

        void damage(int index) {
            alvos[index].damage(multiDmg);
        }

        float getPoints() {
            float aux = points;
            points = 0;

            return aux;
        }

        void createMatch () {

            int lines = generateRandomNumber(4, 12);
            int startPointY = generateRandomNumber(5, 8);
            int startPointX;
            startPointY *= 50;
            startPointY += 1;
            multiDmg = 1;

            for(int i = 0; i < lines; i++) {

                startPointX = generateRandomNumber(0, 4);
                startPointX *= 50;
                int max = (500 - startPointX)/50;

                int targets = generateRandomNumber(0, max);
                for(int j = 0; j < targets; j++) {
                    int life = generateRandomNumber(0, 80);
                    insereAlvo(startPointX, startPointY, life);
                    startPointX += offset;
                }
                startPointY += 50;
            }
        }

        void deleteMatch () {
            for(int i = alvos.size(); i > 0; i--) {
                alvos.erase(alvos.begin() + i);
            }
        }

        void restart() {
            deleteMatch();
            gameOver = false;
        }
};

#endif
