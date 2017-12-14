#pragma once
#ifndef _BREPOPERATION_H_
#define _BREPOPERATION_H_
#include "BaseClass.h"

class BrepOperation {
public:
	BrepOperation();
	~BrepOperation();

	list<Solid*> *solidList;

	Solid* mvfs(double x, double y, double z, Vertex *v_in, Loop *lp); //从无到有构造一个新的点，一个环一个面,//生成含有一个点的面，并且构成一个新的体
	Vertex* mev(double x, double y, double z, Vertex *v_in, Loop *lp_in);//在一个给定的环上，构造一个新的点，再构造一个连接新点和一个给定点的边
	Loop* mef(Vertex *v_in1, Vertex *v_in2, Loop *lp_in);  //以两个给定的点为端点，构造一条新的边，同时构造一个新的面，新的环
	Loop* kemr(Loop *lp_in, Vertex *v_in_outerlp, Vertex *v_in_innerlp);//消去环中的一条边（两个半边），构造一个内环
	int kfmrh(Loop *lp_inner, Loop *lp_otherouter);//删除一个面，将其定义为一个面的内环，进而在体中构造一个柄，或者将两个体 合并成一个体

	int sweep(double dirx, double diry , double dirz, Face* f_in, list<Loop*>* lp_innner_in, Loop* lp_in);//一个面沿着一定方向运动一定距离，并且建立新的定点新的边新的面

};


#endif // !
