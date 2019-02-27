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
	bool AddHalfEdge(HalfEdge *he);//�������ӵ���
	HalfEdge* GetHalfEdge(){ return halfE; }; 
	bool IsInLoop(Vertex* vertex);//�ж�������Ƿ��ڻ���
	HalfEdge* FindHalfEdge(Vertex* vertex);//ȡһ�����ڵİ��
};


