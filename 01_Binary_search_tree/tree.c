#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

typedef struct pair
{
	Node* parent;
	Node* child;
} Pair;

static void AddNode(Node* new_node, Node* root);
static void InOrder(const Node* root, void(*pfun)(Item item));
static void DeleteNode(Node** ptr);
static Pair SeekItem(const Item* pi, const Tree* ptree);
static void DeleteAllNodes(Node* root);

static int(*compare_func)(const Item item1, const Item item2) = NULL;



/*****************************************************************************
*                                                                            *
* @ NAME      : InitializeTree						     *
* @ FUNCTION  : binary tree Inititalize					     *
* @ PARAMETER :	Tree 구조체 포인터, item.character 문자열 비교함수		  
* @ RETURN	  : 없음							   *								 *
*									     *										 *
*****************************************************************************/

void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2))
{
	ptree->root = NULL;
	ptree->size = 0;
	compare_func = comp_func;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeIsEmpty						     *
* @ FUNCTION  : Tree에 노드가 0인지 확인				      *
		1. Tree의 size가 0인지 확인                                   *
* @ PARAMETER :	Tree 구조체 포인터					        *
* @ RETURN    : bool 성공 true					           *
*									     *										 *
*****************************************************************************/

bool TreeIsEmpty(const Tree* ptree)
{
	return ptree->size == 0;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeIsFull												     *
* @ FUNCTION  : Tree에 노드가 Max인지 확인									 *
				1. Tree의 size가 MAXITEMS인지 확인							 *
* @ PARAMETER :	Tree 구조체 포인터											 *
* @ RETURN	  : bool 성공 true												 *
*																			 *
*****************************************************************************/

bool TreeIsFull(const Tree* ptree)
{
	return ptree->size == MAXITEMS;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeItemCount				                     *
* @ FUNCTION  : Tree에 노드가 size 확인					*
* @ PARAMETER :	Tree 구조체 포인터						*
* @ RETURN	  : (int) Tree의 size 					    *
*																			 *
*****************************************************************************/

int TreeItemCount(const Tree* ptree)
{
	return ptree->size;
}


/*****************************************************************************
*                                                                            *
* @ NAME      : AddItem							     *
* @ FUNCTION  : Tree에 아이템 추가하기					       *
* @ PARAMETER :	추가할 Item포인터, Tree 구조체 포인터			   *
* @ RETURN	  : (bool) 성공 true; 			                   *
*																			 *
*****************************************************************************/

bool AddItem(const Item* pi, Tree* ptree)
{
	// 새로운 노드 만들기 (동적할당)
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL)
	{
		printf("Allocate_Error\n");
		exit(1);
	}
	new_node->item = *pi;
	new_node->left = NULL;
	new_node->right = NULL;

	if (ptree->root == NULL)		// Tree의 root가 없을 때
		ptree->root = new_node;
	else
		AddNode(new_node, ptree->root); // Node 추가 함수 호출
	
	return true;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : AddNode							     *
* @ FUNCTION  : Tree에 Node 추가하기						 *
*		1. 새로 추가할 Node의 위치 정하기(알파벳 순)			 *
*		2. 알파벳 순이 빠르면 left, 느리면 rigt로 이동		  *
*		3. 그 Node(left or right)가 NULL이면 연결, 아니면 2번 반복	    *
* @ PARAMETER :	추가할 Node포인터, Tree의 Node 포인터			     *
* @ RETURN	  : 없음 							   *
*																			 *
*****************************************************************************/

static void AddNode(Node* new_node, Node* root)
{
	int comp = compare_func(root->item, new_node->item);

	/* 중복된 아이템을 추가할 경우, 추가하지 않음 */
	if (comp == 0)	
	{
		printf("Cannot add duplicate items.\n");
		free(new_node);
		return;
	}
	else if (comp > 0)	// left
	{
		if (root->left == NULL)
			root->left = new_node;
		else
			AddNode(new_node, root->left);
	}
	else			   // right
	{
		if (root->right == NULL)
			root->right = new_node;
		else
			AddNode(new_node, root->right);
	}
}


/*****************************************************************************
*                                                                            *
* @ NAME      : Traverse													 *
* @ FUNCTION  : Tree에 Node, Item의 character문자열 오름차순으로 출력			 *
* @ PARAMETER :	Tree 포인터, 함수 포인터										 *
* @ RETURN	  : 없음 														 *
*																			 *
*****************************************************************************/

void Traverse(const Tree* ptree, void (*pfun)(Item item))
{
	if (ptree != NULL)
		InOrder(ptree->root, pfun);
}


/*****************************************************************************
*                                                                            *
* @ NAME      : AddNode														 *
* @ FUNCTION  : Tree에 Node, Item의 character문자열 오름차순으로 출력			 *
*				1. left로 이동 left가 NULL이면 반환, 아니면 계속 이동			 *
*				2. 1번 반환시 Item 출력										 *
*				3. right로 이동 right가 NULL이면 반환, 아니면 계속 이동		 *
*				4. 1,2,3 반복 (재귀함수)										 *
* @ PARAMETER :	Node포인터, 함수 포인터										 *
* @ RETURN	  : 없음 														 *
*																			 *
*****************************************************************************/

static void InOrder(const Node* root, void(*pfun)(Item item))
{
	if (root == NULL)
		return;

	InOrder(root->left, pfun);
	(*pfun)(root->item);
	InOrder(root->right, pfun);
}


/*****************************************************************************
*                                                                            *
* @ NAME      : InTree														 *
* @ FUNCTION  : 내가 찾는 item이 있는지 확인									 *
* @ PARAMETER :	Item 포인터, Tree 포인터										 *
* @ RETURN	  : (bool) 성공 true, 실패 false 								 *
*																			 *
*****************************************************************************/

bool InTree(const Item* pi, const Tree* ptree)
{
	return (SeekItem(pi, ptree).child == NULL) ? false : true;
}


/*****************************************************************************
*                                                                            *
* @ NAME      : SeekItem						     *
* @ FUNCTION  : 내가 찾는 item이 있는지 확인				     *
*		1. 내가 찾는 item과 tree의 노드들과 문자열 비교		 *
*		2. 찾는 item알파벳 순이 빠르면 left로 이동, 느리면 right 이동   *
*		3. 일치하는 item이 나오면 구조체 Pair반환			  *	
* @ PARAMETER :	Item 포인터, Tree 포인터				        *
* @ RETURN	  : 구조체 Pair 						  *
*																			 *
*****************************************************************************/

static Pair SeekItem(const Item* pi, const Tree* ptree)
{
	Pair look;
	look.parent = NULL;
	look.child = ptree->root;

	if (look.child == NULL)
		return look;

	while (look.child != NULL)
	{
		int comp = compare_func(look.child->item, *pi);

		if (comp > 0)	// left
		{
			look.parent = look.child;
			look.child = look.child->left;
		}
		else if (comp < 0) // right
		{
			look.parent = look.child;
			look.child = look.child->right;
		}
		else
			break;
	}

	return look;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : DeleteItem													 *
* @ FUNCTION  : 해당 Item 제거												 *
*				1. SeekItem함수 호출. 삭제할 Item이있는 Node을 child로 받아옴	 *
*				2. Parent Node가 right or left or root에서 왔는지 체크		 *
*				3. DeleteNode함수 호출										 *
*				4. Tree Item개수 -1											 *
* @ PARAMETER :	제거할 Item 포인터, Tree 포인터								 *
* @ RETURN	  : (bool) 성공 true, 실패 false 								 *
*																			 *
*****************************************************************************/

bool DeleteItem(const Item* pi, Tree* ptree)
{
	Pair look = SeekItem(pi, ptree);
	
	if (look.child == NULL)
		return false;

	if (look.parent == NULL)
		DeleteNode(&ptree->root);
	else if (look.parent->left == look.child)
		DeleteNode(&look.parent->left);
	else
		DeleteNode(&look.parent->right);

	ptree->size--;

	return true;
}


/*****************************************************************************
*                                                                            *
* @ NAME      : DeleteNode													 *
* @ FUNCTION  : 해당 Item Node 제거											 *
*				1. 삭제할 Node의 Child Node의 left, right가 NULL인지 체크		 *
*				2. 삭제할 Node의 Parent Node와 Child Node 연결				 *
*				3. free함수로 제거											 *
* @ PARAMETER :	삭제할 Node의 Parent Node의 left or right (Node 이중포인터)	 *
* @ RETURN	  : 없음							 								 *
*																			 *
*****************************************************************************/

static void DeleteNode(Node** ptr)
{
	// leaf
	Node* temp;

	if ((*ptr)->left == NULL)
	{
		temp = *ptr;
		*ptr = (*ptr)->right;
		free(temp);
	}
	else if ((*ptr)->right == NULL)
	{
		temp = *ptr;
		*ptr = (*ptr)->left;
		free(temp);
	}
	else
	{
		temp = *ptr;
		Node* search;
		for (search = (*ptr)->left; search->right != NULL; search = search->right)
			continue;

		search->right = (*ptr)->right;
		(*ptr) = (*ptr)->left;
		free(temp);
	}
}

Item* TreeSearch(Tree* tree, const Item key)
{
	return &(SeekItem(&key, tree).child->item);
}


/*****************************************************************************
*                                                                            *
* @ NAME      : DeleteAll													 *
* @ FUNCTION  : 해당 Tree 모든 Node 제거										 *
*				1. DeleteAllNodes호출 모든 Node제거							 *
*				2. Tree의 Item개수 0											 *
*				3. Tree의 root는 NULL										 *
* @ PARAMETER :	Tree 포인터													 *
* @ RETURN	  : 없음							 								 *
*																			 *
*****************************************************************************/

void DeleteAll(Tree* ptree)
{
	if (ptree == NULL)
		return;

	DeleteAllNodes(ptree->root);
	ptree->root = NULL;
	ptree->size = 0;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : DeleteAllNodes												 *
* @ FUNCTION  : 해당 Tree 모든 Node 제거 (재귀함수)							 *
*				1. Left이동하기 전, right을 Node* pright에 저장				 *
*				2. Left로 이동->left가 NULL이면 return; NULL이 아니면			 *
*				   다음 Node의 Left로 이동									 *
*				3. left가 NULL로 return;->Left Node는 free 제거				 *						 *
*				4. (pright)right로 이동->right가 NULL이면 return;				 *
*				   NULL이 아니면 다음 Node의 Left로 이동						 *
*				5. 1,2,3,4 반복												 *
* @ PARAMETER :	Node 포인터													 *
* @ RETURN	  : 없음							 								 *
*																			 *
*****************************************************************************/

static void DeleteAllNodes(Node* root)
{
	if (root == NULL)
		return;

	Node* pright = root->right;
	DeleteAllNodes(root->left);
	free(root);
	DeleteAllNodes(pright);
}
