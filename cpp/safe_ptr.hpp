#ifndef SAFE_PTR_HPP_INCLUDED
#define SAFE_PTR_HPP_INCLUDED

class ship;
class star;
class board;

/* If you store a ship reference or pointer directly I will personally
   eat your babies. Use this class. Seriously, I will eat your babies.
   I went through quite some problems before I figured out that I needed
   something like this. */
class ship_ptr
{
    private:
        board* target;
        int id;
    public:
        ship_ptr ();
        ship_ptr (board* ztarget, int zid);
        ship& operator* ();
        ship* operator-> ();
        bool operator== (const ship_ptr& z1);
        int getID ();
        board* getTarget ();
        void makeNull ();
        ship_ptr& operator= (const ship_ptr& z1);
        bool isNull ();
        bool isValid ();
};

// Ditto
class star_ptr
{
    private:
        board* target;
        int id;
    public:
        star_ptr ();
        star_ptr(board* ztarget, int zid);
        star& operator* ();
        star* operator-> ();
        bool operator== (const star_ptr& z1);
        int getID ();
        board* getTarget ();
        void makeNull ();
        star_ptr& operator= (const star_ptr& z1);
        bool isNull ();
        bool isValid ();
};

#endif // SAFE_PTR_HPP_INCLUDED
