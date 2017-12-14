#pragma once
#ifndef _BREPOPERATION_H_
#define _BREPOPERATION_H_
#include "BaseClass.h"

class BrepOperation {
public:
	BrepOperation();
	~BrepOperation();

	list<Solid*> *solidList;

	Solid* mvfs(double x, double y, double z, Vertex *v_in, Loop *lp); //���޵��й���һ���µĵ㣬һ����һ����,//���ɺ���һ������棬���ҹ���һ���µ���
	Vertex* mev(double x, double y, double z, Vertex *v_in, Loop *lp_in);//��һ�������Ļ��ϣ�����һ���µĵ㣬�ٹ���һ�������µ��һ��������ı�
	Loop* mef(Vertex *v_in1, Vertex *v_in2, Loop *lp_in);  //�����������ĵ�Ϊ�˵㣬����һ���µıߣ�ͬʱ����һ���µ��棬�µĻ�
	Loop* kemr(Loop *lp_in, Vertex *v_in_outerlp, Vertex *v_in_innerlp);//��ȥ���е�һ���ߣ�������ߣ�������һ���ڻ�
	int kfmrh(Loop *lp_inner, Loop *lp_otherouter);//ɾ��һ���棬���䶨��Ϊһ������ڻ������������й���һ���������߽������� �ϲ���һ����

	int sweep(double dirx, double diry , double dirz, Face* f_in, list<Loop*>* lp_innner_in, Loop* lp_in);//һ��������һ�������˶�һ�����룬���ҽ����µĶ����µı��µ���

};


#endif // !
