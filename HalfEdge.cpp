#include "HalfEdge.h"
#include<stdio.h>

HalfEdge::HalfEdge()
{
	static int id = 0;
	halfedgeID = id++;
	next = prev = bro = NULL;
	loop = NULL;
	edge = NULL;
	sv = NULL;
}

HalfEdge::~HalfEdge()
{}

bool HalfEdge::SetVertex(Vertex *vertex)
{
	if (!vertex)
		return false;
	sv = vertex;
	return true;
}