#include "Solid.h"
#include "Face.h"
#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"
#include <stdio.h>

Solid::Solid()
{
	next = prev = NULL;
	face = NULL;
	edge = NULL;
	vertex = NULL;
}

Solid::~Solid()
{}

bool Solid::AddFace(Face *pFace)
{
	if (!pFace)
	{
		return false;
	}
	Face *ptemp = face;
	if (!ptemp)
	{
		face = pFace;
	}
	else
	{
		while (ptemp->next)
		{
			ptemp = ptemp->next;
		}
		ptemp->next = pFace;
		pFace->prev = ptemp;
	}
	pFace->solid = this;
	return true;
}

bool Solid::AddEdge(Edge *pEdge)
{
	if (!pEdge)
	{
		return false;
	}
	Edge *ptemp = edge;
	if (!ptemp)
	{
		edge = pEdge;
	}
	else
	{
		while (ptemp->next)
		{
			ptemp = ptemp->next;
		}
		ptemp->next = pEdge;
		pEdge->prev = ptemp;
	}
	return true;
}

bool Solid::AddVertex(Vertex *pVertex)
{
	if (!pVertex)
	{
		return false;
	}
	Vertex *ptemp = vertex;
	if (!ptemp)
	{
		vertex = pVertex;
	}
	else
	{
		while (ptemp->next)
		{
			ptemp = ptemp->next;
		}
		ptemp->next = pVertex;
		pVertex->prev = ptemp;
	}
	return true;
}

Vertex* Solid::FindVertex(double *point)
{
	Vertex *ptemp = vertex;
	for (; ptemp; ptemp = ptemp->next)
	{
		if (ptemp->VCoord[0] == point[0] && ptemp->VCoord[1] == point[1] && ptemp->VCoord[2] == point[2])
		{
			return ptemp;
		}
	}
	return NULL;
}

Face * Solid::GetFaceEnd()
{
	Face *tempF = face;
	while (tempF->next)
	{
		tempF = tempF->next;
	}
	return tempF;
}

Edge* Solid::FindEdge(Vertex* v1,Vertex* v2)
{
	Edge* tempE = edge;
	while (tempE)
	{
		if ((tempE->he1->GetVertex() == v1 && tempE->he2->GetVertex() == v2) || (tempE->he1->GetVertex() == v2 && tempE->he2->GetVertex() == v1))
		{
			return tempE;
		}
		tempE = tempE->next;
	}
	return tempE;
}