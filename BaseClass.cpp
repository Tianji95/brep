#include "BaseClass.h"
Solid::Solid() {
	this->edges = new list<Edge*>;
	this->faces = new list<Face*>;
	this->prev = nullptr;
	this->next = nullptr;
}

Solid::~Solid() {

}

Face::Face() {
	this->loops = new list<Loop*>;
	this->s = nullptr;
	this->prev = nullptr;
	this->next = nullptr;
}

Face::~Face() {

}

Loop::Loop() {
	this->face = nullptr;
	this->halfEdges = new list<HalfEdge*>;
	this->isinner = 0;
	this->prev = nullptr;
	this->next = nullptr;
}
Loop::~Loop() {

}



Edge::Edge() {
	this->halfEl = nullptr;
	this->halfEr = nullptr;
	this->s = nullptr;
	this->prev = nullptr;
	this->next = nullptr;

}

Edge::~Edge() {

}

HalfEdge::HalfEdge() {
	this->startVertex = nullptr;
	this->endVertex = nullptr;
	this->lp = nullptr;
	this->e = nullptr;
	this->prev = nullptr;
	this->next = nullptr;

}

HalfEdge::~HalfEdge() {

}




Vertex::Vertex() {
	this->prev = nullptr;
	this->next = nullptr;
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->xPosition = 0.0;
	this->yPosition = 0.0;
	this->zPosition = 0.0;

}
Vertex::Vertex(const double x, const double y, const double z) {
	this->prev= nullptr;
	this->next = nullptr;
	this->xPosition = x;
	this->yPosition = y;
	this->zPosition = z;
}
Vertex::Vertex(const Vertex& other) {
	this->prev = other.prev;
	this->next = other.next;
	this->r = other.r;
	this->g = other.g;
	this->b = other.b;
	this->xPosition = other.xPosition;
	this->yPosition = other.yPosition;
	this->zPosition = other.zPosition;
}
Vertex::~Vertex() {

}

void Vertex::setVertexColor(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

void Vertex::setVertexPosition(double x, double y, double z) {
	this->xPosition = x;
	this->yPosition = y;
	this->zPosition = z;
}

double Vertex::getVertexXPosition()  {
	return this->xPosition;
}

double Vertex::getVertexYPosition()  {
	return this->yPosition;
}

double Vertex::getVertexZPosition()  {
	return this->zPosition;
}

void Vertex::draw() const {

}
