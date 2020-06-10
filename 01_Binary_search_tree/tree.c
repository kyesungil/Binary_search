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
* @ NAME      : InitializeTree											     *
* @ FUNCTION  : binary tree Inititalize									     *
* @ PARAMETER :	Tree ����ü ������, item.character ���ڿ� ���Լ�				 *
* @ RETURN	  : ����															 *
*																			 *
*****************************************************************************/

void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2))
{
	ptree->root = NULL;
	ptree->size = 0;
	compare_func = comp_func;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeIsEmpty												     *
* @ FUNCTION  : Tree�� ��尡 0���� Ȯ��										 *
				1. Tree�� size�� 0���� Ȯ��									 *
* @ PARAMETER :	Tree ����ü ������											 *
* @ RETURN	  : bool ���� true												 *
*																			 *
*****************************************************************************/

bool TreeIsEmpty(const Tree* ptree)
{
	return ptree->size == 0;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeIsFull												     *
* @ FUNCTION  : Tree�� ��尡 Max���� Ȯ��									 *
				1. Tree�� size�� MAXITEMS���� Ȯ��							 *
* @ PARAMETER :	Tree ����ü ������											 *
* @ RETURN	  : bool ���� true												 *
*																			 *
*****************************************************************************/

bool TreeIsFull(const Tree* ptree)
{
	return ptree->size == MAXITEMS;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : TreeItemCount												 *
* @ FUNCTION  : Tree�� ��尡 size Ȯ��										 *
* @ PARAMETER :	Tree ����ü ������											 *
* @ RETURN	  : (int) Tree�� size 											 *
*																			 *
*****************************************************************************/

int TreeItemCount(const Tree* ptree)
{
	return ptree->size;
}


/*****************************************************************************
*                                                                            *
* @ NAME      : AddItem														 *
* @ FUNCTION  : Tree�� ������ �߰��ϱ�										 *
* @ PARAMETER :	�߰��� Item������, Tree ����ü ������							 *
* @ RETURN	  : (bool) ���� true; 											 *
*																			 *
*****************************************************************************/

bool AddItem(const Item* pi, Tree* ptree)
{
	// ���ο� ��� ����� (�����Ҵ�)
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL)
	{
		printf("Allocate_Error\n");
		exit(1);
	}
	new_node->item = *pi;
	new_node->left = NULL;
	new_node->right = NULL;

	if (ptree->root == NULL)		// Tree�� root�� ���� ��
		ptree->root = new_node;
	else
		AddNode(new_node, ptree->root); // Node �߰� �Լ� ȣ��
	
	return true;
}

/*****************************************************************************
*                                                                            *
* @ NAME      : AddNode														 *
* @ FUNCTION  : Tree�� Node �߰��ϱ�											 *
*				1. ���� �߰��� Node�� ��ġ ���ϱ�(���ĺ� ��)					 *
*				2. ���ĺ� ���� ������ left, ������ rigt�� �̵�					 *
*				3. �� Node(left or right)�� NULL�̸� ����, �ƴϸ� 2�� �ݺ�		 *
* @ PARAMETER :	�߰��� Node������, Tree�� Node ������							 *
* @ RETURN	  : ���� 														 *
*																			 *
*****************************************************************************/

static void AddNode(Node* new_node, Node* root)
{
	int comp = compare_func(root->item, new_node->item);

	/* �ߺ��� �������� �߰��� ���, �߰����� ���� */
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
* @ FUNCTION  : Tree�� Node, Item�� character���ڿ� ������������ ���			 *
* @ PARAMETER :	Tree ������, �Լ� ������										 *
* @ RETURN	  : ���� 														 *
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
* @ FUNCTION  : Tree�� Node, Item�� character���ڿ� ������������ ���			 *
*				1. left�� �̵� left�� NULL�̸� ��ȯ, �ƴϸ� ��� �̵�			 *
*				2. 1�� ��ȯ�� Item ���										 *
*				3. right�� �̵� right�� NULL�̸� ��ȯ, �ƴϸ� ��� �̵�		 *
*				4. 1,2,3 �ݺ� (����Լ�)										 *
* @ PARAMETER :	Node������, �Լ� ������										 *
* @ RETURN	  : ���� 														 *
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
* @ FUNCTION  : ���� ã�� item�� �ִ��� Ȯ��									 *
* @ PARAMETER :	Item ������, Tree ������										 *
* @ RETURN	  : (bool) ���� true, ���� false 								 *
*																			 *
*****************************************************************************/

bool InTree(const Item* pi, const Tree* ptree)
{
	return (SeekItem(pi, ptree).child == NULL) ? false : true;
}


/*****************************************************************************
*                                                                            *
* @ NAME      : SeekItem													 *
* @ FUNCTION  : ���� ã�� item�� �ִ��� Ȯ��									 *
*				1. ���� ã�� item�� tree�� ����� ���ڿ� ��				 *
*				2. ã�� item���ĺ� ���� ������ left�� �̵�, ������ right �̵�	 *
*				3. ��ġ�ϴ� item�� ������ ����ü Pair��ȯ						 *	
* @ PARAMETER :	Item ������, Tree ������										 *
* @ RETURN	  : ����ü Pair 													 *
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
* @ FUNCTION  : �ش� Item ����												 *
*				1. SeekItem�Լ� ȣ��. ������ Item���ִ� Node�� child�� �޾ƿ�	 *
*				2. Parent Node�� right or left or root���� �Դ��� üũ		 *
*				3. DeleteNode�Լ� ȣ��										 *
*				4. Tree Item���� -1											 *
* @ PARAMETER :	������ Item ������, Tree ������								 *
* @ RETURN	  : (bool) ���� true, ���� false 								 *
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
* @ FUNCTION  : �ش� Item Node ����											 *
*				1. ������ Node�� Child Node�� left, right�� NULL���� üũ		 *
*				2. ������ Node�� Parent Node�� Child Node ����				 *
*				3. free�Լ��� ����											 *
* @ PARAMETER :	������ Node�� Parent Node�� left or right (Node ����������)	 *
* @ RETURN	  : ����							 								 *
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
* @ FUNCTION  : �ش� Tree ��� Node ����										 *
*				1. DeleteAllNodesȣ�� ��� Node����							 *
*				2. Tree�� Item���� 0											 *
*				3. Tree�� root�� NULL										 *
* @ PARAMETER :	Tree ������													 *
* @ RETURN	  : ����							 								 *
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
* @ FUNCTION  : �ش� Tree ��� Node ���� (����Լ�)							 *
*				1. Left�̵��ϱ� ��, right�� Node* pright�� ����				 *
*				2. Left�� �̵�->left�� NULL�̸� return; NULL�� �ƴϸ�			 *
*				   ���� Node�� Left�� �̵�									 *
*				3. left�� NULL�� return;->Left Node�� free ����				 *						 *
*				4. (pright)right�� �̵�->right�� NULL�̸� return;				 *
*				   NULL�� �ƴϸ� ���� Node�� Left�� �̵�						 *
*				5. 1,2,3,4 �ݺ�												 *
* @ PARAMETER :	Node ������													 *
* @ RETURN	  : ����							 								 *
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