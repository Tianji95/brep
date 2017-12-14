#include <glut.h>

#ifndef _WIN32
#define __stdcall
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include "brepStruct\BaseClass.h"
#include "brepStruct\BrepOperation.h"
using std::stringstream;
using std::cout;
using std::cerr;
using std::endl;
using std::ends;

#ifndef CALLBACK
#define CALLBACK
#endif

void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
BrepOperation* makeBrepConstruct();
void readBrepConstruct(BrepOperation* b);
void drawFace(Face* f);

void initGL();
int  initGLUT(int argc, char **argv);
void initLights();

float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode = 0;
list<GLuint> faceIdList;
double radio = 0.5;



BrepOperation* makeBrepConstruct() {
	BrepOperation *b = new BrepOperation();
	Solid *s = new Solid();
	Loop *lp1 = new Loop();
	Loop *lp_bottom = new Loop();
	Loop *lp_back = new Loop();
	Loop *lp_left = new Loop();
	Loop *lp_front = new Loop();
	Loop *lp_right = new Loop();
	Loop *lp_inner = new Loop();
	Loop *lp_inner_bottom = new Loop();
	Loop *lp_inner_back = new Loop();
	Loop *lp_inner_left = new Loop();
	Loop *lp_inner_right = new Loop();
	Loop *lp_inner_front = new Loop();
	Loop *lp_inner_top = new Loop();

	Vertex *v_start = new Vertex();
	Vertex *v1 = new Vertex();
	Vertex *v2 = new Vertex();
	Vertex *v3 = new Vertex();
	Vertex *v4 = new Vertex();
	Vertex *v5 = new Vertex();
	Vertex *v6 = new Vertex();
	Vertex *v7 = new Vertex();
	Vertex *v8 = new Vertex();
	Vertex *v9 = new Vertex();
	Vertex *v10 = new Vertex();
	Vertex *v11 = new Vertex();
	Vertex *v12 = new Vertex();
	Vertex *v13 = new Vertex();
	Vertex *v14 = new Vertex();
	Vertex *v15 = new Vertex();
	Vertex *v16 = new Vertex();
	Vertex *v_prev = new Vertex();

	//生成底面，从上看
	//  v1-------v4
	//  |        |
	//  |        |
	//  |        |
	//  v2-------v3
	//list<int> int_list;
	//int_list.push_back(1);
	//int_list.push_back(2);
	//int_list.push_back(3);

	//list<int>::iterator i_iter;
	//for (i_iter = int_list.begin(); (*i_iter)!=2; i_iter++);
	//int_list.insert(i_iter, 4);
	//int_list.insert(i_iter, 5);
	//for (i_iter = int_list.begin(); i_iter != int_list.end(); i_iter++) {
	//	if (*(i_iter) == 2) {
	//		int_list.erase(i_iter++);
	//	}
	//	cout << *i_iter << endl;
	//}

	s = b->mvfs(-1.0f, -1.0f, -1.0f, v1, lp1);
	v2 = b->mev(-1.0f, -1.0f, 1.0f, v1, lp1);
	v3 = b->mev(1.0f, -1.0f, 1.0f, v2, lp1);
	v4 = b->mev(1.0f, -1.0f, -1.0f, v3, lp1);
	lp_bottom = b->mef(v4, v1, lp1);

	list<HalfEdge*>::iterator heiter1;
	for (heiter1 = lp1->halfEdges->begin(); heiter1 != lp1->halfEdges->end(); heiter1++) {
		cout << (*heiter1)->startVertex->getVertexXPosition() << " " << (*heiter1)->startVertex->getVertexYPosition() << " " << (*heiter1)->startVertex->getVertexZPosition() << endl;
	}
	for (heiter1 = lp_bottom->halfEdges->begin(); heiter1 != lp_bottom->halfEdges->end(); heiter1++) {
		cout << (*heiter1)->startVertex->getVertexXPosition() << " " << (*heiter1)->startVertex->getVertexYPosition() << " " << (*heiter1)->startVertex->getVertexZPosition() << endl;
	}

	//生成后面，从前看
	//  v5-------v6
	//  |        |
	//  |        |
	//  |        |
	//  v1-------v4
	v5 = b->mev(-1.0f, 1.0f, -1.0f, v1, lp1);
	v6 = b->mev(1.0f, 1.0f, -1.0f, v4, lp1);
	lp_back = b->mef(v6, v5, lp1);

	////生成左面，从左看
	////  v5-------v7
	////  |        |
	////  |        |
	////  |        |
	////  v1-------v2
	v7 = b->mev(-1.0f, 1.0f, 1.0f, v5, lp1);
	lp_left = b->mef(v7, v2, lp1);

	////生成前面，从前看
	////  v7-------v8
	////  |        |
	////  |        |
	////  |        |
	////  v2-------v3
	v8 = b->mev(1.0f, 1.0f, 1.0f, v7, lp1);
	lp_front = b->mef(v8, v3, lp1);

	////生成右面，同时生成上面，从右看，上面的环其实就是lp1
	////  v8-------v6
	////  |        |
	////  |        |
	////  |        |
	////  v3-------v4
	lp_right = b->mef(v8, v6, lp1);

	////生成底面的内环，从V1开始添加桥边
	////  v1-------v4
	////  | |      |  
	////  | v9--v12|
	////  | |    | |
	////  | v10-v11|
	////  |        |
	////  v2-------v3
	v9 = b->mev(-0.5f, -1.0f, -0.5f, v1, lp_bottom);
	v10= b->mev(-0.5f, -1.0f, 0.5f, v9, lp_bottom);
	v11 = b->mev(0.5f, -1.0f, 0.5f, v10, lp_bottom);
	v12 = b->mev(0.5f, -1.0f, -0.5f, v11, lp_bottom);
	//list<HalfEdge*>::iterator heiter;
	//for (heiter = lp_bottom->halfEdges->begin(); heiter != lp_bottom->halfEdges->end(); heiter++) {
	//	cout << (*heiter)->startVertex->getVertexXPosition() << " " << (*heiter)->startVertex->getVertexYPosition() << " " << (*heiter)->startVertex->getVertexZPosition() << endl;
	//}
	lp_inner = b->mef(v12, v9, lp_bottom);
	lp_inner_bottom = b->kemr(lp_bottom, v1, v9);
	////生成内部的后面，从前看
	////  v13-----v16
	////  |        |
	////  |        |
	////  |        |
	////  v9------v12
	v13 = b->mev(-0.5f, 1.0f, -0.5f, v9, lp_inner);
	v16 = b->mev(0.5f, 1.0f, -0.5f, v12, lp_inner);
	lp_inner_back = b->mef(v13, v16, lp_inner);


	//////生成内部的左面，从左看
	//////  v13------v14
	//////  |        |
	//////  |        |
	//////  |        |
	//////  |        |
	//////  v9-------v10
	v14 = b->mev(-0.5f, 1.0f, 0.5f, v13, lp_inner);
	lp_inner_left = b->mef(v10, v14, lp_inner);

	//////生成内部的前面，从前看
	//////  v14-----v15
	//////  |        |
	//////  |        |
	//////  |        |
	//////  v10-----v11
	v15 = b->mev(0.5f, 1.0f, 0.5f, v14, lp_inner);
	lp_inner_front = b->mef(v11, v15, lp_inner);

	//////生成内部的右面，同时生成上面，从右看，上面的环其实就是lp_inner_bottom
	//////  v15-----v16
	//////  |        |
	//////  |        |
	//////  |        |
	//////  v11-----v12
	lp_inner_right = b->mef(v16, v15, lp_inner);
	lp_inner_top = lp_inner;

	cout << "position" << endl;
	list<HalfEdge*>::iterator heiter;
	for (heiter = lp_inner_top->halfEdges->begin(); heiter != lp_inner_top->halfEdges->end(); heiter++) {
		cout << (*heiter)->startVertex->getVertexXPosition() << " " << (*heiter)->startVertex->getVertexYPosition() << " " << (*heiter)->startVertex->getVertexZPosition() << endl;
	}

	for (heiter = lp1->halfEdges->begin(); heiter != lp1->halfEdges->end(); heiter++) {
		cout << (*heiter)->startVertex->getVertexXPosition() << " " << (*heiter)->startVertex->getVertexYPosition() << " " << (*heiter)->startVertex->getVertexZPosition() << endl;
	}
	cout << lp_bottom->face->loops->size();
	b->kfmrh(lp_inner_top, lp1);



	Solid *s2 = new Solid();
	Loop *lp21 = new Loop();
	Loop *lp2_front = new Loop();
	Loop *lp2_inner = new Loop();
	Loop *lp2_inner2 = new Loop();

	Vertex *v2_start = new Vertex();
	Vertex *v21 = new Vertex();
	Vertex *v22 = new Vertex();
	Vertex *v23 = new Vertex();
	Vertex *v24 = new Vertex();
	Vertex *v25 = new Vertex();
	Vertex *v26 = new Vertex();
	Vertex *v27 = new Vertex();
	Vertex *v28 = new Vertex();

	Vertex *v35 = new Vertex();
	Vertex *v36 = new Vertex();
	Vertex *v37 = new Vertex();
	Vertex *v38 = new Vertex();

	list<Loop*> *inner_list = new list<Loop*>;
	//底面
	s2 = b->mvfs(1.5f,  -1.0f, -1.0f, v21, lp21);
	v22 = b->mev(1.5f, -1.0f, 1.0f, v21, lp21);
	v23 = b->mev(3.5f, -1.0f, 1.0f, v22, lp21);
	v24 = b->mev(3.5f, -1.0f, -1.0f, v23, lp21);
	lp2_front = b->mef(v24, v21, lp21);


	////生成内部的底环
	v25 = b->mev(1.8f, -1.0f, -0.8f, v21, lp2_front);
	v26 = b->mev(1.8f, -1.0f, -0.3f, v25, lp2_front);
	v27 = b->mev(2.3f, -1.0f, -0.3f, v26, lp2_front);
	v28 = b->mev(2.3f, -1.0f, -0.8f, v27, lp2_front);


	lp2_inner = b->mef(v28, v25, lp2_front);//生成里面的环
	inner_list->push_back(lp2_inner);
    b->kemr(lp2_front, v21, v25);


	v35 = b->mev(2.8f, -1.0f, 0.3f, v24, lp2_front);
	v36 = b->mev(2.8f, -1.0f, 0.8f, v35, lp2_front);
	v37 = b->mev(3.2f, -1.0f, 0.8f, v36, lp2_front);
	v38 = b->mev(3.2f, -1.0f, 0.3f, v37, lp2_front);

	lp2_inner2 = b->mef(v38, v35, lp2_front);//生成里面的环
	b->kemr(lp2_front, v24, v35);
	inner_list->push_back(lp2_inner2);

	b->sweep(0, 2, 0, lp2_front->face, inner_list, lp21);



	Solid *s4 = new Solid();
	Loop *lp41 = new Loop();
	Loop *lp4_right = new Loop();
	Vertex *v41 = new Vertex();
	Vertex *v42 = new Vertex();
	Vertex *v43 = new Vertex();
	Vertex *v44 = new Vertex();

	list<Loop*> *inner_list4 = new list<Loop*>;
	//右面
	s4 = b->mvfs(-1.0f, -1.0f, -1.0f, v41, lp41);
	v42 = b->mev(-1.0f, -1.0f, 1.0f, v41, lp41);
	v43 = b->mev(-1.0f, 1.0f, 1.0f, v42, lp41);
	v44 = b->mev(-1.0f, 1.0f, -1.0f, v43, lp41);
	lp4_right = b->mef(v44, v41, lp41);
	b->sweep(-1, 0, 0, lp4_right->face, inner_list4, lp41);
	//合并两个体
	b->kfmrh(lp4_right, lp_left);

	return b;
}
GLfloat theColor[5][3] = { { 0,0,1 },{ 0,1,0 },{ 1,0,0 }, {0,1,1} ,{1,1,0} };
int colorCount = 0;
void readBrepConstruct(BrepOperation* b) {
	list<Solid*>::iterator s_iter;
	list<Face*>::iterator f_iter;
	for (s_iter = b->solidList->begin(); s_iter != b->solidList->end(); s_iter++) {
		for (f_iter = (*s_iter)->faces->begin(); f_iter != (*s_iter)->faces->end(); f_iter++){
			drawFace(*f_iter);
			colorCount++;
		}
	}

}

void drawFace(Face* f) {
	GLuint id = glGenLists(1);
	list<Loop*>::iterator lp_iter;
	list<HalfEdge*>::iterator he_iter;

	GLUtesselator *tess = gluNewTess(); 
	gluTessCallback(tess, GLU_TESS_BEGIN, (void(__stdcall*)(void))glBegin);
	gluTessCallback(tess, GLU_TESS_END, (void(__stdcall*)(void))glEnd);
	gluTessCallback(tess, GLU_TESS_VERTEX, (void(__stdcall*)())glVertex3dv);

	//, { 2,0,0 }, { -2,3,0 }, { -2,0,0 }
	GLdouble quad2[8][3] = { { -2,3,0 },{ -2,0,0 },{ 2,0,0 },{ 2,3,0 },
	{ -1,2,0 },{ -1,1,0 },{ 1,1,0 },{ 1,2,0 } };
	GLdouble point[100][3];
	int point_idx = 0;
	glNewList(id, GL_COMPILE);
	glColor3f(theColor[colorCount % 5][0], theColor[colorCount % 5][1], theColor[colorCount % 5][2]);
	//gluTessBeginPolygon(tess, 0);  
	//gluTessBeginContour(tess);
	//gluTessVertex(tess, quad2[4], quad2[4]);
	//gluTessVertex(tess, quad2[5], quad2[5]);
	//gluTessVertex(tess, quad2[6], quad2[6]);
	//gluTessVertex(tess, quad2[7], quad2[7]);// with NULL data                    // outer quad
	//gluTessEndContour(tess);
	//gluTessBeginContour(tess);                      // inner quad (hole)
	//gluTessVertex(tess, quad2[0], quad2[0]);
	//gluTessVertex(tess, quad2[1], quad2[1]);
	//gluTessVertex(tess, quad2[2], quad2[2]);
	//gluTessVertex(tess, quad2[3], quad2[3]);
	//gluTessEndContour(tess);
	//gluTessEndPolygon(tess);

	gluTessBeginPolygon(tess, 0);
	for (lp_iter = f->loops->begin(); lp_iter != f->loops->end();lp_iter++) {
		gluTessBeginContour(tess);
		if ((*lp_iter)->isinner == 1) {
			cout << f->loops->size() << endl;

			for (he_iter = (*lp_iter)->halfEdges->begin(); he_iter != (*lp_iter)->halfEdges->end(); he_iter++) {
				point[point_idx][0] = (*he_iter)->startVertex->getVertexXPosition();
				point[point_idx][1] = (*he_iter)->startVertex->getVertexYPosition();
				point[point_idx][2] = (*he_iter)->startVertex->getVertexZPosition();
				cout << point[point_idx][0] << " " << point[point_idx][1] << " " << point[point_idx][2] << endl;
				gluTessVertex(tess, point[point_idx], point[point_idx]);
				point_idx++;
			}
		}
		else {
			for (he_iter = (*lp_iter)->halfEdges->begin(); he_iter != (*lp_iter)->halfEdges->end(); he_iter++) {
				point[point_idx][0] = (*he_iter)->startVertex->getVertexXPosition();
				point[point_idx][1] = (*he_iter)->startVertex->getVertexYPosition();
				point[point_idx][2] = (*he_iter)->startVertex->getVertexZPosition();
				cout << point[point_idx][0] << " " << point[point_idx][1] << " " << point[point_idx][2] << endl;
				gluTessVertex(tess, point[point_idx], point[point_idx]);
				point_idx++;
			}
		}
		gluTessEndContour(tess);
	}
	gluTessEndPolygon(tess);


	glEndList();
	gluDeleteTess(tess);
	faceIdList.push_back(id);
}



int main(int argc, char **argv)
{


	initGLUT(argc, argv);
	initGL();

	BrepOperation *b = new BrepOperation();
	b = makeBrepConstruct();
	readBrepConstruct(b);

	glutMainLoop(); /* Start GLUT event-processing loop */

	return 0;
}


int initGLUT(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600); 

	glutInitWindowPosition(300, 200);
	int handle = glutCreateWindow(argv[0]);
	glutDisplayFunc(displayCB);
	glutTimerFunc(1, timerCB, 1);             // redraw only every given millisec
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	return handle;
}

void initGL()
{
	glShadeModel(GL_SMOOTH);  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);                   // background color
	glClearStencil(0);
	glClearDepth(1.0f);                        
	glDepthFunc(GL_LEQUAL);

	initLights();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}


void initLights()
{
	GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f }; 
	GLfloat lightKd[] = { .7f, .7f, .7f, 1.0f }; 
	GLfloat lightKs[] = { 1, 1, 1, 1 };      
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	float lightPos[4] = { 0, 0, 20, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);
}

void displayCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();

	cameraAngleX = cameraAngleX + radio;
	cameraAngleY = cameraAngleY + radio * 2;
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0); 
	glRotatef(cameraAngleY, 0, 1, 0);  


	list<GLuint>::iterator id_iter;
	for (id_iter = faceIdList.begin(); id_iter != faceIdList.end(); id_iter++) {
		glCallList(*id_iter);
	}

	glPopMatrix();
	glutSwapBuffers();
}


void reshapeCB(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	float aspectRatio = (float)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(w) / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}

void idleCB()
{
	glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':
	case 'D':
		drawMode = ++drawMode % 2;
		if (drawMode == 0)     
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			//glDisable(GL_CULL_FACE);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		break;

	default:
		;
	}
	glutPostRedisplay();
}
