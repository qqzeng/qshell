#include <stdio.h>
#include <stdlib.h>
#include "qshell.h"

typedef struct LNode{
	int data;
	struct LNode *next;
}LNode;


/*
//初始化链表：①此为尾插法；
//			  ②且含有头结点，而头结点不含数据域
LNode * _createLNode(LNode *L){

	printf("please enter the elements belongs to the LinkList end with 0!\n");
	LNode *p,*q;
	L = (LNode *)malloc(sizeof(LNode));
	L->next = NULL;
	q = p = (LNode *)malloc(sizeof(LNode));
	p->next = L->next;
	L->next = p;
	scanf("%d", &p->data);
	if (p->data == 0) {
		free(L->next); L->next = NULL;
	}
	else{
		while (p->data){
			p = (LNode *)malloc(sizeof(LNode));
			scanf("%d", &p->data);
			if (p->data == 0) break;
			p->next = q->next;
			q->next = p;
			q = p;
		}
	}
	return L;
}
*/
LNode * _createLNode(LNode *L){
	printf("please enter the elements belongs to the LinkList end with 0!\n");
	LNode *p, *q;
	L = (LNode *)malloc(sizeof(LNode));
	L->next = NULL;
	q = p = (LNode *)malloc(sizeof(LNode));
	scanf("%d", &p->data);
	if (p->data) {
		p->next = L->next;
		L->next = p;
		p = (LNode *)malloc(sizeof(LNode));
		scanf("%d", &p->data);
		while (p->data){
			p->next = q->next;
			q->next = p;
			q = p;
			p = (LNode *)malloc(sizeof(LNode));
			scanf("%d", &p->data);
		}
	}
	else{
		free(p);
	}
	return L;
}
/*
//初始化链表：①此为头插法；
//			  ②且含有头结点，而头结点不含数据域
LNode* createLNode(LNode *L){
	
	printf("please enter the elements belongs to the LinkList end with 0!\n");
	//循环输入，以0结束，且非首0不包含在链表当中
	LNode *p;
	L = (LNode *)malloc(sizeof(LNode));
	L->next = NULL;
	p = (LNode *)malloc(sizeof(LNode));
	p -> next = L->next;
	L->next = p;
	scanf("%d", &p->data);
	if (p->data == 0) {
		free(L->next); L->next = NULL;
	}else{
		while (p->data){
			p = (LNode *)malloc(sizeof(LNode));
			scanf("%d", &p->data);
			if (p->data == 0) break;
			p->next = L->next;
			L->next = p;
		}
	}
	return L;
}
*/
LNode* createLNode(LNode *L){

	printf("please enter the elements belongs to the LinkList end with 0!\n");
	//循环输入，以0结束，且非首0不包含在链表当中
	LNode *p;
	L = (LNode *)malloc(sizeof(LNode));
	L->next = NULL;
	p = (LNode *)malloc(sizeof(LNode));
	scanf("%d", &p->data);
	if (p->data) {
		while (p->data){
			p->next = L->next;
			L->next = p;
			p = (LNode *)malloc(sizeof(LNode));
			scanf("%d", &p->data);
		}
	}else{
		free(p);
	}
	return L;
}

//循环打印链表元素
void printLinkList(LNode *L){
	LNode *p;
	p = L ->next;// p 指向链表的第一个结点！
	if (!p){
		printf("链表为空！\n");
		return;
	}
	while (p){
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}


//求链表的长度
int getLength(LNode *L){
	LNode *p = L;
	int length = 0;
	while (p->next != NULL){
		p = p->next;
		length++;
	}
	return length;
}

//删除链表中位置指定的元素，并返回其值
int deleteElement(LNode *L, int i){
	if (i < 1 || i >getLength(L)){
		printf("删除位置不合法！\n");
		return 0;
	}
	int e = 0;
	LNode *p = L;
	int j = 0;
	while (p->next && j < i - 1){//这样较为安全
		p = p->next;
		j++;
	}
	if (!(p->next) || j > i - 1) {
		printf("删除位置不合法！\n");
		return 0;
	}
	LNode *q = p->next;
	p->next = q->next;
	e = q->data;
	free(q);
	return e;
}

LNode * InsertElement(LNode *L, int i, int e){
	if (i < 1 || i > getLength(L) + 1){
		printf("插入位置不合法！\n");
		return 0;
	}
	LNode *p = L;
	int j = 0;
	while (p->next && j < i - 1){
		//这样较为安全 要理解 p->next 与 p 的区别，因为我们在插入或者是删除结点的时候，都是从表头结点遍历到 目标结点的前一个结点；
		p = p->next;
		j++;
	}
	if ((p->next==NULL) || j > i - 1) {
		printf("插入位置不合法！\n");
		return 0;
	}
	LNode *q = (LNode *)malloc(sizeof(LNode));
	q->data = e;
	q->next = p->next;
	p->next = q;
	return L;
}

//合并两个非递减的顺序的链表
LNode * MergeLinkList(LNode *L1, LNode *L2){

	LNode *pc = L1;
	LNode *head = pc;
	LNode *p1 = L1->next;
	LNode *p2 = L2->next;

	while (p1 && p2){
		if (p1->data <= p2->data){
			pc->next = p1;
			pc = p1;
			p1 = p1->next;
		}
		else{
			pc->next = p2;
			pc = p2;
			p2 = p2->next;
		}
	}
	pc->next = p1 == NULL ? p2 : p1;
	return head;
}

int ModifyLinkList(LNode *L, int i, int newElement){
	if (i > getLength(L) || i < 1){
		printf("修改元素的位置不合法！\n");
		return 0;
	}
	LNode *p = L;
	int j = 0;
	while (p->next && j < i - 1){
		p = p->next;
		j++;
	}
	if (!(p->next) || j > i - 1){
		printf("修改元素的位置不合法！\n");
		return 0;
	}
	p->next->data = newElement;
	return 1;
}
int main(){

	LNode *p = NULL;
	p = _createLNode(p);
	printLinkList(p);
	
	int i = 0;
	printf("\n please enter the position of the aimed element but the HeadNode1!\n");
	scanf("%d", &i);
	int ele1 = deleteElement(p,i);
	printLinkList(p);
	printf("\n please enter the aimed element  and its position but the HeadNode \n");
	int pos = 0;
	int ele2 = 0;
	scanf("%d %d", &i,&ele2);
	InsertElement(p,i, ele2);
	printLinkList(p);
	
	/*
	LNode *q = NULL;
	q= _createLNode(q);
	printLinkList(q);

	p = MergeLinkList(p, q);
	printLinkList(p);

	printf("the length of qLinkList is %d,and the length of pLinkList is %d\n", getLength(q),getLength(p));
	int j = 0, newElement = 0;
	printf("please enter the position of the newElement and its value\n");
	scanf("%d%d", &j, &newElement);
	ModifyLinkList(p, j, newElement);
	printLinkList(p);
	*/
	return 0;
}