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
	HalfEdge *he1;//����
	HalfEdge *he2;//�Ұ��
};


