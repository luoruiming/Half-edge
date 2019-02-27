#pragma once

class Vertex
{
public:
	Vertex();
	virtual ~Vertex();

	Vertex *next;
	Vertex *prev;
	int vertexID;
	double VCoord[3];
	bool SetCoord(double *point);
};


