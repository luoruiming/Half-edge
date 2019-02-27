#include "Vertex.h"
#include <stdio.h>

Vertex::Vertex()
{
	VCoord[0] = VCoord[1] = VCoord[2] = 0;
	next = prev = NULL;
	static int id = 0;
	vertexID = id++;
}

Vertex::~Vertex()
{}

bool Vertex::SetCoord(double *point)
{
	VCoord[0] = point[0];
	VCoord[1] = point[1];
	VCoord[2] = point[2];
	return true;
}