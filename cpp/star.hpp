#ifndef STAR_HPP_INCLUDED
#define STAR_HPP_INCLUDED

#include <vector>
#include "object.hpp"
#include "board.hpp"
#include "ship.hpp"
#include "safe_ptr.hpp"

class board;

enum starSizeEnum {tiny, normal, large};
enum starTypeEnum {standard, defense, economy, naval};

class star : public object
{
private:
    // The power or size of the star
    starSizeEnum starSize;

    // Used to figure out when the star needs to generate a ship
    int genCount;

    // The player the ship belongs to
    int player;

    // The type of star
    starTypeEnum starType;

    // The decoration for the star
    int decoration;

public:
    // A ship will be generated every genRate frames
    int genRate;

    // Sets the star type
    void setStarType (starTypeEnum ztype);
    starTypeEnum getStarType() { return starType; }

    // Has the star take a step forward in time
    void step();

    // Gets and changes the player
    int getPlayer () { return player; }
    int setPlayer (int zplayer) { player = zplayer; zone.reset(); }

    /* The amount of ships tha this star raises the max ship count
       for the player by. This number will be scaled by the size of
       the star's zone relative to the zone's maximum size */
    int maxShips;

    // A star_ptr to itself
    star_ptr self;

    // The star's orbit
    class _orbit
    {
    private:
        // The vector of orbiting ships
        std::vector<ship_ptr> ships;

        // The parent star
        star_ptr owner;

        // The steps taken and the amount of steps per satelite generated
        int satGenRate;
        int satSteps;
        int satMax;

    public:
        // Sets the owner of the orbit
        void setOwner (star_ptr zstar);

        // Sets the satellite generation rate of the star, and the maximum amount of satellites
        void setSatGenRate (int zvalue) { satGenRate = zvalue; }
        void setSatMax (int zvalue) { satMax = zvalue; }

        // Adds a ship into the star's orbit
        void add (ship_ptr zship);

        // Generates a ship and adds it into the star's orbit
        ship_ptr gen ();

        _orbit();

        void draw();

        void step();

        // Removes the specified ship from the vector of orbiting ships
        void rem (ship_ptr zship);

        // Deletes the last ship in the list OR if satellites are active, will attack the satellites
        void del ();

        // Attacks the star with the specified amount of force
        void attack (float zforce);

        // Returns a pointer to one of the hsips that are in orbit
        ship_ptr get ();

        // Sends a number of ships to another star
        void send(int znum, star_ptr ztarget, float zforce);

        // Gets the number of ships in orbit
        int getNum () { return ships.size(); }
        int getSatNum () { return sats.size(); }

        // When a ship moves through the star's zone it asks whether it should explode accordingly
        bool shipEnter(ship_ptr zship);

        class _satellite : public object
        {
        private:
            star_ptr owner;

            // Its position along the star
            float otheta;

            // The satellites current health
            float health;

            // The satellites max health
            float maxHealth;

            // The amount of steps it takes to regain 1 health
            int healthGen;
            int healthSteps;

            // The amount of steps it takes for the satellite to prepare to shoot
            int primer;
            int primerStep;

        public:
            void step();
            void setOwner (star_ptr zstar) { owner = zstar; step(); }
            void draw();

            // Sets the health regeneration rate and max health
            void setHealthGen(int zvalue) { healthGen = zvalue; }
            void setMaxHealth(int zvalue) { maxHealth = zvalue; }

            // CHecks if a satellite is ready to fire, and fires if it is
            bool isPrimed();

            // Default constructor
            _satellite();

            // Attack the satellite (if returns true, satellite is destroyed)
            float hit(float zforce);
        };

    private:
        // The satellites in orbit
        std::vector<_satellite> sats;

    } orbit;

    // The zone of influence
    class _zone : public object
    {
    private:
        star_ptr owner;

        // The amount of steps taken by the zone
        int stepsTaken;

        // The full size of the zone
        float fullSize;

        // The amount of steps required for the zone to grow to its full size
        int growthRate;

    public:
        void setGrowthRate(int znum);
        void setFullSize(int znum);
        void reset ();

        float getFullSize () { return fullSize; }

        void step();

        _zone();

        void setOwner (star_ptr zstar) { owner = zstar; }

        // When a ship moves through the star's zone it asks whether it should explode accordingly
        bool shipEnter(ship_ptr zship);

        void draw();
    } zone;

    // Overrides setPos so that the zone is moved accordingly
    void setPos (float zx, float zy, float zradius);

    // The construcotor
    star(board* zowner);

    // Creates a star with no owner. For internal use by a Map only.
    star();

    // Draws the star
    void draw();

    // Sets the size of the star
    void setSize (starSizeEnum sz);
    starSizeEnum getSize ();
};

#endif // STAR_HPP_INCLUDED
