#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "object.hpp"
#include <vector>

class board;

class menuItem : public object
{
public:
    menuItem(char* name, int zwidth, int zheight);

    void setOverImage(char* name);

    void mouseMove(int x, int y);

    void draw();
    void (*click)(board* zowner);
    char* tooltip;
private:
    bool showOver;

    int image;
    int overImage;
};

class menu : public object
{
private:
    std::vector<menuItem> leftItems;
    std::vector<menuItem> rightItems;
    board* ownerBoard;

public:
    int spacing;
    int vertpadding;
    void addItem (menuItem zitem, bool zleft);
    void draw();
    void mouseMove(int x, int y);
    void click(int x, int y);
    menu(board* zownerBoard);
};


#endif // MENU_HPP_INCLUDED
