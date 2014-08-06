#include <windows.h>

#include <iostream>
#include <gl/gl.h>
#include "star.hpp"
#include "board.hpp"
#include "ship.hpp"
#include "menu.hpp"

HDC tWinDC;
HGLRC hWinRC;
bool playing;
board gameBoard;

bool Attach(HDC zhDC, HGLRC zhRC)
{
    // Check if a window is already attached
    if (tWinDC != NULL) return false;

    tWinDC = zhDC;
    hWinRC = zhRC;
    return true;
}

void Play()
{
    // The game is now active
    playing = true;

    // Create a new board to hold everything
    gameBoard.hDC = tWinDC;
    gameBoard.hRC = hWinRC;

    // Add a new star to the board
    star_ptr tmp = gameBoard.addStar();
    tmp->setSize (tiny);
    tmp->setPos (50.0f,50.0f, star::nochange);
    tmp->setStarType(naval);

    star_ptr tmp2 = gameBoard.addStar();
    tmp2->setSize (large);
    tmp2->setPos (150.0f, 150.0f, star::nochange);
    tmp2->setPlayer(1);
    tmp2->setStarType(defense);

    star_ptr tmp3 = gameBoard.addStar();
    tmp3->setSize(tiny);
    tmp3->setPos(250.0f,200.0f, star::nochange);
    tmp3->setPlayer(0);
    tmp3->setStarType(naval);

    star_ptr tmp4 = gameBoard.addStar();
    tmp4->setSize(tiny);
    tmp4->setPos(250.0f,50.0f, star::nochange);
    tmp4->setPlayer(0);
    tmp4->setStarType(economy);

    star_ptr tmp5 = gameBoard.addStar();
    tmp5->setSize(tiny);
    tmp5->setPos(250.0f,100.0f, star::nochange);
    tmp5->setPlayer(0);
    tmp5->setStarType(economy);
}

void Step()
{
    // Step forward one unit in time
    gameBoard.step();

    // Draw the board
    gameBoard.draw();
}

void SetFPS (int zfps)
{
    if (gameBoard.fps == NULL) gameBoard.fps = (char*) malloc(11);
    sprintf(gameBoard.fps, "%i", zfps);
}

void MouseClick(int x, int y, bool zctrl, bool zshift, bool zdbl, bool zdown)
{
    gameBoard.click(x, y, zctrl, zshift, zdbl, zdown);
}

void MouseMove(int x, int y, bool zctrl, bool zshift, bool zdown)
{
    gameBoard.mouseMove(x, y, zctrl, zshift, zdown);
}

bool Detach()
{
    if (tWinDC == NULL) return false;
    tWinDC = NULL;
    playing = false;
    return true;
}
