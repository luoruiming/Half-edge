#pragma once

class HalfEdge;

class Edge
{
public:
	Edge();
	virtual ~Edge();

public:
	int edgeID;
	Edge *next;
	Edge *prev;
	HalfEdge *he1;//×ó°ë±ß
	HalfEdge *he2;//ÓÒ°ë±ß
};


