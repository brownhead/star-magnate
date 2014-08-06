#ifndef TRIG_HPP_INCLUDED
#define TRIG_HPP_INCLUDED

#define SQR(i) ((i) * (i))
void moveDir (float* zx, float* zy, float Degrees, float Force);
float getDist (float x1, float y1, float x2, float y2);
struct rectf
{
    float left;
    float top;
    float right;
    float bottom;
};
const float PI = 3.14159265;

#endif // TRIG_HPP_INCLUDED
