#include "res.hpp"
#include "getAvailableKey.hpp"

#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <string.h>

std::map<int, res::image> res::images;

int res::load (char* Name, int width, int height, bool zmonochrome)
{
    // Loops through the images map and checks to see if we've already loaded this resource
    for (std::map<int, image>::iterator i = images.begin(); i != images.end(); ++i)
    {
        if (strcmp(i->second.name, Name) == 0)
        {
            // Another reference to this resource exists
            ++i->second.usage;

            // Return the pre-loaded resource
            return i->first;
        }
    }

    // Load the bitmap from memory
    HBITMAP newBMP = (HBITMAP)LoadImage (GetModuleHandle("libStar Magnate.dll"), Name, IMAGE_BITMAP, width, height, 0);

    // Check to see if it was successful
    if (newBMP == NULL) return -1;

    image newImage;

    // Add the name to the vector
    char* newName = (char*)malloc(strlen(Name) + 1);
    strcpy(newName, Name);
    newImage.name = newName;

    // Save the width and height
    newImage.width = width;
    newImage.height = height;

    // Add the bitmap handle to the image struct
    newImage.hbmp = newBMP;

    // Add the byte array to the image struct
    newImage.gl = BMPToGLubyte(newBMP, zmonochrome);

    // Make sure we know whether this new image is monochrome or not
    newImage.monochrome = zmonochrome;

    // Theres one reference to this image alive
    newImage.usage = 1;

    // Find a useable key
    int newKey = getAvailableKey<image>(&images);

    // Add the new image to the images vector
    images[newKey] = newImage;

    return newKey;
}

void res::unload (int index)
{
    // Decrement the usage of the image and continue if the usage is 0
    if (--images[index].usage > 0) return;

    // Free the GLubyte array
    free(images[index].gl);

    // Unlaod the bitmap from memory
    DeleteObject(images[index].hbmp);

    // Delete the item
    images.erase(index);
}

res::image res::get(int index)
{
    // Return the information on the image
    return images[index];
}

GLubyte* res::BMPToGLubyte (HBITMAP hBMP, bool zmonochrome)
{
    BITMAP BMP;
    BITMAPINFO BMPInfo;
    HANDLE OldBMP;
    HDC BufferDC;
    int BMPHeight, BMPWidth;

    // Go from bitmap handle to full on bitmap. This is a big step for little timmy
    GetObject(hBMP, sizeof(BITMAP), &BMP);

    // Create a buffer to hold our bitmap
    BufferDC = CreateCompatibleDC (GetDC(0));

    // Select the bitmap into our buffer
    OldBMP = SelectObject (BufferDC, hBMP);

    // Save the height and width
    BMPHeight = BMP.bmHeight;
    BMPWidth = BMP.bmWidth;

    // Set the bitmap header info
    BMPInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   // The size of the strucutre
    BMPInfo.bmiHeader.biWidth = BMP.bmWidth;               // The width of the bitmap
    BMPInfo.bmiHeader.biHeight = BMP.bmHeight;             /* The height of the bitmap (Negative
                                                              to signify that the bitmap should
                                                              be organized top to bottom rather
                                                              than bottom to top */
    BMPInfo.bmiHeader.biPlanes = 1;                        /* The number of color planes (always
                                                              equals 1) */
    BMPInfo.bmiHeader.biBitCount = (zmonochrome ? 1 : 24); /* The bits per pixel (Amount of binary
                                                              bits (0, 1) required to store a
                                                              single pixel). We force it to make
                                                              everything uniform */
    BMPInfo.bmiHeader.biCompression = BI_RGB;              /* The comptression type, can be either
                                                              BI_RGB or BI_PALLETE (Or something
                                                              like that). Tells the computer
                                                              whether the image holds an absolute
                                                              RGB value for each pixel or just a
                                                              number pointing to a entry in a
                                                              pallete */
    BMPInfo.bmiHeader.biSizeImage = ((((BMPWidth * BMPInfo.bmiHeader.biBitCount) + 31) / 32) * 4) * BMPHeight; // The size of the actual image in bytes

    GLubyte* ByteArray = (GLubyte*) malloc(BMPInfo.bmiHeader.biSizeImage);

    // Gets the device independant bits (The actual pixels in a dc)
    GetDIBits (BufferDC, hBMP, 0, BMPHeight, ByteArray, &BMPInfo, DIB_RGB_COLORS);

    // Clean up
    SelectObject (BufferDC, OldBMP);
    DeleteDC (BufferDC);

    return ByteArray;
}
