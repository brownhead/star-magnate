#include <math.h>
#include <windows.h>
#include <iostream>

#include "trig.hpp"

void moveDir (float* zx, float* zy, float zrad, float zforce)
{
    (*zx) = zforce * cos(zrad);
    (*zy) = zforce * sin(zrad);
}

float getDist (float x1, float y1, float x2, float y2)
{
    return sqrt(SQR(x2 - x1) + SQR(y2 - y1));
}
