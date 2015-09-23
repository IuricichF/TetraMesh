#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

typedef int VertexIndex;
typedef int TetraIndex;

typedef unsigned short int PositionIndex;

static TetraIndex reservedTetraIndexes=2;
static TetraIndex maxTetraNumber=UINTPTR_MAX-reservedTetraIndexes;

static TetraIndex noAdjacentTetraIndex=maxTetraNumber+1;
static TetraIndex uninitializedTetraIndex=maxTetraNumber+2;


#endif // DEFINE_H
