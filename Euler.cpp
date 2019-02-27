#include"Euler.h"
#include<fstream>
#include<stdio.h>
#include<iostream>
#include<algorithm>

//生成含有一个点的面，并且构成一个新的体
Solid *mvfs(double point[3])
{
	Solid *solid = new Solid;
	Loop *loop = new Loop;
	Face *face = new Face;
	Vertex *vertex = new Vertex;

	vertex->SetCoord(point);
	solid->AddVertex(vertex);
	face->AddLoop(loop);
	solid->AddFace(face);
	
	return solid;
}

//生成一个新的点，连接该点到已有点，构造一条新的边
HalfEdge *mev(double *p1, double *p2, Loop *loop)
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge;
	Vertex *vertex = new Vertex;
	HalfEdge *he1 = new HalfEdge, *he2 = new HalfEdge;
	Vertex* v1 = solid->FindVertex(p1);
	if (!v1)
	{
		puts("Point1 不在实体中！");
		return NULL;
	}

	he1->SetVertex(v1);
	vertex->SetCoord(p2);
	he2->SetVertex(vertex);

	he1->next = he1->bro = he2;
	he2->prev = he2->bro = he1;
	HalfEdge *he = loop->GetHalfEdge();
	if (!he)
	{
		loop->halfE = he1;
		he1->prev = he2;
		he2->next = he1;
	}
	else
	{
		while (he->next->GetVertex() != v1)
		{
			he = he->next;
		}
		he2->next = he->next;
		he->next->prev = he2;
		he->next = he1;
		he1->prev = he;
	}
	he1->loop = he2->loop = loop;

	he1->edge = he2->edge = edge;
	edge->he1 = he1;
	edge->he2 = he2;

	solid->AddVertex(vertex);//将新点添加进实体
	solid->AddEdge(edge);//将新边添加进实体

	return he1;
}

//连接面f1上的两个点v1，v2，生成一条新边e，并产生一个新面
Loop *mef(double *p1, double *p2, Loop *loop)
{
	Solid* solid = loop->face->solid;
	Vertex* v1 = solid->FindVertex(p1);
	Vertex* v2 = solid->FindVertex(p2);

	if (!v1)
	{
		puts("mef: Point1 不在实体中！");
		return NULL;
	}
	if (!v2)
	{
		puts("mef: Point2 不在实体中！");
		return NULL;
	}
	if (!(loop->IsInLoop(v1) && loop->IsInLoop(v2)))
	{
		puts("mef: 两点不在同一环内！");
		return NULL;
	}

	Face* face = new Face;
	Loop* inLoop = new Loop;
	Edge* edge = new Edge;
	HalfEdge *he1 = new HalfEdge, *he2 = new HalfEdge;
	HalfEdge* tHE1 = loop->FindHalfEdge(v1), *tHE2 = loop->FindHalfEdge(v2);

	inLoop->AddHalfEdge(he2);//将he2作为新环inLoop的半边入口
	HalfEdge *ptemp = tHE1;
	while (ptemp)
	{
		if (ptemp == tHE2)break;
		ptemp->loop = inLoop;
		ptemp = ptemp->next;
	}
	he2->next = tHE1;
	ptemp = tHE1->prev;
	tHE1->prev = he2;
	tHE2->prev->next = he2;
	he2->prev = tHE2->prev;

	ptemp->next = he1;
	he1->prev = ptemp;
	he1->next = tHE2;
	tHE2->prev = he1;
	he1->loop = loop;
	loop->halfE = he1;//将he1作为旧环loop的半边入口

	he1->edge = he2->edge = edge;
	edge->he1 = he1;
	edge->he2 = he2;
	he1->bro = he2;
	he2->bro = he1;
	he1->SetVertex(v1);
	he2->SetVertex(v2);

	solid->AddEdge(edge);//将新边添加进实体
	face->AddLoop(inLoop);//将新环添加进新面
	solid->AddFace(face);//将新面添加进实体

	return inLoop;//返回新建的内环
}

//删除一条边e，生成该边某一邻面上的新内环
Loop *kemr(double *p1, double *p2, Loop *loop)
{
	Solid *solid = loop->face->solid;
	Vertex *v1 = solid->FindVertex(p1), *v2 = solid->FindVertex(p2);
	if (!v1)
	{
		puts("kemr: p1 不在实体中！");
		return NULL;
	}
	if (!v2)
	{
		puts("kemr: p2 不在实体中！");
		return NULL;
	}
	if (!(loop->IsInLoop(v1) && loop->IsInLoop(v2)))
	{
		puts("kemr: 两点不在同一个环中！");
		return NULL;
	}
	HalfEdge *he1 = loop->GetHalfEdge();
	while (he1)
	{
		if (he1->GetVertex() == v1&&he1->next->GetVertex() == v2)
		{
			break;
		}
		he1 = he1->next;
	}
	HalfEdge *he2 = he1->bro;
	Loop *newLoop = new Loop;
	if (he1->next != he2)
	{
		newLoop->AddHalfEdge(he1->next);
		HalfEdge* ptemp = he1->next->next;
		while (ptemp)
		{
			if (ptemp == he2)
				break;
			ptemp->loop = newLoop;
			ptemp = ptemp->next;
		}
		he1->next->prev = he2->prev;
		he2->prev->next = he1->next;
	}
	loop->face->AddLoop(newLoop);
	loop->halfE = he1->prev;

	he1->prev->next = he2->next;
	he2->next->prev = he1->prev;
	Edge *pedge = solid->GetEdge();
	while (pedge)
	{
		if (pedge->he1 == he1 || pedge->he2 == he1)
			break;
		pedge = pedge->next;
	}
	if (pedge->next&&pedge->prev)
	{
		pedge->next->prev = pedge->prev;
		pedge->prev->next = pedge->next;
	}
	else if (pedge->next == NULL&&pedge->prev != NULL)
	{
		pedge->prev->next = NULL;
	}
	else if (pedge->prev == NULL&&pedge->next != NULL)
	{
		pedge->next->prev = NULL;
		solid->edge = pedge->next;
	}
	else
	{
		solid->edge = NULL;
	}
	delete pedge;
	delete he1;
	delete he2;

	return newLoop;
}

//删除与面f1相接触的一个面f2，生成面f1上的一个内环，并形成体上的一个通孔
void kfmrh(Loop *outloop, Loop *inloop)
{
	Face *tFace = inloop->GetFace();

	outloop->face->AddLoop(inloop);//将里面小面的环添加到外面大面的环中
	if (tFace->next && tFace->prev)
	{
		tFace->next->prev = tFace->prev;
		tFace->prev->next = tFace->next;
	}
	else if (tFace->next == NULL && tFace->prev != NULL)
	{
		tFace->prev->next = NULL;
	}
	else if (tFace->prev == NULL && tFace->next != NULL)
	{
		tFace->next->prev = NULL;
		outloop->face->solid->face = tFace->next;
	}
	else
	{
		outloop->face->solid->face = NULL;
	}
	delete tFace;//删掉里面的小面，得到通孔
}

Solid *MakeSolid()//生成带2个通孔的立方体
{
	double points[][3] = { { 0.0, 0.0, 0.0 }, { 60.0, 0.0, 0.0 }, { 60.0, 60.0, 0.0 }, { 0.0, 60.0, 0.0 },
	{ 0.0, 0.0, 60.0 }, { 60.0, 0.0, 60.0 }, { 60.0, 60.0, 60.0 }, { 0.0, 60.0, 60.0 },
	{ 12.0, 12.0, 0.0 }, { 24.0, 12.0, 0.0 }, { 24.0, 24.0, 0.0 }, { 12.0, 24.0, 0.0 },
	{ 12.0, 12.0, 60.0 }, { 24.0, 12.0, 60.0 }, { 24.0, 24.0, 60.0 }, { 12.0, 24.0, 60.0 },  
	{ 36.0, 12.0, 0.0 },{ 48.0, 12.0, 0.0 },{ 48.0, 24.0, 0.0 },{ 36.0, 24.0, 0.0 },
	{ 36.0, 12.0, 60.0 },{ 48.0, 12.0, 60.0 },{ 48.0, 24.0, 60.0 },{ 36.0, 24.0, 60.0 }};

	//生成初始点
	Solid * pSolid = mvfs(points[0]);
	Loop *initLoop = pSolid->GetFace()->GetLoop();//initLoop不含半边
	//构造一个面
	mev(points[0], points[1], initLoop);
	mev(points[1], points[2], initLoop);
	mev(points[2], points[3], initLoop);
	mef(points[3], points[0], initLoop);
	//he6作为initLoop（旧面旧环）的半边入口，he7作为新环（新面1）的半边入口

	//在这个面基础上构造一个立方体
	mev(points[0], points[4], initLoop);
	mev(points[1], points[5], initLoop);
	mef(points[4], points[5], initLoop);//he12作为initLoop的半边入口，he13作为新环（新面2）的半边入口
	mev(points[2], points[6], initLoop);
	mef(points[5], points[6], initLoop);//he16作为initLoop的半边入口，he17作为新环（新面3）的半边入口
	mev(points[3], points[7], initLoop);
	mef(points[6], points[7], initLoop);//he20作为initLoop的半边入口，he21作为新环（新面4）的半边入口
	mef(points[7], points[4], initLoop);//he22作为initLoop（顶面0）的半边入口，he23作为新环（新面5）的半边入口

	/* 造第一个通孔 */
	initLoop = pSolid->GetFace()->next->GetLoop();//取底面1的环
	Loop *tLoop = pSolid->GetFace()->GetLoop();//取顶面0的环
	//在底面1上构造桥边
	mev(points[0], points[8], initLoop);
	//删桥边，生成一个新的（空）环
	initLoop = kemr(points[0], points[8], initLoop);
	//在空环上构造要挖空的面
	mev(points[8], points[9], initLoop);
	mev(points[9], points[10], initLoop);
	mev(points[10], points[11], initLoop);
	//产生新面6及相应的环
	Loop *loop_a = mef(points[11], points[8], initLoop);//loop_a是底面6的环，initLoop是底面1的一个内环
	//挖空内部，贯穿立方体
	mev(points[8], points[12], loop_a);
	mev(points[9], points[13], loop_a);
	mef(points[13], points[12], loop_a);
	mev(points[10], points[14], loop_a);
	mef(points[14], points[13], loop_a);
	mev(points[11], points[15], loop_a);
	mef(points[15], points[14], loop_a);
	mef(points[12], points[15], loop_a);
	//删掉loop_a所代表的面，并将loop_a并入顶面tLoop，形成通孔1
	kfmrh(tLoop, loop_a);

	/* 造第二个通孔 */
	initLoop = pSolid->GetFace()->next->GetLoop();//取底面1的环
	tLoop = pSolid->GetFace()->GetLoop();//取顶面0的环
	//在底面上构造桥边										
	mev(points[0], points[16], initLoop);
	//删桥边，生成一个新的（空）环
	initLoop = kemr(points[0], points[16], initLoop);
	//在空环上构造要挖空的面
	mev(points[16], points[17], initLoop);
	mev(points[17], points[18], initLoop);
	mev(points[18], points[19], initLoop);
	//产生新面及相应的环
	Loop *loop_b = mef(points[19], points[16], initLoop);
	//挖空内部，贯穿立方体
	mev(points[16], points[20], loop_b);
	mev(points[17], points[21], loop_b);
	mef(points[21], points[20], loop_b);
	mev(points[18], points[22], loop_b);
	mef(points[22], points[21], loop_b);
	mev(points[19], points[23], loop_b);
	mef(points[23], points[22], loop_b);
	mef(points[20], points[23], loop_b);
	//删掉loop_b所代表的面，并将loop_b并入顶面tLoop，形成通孔2
	kfmrh(tLoop, loop_b);

	return pSolid;
}