#include "Euler.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <gl\glut.h>
#include <windows.h>
#include <time.h>

Solid *m_Solid;

//产生一个网格化变量
GLUtesselator* tess = gluNewTess();

//绕x，y，z轴旋转
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

#define NUMFACE 30

int color_faces[NUMFACE][3];//存储随机产生的各面颜色
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

//网格绘图函数
void printSolid(Solid *solid)
{
	Face *face = solid->GetFace();
	int index = 0;
	while (face)
	{//依次绘制各面
		gluTessBeginPolygon(tess, NULL);//开始绘制多边形

		Loop *temploop = face->GetLoop();
		//采用随机产生的颜色来绘制面
		unsigned char red = color_faces[index][0];
		unsigned char green = color_faces[index][1];
		unsigned char blue = color_faces[index][2];
		while (temploop)
		{    //遍历当前面的各点
			glColor3b(red, green, blue);
			HalfEdge *halfedge = temploop->GetHalfEdge();
			Vertex *start = halfedge->GetVertex();
			gluTessBeginContour(tess);//开始绘制多边形
			gluTessVertex(tess, start->VCoord, start->VCoord);
			Vertex *tmp = halfedge->next->GetVertex();
			halfedge = halfedge->next;
			while (tmp != start)
			{
				gluTessVertex(tess, tmp->VCoord, tmp->VCoord);
				halfedge = halfedge->next;
				tmp = halfedge->GetVertex();
			}
			gluTessEndContour(tess);//结束绘制轮廓线
			temploop = temploop->next;
		}
		gluTessEndPolygon(tess);//结束绘制多边形
		face = face->next;
		index++;
	}
}

void RenderScene(void)//OpenGL主绘函数
{
	//清除颜色及深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	//旋转图形
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	//指定逆时针绕法的多边形为多边形正面
	glFrontFace(GL_CCW);
	
	printSolid(m_Solid);//将三维实体画出来
	
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

	//刷新窗口
	glutPostRedisplay();
}

bool InitGL()//OpenGL环境初始化函数
{
	glClearColor(0.0, 0.0, 0.0, 0.0);//背景黑色
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);//开启深度测试

	gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);

	return true;
}

int main(int argc, char* argv[])
{
	m_Solid = MakeSolid();//基于欧拉操作生成实体

	srand(time(NULL));
	//对每个面产生一个随机颜色
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//打开双缓存，颜色缓存，深度缓存
	glutInitWindowPosition(100, 100);//窗口位置
	glutInitWindowSize(1000, 800);//窗口大小
	glutCreateWindow("A cube with two handles");//窗口标题
	InitGL();//环境初始化
	glutReshapeFunc(reshape);//处理窗口改变
	glutSpecialFunc(KeyBoard);//处理键盘事件
	glutDisplayFunc(RenderScene);//处理绘图
		
	glutMainLoop();
	return 0;
}