#ifndef LEHMER_DEF
#define LEHMER_DEF

#include "defs.h"

#define LEHMER_A 16807
#define LEHMER_M 2147483647


static uint32_t lehmer = 1;

uint32_t Lehmer32()
{
    lehmer = (LEHMER_A * lehmer) % LEHMER_M;
    return lehmer;
}

#endif




