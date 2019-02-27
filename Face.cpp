#include "Face.h"
#include"Loop.h"
#include<stdio.h>

Face::Face()
{
	static int id = 0;
	faceID = id++;
	next = prev = NULL;
	solid = NULL;
	loop = NULL;
}

Face::~Face()
{}

bool Face::AddLoop(Loop *pLoop)
{
	if (!pLoop)
	{
		return false;
	}

	Loop *ptemp = loop;

	if (!ptemp)
		loop = pLoop;
	else
	{
		while (ptemp->next)
		{
			ptemp = ptemp->next;
		}
		ptemp->next = pLoop;
		pLoop->prev = ptemp;
	}
	pLoop->face = this;

	return true;
}