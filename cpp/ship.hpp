#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "object.hpp"
#include "star.hpp"
#include "board.hpp"
#include "typedef.hpp"
#include "safe_ptr.hpp"

class board;
class star;

enum shipStatus {moving,orbiting,idle,exploding};

class ship : public object
{
private:
    /* For use when moving. The target X and Y steps (like the slope) and the amount of steps
       it will take to get there. */
    float txs, tys;
    int tc;
    float tdir;

    // This is the effective power fo the ship. If it was sent with a surrounding force it will be high
    float tforce;

    /* For use when orbiting. The radius of the orbit (distance from the star), and the
       star that the ship is orbiting. And the position of the ship in orbit (theta) */
    float orad, otheta;
    star_ptr ts;

    // For use when exploding
    int exStep;

    // The status of the ship
    shipStatus status;

    // The player the ship belongs to
    int player;

    // The amount of health the ship has
    float health;

public:
    void setPlayer(int zplayer);
    int getPlayer() { return player; }

    // A ship_ptr object pointing to itself
    ship_ptr self;

    // Returns the status of the ship
    shipStatus getStatus() { return status; }

    // Destroys the ship
    void del ();

    // Attacks the ship for a certain amount of damage, returning the leftover damage
    float attack (float zpower);

    // The constructor
    ship();

    // Takes one step in time
    void step();

    // Orbit the target star
    void setOrbit (star_ptr zstar);

    // Moves the ship to the target
    void setDest (star_ptr zstar, float zforce);

    void setIdle () { status = idle; }

    // Draws the ship
    void draw();
};
#endif // SHIP_H_INCLUDED
