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
	Loop *loop;//�����Ļ�
	Edge *edge;//�����ı�

public:
	Vertex *sv;//�׵�
	HalfEdge();
	virtual ~HalfEdge();
	bool SetVertex(Vertex *vertex);
	Vertex* GetVertex(){ return sv; };
	Loop* GetLoop(){ return loop; };
	Edge* GetEdge(){ return edge; };

};


