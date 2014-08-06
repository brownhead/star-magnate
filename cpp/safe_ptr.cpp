#include "safe_ptr.hpp"
#include "board.hpp"

ship_ptr::ship_ptr ()
{
    id = -1;
}

ship_ptr::ship_ptr (board* ztarget, int zid)
{
    target = ztarget; id = zid;
}

ship& ship_ptr::operator* ()
{
    return target->getShip(id);
}

ship* ship_ptr::operator-> ()
{
    return &target->getShip(id);
}

bool ship_ptr::operator== (const ship_ptr& z1)
{
    return (z1.id == id) && (z1.target == target);
}

int ship_ptr::getID ()
{
    return id;
}

board* ship_ptr::getTarget ()
{
    return target;
}

ship_ptr& ship_ptr::operator= (const ship_ptr& z1)
{
    if (this != &z1)
    {
        id = z1.id;
        target = z1.target;
    }
    return *this;
}

void ship_ptr::makeNull ()
{
    id = -1;
}

bool ship_ptr::isNull ()
{
    if (id == -1) return true;
    return false;
}

star_ptr::star_ptr ()
{
    id = -1;
}

star_ptr::star_ptr(board* ztarget, int zid)
{
    target = ztarget; id = zid;
}

star_ptr& star_ptr::operator= (const star_ptr& z1)
{
    if (this != &z1)
    {
        id = z1.id;
        target = z1.target;
    }
    return *this;
}

star& star_ptr::operator* ()
{
    return target->getStar(id);
}

star* star_ptr::operator-> ()
{
    return &target->getStar(id);
}

bool star_ptr::operator== (const star_ptr& z1)
{
    return (z1.id == id) && (z1.target == target);
}

int star_ptr::getID ()
{
    return id;
}

board* star_ptr::getTarget ()
{
    return target;
}

void star_ptr::makeNull ()
{
    id = -1;
}

bool star_ptr::isNull ()
{
    if (id == -1) return true;
    return false;
}
