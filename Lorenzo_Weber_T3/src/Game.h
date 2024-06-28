#ifndef __GAME_H__
#define __GAME_H__

#include "gl_canvas2d.h"
#include "Canhao.h"
#include "AlvoManager.h"
#include "PwrManager.h"

#include <math.h>

class Game {
    public:

    int sw, sh;
    float points = 0;
    int restante;
    char strestante[10];
    char strpoints[10];
    int wave = 0;
    bool newmatch;
    int wins = -1;
    char strwins[10];
    int multiPts = 1;
    Vector2 last;

    int chance = 40;

    bool game, menu, play, gameover, extraLife;

    Canhao * ca;
    AlvoManager * am;
    PwrManager * pw;

    Game() {

        ca = new Canhao(30);
        am = new AlvoManager();
        pw = new PwrManager();

        newmatch = true;

        game = play = gameover = extraLife = false;
        menu = true;
        last.set(-1, -1);
    }

    void render(int mx, int my, int _sw, int _sh, float fps) {
        sw = _sw;
        sh = _sh;
        if(game) {
            renderGame(mx, my, sw, sh, fps);
        }
        if(menu) {
            renderMenu(mx, my, sw, sh, fps);
        }
    }

    void renderGame(int mx, int my, int sw, int sh, float fps) {
        ca->render(mx, my, sw, sh - (sh*0.2), fps);
        am->render(sw, sh);
        pw->render(ca->o, sw, sh, fps);
        drawLayout();
        updateGame();
    }

    void renderMenu (int mx, int my, int sw, int sh, float fps) {
        ca->render(mx, my, sw, sh , fps);
        ca->gameOver = true;
        drawMenu();
    }

    void updateGame() {
        colision();
        checkWave();
        gameOver();
        win();
        Points();
        newMatch();
        DropPwr();
        restante = ca->restante;
        sprintf(strestante, "%d", restante);
        sprintf(strpoints, "%.0f", points);
    }

    void clickListener(int button, int state) {
        ca->clickListener(button, state);
    }

    void keyboardListener(int key, int type) {
        ca->keyboardListener(key, type);

        if(am->win && key == 99 && type && !ca->bm->bolas.size()) {
            newmatch = true;
            am->win = false;
            ca->win = false;
        }

        if(menu && key == 112 && type) {
            play = true;
        }

        if(play && key == 101 && type) {
            chance = 40;
        }
        if(play && key == 109 && type) {
            chance = 30;
        }
        if(play && key == 104 && type) {
            chance = 10;
        }

        if(play && key == 99 && type) {
            menu = play = false;
            game = true;
            ca->gameOver = false;
        }
        if(!play && !game && key == 113) {
            exit(0);
        }
        if(gameover && type && key == 109) {
            menu = true;
            game = play = false;
            restart();
        }
        if(gameover && type && key == 114) {
            ca->gameOver = false;
            restart();
        }
    }

    void checkWave () {
        if(wave != ca->wave) {
            wave = ca->wave;
            am->down();
        }
    }

    void Points() {
        points += am->getPoints() * multiPts;
    }

    void drawLayout() {
        int screenY = sh - (sh * 0.2);
        CV::color(0.066, 0.121, 0.16);
        CV::rectFill(0, sh, sw, screenY);
        CV:: color(1,1,1);
        CV::text(70, sh - 60, "Balas: ");
        CV::text(140, sh - 60, strestante);
        CV::text(330, sh - 60, "Pontos: ");
        CV::text(410, sh - 60, strpoints);
        int x1 = 150;
        int x2 = x1 + 30;
        int y1 = sh - 95;
        int y2 = y1 - 30;
        if(ca->stamin > 1) {
            CV::color(0,0.819,0.462);
            CV::rectFill(x1, y1, x2, y2);
        }
        x1 = x2 + 25;
        x2 = x1 + 30;
        if(multiPts > 1) {
            CV::color(0.729, 0.858, 0);
            CV::rectFill(x1, y1, x2, y2);
        }
        x1 = x2 + 25;
        x2 = x1 + 30;
        if(am->multiDmg > 1) {
            CV::color(0.819, 0.419, 0.015);
            CV::rectFill(x1, y1, x2, y2);
        }
        x1 = x2 + 25;
        x2 = x1 + 30;
        if(extraLife) {
            CV::color(0.2, 0.972, 1);
            CV::rectFill(x1, y1, x2, y2);
        }
    }

    void drawMenu () {
        if(!play) {
            CV::text(sw/2 - 30, sh/1.4, "Brickz");
            CV::text(sw/2 - 80, sh/1.6, "Press P to play!");
            CV::text(sw/2 - 80, sh/1.7, "Press Q to quit");
        }
        else if(play){
            drawControls();
        }
    }

    void drawControls () {

        CV::text(sw/2 - 30, sh/1.4, "Brickz");
        CV::text(sw/9.5, sh/1.55, "Use your mouse to aim to the targets!");
        CV::text(sw/20, sh/1.65, "Press mouse 1 to shoot and mouse 2 to speed up");
        CV::text(sw/7, sh/1.75, "Use A to go left and D to go right");
        CV::color(0,0,0);
        if(chance == 40) CV::color(0.1, 0.8, 0.7);
        CV::text(sw/10 - 20, sh/1.95, "Press: E for easy,");
        CV::color(0,0,0);
        if(chance == 30) CV::color(0.1, 0.8, 0.7);
        CV::text(sw/2 - 30, sh/1.95, "M for medium,  ");
        CV::color(0,0,0);
        if(chance == 10) CV::color(0.1, 0.8, 0.7);
        CV::text(sw/1.4, sh/1.95, "H for hard");
        CV::color(0,0,0);
        CV::text(sw/2 - 90, sh/2.1, "Press c to continue");


        int x1 = 95;
        int x2 = x1 + 30;
        int y1 = sh/2.2;
        int y2 = y1 - 30;
        CV::color(0,0.819,0.462); // speed cola
        CV::rectFill(x1, y1, x2,y2);

        x1 = x2 + 25;
        x2 = x1 + 30;
        CV::color(0.729, 0.858, 0); // Double points
        CV::rectFill(x1, y1, x2, y2);

        x1 = x2 + 25;
        x2 = x1 + 30;
        CV::color(0.819, 0.419, 0.015); // Double tap
        CV::rectFill(x1, y1, x2, y2);

        x1 = x2 + 25;
        x2 = x1 + 30;
        CV::color(0.2, 0.972, 1); // Quick Revive
        CV::rectFill(x1, y1, x2, y2);

        x1 = x2 + 25;
        x2 = x1 + 30;
        CV::color(0.709, 0.121, 0.078); // Juggernog
        CV::rectFill(x1, y1, x2, y2);

        x1 = x2 + 25;
        x2 = x1 + 30;
        CV::color(0.376, 0.031, 0.768); // Max Ammo
        CV::rectFill(x1, y1, x2, y2);

        x1 = 30;
        CV::color(0,0.819,0.462);
        CV::text(x1, sh/3, "Speed Cola: Adds movement speed");
        CV::color(0.729, 0.858, 0);
        CV::text(x1, sh/3 - 30, "Double Points: Multiply the points by 2");
        CV::color(0.819, 0.419, 0.015);
        CV::text(x1, sh/3 - 60, "Double Tap: Doubles the damage");
        CV::color(0.2, 0.972, 1);
        CV::text(x1, sh/3 - 90, "Quick Revive: Gives an extra life");
        CV::color(0.709, 0.121, 0.078);
        CV::text(x1, sh/3 - 120, "Juggernog: Targets go up by 1 or 2 tiles");
        CV::color(0.376, 0.031, 0.768);
        CV::text(x1, sh/3 - 150, "Max Ammo: Reloads the weapon");
    }

    void gameOver() {
        if(am->gameOver) {
            if(extraLife) {
                am->restart();
                am->createMatch();
                extraLife = false;
            }
            else {
                sprintf(strwins, "%d", wins);
                ca->gameOver = true;
                gameover = true;
                CV::color(0,0,0);
                CV::rect(sw/2 - 165, sh/2 + 40, sw/2 + 125, sh/2 - 100);
                CV::rect(sw/2 - 166, sh/2 + 41, sw/2 + 126, sh/2 - 101);
                CV::color(1, 0.411, 0.411);
                CV::rectFill(sw/2 - 165, sh/2 + 40, sw/2 + 125, sh/2 - 100);
                CV::color(0.1,0.1,0.1);
                CV::text(sw/2 - 65, sh/2, "GAME OVER!");
                CV::text(sw/2 - 140, sh/2 - 30, "You died after");
                CV::text(sw/2 + 10 , sh/2 - 30, strwins);
                CV::text(sw/2 + 40, sh/2 - 30, "wins");
                CV::text(sw/2 - 140, sh/2 - 60, "Press M to go to the menu");
                CV::text(sw/2 - 140, sh/2 - 80, "Or press R to restart");
            }
        }
    }

    void win() {
        if(am->win) {
            ca->win = true;
            CV::color(0,0,0);
            CV::rect(sw/2 - 165, sh/2 + 40, sw/2 + 145, sh/2 - 90);
            CV::rect(sw/2 - 166, sh/2 + 41, sw/2 + 146, sh/2 - 91);
            CV::color(0.47, 1, 0.5);
            CV::rectFill(sw/2 - 165, sh/2 + 40, sw/2 + 145, sh/2 - 90);
            CV::color(0.1, 0.1, 0.1);
            CV::text(sw/2 - 45, sh/2, "YOU WON!");
            CV::text(sw/2 - 100, sh/2 - 60, "Press c to continue");
            CV::text(sw/2 - 80, sh/2 - 30, "Points: ");
            CV::text(sw/2 + 20, sh/2 - 30, strpoints);
        }
    }

    void newMatch () {
        if(newmatch) {
            wins++;
            multiPts = 1;
            ca->stamin = 1;
            am->createMatch();
            newmatch = false;
        }
    }

    void restart () {
        wins = -1;
        points = 0;
        wave = 0;
        am->restart();
        newmatch = true;
        gameover = false;
    }

    void colision () {
        float x_ponto, y_ponto;
        bool esq, dir, bai, cim, quina;
        for(int i = 0; i < ca->bm->bolas.size(); i++) {
            for(int j = 0; j < am->alvos.size();j++) {
                int raio = ca->bm->bolas[0].raio;
                esq = dir = bai = cim = quina = false;
                x_ponto = ca->bm->bolas[i].pos.x;
                y_ponto = ca->bm->bolas[i].pos.y;


                if(x_ponto < am->alvos[j].display[0].x) {
                    x_ponto = am->alvos[j].display[0].x;
                    esq = true;
                }
                else if (x_ponto > am->alvos[j].display[2].x) {
                    x_ponto = am->alvos[j].display[2].x;
                    dir = true;
                }

                if(y_ponto < am->alvos[j].display[0].y) {
                    y_ponto = am->alvos[j].display[0].y;
                    bai = true;
                }
                else if ( y_ponto > am->alvos[j].display[1].y) {
                    y_ponto = am->alvos[j].display[1].y;
                    cim = true;
                }

                float dx = ca->bm->bolas[i].pos.x - x_ponto;
                float dy = ca->bm->bolas[i].pos.y - y_ponto;

                float dist = (dx * dx) + (dy * dy) ;
                bool touch = dist <= raio * raio;

                if(touch && (esq || dir))  {
                    if(esq) {
                        ca->bm->bolas[i].pos.x = am->alvos[j].display[0].x;
                        ca->bm->bolas[i].pos.x -= raio;
                    }
                    else {
                        ca->bm->bolas[i].pos.x = am->alvos[j].display[2].x;
                        ca->bm->bolas[i].pos.x += raio;
                    }

                    ca->bm->bolas[i].alterax();
                    am->damage(j);
                    ca->bm->impacts++;
                }
                else if(touch && (bai || cim)) {

                    if(cim) {
                        ca->bm->bolas[i].pos.y = am->alvos[j].display[2].y;
                        ca->bm->bolas[i].pos.y += raio;
                    }
                    else {
                        ca->bm->bolas[i].pos.y = am->alvos[j].display[0].y;
                        ca->bm->bolas[i].pos.y -= raio;
                    }
                    ca->bm->bolas[i].alteray();
                    am->damage(j);
                    ca->bm->impacts++;
                }
            }
        }
    }

    // Power Ups

    void maxAmmo () {
        ca->restante = ca->qtd;
    }

    void juggerNog () {
        int random = generateRandomNumber(1, 100);
        int tiles = 1;

        if(random < 20) tiles = 2;

        am->up(tiles);
    }

    void doubleTap () {
        am->multiDmg = 2;
    }

    void doublePoints () {
        multiPts = 2;
    }

    void quickRevive () {
        extraLife = true;
    }

    void speedCola () {
        ca->stamin = 4;
    }

    void DropPwr() {
        if(last.x != am->last.x || last.y != am->last.y) {
            int random = generateRandomNumber(1, 100);
            last = am->last;

            if(random < chance) {
                random = generateRandomNumber(1, 100);
                if(random >= 1 && random < 25) pw->insertPwr(1, last, std::bind(&Game::maxAmmo, this), "Max Ammo!");
                else if(random >= 25 && random < 40) pw->insertPwr(2, last, std::bind(&Game::juggerNog, this), "Jugger Nog!");
                else if(random >= 40 && random < 50) pw->insertPwr(3, last, std::bind(&Game::doubleTap, this), "Double Tap!");
                else if(random >= 50 && random < 70) pw->insertPwr(4, last, std::bind(&Game::doublePoints, this), "Double Points!");
                else if(random >= 70 && random < 75) pw->insertPwr(5, last, std::bind(&Game::quickRevive, this), "Quick Revive!");
                else if(random >= 75) pw->insertPwr(6, last, std::bind(&Game::speedCola, this), "Speed Cola!");
            }
        }
    }

};

#endif
