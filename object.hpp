#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <windows.h>
#include "trig.hpp"

class object
{
private:
    // Checks to see if the point intersects with the object, if the object is a square or rectangle
    bool hitTestS (float zx, float zy);

    // Checks to see if the point intersects with the object, if the object is round
    bool hitTestR (float zx, float zy);

protected:
    // The position of the center of the object
    float x,y;

    // The dimensions of the object
    float width,height;

    // The radius of the object, saved for performance reasons
    float radius;

    // Sets whether or not the object is a circle
    bool isCircle;

public:
    // Constructor
    object();

    // Get the x cordinate of the center of the object
    float getX () { return x; }

    // Get the y cordinate of the center of the object
    float getY () { return y; }

    // Get the width of the object
    float getWidth () { return width; }

    // Get the height of the object
    float getHeight () { return height; }

    // Get the left value of the rect for the object
    float getLeft () { return x - width / 2.0f; }

    // Get the top value for the rect of the object
    float getTop () { return y - height / 2.0f; }

    // Get the radius of the object (or the width/2)
    float getRadius () { return radius; }

    // Returns a rect containing the object
    rectf getRect ();

    // A constant value used to signify that an argument is not to be changed
    static const float nochange;

    // Sets the position of the object via x,y,width, and height
    virtual void setPos (float zleft, float ztop, float zwidth, float zheight);

    // Sets the position of the object via x,y, and radius
    virtual void setPos (float zx, float zy, float zradius);

    // Detects whether the point intersects with the object or not
    virtual bool hitTest(float zx, float zy);

    // Draws the object
    virtual void draw ();
};

#endif // OBJECT_HPP_INCLUDED
