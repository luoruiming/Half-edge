#pragma once

#include "Solid.h"
#include "Face.h"
#include "Vertex.h"
#include "Loop.h"
#include "HalfEdge.h"
#include "Edge.h"

Solid *mvfs(double point[3]);//���ɺ���һ������棬���ҹ���һ���µ���
HalfEdge *mev(double *point1, double *point2, Loop *loop);//����һ���µĵ㣬���Ӹõ㵽���е㣬����һ���µı�		  
Loop *mef(double *point1, double *point2, Loop *loop);//������f1�ϵ�������v1��v2������һ���±�e��������һ������									
Loop *kemr(double *point1, double *point2, Loop *loop); //ɾ��һ����e�����ɸñ�ĳһ�����ϵ��µ��ڻ�
void kfmrh(Loop *outloop, Loop *inloop);//ɾ������f1��Ӵ���һ����f2��������f1�ϵ�һ���ڻ������γ����ϵ�һ��ͨ��

Solid *MakeSolid();//���ɴ�2��ͨ�׵�������
