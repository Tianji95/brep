#pragma once
#ifndef _BASECLASS_H_
#define _BASECLASS_H_
#include <list>
using namespace std;

class Solid;
class Face;
class Loop;
class Edge;
class HalfEdge;
class Vertex;



class Solid {
public:
	Solid();
	~Solid();

	list<Face*> *faces;
	list<Edge*> *edges;

	Solid* next;
	Solid* prev;

};


class Face {
public:
	Face();
	Face(const Face&);
	~Face();


	list<Loop*> *loops;
	Solid* s;

	Face* prev;
	Face* next;

};

class Loop {
public:
	Loop();
	Loop(const Loop&);
	~Loop();

	Face *face;
	list<HalfEdge*> *halfEdges;
	Loop* prev;
	Loop* next;
	bool isinner = 0;


};

class Edge {
public:
	Edge();
	Edge(const Edge&);
	~Edge();

	HalfEdge *halfEr;
	HalfEdge *halfEl;
	Solid *s;

	Edge* next;
	Edge* prev;


};


class HalfEdge {
public:
	HalfEdge();
	HalfEdge(Vertex start, Vertex end);
	~HalfEdge();

	Vertex *startVertex;
	Vertex *endVertex;
	Loop* lp;
	Edge* e;

	HalfEdge *prev;
	HalfEdge *next;


};


class Vertex {
public:
	Vertex();
	Vertex(const double x, const double y, const double z);
	Vertex(const Vertex&);
	~Vertex();

	void setVertexColor(int r, int g, int b);
	void setVertexPosition(double x, double y, double z);
	double getVertexXPosition();
	double getVertexYPosition();
	double getVertexZPosition();
	void draw() const;

	//int id;
	Vertex *prev, *next;
private:
	double xPosition;
	double yPosition;
	double zPosition;
	int r;
	int g;
	int b;

};

#endif // !_BASECLASS_H_
