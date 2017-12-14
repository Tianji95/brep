#include "BrepOperation.h"
#include <iostream>
using namespace std;


BrepOperation::BrepOperation() {
	solidList = new list<Solid*>;
}
BrepOperation::~BrepOperation() {

}



//从无到有构造一个新的点，一个环一个面,//生成含有一个点的面，并且构成一个新的体
Solid* BrepOperation::mvfs(double x, double y, double z, Vertex *v_in, Loop* lp) {
	Face* f = new Face();
	Solid* s = new Solid();
	v_in->setVertexPosition(x, y, z);

	//设置环
	lp->face = f;

	//设置面
	f->s = s;
	f->loops->push_back(lp);

	//设置体
	s->faces->push_back(f);
	this->solidList->push_back(s);
	return s;

}

//在一个给定的环上，构造一个新的点，再构造一个连接新点和一个给定点的边
Vertex* BrepOperation::mev(double x, double y, double z, Vertex *v_in, Loop *lp_in) {
	Solid *s = lp_in->face->s;
	Vertex* newv = new Vertex();
	HalfEdge* her = new HalfEdge();
	HalfEdge* hel = new HalfEdge();
	Edge* e = new Edge();

	//设置点
	newv->setVertexPosition(x, y, z);
	newv->prev = v_in;
	v_in->next = newv;

	//设置半边和环
	her->startVertex = hel->endVertex = v_in;
	her->endVertex = hel->startVertex = newv;
	her->lp = hel->lp = lp_in;
	hel->e = her->e = e;

	if (lp_in->halfEdges->empty()) {
		//如果环是空的
		//cout<<"empty"<<endl;
		lp_in->halfEdges->push_back(her);
		lp_in->halfEdges->push_back(hel);
		her->prev = her->next = hel;
		hel->next = hel->prev = her;
	}
	else{
		//cout << "not empty" << endl;
		//如果环非空
		list<HalfEdge*>::iterator heIter;
		for (heIter = lp_in->halfEdges->begin(); (*heIter)->startVertex != v_in; heIter++);
		HalfEdge *nowEH = *heIter;
		her->prev = nowEH->prev;
		her->next = hel;
		lp_in->halfEdges->insert(heIter, her);

		lp_in->halfEdges->insert(heIter, hel);
		hel->prev = her;
		hel->next = nowEH;
		nowEH->prev->next = her;
		nowEH->prev = hel;
	}

	//设置边
	e->halfEr = her;
	e->halfEl = hel;
	e->s = s;
	s->edges->push_back(e);
	return newv;
}

//以两个给定的点为端点，构造一条新的边，同时构造一个新的面，新的环,返回的新环是顺时针的
Loop* BrepOperation::mef(Vertex *v_in1, Vertex *v_in2, Loop *lp_in) {
	Solid *s = lp_in->face->s;//用来给下面的元素归属
	Edge *e = new Edge();
	HalfEdge *her = new HalfEdge();
	HalfEdge *hel = new HalfEdge();
	Loop *newLoop = new Loop();
	Face *newFace = new Face();

	//初始化新的边和半边
	hel->endVertex = her->startVertex = v_in1;

	hel->startVertex = her->endVertex = v_in2;
	hel->e = her->e = e;
	e->halfEl = hel;
	e->halfEr = her;
	e->s = s;
	s->edges->push_back(e);


	//这里要注意的是如果是有内环的话，那么点v_in1起点就有可能有两个边，需要找到第二条边
	list<HalfEdge*>::iterator heIter;
	HalfEdge *tempHE1, *tempHE11,*tempHE2;
	for (heIter = lp_in->halfEdges->begin(); (*heIter)->startVertex != v_in1; heIter++);
	tempHE1 = *heIter;


	for (heIter = lp_in->halfEdges->begin(); (*heIter)->startVertex != v_in2 ; heIter++);
	tempHE2 = *heIter;
	heIter++;
	for (; heIter != lp_in->halfEdges->end(); heIter++) {
		if ((*heIter)->startVertex == v_in2) {
			//cout << "1111111111111111" << endl;
			//cout << (*heIter)->startVertex->getVertexXPosition() << " " << (*heIter)->startVertex->getVertexYPosition() << " " << (*heIter)->startVertex->getVertexZPosition() << endl;
			//cout << (*heIter)->endVertex->getVertexXPosition() << " " << (*heIter)->endVertex->getVertexYPosition() << " " << (*heIter)->endVertex->getVertexZPosition() << endl;
			tempHE2 = *heIter;
		}
	}

	//加入新的边，构成新的环
	her->prev = tempHE1->prev;
	tempHE1->prev->next = her;
	her->next = tempHE2;
	hel->next = tempHE1;
	tempHE1->prev = hel;
	hel->prev = tempHE2->prev;
	tempHE2->prev->next = hel;
	tempHE2->prev = her;


	newLoop->halfEdges->push_back(hel);
	Vertex *tempV = hel->startVertex;
	HalfEdge *tempE = hel->next;

	while (tempE->startVertex != tempV) {
		newLoop->halfEdges->push_back(tempE);
		for (heIter = lp_in->halfEdges->begin(); heIter != lp_in->halfEdges->end(); heIter++) {
			if (*heIter == tempE) {
				heIter = lp_in->halfEdges->erase(heIter);
				break;
			}
		}
		tempE->lp = newLoop;
		tempE = tempE->next;
	}

	for (heIter = lp_in->halfEdges->begin(); (*heIter)!= tempHE2; heIter++);
	lp_in->halfEdges->insert(heIter,her);

	newLoop->face = newFace;
	newLoop->prev = lp_in;
	newLoop->next = lp_in->next;
	lp_in->next = newLoop;

	//添加一个新的面
	newFace->loops->push_back(newLoop);
	newFace->s = s;
	newFace->prev = lp_in->face;
	newFace->next = lp_in->face->next;

	s->faces->push_back(newFace);
	s->edges->push_back(e);
	return newLoop;

}

//消去环中的一条边（两个半边），构造一个内环
Loop* BrepOperation::kemr(Loop *lp_in, Vertex *v_in_outerlp, Vertex *v_in_innerlp) {
	Face* face = lp_in->face;
	Loop* newLoop = new Loop();
	Solid* s = lp_in->face->s;
	HalfEdge *delHEr, *delHEl, *tempHE;

	list<HalfEdge*>::iterator heIter1;
	list<HalfEdge*>::iterator heIter2;
	list<HalfEdge*>::iterator heIter3;
	for (heIter1 = lp_in->halfEdges->begin(); (*heIter1)->endVertex != v_in_outerlp||(*heIter1)->startVertex != v_in_innerlp; heIter1++);
	delHEr = *heIter1;
	for (heIter2 = lp_in->halfEdges->begin(); (*heIter2)->endVertex != v_in_innerlp||(*heIter2)->startVertex != v_in_outerlp; heIter2++);
	delHEl = *heIter2;

	//处理内环
	delHEl->next->prev = delHEr->prev;
	delHEr->prev->next = delHEl->next;
	//处理外环
	delHEl->prev->next = delHEr->next;
	delHEr->next->prev = delHEl->prev;

	//删除两个半边
	for (heIter1 = lp_in->halfEdges->begin(); (*heIter1)->endVertex != v_in_outerlp || (*heIter1)->startVertex != v_in_innerlp; heIter1++);
	heIter1 = lp_in->halfEdges->erase(heIter1++);
	for (heIter2 = lp_in->halfEdges->begin(); (*heIter2)->endVertex != v_in_innerlp || (*heIter2)->startVertex != v_in_outerlp; heIter2++);
	lp_in->halfEdges->erase(heIter2);

	for (heIter3 = lp_in->halfEdges->begin(); (*heIter3) != delHEl->next; heIter3++);
	lp_in->halfEdges->erase(heIter3);



	//创建新的环，并且放到面里面
	newLoop->halfEdges->push_back(delHEl->next);
	delHEl->next->lp = newLoop;
	for (tempHE = delHEl->next->next; tempHE != delHEl->next; tempHE = tempHE->next) {
		tempHE->lp = newLoop;
		newLoop->halfEdges->push_back(tempHE);
		for (heIter1 = lp_in->halfEdges->begin(); *heIter1 != tempHE; heIter1++);
		heIter1 = lp_in->halfEdges->erase(heIter1++);
	}
	newLoop->face = face;
	newLoop->isinner = 1;
	face->loops->push_back(newLoop);

	delete delHEl;
	delete delHEr;
	return newLoop;

}


//删除一个面，将其定义为一个面的内环，进而在体中构造一个柄，或者将两个体 合并成一个体
int BrepOperation::kfmrh(Loop *lp_inner, Loop *lp_outer) {
	Solid *s_inner = lp_inner->face->s;
	Solid *s_outer = lp_outer->face->s;
	Face *f_inner = lp_inner->face;
	Face *f_outer = lp_outer->face;


	lp_inner->isinner = 1;
	if (!f_outer->loops->empty()) {
		f_outer->loops->back()->next = lp_inner;
		lp_inner->prev = f_outer->loops->back();
	}
	f_outer->loops->push_back(lp_inner);
	lp_inner->face = f_outer;

	//删除面
	list<Face*>::iterator tempFace;
	for (tempFace = s_inner->faces->begin(); *tempFace != f_inner; tempFace++);
	s_inner->faces->erase(tempFace);

	//处理体，如果是两个体合成的话应该把所有面边转移过来
	list<Edge*>::iterator tempEdge;
	if (s_inner != s_outer) {
		for (tempFace = s_inner->faces->begin(); tempFace != s_inner->faces->end(); tempFace++) {
			(*tempFace)->s = s_outer;
			(*tempFace)->prev = s_outer->faces->back();
			s_outer->faces->back()->next = *tempFace;
			s_outer->faces->push_back(*tempFace);
		}
		for (tempEdge = s_inner->edges->begin(); tempEdge != s_inner->edges->end(); tempEdge++) {
			(*tempEdge)->s = s_outer;
			(*tempEdge)->prev = s_outer->edges->back();
			s_outer->edges->back()->next = *tempEdge;
			s_outer->edges->push_back(*tempEdge);
		}
		if (s_inner->prev) {
			s_inner->prev->next = s_inner->next;
		}
		if (s_inner->next) {
			s_inner->next->prev = s_inner->prev;
		}

	}
	return 1;
}

//一个面沿着一定方向运动一定距离，并且建立新的定点新的边新的面,lp_in是最外面的环
int BrepOperation::sweep(double dirx, double diry, double dirz, Face* f_in, list<Loop*>* lp_innner_list, Loop* lp_in) {
	Vertex *tempV, *firstTempV;
	Vertex *newV1, *newV2, *firstNewV;
	Loop *newOuterLoop = new Loop();
	Loop *newinnerLoop = new Loop();
	double newX, newY, newZ;

	list<Loop*>::iterator lpIter;
	list<HalfEdge*>::iterator heIter;
	cout << "333333" << endl;
	cout << f_in->loops->size() << endl;




	heIter = lp_in->halfEdges->begin();
	firstTempV = tempV = (*heIter)->startVertex;
	newX = tempV->getVertexXPosition() + dirx;
	newY = tempV->getVertexYPosition() + diry;
	newZ = tempV->getVertexZPosition() + dirz;
	cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;

	newV2 = firstNewV = newV1 = mev(newX, newY, newZ, tempV, lp_in);
	heIter++;
	for (; heIter != lp_in->halfEdges->end(); heIter++) {
		tempV = (*heIter)->startVertex;
		newX = tempV->getVertexXPosition() + dirx;
		newY = tempV->getVertexYPosition() + diry;
		newZ = tempV->getVertexZPosition() + dirz;
		cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;
		newV2 = mev(newX, newY, newZ, tempV, lp_in);
		mef(newV1, newV2, lp_in);
		newV1 = newV2;
	}
	newOuterLoop = mef(firstNewV, newV1, lp_in);

	if (!lp_innner_list->empty()) {
		for (lpIter = lp_innner_list->begin(); lpIter != lp_innner_list->end(); lpIter++) {
			heIter = (*lpIter)->halfEdges->begin();
			firstTempV = tempV = (*heIter)->startVertex;
			newX = tempV->getVertexXPosition() + dirx;
			newY = tempV->getVertexYPosition() + diry;
			newZ = tempV->getVertexZPosition() + dirz;
			cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;

			newV2 = firstNewV = newV1 = mev(newX, newY, newZ, tempV, (*lpIter));
			heIter++;
			for (; heIter != (*lpIter)->halfEdges->end(); heIter++) {
				tempV = (*heIter)->startVertex;
				newX = tempV->getVertexXPosition() + dirx;
				newY = tempV->getVertexYPosition() + diry;
				newZ = tempV->getVertexZPosition() + dirz;
				cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;
				newV2 = mev(newX, newY, newZ, tempV, (*lpIter));
				mef(newV1, newV2, (*lpIter));
				newV1 = newV2;
			}
			newinnerLoop = mef(firstNewV, newV1, (*lpIter));
			this->kfmrh(newinnerLoop, newOuterLoop);
		}
	}

	//for (lpIter = f_in->loops->begin(); lpIter != f_in->loops->end(); lpIter++) {
	//	if ((*lpIter)->isinner != 1) {
	//		continue;
	//	}
	//	cout << f_in->loops->size() << endl;
	//	heIter = (*lpIter)->halfEdges->begin();
	//	firstTempV = tempV = (*heIter)->startVertex;
	//	newX = tempV->getVertexXPosition() + dirx;
	//	newY = tempV->getVertexYPosition() + diry;
	//	newZ = tempV->getVertexZPosition() + dirz;
	//	cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;

	//	newV2 = firstNewV = newV1 = mev(newX, newY, newZ, tempV, *lpIter);
	//	heIter++;
	//	for (; heIter != (*lpIter)->halfEdges->end(); heIter++) {
	//		tempV = (*heIter)->startVertex;
	//		newX = tempV->getVertexXPosition() + dirx;
	//		newY = tempV->getVertexYPosition() + diry;
	//		newZ = tempV->getVertexZPosition() + dirz;
	//		cout << tempV->getVertexXPosition() << " " << tempV->getVertexYPosition() << " " << tempV->getVertexZPosition() << endl;
	//		newV2 = mev(newX, newY, newZ, tempV, *lpIter);
	//		mef(newV1, newV2, *lpIter);
	//		newV1 = newV2;
	//	}
	//	newinnerLoop = mef( firstNewV, newV1, *lpIter);
	//	this->kfmrh(*lpIter, newOuterLoop);
	//}
	
	return 1;
}