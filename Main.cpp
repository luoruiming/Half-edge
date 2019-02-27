#include "Euler.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <gl\glut.h>
#include <windows.h>
#include <time.h>

Solid *m_Solid;

//����һ�����񻯱���
GLUtesselator* tess = gluNewTess();

//��x��y��z����ת
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

#define NUMFACE 30

int color_faces[NUMFACE][3];//�洢��������ĸ�����ɫ
int idx = 0;

void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}
void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte *estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK endCallback(void)
{
	glEnd();
}
void CALLBACK vertexCallback(GLvoid *vertex)
{
	const GLdouble *pointer;
	pointer = (GLdouble *)vertex;
	glVertex3dv(pointer);
}

void reshape(int width, int height)
{
	if (height == 0) height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat nRange = 150.0f;

	if (width <= height)
		glOrtho(-nRange, nRange, -nRange*height / width, nRange*height / width, -nRange, nRange);
	else
		glOrtho(-nRange*width / height, nRange*width / height, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//�����ͼ����
void printSolid(Solid *solid)
{
	Face *face = solid->GetFace();
	int index = 0;
	while (face)
	{//���λ��Ƹ���
		gluTessBeginPolygon(tess, NULL);//��ʼ���ƶ����

		Loop *temploop = face->GetLoop();
		//���������������ɫ��������
		unsigned char red = color_faces[index][0];
		unsigned char green = color_faces[index][1];
		unsigned char blue = color_faces[index][2];
		while (temploop)
		{    //������ǰ��ĸ���
			glColor3b(red, green, blue);
			HalfEdge *halfedge = temploop->GetHalfEdge();
			Vertex *start = halfedge->GetVertex();
			gluTessBeginContour(tess);//��ʼ���ƶ����
			gluTessVertex(tess, start->VCoord, start->VCoord);
			Vertex *tmp = halfedge->next->GetVertex();
			halfedge = halfedge->next;
			while (tmp != start)
			{
				gluTessVertex(tess, tmp->VCoord, tmp->VCoord);
				halfedge = halfedge->next;
				tmp = halfedge->GetVertex();
			}
			gluTessEndContour(tess);//��������������
			temploop = temploop->next;
		}
		gluTessEndPolygon(tess);//�������ƶ����
		face = face->next;
		index++;
	}
}

void RenderScene(void)//OpenGL���溯��
{
	//�����ɫ����Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//��תͼ��
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	//ָ����ʱ���Ʒ��Ķ����Ϊ���������
	glFrontFace(GL_CCW);
	
	printSolid(m_Solid);//����άʵ�廭����
	
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void KeyBoard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN) xRot += 5.0f;
	if (key == GLUT_KEY_LEFT) yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT) yRot += 5.0f;
	if (key == GLUT_KEY_PAGE_UP) zRot -= 5.0f;
	if (key == GLUT_KEY_PAGE_DOWN) zRot += 5.0f;

	if (xRot > 356.0f) xRot = 0.0f;
	if (xRot < -1.0f) xRot = 355.0f;
	if (yRot > 356.0f) yRot = 0.0f;
	if (yRot < -1.0f) yRot = 355.0f;
	if (zRot > 356.0f) zRot = 0.0f;
	if (zRot < -1.0f) zRot = 355.0f;

	//ˢ�´���
	glutPostRedisplay();
}

bool InitGL()//OpenGL������ʼ������
{
	glClearColor(0.0, 0.0, 0.0, 0.0);//������ɫ
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);//������Ȳ���

	gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);

	return true;
}

int main(int argc, char* argv[])
{
	m_Solid = MakeSolid();//����ŷ����������ʵ��

	srand(time(NULL));
	//��ÿ�������һ�������ɫ
	for (int i = 0; i < NUMFACE; i++)
	{
		unsigned char red = rand() % 255;
		unsigned char green = rand() % 255;
		unsigned char blue = rand() % 255;
		color_faces[idx][0] = red;
		color_faces[idx][1] = green;
		color_faces[idx][2] = blue;
		idx++;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//��˫���棬��ɫ���棬��Ȼ���
	glutInitWindowPosition(100, 100);//����λ��
	glutInitWindowSize(1000, 800);//���ڴ�С
	glutCreateWindow("A cube with two handles");//���ڱ���
	InitGL();//������ʼ��
	glutReshapeFunc(reshape);//�����ڸı�
	glutSpecialFunc(KeyBoard);//��������¼�
	glutDisplayFunc(RenderScene);//�����ͼ
		
	glutMainLoop();
	return 0;
}