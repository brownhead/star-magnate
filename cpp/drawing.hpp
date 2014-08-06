#ifndef DRAWING_HPP
#define DRAWING_HPP

#include "object.hpp"
extern HDC tWinDC;
void drawRough (object* obj);
void drawRough2 (object* obj);
void drawCircle (float x, float y, float zradius, float zr, float zg, float zb, float za);
void drawCircle (float x, float y, float zradius, COLORREF zcolor, float za);

#endif // DAWING_HPP
