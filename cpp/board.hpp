#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <gl/gl.h>

#include "ship.hpp"
#include "menu.hpp"
#include "typedef.hpp"
#include "safe_ptr.hpp"
#include "star.hpp"
#include "object.hpp"

class ship;
class star;
class star_ptr;
class ship_ptr;

// The root class for the game
class board : public object
{
private:
    // Holds the max ships that each player can support
    std::vector<int> maxShips;

    // Holds the number of ships that each player has
    std::vector<int> numShips;

    // A list holding all the ships on the board
    std::map<int,ship> ships;

    // A vector of the player colors
    std::vector<COLORREF> playerColors;

    /* The queue of ships and stars to delete. This is neccassary
       because during the step function an iterator is used to
       run through the ships and stars maps. If a ship or star is
       deleted while that iteration is still going, shit will hit
       the fan, and thats no good. */
    std::queue<ship_ptr> ships_del;
    std::queue<star_ptr> stars_del;

    // Defines a line
    struct line
    {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    // Holds cordinates for dashed lines to be drawn during the next draw sequence.
    std::vector<line> pathLines;

    /* When the user clicks and drags the mouse across the screen a selection
       box appears. This class defines that selection box */
    class _dragBox : public object
    {
    public:
        // Holds the anchor points of the drag box (the first mouse down)
        int anchorX, anchorY;

        // Whether or not the drag box needs to be drawn or not
        bool active;

        // Draws the drag box
        void draw();

        // Default constructor
        _dragBox();
    } dragBox;

public:
    // The board's menu
    menu topMenu;

    // The current fps
    char* fps;

    // Gets the maximum amount of ships a certain player can have
    int getMaxShips (int zplayer);

    // Sets the maximum amount of ships a certain player can have
    void setMaxShips (int zplayer, int zvalue);

    // Add to the number of ships a player currently owns
    int addNumShips (int zplayer);

    // Decrement the number of ships a player currently owns
    int decNumShips (int zplayer);

    // Get the number of ships a player currently owns
    int getNumShips (int zplayer);

    // Gets the color associated with a player
    COLORREF getColor(int zplayer);

    // Holds the selected stars
    std::vector<star_ptr> selectedStars;

    // Adds a star to the selection vector
    void addSelectedStar (star_ptr zstar);

    // Removes a star from the selection vector
    void delSelectedStar (star_ptr zstar);

    // Removes all stars from the selection vector
    void clsSelectedStars ();

    // Checks if a star is selected
    bool isStarSelected (star_ptr zstar);

    // The user has clicked somewhere
    void click(int zx, int zy, bool zctrl, bool zshift, bool zdbl, bool zdown);

    // The user has moved the mouse over the game
    void mouseMove(int zx, int zy, bool zctrl, bool zshift, bool zdown);

    // Check to see if a ship is in an enemy's zone
    star_ptr* inZone (ship_ptr zship, int* zsize);

    // A list holding all the stars on the board
    std::map<int,star> stars;

    // The device and render contexts to draw to
    HDC hDC;
    HGLRC hRC;

    // Construct me? NO! Construct YOU!
    board();

    // The default width and height
    static const int stdwidth, stdheight;

    // Add a star
    star_ptr addStar ();

    // Delete a star
    void remStar (star_ptr zstar);

    // Retrieves a direct reference to a star from its id
    star& getStar (int zid);

    // Add a ship
    ship_ptr addShip();

    // Remove a ship from existence
    void remShip (ship_ptr zship);

    // Retrieve a direct reference to a ship from its id
    ship& getShip(int zid);

    // Draw the board and all of its sub-objects
    void draw();

    // Processes all of the sub-objects movements (calls their step functions)
    void step();
};

#endif // BOARD_HPP_INCLUDED
