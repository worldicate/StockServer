#ifndef __BSTTREE_H__
#define __BSTTREE_H__
#include"csapp.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NTHREADS 4
#define SBUFSIZE 16
#define SUCCESS 1 
#define FAIL 0
#define leftFail 2

typedef struct _bdata {
	int ID;
	int left_stock;
	int price;
	int readcnt;
	
}Bdata;

typedef struct _BTNode
{
	Bdata data;
	sem_t mutex;
	sem_t w;
	struct _BTNode* left;
	struct _BTNode* right;
} BTNode;

//////////////////////////////
BTNode* BTreeinit(void);
void SetData(BTNode* bt, Bdata data);
BTNode* getleft(BTNode* bt);
BTNode* getright(BTNode* bt);
void makeleft(BTNode* main, BTNode* sub);
void makeright(BTNode* main, BTNode* sub);
//////////////////////////////////////////

///// BST ///////////////////////
typedef void funcptr(Bdata data);
void BSTInit(BTNode** pRoot);
void BSTInsert(BTNode** pRoot, Bdata data);
BTNode* BSTSearch(BTNode* bst, int  target);
void inorder(BTNode* bt, funcptr ptr, int connfd);
////////////////////


////stock func 
void Showfunc(int connfd);
int  Sellfunc(int stockID, int stockcount);
int  Buyfunc(int stockID, int stockcount);
void Filewrite();
void inorderwrite(BTNode * bt, FILE* output);
//////////////////////
// global variable
BTNode* bstRoot;



////////////////////

#endif
