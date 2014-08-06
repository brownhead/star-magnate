#ifndef DRAWING_HPP_INCLUDED
#define DRAWING_HPP_INCLUDED

#include <gl/gl.h>
#include <math.h>

#include "drawing.hpp"

void drawRough (object* obj)
{
    // Get the dimensions of the object
    rectf dim;
    dim = obj->getRect();

    // Draw the object
    BitBlt (tWinDC, (int)dim.left, (int)dim.top, int(dim.right - dim.left), int(dim.bottom - dim.top), NULL, 0, 0, WHITENESS);
}

void drawRough2 (object* obj)
{
    // Get the dimensions of the object
    rectf dim;
    dim = obj->getRect();

    // Draw the object
    BitBlt (tWinDC, (int)dim.left, (int)dim.top, int(dim.right - dim.left), int(dim.bottom - dim.top), NULL, 0, 0, BLACKNESS);
}

void drawCircle (float x, float y, float zradius, COLORREF zcolor, float za)
{
    drawCircle (x, y, zradius, GetRValue(zcolor) / 255.0f, GetGValue(zcolor) / 255.0f, GetBValue(zcolor) / 255.0f, za);
}

void drawCircle (float x, float y, float zradius, float zr, float zg, float zb, float za)
{
    glBegin(GL_TRIANGLE_FAN);
      glColor4f(zr, zg, zb, za);
        for (float i = 0.0f; i < 2.0f * 3.142f; i += 0.126f)
        {
        	glVertex2f(x + zradius * sin(i), y + zradius * cos(i));
        }
    glEnd();
}

#endif
