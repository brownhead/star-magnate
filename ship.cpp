#include <math.h>
#include <iostream>
#include <gl/gl.h>

#include "trig.hpp"
#include "ship.hpp"
#include "trig.hpp"
#include "drawing.hpp"

void ship::del()
{
    self.getTarget()->decNumShips(player);
    exStep = 0;
    status = exploding;
}

ship::ship()
{
    // Sets the size of the ship, and set its location to the top left of the screen
    setPos (0, 0, 12);

    // Make sure ts is NULL
    ts.makeNull();

    // The star initially belongs to player 0
    player = -1;

    // The ship's health
    health = 1.0f;

    // Set the ship to idle
    status = idle;
}

float ship::attack (float zforce)
{
    if (zforce > health)
    {
        self.getTarget()->decNumShips(player);
        del();
        return fabs(health - zforce);
    }
    else
    {
        health -= zforce;
        return -1.0f;
    }
}

void ship::setOrbit(star_ptr zstar)
{
    // If the ship is already orbiting a star, alert the star that the ship is exiting orbit
    if (status == orbiting && !ts.isNull()) ts->orbit.rem(self);

    // Check to see if were about to engage an enemy star
    if (zstar->getPlayer() != player)
    {
        // Is the star protected?
        if (zstar->orbit.getNum() + zstar->orbit.getSatNum() == 0)
        {
            zstar->setPlayer(player);
        }
        else
        {
            // Attack the star
            zstar->orbit.attack(tforce);

            // Destroy thyself
            self.getTarget()->remShip(self);

            return;
        }
    }

    // The ship is now orbitting
    status = orbiting;

    // Set the target star
    ts = zstar;

    // Set the radius
    orad = zstar->getRadius() + 10.0f;

    // Alert the star that we are entering its orbit
    zstar->orbit.add(self);

    // Ensure that the ship is correctly placed in orbit
    step();
}

void ship::setDest(star_ptr zstar, float zforce)
{
    if (ts == zstar) return;

    tforce = zforce;

    // Make sure to alert any star were orbiting that we are leaving
    if (status == orbiting && !ts.isNull())
    {
        // Alerts the star that we are leaving
        ts->orbit.rem(self);

        // Clears the ts pointer
        ts.makeNull();
    }

    // The ship isn't orbitting anymore
    status = moving;

    // Set the target star
    ts = zstar;

    // Get the direction in radians that we must move
    tdir = atan2(ts->getY() - y, ts->getX() - x);

    // Set the steps with the direction we just got
    txs = 10.0f * cos(tdir);
    tys = 10.0f * sin(tdir);

    // Set the count to the amount of steps its going to take to get there
    tc = int((getDist(x, y, ts->getX(), ts->getY()) - 20) / 10.0f);
}

void ship::step()
{
    switch(status)
    {
    case orbiting:
        // Increment theta
        otheta += 0.2094f; // Theta + PI/15
        if (otheta >= 6.2831f) otheta = 0.0f;

        x = (int)(orad * sin(otheta) + ts->getX());
        y = (int)(orad * cos(otheta) + ts->getY());

        break;

    case moving:
        int size;
        star_ptr* hits = self.getTarget()->inZone(self, &size);
        if (hits != NULL)
        {
            for (int i = 0; i < size; i++)
            {
                if (hits[i]->zone.shipEnter(self))
                {
                    del();
                    return;
                }
            }
        }

        // Frees the array that was returned
        free (hits);

        // Add the target xy steps to the xy of the ship
        x += txs;
        y += tys;

        // If we've arrived, start orbiting
        if (--tc <= 0) setOrbit (ts);

        break;

    case exploding:
        // Deletes the ship from memory
        if (++exStep > 20) self.getTarget()->remShip(self);

        break;
    }
}

void ship::setPlayer (int zplayer)
{
    // Add the ship to the new player's ship count
    if (player != -1) self.getTarget()->decNumShips(player);
    player = zplayer;
    self.getTarget()->addNumShips(player);
}

void ship::draw()
{
    if (status == idle) return;

    if (status == exploding)
    {
        float scale = exStep / 20.0f;

        drawCircle (x, y, 6.0f * scale, self.getTarget()->getColor(player), 1.0f * (1.0f - scale));
        drawCircle (x, y - 4, 4.0f * scale, self.getTarget()->getColor(player), 1.0f * (1.0f - scale));
        drawCircle (x + 2, y + 2, 4.0f * scale, self.getTarget()->getColor(player), 1.0f * (1.0f - scale));
        drawCircle (x - 2, y + 2, 4.0f * scale, self.getTarget()->getColor(player), 1.0f * (1.0f - scale));
    }
    else
    {
        float idir = (status == orbiting) ? otheta + PI/2: tdir;\
        float ix = 8.0f * sin(idir), iy = 8.0f * cos(idir);

        glBegin(GL_TRIANGLES);
          // Get the color of the ship
          COLORREF color = self.getTarget()->getColor(player);

          // Assign the color of the ship as the current color
          glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));

          // Get the tip of the triangle
          glVertex2f(x + ix, y + iy);
          float vX = y - y + iy, vY = -(x - x + ix);
          glVertex2f(x + vX / 2.0, y + vY / 2.0);
          glVertex2f(x - vX / 2.0, y - vY / 2.0);
        glEnd();
    }
}
