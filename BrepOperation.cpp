#include "BrepOperation.h"
#include <iostream>
using namespace std;


BrepOperation::BrepOperation() {
	solidList = new list<Solid*>;
}
BrepOperation::~BrepOperation() {

}



//���޵��й���һ���µĵ㣬һ����һ����,//���ɺ���һ������棬���ҹ���һ���µ���
Solid* BrepOperation::mvfs(double x, double y, double z, Vertex *v_in, Loop* lp) {
	Face* f = new Face();
	Solid* s = new Solid();
	v_in->setVertexPosition(x, y, z);

	//���û�
	lp->face = f;

	//������
	f->s = s;
	f->loops->push_back(lp);

	//������
	s->faces->push_back(f);
	this->solidList->push_back(s);
	return s;

}

//��һ�������Ļ��ϣ�����һ���µĵ㣬�ٹ���һ�������µ��һ��������ı�
Vertex* BrepOperation::mev(double x, double y, double z, Vertex *v_in, Loop *lp_in) {
	Solid *s = lp_in->face->s;
	Vertex* newv = new Vertex();
	HalfEdge* her = new HalfEdge();
	HalfEdge* hel = new HalfEdge();
	Edge* e = new Edge();

	//���õ�
	newv->setVertexPosition(x, y, z);
	newv->prev = v_in;
	v_in->next = newv;

	//���ð�ߺͻ�
	her->startVertex = hel->endVertex = v_in;
	her->endVertex = hel->startVertex = newv;
	her->lp = hel->lp = lp_in;
	hel->e = her->e = e;

	if (lp_in->halfEdges->empty()) {
		//������ǿյ�
		//cout<<"empty"<<endl;
		lp_in->halfEdges->push_back(her);
		lp_in->halfEdges->push_back(hel);
		her->prev = her->next = hel;
		hel->next = hel->prev = her;
	}
	else{
		//cout << "not empty" << endl;
		//������ǿ�
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

	//���ñ�
	e->halfEr = her;
	e->halfEl = hel;
	e->s = s;
	s->edges->push_back(e);
	return newv;
}

//�����������ĵ�Ϊ�˵㣬����һ���µıߣ�ͬʱ����һ���µ��棬�µĻ�,���ص��»���˳ʱ���
Loop* BrepOperation::mef(Vertex *v_in1, Vertex *v_in2, Loop *lp_in) {
	Solid *s = lp_in->face->s;//�����������Ԫ�ع���
	Edge *e = new Edge();
	HalfEdge *her = new HalfEdge();
	HalfEdge *hel = new HalfEdge();
	Loop *newLoop = new Loop();
	Face *newFace = new Face();

	//��ʼ���µıߺͰ��
	hel->endVertex = her->startVertex = v_in1;

	hel->startVertex = her->endVertex = v_in2;
	hel->e = her->e = e;
	e->halfEl = hel;
	e->halfEr = her;
	e->s = s;
	s->edges->push_back(e);


	//����Ҫע�������������ڻ��Ļ�����ô��v_in1�����п����������ߣ���Ҫ�ҵ��ڶ�����
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

	//�����µıߣ������µĻ�
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

	//���һ���µ���
	newFace->loops->push_back(newLoop);
	newFace->s = s;
	newFace->prev = lp_in->face;
	newFace->next = lp_in->face->next;

	s->faces->push_back(newFace);
	s->edges->push_back(e);
	return newLoop;

}

//��ȥ���е�һ���ߣ�������ߣ�������һ���ڻ�
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

	//�����ڻ�
	delHEl->next->prev = delHEr->prev;
	delHEr->prev->next = delHEl->next;
	//�����⻷
	delHEl->prev->next = delHEr->next;
	delHEr->next->prev = delHEl->prev;

	//ɾ���������
	for (heIter1 = lp_in->halfEdges->begin(); (*heIter1)->endVertex != v_in_outerlp || (*heIter1)->startVertex != v_in_innerlp; heIter1++);
	heIter1 = lp_in->halfEdges->erase(heIter1++);
	for (heIter2 = lp_in->halfEdges->begin(); (*heIter2)->endVertex != v_in_innerlp || (*heIter2)->startVertex != v_in_outerlp; heIter2++);
	lp_in->halfEdges->erase(heIter2);

	for (heIter3 = lp_in->halfEdges->begin(); (*heIter3) != delHEl->next; heIter3++);
	lp_in->halfEdges->erase(heIter3);



	//�����µĻ������ҷŵ�������
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


//ɾ��һ���棬���䶨��Ϊһ������ڻ������������й���һ���������߽������� �ϲ���һ����
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

	//ɾ����
	list<Face*>::iterator tempFace;
	for (tempFace = s_inner->faces->begin(); *tempFace != f_inner; tempFace++);
	s_inner->faces->erase(tempFace);

	//�����壬�����������ϳɵĻ�Ӧ�ð��������ת�ƹ���
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

//һ��������һ�������˶�һ�����룬���ҽ����µĶ����µı��µ���,lp_in��������Ļ�
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