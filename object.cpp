#include <math.h>
#include <iostream>// NOTE (John#1#): Take out when done debugging
#include <limits.h>
#include <float.h>

#include "object.hpp"
#include "drawing.hpp"

const float object::nochange = FLT_MIN;

object::object()
{
    // Set all initial values to 0
    x = y = width = height = radius = 0;

    // The object is square by default
    isCircle = false;
}

rectf object::getRect ()
{
    // Create a rectf struct and populate it
    rectf rV;
    rV.left   = getLeft();
    rV.top    = getTop();
    rV.right  = rV.left + getWidth();
    rV.bottom = rV.top - getHeight();

    // Return the rectf
    return rV;
}

void object::setPos (float zleft, float ztop, float zwidth, float zheight)
{
    // Set the width value
    if (zwidth != nochange)
    {
        width = zwidth;
        radius = zwidth / 2.0f;
    }

    // Set the height value
    if (zheight != nochange) height = zheight;

    // Set the x cordinate
    if (zleft != nochange) x = zleft + width / 2.0f;

    // Set the y cordinate
    if (ztop != nochange) y = ztop + height / 2.0f;

    // The object is now a square
    isCircle = false;
}

void object::setPos (float zx, float zy, float zradius)
{
    // Set the width and height
    if (zradius != nochange)
    {
        width = height = zradius * 2.0f;
        radius = zradius;
    }

    // Set the x cordinate
    if (zx != nochange) x = zx;

    // Set the y cordinate
    if (zy != nochange) y = zy;

    // The object is now a circle
    isCircle = true;
}

void object::draw()
{
    // Defaults to drawing a white square of the object
    drawRough (this);
}

bool object::hitTest(float zx, float zy)
{
    // If its a circle, use hitTestR (round)
    if (isCircle) return hitTestR(zx,zy);

    // If its a scuare or rectangle, use hitTestS (square)
    return hitTestS(zx,zy);
}

bool object::hitTestS(float zx, float zy)
{
    // Check to see if the point is in bounds of the object
    if (zx >= getLeft() && zx <= getLeft() + width &&
        zy <= getTop() && zy >= getTop() - height)
          return true;

    // If not, return false
    return false;
}

bool object::hitTestR(float zx, float zy)
{
    // See whether the points given are within range of the object
    if (SQR(x - zx) + SQR(y - zy) <= SQR(width / 2)) return true;

    // If not, return false
    return false;
}
