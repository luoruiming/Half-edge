#pragma once

class Vertex;
class Edge;
class Face;

class Solid
{
public:
	Solid *next;
	Solid *prev;
	Face *face;
	Edge *edge;
	Vertex *vertex;

public:
	Solid();
	virtual ~Solid();
	bool AddFace(Face *pFace);
	bool AddEdge(Edge *pEdge);
	bool AddVertex(Vertex *pVertex);
	Face* GetFace(){ return face; };
	Edge* GetEdge(){ return edge; };
	Vertex* GetVertex(){ return vertex; };
	Vertex* FindVertex(double *point);
	Face* GetFaceEnd();
	Edge* FindEdge(Vertex* pVertex1, Vertex* pVertex2);
};


