#include "set.h"

/*
* This function returns an empty set.  You can assign its return value
* to a variable of type Set.
*/
Set set_empty(void) {
    Set a = 0x00;
    return a;
}

/*
* Return set in which every possible member is part of the set
*/
Set set_universal(void) {
    Set a = 0xff;
    return a;
}

/*
* Determine whether the Set s has member x.
*
*   s:  a variable of type Set.
*   x:  an integer value from 0 through 31.
*
* Return value:  true (== 1) if x is in s
*                false (== 0) otherwise
*/
bool set_member(Set s, int x) {
    Set mask = 0x01 << x;
    if ((s & mask) == mask) {
        return true;
    } else {
        return false;
    }
}

/*
* Add the member x to the Set s.
*
*   s:  a variable of type Set.
*   x:  an integer value from 0 through 31.
*
* Return value:  the new Set, which will contain member x.
*/
Set set_insert(Set s, int x) {
    Set mask = 0x01 << x;
    s |= mask;
    return s;
}

/*
* Remove the member x from the Set s.
*
*   s:  a variable of type Set.
*   x:  an integer value from 0 through 31.
*
* Return value:  the new Set, which will not contain member x.
*/
Set set_remove(Set s, int x) {
    Set mask = 0x01 << x;
    s &= ~(mask);
    return s;
}

/*
* Compute the union of two sets.
*
*   s:  a variable of type Set.
*   t:  a variable of type Set.
*
* Return value:  the new Set, which will contain members that are in
*                either s or t.
*/
Set set_union(Set s, Set t) {
    Set u = 0x00;
    u = s | t;
    return u;
}

/*
* Compute the intersection of two sets.
*
*   s:  a variable of type Set.
*   t:  a variable of type Set.
*
* Return value:  the new Set, which will contain only members that are in
*                both s and t.
*/
Set set_intersect(Set s, Set t) {
    Set i = 0x00;
    i = s & t;
    return i;
}

/*
* Compute the difference of two sets.
*
*   s:  a variable of type Set.
*   t:  a variable of type Set.
*
* Return value:  the new Set, which will contain only members that are in
*                s but NOT in t.
*/
Set set_difference(Set s, Set t) {
    Set x = 0x00;
    Set d = 0x00;
    x = s ^ t;
    d = s & x;
    return d;
}

/*
* Compute the complement of a set.
*
*   s:  a variable of type Set.
*
* Return value:  the new Set, which will contain only members that
*                are NOT in s.
*/
Set set_complement(Set s) {
    Set c = 0x00;
    c = ~s;
    return c;
}
