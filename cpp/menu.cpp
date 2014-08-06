#include <GL/gl.h>
#include <iostream>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

#include "res.hpp"
#include "menu.hpp"

menuItem::menuItem(char* name, int zwidth, int zheight)
{
    // Load the image from our resource file
    image = res::load(name, zwidth, zheight);
    assert(image != -1);

    overImage = -1;

    // Set the width and height
    width = zwidth;
    height = zheight;
}

void menuItem::mouseMove(int x, int y)
{
    showOver = hitTest((float)x, (float)y);
}

void menuItem::setOverImage(char* name)
{
    // Load the image from our resource file
    overImage = res::load(name, (int)width, (int)height);
    assert(overImage != -1);
}

void menuItem::draw()
{
    // Set the raster position
    glRasterPos2f(getLeft(), getTop() - height);

    // Decide which image were going to draw
    int drawImage;
    if (showOver && overImage != -1) drawImage = overImage; else drawImage = image;

    // Draw those pixels
    glDrawPixels((int)width, (int)height, GL_BGR, GL_UNSIGNED_BYTE, res::get(drawImage).gl);
}


void menu::addItem(menuItem zitem, bool zleft)
{
    menuItem item = zitem;
    if (zleft)
    {
        // LastX holds the rightmost edge of the rightmost item in the left menu
        float lastX;

        // If there are no items on the left side of the menu, lastX = 0, else calculate its value
        if (leftItems.empty()) lastX = 0; else lastX = leftItems.back().getLeft() + leftItems.back().getWidth();

        // Set the item's position (lastX + spacing)
        item.setPos (lastX + spacing, vertpadding, menuItem::nochange, menuItem::nochange);
        leftItems.push_back(item);
    }
    else
    {
        // lastX holds the leftmost edge of the leftmost item in the right menu.
        float lastX;

        // If there are no items on the right side of the menu, lastX = the width of the entire menu, otherwise calculate lastX
        if (rightItems.empty()) lastX = width; else lastX = rightItems.back().getLeft();

        // Set the position of the item (lastX - the item-to-be-added's width - spacing)
        item.setPos (lastX - item.getWidth() - (rightItems.empty() ? 0 : spacing), getTop() - vertpadding, menuItem::nochange, menuItem::nochange);

        // Add the item
        rightItems.push_back(item);
    }
}

menu::menu(board* zownerBoard)
{
    ownerBoard = zownerBoard;
    spacing = 1;
    vertpadding = 0;
}

void menu::draw()
{
    for (int i = 0; i < leftItems.size(); ++i) leftItems[i].draw();
    for (int i = 0; i < rightItems.size(); ++i) rightItems[i].draw();
}

void menu::mouseMove(int x, int y)
{
    for (int i = 0; i < leftItems.size(); ++i) leftItems[i].mouseMove(x, y);
    for (int i = 0; i < rightItems.size(); ++i) rightItems[i].mouseMove(x, y);
}

void menu::click(int x, int y)
{
    // Check to see if any of the left aligned buttons were hit
    for (int i = 0; i < leftItems.size(); ++i)
    {
        if (leftItems[i].hitTest(x, y))
        {
            if (leftItems[i].click != NULL) leftItems[i].click(ownerBoard);
        }
    }

    // Check to see if any of the right aligned buttons were hit
    for (int i = 0; i < rightItems.size(); ++i)
    {
        if (rightItems[i].hitTest(x, y))
        {
            if (rightItems[i].click != NULL) rightItems[i].click(ownerBoard);
        }
    }
}
