#pragma once

class Vertex;
class Loop;
class Edge;

class HalfEdge
{
public:
	int halfedgeID;
	HalfEdge *next;
	HalfEdge *prev;
	HalfEdge *bro;
	Loop *loop;//依附的环
	Edge *edge;//依附的边

public:
	Vertex *sv;//首点
	HalfEdge();
	virtual ~HalfEdge();
	bool SetVertex(Vertex *vertex);
	Vertex* GetVertex(){ return sv; };
	Loop* GetLoop(){ return loop; };
	Edge* GetEdge(){ return edge; };

};


