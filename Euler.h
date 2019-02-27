#pragma once

#include "Solid.h"
#include "Face.h"
#include "Vertex.h"
#include "Loop.h"
#include "HalfEdge.h"
#include "Edge.h"

Solid *mvfs(double point[3]);//生成含有一个点的面，并且构成一个新的体
HalfEdge *mev(double *point1, double *point2, Loop *loop);//生成一个新的点，连接该点到已有点，构造一条新的边		  
Loop *mef(double *point1, double *point2, Loop *loop);//连接面f1上的两个点v1，v2，生成一条新边e，并产生一个新面									
Loop *kemr(double *point1, double *point2, Loop *loop); //删除一条边e，生成该边某一邻面上的新的内环
void kfmrh(Loop *outloop, Loop *inloop);//删除与面f1相接触的一个面f2，生成面f1上的一个内环，并形成体上的一个通孔

Solid *MakeSolid();//生成带2个通孔的立方体
