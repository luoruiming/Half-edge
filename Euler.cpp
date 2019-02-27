#include"Euler.h"
#include<fstream>
#include<stdio.h>
#include<iostream>
#include<algorithm>

//���ɺ���һ������棬���ҹ���һ���µ���
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

//����һ���µĵ㣬���Ӹõ㵽���е㣬����һ���µı�
HalfEdge *mev(double *p1, double *p2, Loop *loop)
{
	Solid *solid = loop->face->solid;
	Edge *edge = new Edge;
	Vertex *vertex = new Vertex;
	HalfEdge *he1 = new HalfEdge, *he2 = new HalfEdge;
	Vertex* v1 = solid->FindVertex(p1);
	if (!v1)
	{
		puts("Point1 ����ʵ���У�");
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

	solid->AddVertex(vertex);//���µ���ӽ�ʵ��
	solid->AddEdge(edge);//���±���ӽ�ʵ��

	return he1;
}

//������f1�ϵ�������v1��v2������һ���±�e��������һ������
Loop *mef(double *p1, double *p2, Loop *loop)
{
	Solid* solid = loop->face->solid;
	Vertex* v1 = solid->FindVertex(p1);
	Vertex* v2 = solid->FindVertex(p2);

	if (!v1)
	{
		puts("mef: Point1 ����ʵ���У�");
		return NULL;
	}
	if (!v2)
	{
		puts("mef: Point2 ����ʵ���У�");
		return NULL;
	}
	if (!(loop->IsInLoop(v1) && loop->IsInLoop(v2)))
	{
		puts("mef: ���㲻��ͬһ���ڣ�");
		return NULL;
	}

	Face* face = new Face;
	Loop* inLoop = new Loop;
	Edge* edge = new Edge;
	HalfEdge *he1 = new HalfEdge, *he2 = new HalfEdge;
	HalfEdge* tHE1 = loop->FindHalfEdge(v1), *tHE2 = loop->FindHalfEdge(v2);

	inLoop->AddHalfEdge(he2);//��he2��Ϊ�»�inLoop�İ�����
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
	loop->halfE = he1;//��he1��Ϊ�ɻ�loop�İ�����

	he1->edge = he2->edge = edge;
	edge->he1 = he1;
	edge->he2 = he2;
	he1->bro = he2;
	he2->bro = he1;
	he1->SetVertex(v1);
	he2->SetVertex(v2);

	solid->AddEdge(edge);//���±���ӽ�ʵ��
	face->AddLoop(inLoop);//���»���ӽ�����
	solid->AddFace(face);//��������ӽ�ʵ��

	return inLoop;//�����½����ڻ�
}

//ɾ��һ����e�����ɸñ�ĳһ�����ϵ����ڻ�
Loop *kemr(double *p1, double *p2, Loop *loop)
{
	Solid *solid = loop->face->solid;
	Vertex *v1 = solid->FindVertex(p1), *v2 = solid->FindVertex(p2);
	if (!v1)
	{
		puts("kemr: p1 ����ʵ���У�");
		return NULL;
	}
	if (!v2)
	{
		puts("kemr: p2 ����ʵ���У�");
		return NULL;
	}
	if (!(loop->IsInLoop(v1) && loop->IsInLoop(v2)))
	{
		puts("kemr: ���㲻��ͬһ�����У�");
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

//ɾ������f1��Ӵ���һ����f2��������f1�ϵ�һ���ڻ������γ����ϵ�һ��ͨ��
void kfmrh(Loop *outloop, Loop *inloop)
{
	Face *tFace = inloop->GetFace();

	outloop->face->AddLoop(inloop);//������С��Ļ���ӵ��������Ļ���
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
	delete tFace;//ɾ�������С�棬�õ�ͨ��
}

Solid *MakeSolid()//���ɴ�2��ͨ�׵�������
{
	double points[][3] = { { 0.0, 0.0, 0.0 }, { 60.0, 0.0, 0.0 }, { 60.0, 60.0, 0.0 }, { 0.0, 60.0, 0.0 },
	{ 0.0, 0.0, 60.0 }, { 60.0, 0.0, 60.0 }, { 60.0, 60.0, 60.0 }, { 0.0, 60.0, 60.0 },
	{ 12.0, 12.0, 0.0 }, { 24.0, 12.0, 0.0 }, { 24.0, 24.0, 0.0 }, { 12.0, 24.0, 0.0 },
	{ 12.0, 12.0, 60.0 }, { 24.0, 12.0, 60.0 }, { 24.0, 24.0, 60.0 }, { 12.0, 24.0, 60.0 },  
	{ 36.0, 12.0, 0.0 },{ 48.0, 12.0, 0.0 },{ 48.0, 24.0, 0.0 },{ 36.0, 24.0, 0.0 },
	{ 36.0, 12.0, 60.0 },{ 48.0, 12.0, 60.0 },{ 48.0, 24.0, 60.0 },{ 36.0, 24.0, 60.0 }};

	//���ɳ�ʼ��
	Solid * pSolid = mvfs(points[0]);
	Loop *initLoop = pSolid->GetFace()->GetLoop();//initLoop�������
	//����һ����
	mev(points[0], points[1], initLoop);
	mev(points[1], points[2], initLoop);
	mev(points[2], points[3], initLoop);
	mef(points[3], points[0], initLoop);
	//he6��ΪinitLoop������ɻ����İ����ڣ�he7��Ϊ�»�������1���İ�����

	//�����������Ϲ���һ��������
	mev(points[0], points[4], initLoop);
	mev(points[1], points[5], initLoop);
	mef(points[4], points[5], initLoop);//he12��ΪinitLoop�İ����ڣ�he13��Ϊ�»�������2���İ�����
	mev(points[2], points[6], initLoop);
	mef(points[5], points[6], initLoop);//he16��ΪinitLoop�İ����ڣ�he17��Ϊ�»�������3���İ�����
	mev(points[3], points[7], initLoop);
	mef(points[6], points[7], initLoop);//he20��ΪinitLoop�İ����ڣ�he21��Ϊ�»�������4���İ�����
	mef(points[7], points[4], initLoop);//he22��ΪinitLoop������0���İ����ڣ�he23��Ϊ�»�������5���İ�����

	/* ���һ��ͨ�� */
	initLoop = pSolid->GetFace()->next->GetLoop();//ȡ����1�Ļ�
	Loop *tLoop = pSolid->GetFace()->GetLoop();//ȡ����0�Ļ�
	//�ڵ���1�Ϲ����ű�
	mev(points[0], points[8], initLoop);
	//ɾ�űߣ�����һ���µģ��գ���
	initLoop = kemr(points[0], points[8], initLoop);
	//�ڿջ��Ϲ���Ҫ�ڿյ���
	mev(points[8], points[9], initLoop);
	mev(points[9], points[10], initLoop);
	mev(points[10], points[11], initLoop);
	//��������6����Ӧ�Ļ�
	Loop *loop_a = mef(points[11], points[8], initLoop);//loop_a�ǵ���6�Ļ���initLoop�ǵ���1��һ���ڻ�
	//�ڿ��ڲ����ᴩ������
	mev(points[8], points[12], loop_a);
	mev(points[9], points[13], loop_a);
	mef(points[13], points[12], loop_a);
	mev(points[10], points[14], loop_a);
	mef(points[14], points[13], loop_a);
	mev(points[11], points[15], loop_a);
	mef(points[15], points[14], loop_a);
	mef(points[12], points[15], loop_a);
	//ɾ��loop_a��������棬����loop_a���붥��tLoop���γ�ͨ��1
	kfmrh(tLoop, loop_a);

	/* ��ڶ���ͨ�� */
	initLoop = pSolid->GetFace()->next->GetLoop();//ȡ����1�Ļ�
	tLoop = pSolid->GetFace()->GetLoop();//ȡ����0�Ļ�
	//�ڵ����Ϲ����ű�										
	mev(points[0], points[16], initLoop);
	//ɾ�űߣ�����һ���µģ��գ���
	initLoop = kemr(points[0], points[16], initLoop);
	//�ڿջ��Ϲ���Ҫ�ڿյ���
	mev(points[16], points[17], initLoop);
	mev(points[17], points[18], initLoop);
	mev(points[18], points[19], initLoop);
	//�������漰��Ӧ�Ļ�
	Loop *loop_b = mef(points[19], points[16], initLoop);
	//�ڿ��ڲ����ᴩ������
	mev(points[16], points[20], loop_b);
	mev(points[17], points[21], loop_b);
	mef(points[21], points[20], loop_b);
	mev(points[18], points[22], loop_b);
	mef(points[22], points[21], loop_b);
	mev(points[19], points[23], loop_b);
	mef(points[23], points[22], loop_b);
	mef(points[20], points[23], loop_b);
	//ɾ��loop_b��������棬����loop_b���붥��tLoop���γ�ͨ��2
	kfmrh(tLoop, loop_b);

	return pSolid;
}