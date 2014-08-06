#include "star.hpp"
#include "res.hpp"
#include "drawing.hpp"
#include <iostream>
#include <windows.h>
#include <math.h>

void star::_orbit::setOwner (star_ptr zowner)
{
    owner = zowner;
}

void star::_orbit::add (ship_ptr zship)
{
    // Adds the pointer-to-the-ship to the vector
    ships.push_back (zship);
}

ship_ptr star::_orbit::gen()
{
    ship_ptr newShip = owner->self.getTarget()->addShip();
    newShip->setPlayer(owner->player);

    newShip->setOrbit(owner);
    return newShip;
}

void star::_orbit::draw ()
{
    for (int i = 0; i < sats.size(); i++) sats[i].draw();
}

star::_orbit::_orbit ()
{
    satMax = 4;
    satGenRate = 400;
    satSteps = 0;
}

void star::_orbit::rem(ship_ptr zship)
{
    for (int i = 0; i < ships.size(); i++)
        if (zship == ships[i]) { ships.erase(ships.begin() + i); break; }
}

void star::_orbit::del()
{
    throw 29;
    if (sats.size() > 0)
    {
        // Attack a satellite
        //if (sats.back().hit()) sats.pop_back();
    }
    else if (ships.size() > 0)
    {
        // Tells the last ship to destroy itself
        ships.back()->del();

        // Deletes the now invalid pointer from the vector
        ships.pop_back();
    }
    else
    {
        std::cout << "DEL CALL INVALID\n";
        return;
    }
}

ship_ptr star::_orbit::get()
{
    // If there are not mroe ships return a null ship_ptr
    if (ships.empty())
    {
        ship_ptr newNull;
        return newNull;
    }

    return ships.back();
}

void star::_orbit::send(int znum, star_ptr ztarget, float zforce)
{
    for (int i = 0; i < znum; i++)
    {
    	ship_ptr curShip = get();
    	if (curShip.isNull()) return;
    	curShip->setDest(ztarget, zforce);
    }
}

void star::_orbit::attack (float zforce)
{
    float force = zforce;

    if (sats.size() > 0)
    {
        while (!sats.empty())
        {
            force = sats.back().hit(force);
            if (force != -1.0f)
            {
                sats.pop_back();
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        while (force != -1.0f) force = ships.back()->attack(force);
    }
}

starSizeEnum star::getSize ()
{
    return starSize;
}

star::star()
{
    setSize(normal);

    // The star initially belongs to player 0
    player = 0;

    genRate = 100;
    genCount = 0;

    // The decoratino for the star will be negative one to idicate a standard star
    decoration = -1;

    starType = standard;

    maxShips = 10;
}

void star::setSize (starSizeEnum sz)
{
    starSize = sz;
    switch (starSize)
    {
        case tiny:
            setPos (nochange, nochange, 6);
            break;
        case normal:
            setPos (nochange, nochange, 12);
            break;
        case large:
            setPos (nochange, nochange, 20);
            break;
    }
}

void star::draw()
{
    zone.draw();
    orbit.draw();

    glBegin(GL_TRIANGLE_FAN);
      COLORREF color = self.getTarget()->getColor(player);
      glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
      for (float i = 0.0f; i < 2.0f * 3.142f; i += 0.126f)
      {
          glVertex2f(x + getRadius() * sin(i), y + getRadius() * cos(i));
      }
    glEnd();

    if (self.getTarget()->isStarSelected(self))
    {
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
          glColor3f(1.0f, 1.0f, 1.0f);
          for (float i = 0.0f; i < 2.0f * 3.142f; i += 0.126f)
          {
              glVertex2f(x + (getRadius() + 2) * sin(i), y + (getRadius() + 2) * cos(i));
          }
        glEnd();
    }

    // Draw the star's symbol
    if (decoration != -1)
    {
        // Get the bitmap were drawing
        res::image decImage = res::get(decoration);

        glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
        glRasterPos2f(x - decImage.width / 2, y - decImage.height / 2);
        glBitmap(decImage.width, decImage.height, 0.0f, 0.0f, 0.0f, 0.0f, res::get(decoration).gl);
    }
}

void star::setStarType (starTypeEnum ztype)
{
    starType = ztype;
    switch (starType)
    {
        case standard:
            maxShips = 10;
            genRate = 100;
            genCount = 0;
            zone.setFullSize(60);
            zone.setGrowthRate(500);
            orbit.setSatGenRate(400);
            orbit.setSatMax(4);
            if (decoration != -1) res::unload(decoration);
            decoration = -1;
            break;

        case economy:
            maxShips = 50;
            genRate = 0;
            genCount = 0;
            zone.setFullSize(30);
            zone.setGrowthRate(1000);
            orbit.setSatGenRate(0);
            orbit.setSatMax(0);
            if (decoration != -1) res::unload(decoration);
            decoration = res::load("decoration_economy", 24, 24, true);
            break;

        case naval:
            maxShips = 0;
            genRate = 25;
            genCount = 0;
            zone.setFullSize(30);
            zone.setGrowthRate(500);
            orbit.setSatGenRate(0);
            orbit.setSatMax(0);
            if (decoration != -1) res::unload(decoration);
            decoration = res::load("decoration_naval", 24, 24, true);
            break;

        case defense:
            maxShips = 0;
            genRate = 0;
            genCount = 0;
            zone.setFullSize(120);
            zone.setGrowthRate(1000);
            orbit.setSatGenRate(200);
            orbit.setSatMax(14);
            if (decoration != -1) res::unload(decoration);
            decoration = res::load("decoration_defense", 24, 24, true);
            break;
    }
}

void star::step()
{
    if (genRate != 0)
    {
        if (self.getTarget()->getMaxShips(player) > self.getTarget()->getNumShips(player))
        {
            if (genCount >= genRate)
            {
                orbit.gen();
                genCount = 0;
            }
            else
            {
                genCount++;
            }
        }
    }

    zone.step();
    orbit.step();
}

void star::_zone::draw()
{
    glBegin(GL_TRIANGLE_FAN);
      COLORREF color = owner.getTarget()->getColor(owner->player);
      glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), 50);
        for (float i = 0.0f; i < 2.0f * 3.142f; i += 0.126f)
        {
        	glVertex2f(x + getRadius() * sin(i), y + getRadius() * cos(i));
        }
    glEnd();
}

star::_zone::_zone()
{
    stepsTaken = 0;
    growthRate = 0;
    fullSize = 0;
}

void star::_zone::step()
{
    // Expands the zone if needed
    if (stepsTaken <= growthRate) setPos(nochange, nochange, int(fullSize * (float(++stepsTaken) / float(growthRate))));
}

void star::_zone::setFullSize(int znum)
{
    fullSize = znum;
    reset();
}

void star::_zone::setGrowthRate(int znum)
{
    growthRate = znum;
    reset();
}

void star::setPos (float zx, float zy, float zradius)
{
    zone.setPos(zx, zy, nochange);
    if (zradius != nochange)
    {
        width = height = zradius * 2;
        radius = zradius;
    }

    if (zx != nochange) x = zx;
    if (zy != nochange) y = zy;
    isCircle = true;
}

void star::_orbit::step()
{
    // If the amount of satellites that we can support went down, blow some stuff up
    while (sats.size() > satMax) sats.pop_back();

    // Makes all the satellites take a step
    for (int i = 0; i < sats.size(); i++) sats[i].step();

    if (sats.size() < satMax)
    {
        if (++satSteps >= satGenRate)
        {
            satSteps = 0;

            // Create a new satellite
            _satellite newSat;
            newSat.setOwner(owner);

            // Add it to the list
            sats.push_back(newSat);
        }
    }
}

void star::_orbit::_satellite::step ()
{
    // Regenerate health if needed
    if (health < maxHealth)
    {
        if (++healthSteps >= healthGen)
        {
            healthSteps = 0;
            health++;
        }
    }

    if (primerStep < primer) primerStep++;

    // Increment theta
    otheta += 0.05235f; // Theta + PI/60
    if (otheta >= 6.2831f) otheta = 0.0f;

    x = (int)((owner->getRadius() + 20) * sin(otheta) + owner->getX());
    y = (int)((owner->getRadius() + 20) * cos(otheta) + owner->getY());
}

float star::_orbit::_satellite::hit (float zforce)
{
    if (health < zforce)
    {
        health -= zforce;
        return fabs(health);
    }
    else
    {
        health -= zforce;
        return -1.0f;
    }
}

star::_orbit::_satellite::_satellite ()
{
    healthSteps = 0;
    otheta = 0.0f;
    maxHealth = health = 3.0f;
    healthGen = 1000;
    primer = 10;
    primerStep = 0;

    setPos(nochange, nochange, 3);
}

bool star::_orbit::shipEnter (ship_ptr zship)
{
    for (int i = 0; i < sats.size(); i++)
    	if (sats[i].isPrimed()) return true;
    return false;
}

bool star::_zone::shipEnter (ship_ptr zship)
{
    return owner->orbit.shipEnter (zship);
}

void star::_zone::reset ()
{
    stepsTaken = 0;
    setPos(nochange, nochange, 0);
}

bool star::_orbit::_satellite::isPrimed ()
{
    if (primerStep >= primer)
    {
        primerStep = 0;
        return true;
    }
    return false;
}

void star::_orbit::_satellite::draw ()
{
    drawCircle (x, y, getRadius(), owner.getTarget()->getColor(owner->player), 1.0f);
}
