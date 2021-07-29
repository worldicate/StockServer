#include"bsttree.h"
	BTNode* BTreeinit(void)
	{
		BTNode* nd = (BTNode*)Malloc(sizeof(BTNode));
		nd->right = NULL;
		nd->left = NULL;
		Sem_init(&nd->mutex,0,1);
		Sem_init(&nd->w,0,1);
		return nd;
	}
	void SetData(BTNode* bt, Bdata data) { bt->data = data; }
	BTNode* getleft(BTNode* bt) { return bt->left; }
	BTNode* getright(BTNode* bt) { return bt->right; }

	void makeleft(BTNode* main, BTNode* sub)
	{
		if (main->left != NULL)	free(main->left);
		main->left = sub;
	}

	void makeright(BTNode* main, BTNode* sub)
	{
		if (main->right != NULL) free(main->right);
		main->right = sub;
	}


	//// BST 
	void BSTInit(BTNode** pRoot) { *pRoot = NULL; }
	void BSTInsert(BTNode** pRoot, Bdata data)
	{
		BTNode* cNode = *pRoot;
		BTNode* pNode = NULL;
		BTNode* nNode = NULL;
		while (cNode != NULL)
		{
			if (data.ID == cNode->data.ID) return;;
			pNode = cNode;
			if (cNode->data.ID > data.ID) cNode = getleft(cNode);
			else cNode = getright(cNode);
		}

		nNode = BTreeinit();
		SetData(nNode, data);

		if (pNode != NULL) {
			if (data.ID < pNode->data.ID)	makeleft(pNode, nNode);
			else makeright(pNode, nNode);
		}
		else { *pRoot = nNode; }
	}

	BTNode* BSTSearch(BTNode* bst, int  target)
	{
		BTNode* cNode = bst;
		Bdata cd;
		//BTNode * real = NULL;
		while (cNode != NULL)
		{
			cd = cNode->data;
			if (target == cd.ID) return cNode ; 
			else if (target < cd.ID) cNode = getleft(cNode);
			else cNode = getright(cNode);
		}
		
		return cNode;
	}


	void ShowData(Bdata data)
	{
		printf("%d %d %d", data.ID, data.left_stock, data.price);
		printf("\n");
	}

	void Showfunc(int  buf ) {
		
		inorder(bstRoot, ShowData, buf);
	//	buf[strlen(buf)-1]='\n';

	}

	void inorder(BTNode* bt, funcptr ptr, int connfd)
	{
		if (bt == NULL)	return;
		inorder(bt->left, ptr,connfd);

	P(&bt->mutex);
	bt->data.readcnt++;
	if(bt->data.readcnt==1) P(&bt->w);
	V(&bt->mutex);
	char cmd[10000];
	char temp[10];

	sprintf(cmd, "%d", bt->data.ID);
	strcat(cmd," ");
	sprintf(temp, "%d", bt->data.left_stock);
	strcat(cmd, temp);
	strcat(cmd, " ");
	sprintf(temp, "%d", bt->data.price);
	strcat(cmd, temp);
	strcat(cmd, "\n");
	int cmdsize = strlen(cmd);
	Rio_writen(connfd,cmd, cmdsize);

	P(&bt->mutex);
	bt->data.readcnt--;
	if(bt->data.readcnt==0) V(&bt->w);
	V(&bt->mutex);
	

	inorder(bt->right, ptr,  connfd);
	return ;
}
////////////////////////////////////////////////////////////
int Sellfunc(int stockID, int stockcount) {
	BTNode* cTree = BSTSearch(bstRoot, stockID);
	if (cTree == NULL) { 	
		return FAIL; }

		P(&cTree->w);
		cTree->data.left_stock += stockcount;
		V(&cTree->w);
	return SUCCESS;
}
int  Buyfunc(int stockID, int stockcount) {
	BTNode* cTree = BSTSearch(bstRoot, stockID);
	if (cTree == NULL) { 
		return FAIL; }
		
	if (cTree->data.left_stock < stockcount) {  
		return leftFail; };
	P(&cTree->w);
	cTree->data.left_stock -= stockcount;
	V(&cTree->w);
	return SUCCESS;
}

void Writedata(FILE* output, Bdata data) {
	fprintf(output, "%d %d %d\n", data.ID, data.left_stock, data.price);

}void inorderwrite(BTNode* bt, FILE * output) {
	if(bt== NULL) return;
	inorderwrite(bt->left, output);
	Writedata(output, bt->data);
	inorderwrite(bt->right, output);
}

void Filewrite() {
	FILE* output = fopen("stock.txt", "wt");
	if (output == NULL) { printf("file open ERROR"); return; }
	inorderwrite(bstRoot, output);
	fclose(output);
}

void strchstar(char* buf) {

	for (int i = 0; i < strlen(buf); i++) {
		if (buf[i] == '\n') buf[i] = '*';
	}

}
