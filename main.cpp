#include <iostream>
#include <list>
#include <windows.h>

using namespace std;

bool Attach(HDC hDC);
void Play();
void Stop();
bool Detach();

int main()
{
    Attach (GetDC(0));
    Play();
    system ("Pause");
    Detach ();
    return 0;
}
