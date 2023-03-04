#include "lrandom.h"

int randNum(int l, int r)
{
    return l + rand()%(r - l + 1);
}