#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

#include "gl_canvas2d.h"

#include "Vector2.h"
#include "Bola.h"
#include "alvo.h"
#include "Canhao.h"
#include "Game.h"
#include "Frames.h"

int screenWidth = 500, screenHeight = 700;
int mouseX, mouseY;

// ARRUMAR A COLISAO

Game * game;
Frames * frames = NULL;
float fps;
void render()
{
   CV::clear(0.98, 0.992, 1);
   fps = frames->getFrames();
   game->render(mouseX, mouseY, screenWidth, screenHeight, fps);
   CV::color(1,0,0);
}

void keyboard(int key)
{
   game->keyboardListener(key, 1);
}

void keyboardUp(int key)
{
   game->keyboardListener(key, 0);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x;
   mouseY = y;
   game->clickListener(button, state);
}

int main(void)
{
   game = new Game();
   frames = new Frames();
   CV::init(screenWidth, screenHeight, "Brickz - Lorenzo Weber");
   CV::run();
}
