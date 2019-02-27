#pragma once

class Solid;
class Loop;

class Face
{

public:
	int faceID;
	Face *next;
	Face *prev;
	Solid *solid;

public:
	Loop *loop;
	Face();
	virtual ~Face();
	Loop *GetLoop(){ return loop; };
	Solid *GetSolid(){ return solid; }
	bool AddLoop(Loop *loop);

};


