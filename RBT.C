#include<stdio.h>
#include<stdlib.h>
#include<string.h>
enum COLOUR{RED,BLACK};
typedef struct tree
{
	int colour;
	int nVal;
	struct tree*pLeft;
	struct tree*pRight;
	struct tree*pFather;
}Tree;

void RightRotate(Tree **tree)
{
	//右旋
	if( (*tree)==NULL || (*tree)->pLeft==NULL)return;

	Tree *temp=*tree;//A
	Tree *node=(*tree)->pLeft;//B

	//3 child
	//B的右孩子成为A的左孩子
	temp->pLeft=node->pRight;
	node->pRight=temp;//A成为B的右孩子

	if(temp->pFather!=NULL)
	{
		if(temp==temp->pFather->pLeft)
			temp->pFather->pLeft=node;
		else
			temp->pFather->pRight=node;
	}
	else
		*tree=node;//如果A原来是根 那么现在B是根

	//3 father
	//刚刚赋值的 A的左孩子若不是空
	if(temp->pLeft!=NULL)
		temp->pLeft->pFather=temp;
	//A的父亲成为B的父亲
	node->pFather=temp->pFather;//是空就直接赋空
	//B成为A的父亲
	temp->pFather=node;
}
void LeftRotate(Tree **tree)
{
	//左旋
	if( (*tree)==NULL || (*tree)->pRight==NULL)return;

	Tree *temp=*tree;//A
	Tree *node=(*tree)->pRight;//B

	//3 child
	temp->pRight=node->pLeft;
	node->pLeft=temp;

	if(temp->pFather!=NULL)
	{
		if(temp==temp->pFather->pLeft)
			temp->pFather->pLeft=node;
		else
			temp->pFather->pRight=node;
	}
	else
		*tree=node;//如果A原来是根 那么现在B是根

	//3 father
	if(temp->pRight!=NULL)
		temp->pRight->pFather=temp;
	//A的父亲成为B的父亲
	node->pFather=temp->pFather;//是空就直接赋空
	//B成为A的父亲
	temp->pFather=node;
}

//得到叔叔节点写成函数 因为后续循环节点变化要得到不一样的叔叔
Tree *GetUncle(Tree *tree)
{
	if(tree=tree->pFather->pLeft)
		return tree->pFather->pRight;
	else
		return tree->pFather->pLeft;
}

void CreateRBT(Tree **tree,int n)
{
	//1.树是空的
	if(*tree==NULL)
	{
		*tree=(Tree*)malloc(sizeof(Tree));
		(*tree)->colour=BLACK;
		(*tree)->nVal=n;
		(*tree)->pLeft=NULL;
		(*tree)->pRight=NULL;
		(*tree)->pFather=NULL;
		return;
	}
	//2.树非空
	Tree *temp=*tree;
	Tree *newTree=(Tree*)malloc(sizeof(Tree));
	newTree->colour=RED;
	newTree->pLeft=NULL;
	newTree->pRight=NULL;
	newTree->nVal=n;
	newTree->pFather=NULL;
	//先把节点放到对应的位置上 找到父亲
	while(1)
	{
		if(n > temp->nVal)//往右找 看右是否为空
		{
			if(temp->pRight!=NULL)
				temp=temp->pRight;
			else
			{
				newTree->pFather=temp;
				temp->pRight=newTree;
				break;
			}
		}
		else if(n <temp->nVal)
		{
			if(temp->pLeft!=NULL)
				temp=temp->pLeft;
			else
			{
				newTree->pFather=temp;
				temp->pLeft=newTree;
				break;
			}

		}
		else
		{
			printf("error..\n");
			free(newTree);
			newTree=NULL;
			break;
		}
	}
	Tree *pUncle = NULL;
	Tree *pGrandFather=NULL;
	Tree *father=newTree->pFather;
	Tree *pTemp=newTree;//后续当前节点也会发生变化
	//1.父亲是黑色
	if(father->colour==BLACK)return;
	//2.父亲是红色
	while(father->colour==RED)
	{
		pUncle=GetUncle(father);
		pGrandFather=father->pFather;
		//2.1叔叔是红色
		if(pUncle!=NULL&&pUncle->colour==RED)
		{
			pUncle->colour=BLACK;
			father->colour=BLACK;
			pGrandFather->colour=RED;

			// 爷爷成为新的标记节点
			pTemp=pGrandFather;
			father=pTemp->pFather;//必须重新被赋值

			if(father==NULL)
			{
				(*tree)->colour=BLACK;
				break;
			}
			continue;
		}
		//2.2叔叔是黑色  可能是空节点或者是真黑
		if(pUncle==NULL ||pUncle->colour==BLACK)
		{
			//2.2.1父亲是爷爷的左
			if(father==pGrandFather->pLeft)
			{
				//我是父亲的右
				if(pTemp=father->pRight)
				{
					//以父亲为支点进行左旋
					pTemp=father;
					LeftRotate(&pTemp);
					father=pTemp->pFather;//顺序不能更改
				}
				//我是父亲的左
				if(pTemp=father->pLeft)
				{
					//调整颜色以后  以爷爷为支点右旋
					father->colour=BLACK;
					pGrandFather->colour=RED;
					RightRotate(&pGrandFather);
					break;
				}
			}
			//2.2.2父亲是爷爷的右
			if(father==pGrandFather->pRight)
			{
				if(pTemp=father->pLeft)
				{
					pTemp=father;
					RightRotate(&pTemp);
					father=pTemp->pFather;
				}
				if(pTemp==father->pRight)
				{
					father->colour=BLACK;
					pGrandFather->colour=RED;
					LeftRotate(&pGrandFather);
					break;
				}
			}
		}
	}
}
void Print(Tree *tree)
{
	if(tree==NULL)return;
	printf("val==%d\tcol==%d..\n",tree->nVal,tree->colour);
	Print(tree->pLeft);
	Print(tree->pRight);
}
int main()
{
	Tree *tree=NULL;
	int n,x;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&x);
		CreateRBT(&tree,x);
	}
	Print(tree);
	return 0;
}
