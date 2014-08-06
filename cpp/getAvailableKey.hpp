#ifndef GETAVAILABLEKEY_HPP_INCLUDED
#define GETAVAILABLEKEY_HPP_INCLUDED

#include <map>

template <typename valuetype>
int getAvailableKey(const std::map<int, valuetype>* zmap)
{
    // These need to be in this scope because we'll use them later
    int selectedKey = -1, j = 0;

    // Loop through all the items in the map
    for (typename std::map<int, valuetype>::const_iterator i = zmap->begin(); i != zmap->end(); ++i, ++j)
    {
        if (i->first != j)
        {
            selectedKey = j;
            break;
        }
    }

    // If we didn't find a key to use
    if (selectedKey == -1)
    {
        // Make sure were not passed the maximum int value
        if (j != INT_MAX)
            // J should equal zmap.length + 1
            selectedKey = j;
        else
            // If were passed the maximum int value halt the program. This shouldn't happen
            assert("A map is being filled past it's brim." && false);
    }

    return selectedKey;
}

#endif // GETAVAILABLEKEY_HPP_INCLUDED
