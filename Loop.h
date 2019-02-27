#pragma once

class Face;
class HalfEdge;
class Vertex;

class Loop
{
public:
	int loopID;
	Loop* next;
	Loop* prev;
	Face* face;
	HalfEdge* halfE;
public:
	Loop();
	virtual ~Loop();

	Face* GetFace(){ return face; };
	bool AddHalfEdge(HalfEdge *he);//将半边添加到环
	HalfEdge* GetHalfEdge(){ return halfE; }; 
	bool IsInLoop(Vertex* vertex);//判断这个点是否在环里
	HalfEdge* FindHalfEdge(Vertex* vertex);//取一点所在的半边
};


