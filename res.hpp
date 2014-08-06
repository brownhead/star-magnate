#ifndef RES_HPP_INCLUDED
#define RES_HPP_INCLUDED

/* This deals with loading and unloading resources so that I
   don't have to. */

#include <map>
#include <windows.h>
#include <GL/glut.h>

class res
{
public:
    struct image
    {
        char* name;
        HBITMAP hbmp;
        GLubyte* gl;
        bool monochrome;
        int usage;
        int width;
        int height;
    };
    static int load(char* Name, int width, int height, bool zmonochrome = false);
    static void unload(int index);
    static image get(int index);

private:
    static std::map<int, image> images;
    static GLubyte* BMPToGLubyte (HBITMAP hBMP, bool zmonochrome = false);
};

#endif // RES_HPP_INCLUDED
