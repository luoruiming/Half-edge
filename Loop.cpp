#include "Vertex.h"
#include "Loop.h"
#include"HalfEdge.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using std::cout;
using std::endl;

Loop::Loop()
{
	static int id = 0;
	loopID = id++;
	next = prev = NULL;
	face = NULL;
	halfE = NULL;
}

Loop::~Loop()
{}

bool Loop::AddHalfEdge(HalfEdge *he1)
{
	if (!he1)
		return false;
	HalfEdge *ptemp = halfE;
	if (!ptemp)
	{
		halfE = he1;
	}
	else
	{
		while (ptemp->next)
			ptemp = ptemp->next;
		ptemp->next = he1;
		he1->prev = ptemp;
	}
	he1->loop = this;
	return true;
}
bool Loop::IsInLoop(Vertex *vertex)
{
	if (!vertex)
		return false;
	HalfEdge *phe = halfE;
	while (phe)
	{
		if (phe->GetVertex() == vertex)
			return true;
		phe = phe->next;
		if (phe == halfE)
			return false;
	}
	return false;
}
HalfEdge *Loop::FindHalfEdge(Vertex *vertex)
{
	if (!vertex)
		return NULL;
	HalfEdge *ptemp = halfE;
	while (ptemp)
	{
		if (ptemp->GetVertex() == vertex)
			return ptemp;
		ptemp = ptemp->next;
		if (ptemp == halfE)
			return NULL;
	}
	return NULL;
}