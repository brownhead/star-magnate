#include <iostream>
#include <limits.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#include "board.hpp"
#include "drawing.hpp"
#include "getAvailableKey.hpp"

const int board::stdheight = 320;
const int board::stdwidth = 480;

void btnStandard_Click(board* zowner)
{
    for (int i = 0; i < zowner->selectedStars.size(); ++i)
    	if (zowner->selectedStars[i]->getStarType() != standard) zowner->selectedStars[i]->setStarType(standard);
}

board::board() : topMenu(this)
{
    setPos (0,0,stdwidth,stdheight);

    playerColors.push_back(0xFF0000); // Player 1 is Blue
    playerColors.push_back(0x0000FF); // Plyaer 2 is Red
    playerColors.push_back(0x00FF00); // Player 3 is Blue
    playerColors.push_back(0x00FFFF); // Player 4 is Yellow

    // Set the position of the menu
    topMenu.setPos(0.0f, height, getWidth(), 16.0f);

    menuItem newMnuItm("menu_button", 16, 16);
    newMnuItm.setOverImage("menu_button_over");
    newMnuItm.click = btnStandard_Click;
    topMenu.addItem(newMnuItm, false);

    // Give each player a spot to store their max ships
    maxShips.resize(4, 0);

    // Give each player a spot to store their number of ships
    numShips.resize(4, 0);

    fps = NULL;
}

int board::getMaxShips (int zplayer)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Set the value
    return maxShips[zplayer];
}

void board::setMaxShips (int zplayer, int zvalue)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Set the value
    maxShips[zplayer] = zvalue;
}

int board::addNumShips (int zplayer)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Add the ship and return the new value
    return ++numShips[zplayer];
}

int board::decNumShips (int zplayer)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Decrement the ship count and return the new value
    return --numShips[zplayer];
}

int board::getNumShips (int zplayer)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Return the number of ships
    return numShips[zplayer];
}

COLORREF board::getColor(int zplayer)
{
    // Verify the arguments
    assert(zplayer >= 0 && zplayer < maxShips.size());

    // Return the player's color
    return playerColors[zplayer];
}

void glEnable2D()
{
   int vPort[4];

   glGetIntegerv(GL_VIEWPORT, vPort);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LINE_STIPPLE);
   glEnable(GL_BLEND);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
}

void glDisable2D()
{
   glDisable(GL_BLEND);
   glDisable(GL_LINE_STIPPLE);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

void renderBitmapString(float zx, float y, void *font, char *string)
{
  glRasterPos2f(zx,y);

  //int x1=x;
  for (char* c=string; *c != '\0'; c++)
  {
    glutBitmapCharacter(font, *c);
	//x1 = x1 + glutBitmapWidth(font,*c) + spacing;
  }
}

void board::draw()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable 2D drawing
    glEnable2D();

    // Draw the paths from star to star if neccasary
    for (int i = 0; i < pathLines.size(); i++)
    {
        // Set a very thin line width
    	glLineWidth(1);

    	// Save the last value of GL_LINE_BIT
        glPushAttrib (GL_LINE_BIT);

        // This gives us a nice dashed line
        glLineStipple (3, 0xAAAA);

        // Draw the line
        glBegin (GL_LINE_STRIP);
          glColor4ub(255, 255, 255, 100);
          glVertex2f(pathLines[i].x1, pathLines[i].y1);
          glVertex2f(pathLines[i].x2, pathLines[i].y2);
        glEnd ();

        // Reset the FL_LINE_BIT attribute
        glPopAttrib ();
    }

    // Draw the stars and ships
    for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++) i->second.draw();
    for (std::map<int,ship>::iterator i = ships.begin(); i != ships.end(); i++) i->second.draw();

    // Draw the menu
    topMenu.draw();

    // Draw the dragBox if needed
    if (dragBox.active) dragBox.draw();

    // Draw the FPS
    if (fps != NULL)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        renderBitmapString(width - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (unsigned char*) fps) - 25, 12, GLUT_BITMAP_HELVETICA_10, fps);
    }

    // Disable 2D drawing
    glDisable2D();

    // All done, draw the buffer to the window
    SwapBuffers(hDC);
}

ship_ptr board::addShip()
{
    // These need to be in this scope because we'll use them later
    int selectedKey = getAvailableKey<ship>(&ships);

    // Create a new ship pointer
    ship_ptr ptr (this, selectedKey);

    // Set the ship's self property
    ship tmp;
    tmp.self = ptr;

    // Add the ship to the map
    ships.insert(std::make_pair(selectedKey, tmp));

    // Return the pointer
    return ptr;
}

star_ptr board::addStar ()
{
    star tmp;

    // These need to be in this scope because we'll use them later
    int selectedKey = getAvailableKey<star>(&stars);

    // Create a new ship pointer
    star_ptr ptr (this, selectedKey);

    // Set the star's self property
    tmp.self = ptr;

    // Set the star's orbit's owner property
    tmp.orbit.setOwner(ptr);

    // Set the star's zone's owner property
    tmp.zone.setOwner(ptr);
    tmp.zone.setFullSize(60);
    tmp.zone.setGrowthRate(500);

    // Add the ship to the map
    stars.insert(std::make_pair(selectedKey, tmp));

    // Return the pointer
    return ptr;
}

void board::remShip(ship_ptr zship)
{
    // Remove the ship
    ships_del.push(zship);

    // We don't want the ship to go banging around while its waiting to be obliterated
    zship->setIdle();
}

ship& board::getShip(int zid)
{
    // Check to make sure the star exists
    assert(ships.count(zid) != 0);

    return ships[zid];
}

void board::remStar(star_ptr zstar)
{
    // Remove the star
    stars_del.push(zstar);
}

star& board::getStar(int zid)
{
    // Check to make sure the star exists
    assert(stars.count(zid) != 0);

    return stars[zid];
}

void board::step()
{
    // Iterate through the stars and figure out how many ships each player can have
    for (int i = 0; i < maxShips.size(); i++) maxShips[i] = 0;
    for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++)
        maxShips[i->second.getPlayer()] += int(i->second.maxShips * (i->second.zone.getRadius() / i->second.zone.getFullSize()));

    // Tell all the object to take a step
    for (std::map<int,ship>::iterator i = ships.begin(); i != ships.end(); i++) i->second.step();
    for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++) i->second.step();

    // Delete any ships that need to be deleted
    while (!ships_del.empty())
    {
        ships.erase(ships_del.front().getID());
        ships_del.pop();
    }

    // Delete any stars that need to be deleted
    while (!ships_del.empty())
    {
        stars.erase(stars_del.front().getID());
        stars_del.pop();
    }
}

star_ptr* board::inZone (ship_ptr zship, int* zsize)
{
    float zx = zship->getX(), zy = zship->getY();

    star_ptr* hits = NULL;
    (*zsize) = 0;

    for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++)
    {
        if (i->second.getPlayer() != zship->getPlayer())
        {
            if (i->second.zone.hitTest(zx, zy))
            {
                hits = (star_ptr*) realloc(hits, ++(*zsize) * sizeof(star_ptr));
                hits[(*zsize) - 1] = i->second.self;
            }
        }
    }

    return hits;
}

void board::addSelectedStar (star_ptr zstar)
{
    // Add the star
    selectedStars.push_back (zstar);
}

bool board::isStarSelected (star_ptr zstar)
{
    // Iterate through the vector and see if the star is in it
    for (int i = 0; i < selectedStars.size(); i++)
    	if (selectedStars[i] == zstar) return true;

    // If the star wasn't found return false
    return false;
}

void board::clsSelectedStars ()
{
    // Clear the selection vector
    selectedStars.clear();
}

void board::delSelectedStar (star_ptr zstar)
{
    for (int i = 0; i < selectedStars.size(); i++)
    {
    	if (selectedStars[i] == zstar)
    	{
    	    selectedStars.erase(selectedStars.begin() + i);
    	    return;
    	}
    }
}

void board::click(int zx, int zy, bool zctrl, bool zshift, bool zdbl, bool zdown)
{
    if (zdown)
    {
        dragBox.anchorX = zx;
        dragBox.anchorY = zy;
        dragBox.setPos(zx, zy, zx, zy);
    }
    else if (!zdbl)
    {
        // Make sure the dragbox isn't being shown
        if (dragBox.active)
        {
            for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++)
            {
                if (dragBox.hitTest(i->second.getX(), i->second.getY()))
                {
                    if (!isStarSelected(i->second.self) && i->second.getPlayer() == 0) addSelectedStar(i->second.self);
                }
            }
            dragBox.active = false;
            draw();
        }

        // Exit because we really only want to deal with mouse downs
        return;
    }

    for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++)
    {
        if (i->second.hitTest(zx, zy))
        {
            if (i->second.getPlayer() == 0 && (selectedStars.empty() || zctrl || zshift))
            {
                if (isStarSelected(i->second.self))
                {
                    if (zctrl) delSelectedStar(i->second.self);
                }
                else
                {
                    addSelectedStar(i->second.self);
                }
            }
            else
            {
                float force = 0;
                if (i->second.getPlayer() != 0)
                {
                    // Determine the force of the attacking ships
                    int* quads = (int*) malloc(4 * sizeof(int));
                    int total = 0, greatest = 0;
                    for (int j = 0; j < 4; j++) quads[j] = false;
                    for (int j = 0; j < selectedStars.size(); j++)
                    {
                        if (selectedStars[j]->getX() > i->second.getX())
                        {
                            if (selectedStars[j]->getY() > i->second.getY())
                            {
                                quads[1] += selectedStars[j]->orbit.getNum();
                            }
                            else
                            {
                                quads[2] += selectedStars[j]->orbit.getNum();
                            }
                        }
                        else
                        {
                            if (selectedStars[j]->getY() > i->second.getY())
                            {
                                quads[0] += selectedStars[j]->orbit.getNum();
                            }
                            else
                            {
                                quads[3] += selectedStars[j]->orbit.getNum();
                            }
                        }

                        total += selectedStars[j]->orbit.getNum();
                    }

                    for (int j = 0; j < 4; j++) if (quads[j] > greatest) greatest = quads[j];
                    for (int j = 0; j < 4; j++) force += float(quads[j]) / float(greatest);
                }
                else
                {
                    force = 1;
                }

                for (int j = 0; j < selectedStars.size(); j++)
                {
                	selectedStars[j]->orbit.send(zdbl ? selectedStars[j]->orbit.getNum() : ((int)ceil(selectedStars[j]->orbit.getNum() / 2.0f)), i->second.self, force);
                }
            }
            return;
        }
    }
    if (!zshift && !zctrl) clsSelectedStars();
}

void board::mouseMove(int zx, int zy, bool zctrl, bool shift, bool zdown)
{
    // If the mouse isn't down when this is called
    if (!zdown)
    {
        dragBox.active = false;

        // Clear the pathLines vector
        pathLines.clear();

        // If there are some stars that have been selected
        if (!selectedStars.empty())
        {
            // Loop through all the stars on the screen
            for (std::map<int,star>::iterator i = stars.begin(); i != stars.end(); i++)
            {
                // See if the mouse is over any of the stars
                if (i->second.hitTest(x, y))
                {
                    // All selected stars should now be connected to the mouse'd over star by a white dashed line
                    for (int j = 0; j < selectedStars.size(); j++)
                    {
                        // Don't draw a line to yourself
                        if (!(i->second.self == selectedStars[j]->self))
                        {
                            line lin;
                            lin.x1 = i->second.getX();
                            lin.y1 = i->second.getY();
                            lin.x2 = selectedStars[j]->getX();
                            lin.y2 = selectedStars[j]->getY();
                            pathLines.push_back(lin);
                        }
                    }
                }
            }
        }

        // Tell the menu that the mosue moved
        topMenu.mouseMove(zx, zy);
    }
    else
    {
        dragBox.setPos(dragBox.anchorX - (dragBox.anchorX > zx ? dragBox.anchorX - zx : 0),
                       dragBox.anchorY - (dragBox.anchorY < zy ? dragBox.anchorY - zy : 0),
                       abs(zx - dragBox.anchorX),
                       abs(zy - dragBox.anchorY));
        dragBox.active = true;
        draw();
    }
}

board::_dragBox::_dragBox()
{
    active = false;
    anchorX = anchorY = 0;
}

void board::_dragBox::draw()
{
    rectf pos = getRect();

    glBegin(GL_QUADS);
      glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
      glVertex2f(pos.left, pos.top);
      glVertex2f(pos.left, pos.bottom);
      glVertex2f(pos.right, pos.bottom);
      glVertex2f(pos.right, pos.top);
    glEnd();
}
