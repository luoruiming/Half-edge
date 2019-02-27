#include "Edge.h"
#include<stdio.h>

Edge::Edge()
{
	static int id = 0;
	edgeID = id++;
	next = prev = NULL;
	he1 = he2 = NULL;
}

Edge::~Edge()
{
}
