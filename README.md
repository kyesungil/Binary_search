# Binary_search_Tree(BST)  
## 1.1 Tree 구성  
* __Root Node:__ 첫노드  
* __Subtree Node:__ Subree(root->left, root->right) Node들이 또다른 작은 tree을 만든다. 
작은 Tree 들이 연속적으로 있기 때문에 Subtree Node는 여러개가 될수 있다.  
* __Parent-Child Node:__ 예로 root Node는 Parent, root->left or root->right는 Child Node가 된다.  
* __Leaf Node:__ 가장 마지막 Node를 말한다. 이 Node는 left, right Node가 둘다 NULL이다.  
#### code>>  
```c  
typedef struct tree
{
	Node* root;
	int size; // data의 개수
} Tree;  
```  

## 1.2 Node 구성  
* 하나의 Node가 2개의 Node 포인터(left, right)를 갖고있다.(두 포인터는 다음에 추가로 연결되는 data을 저장하기 위한, Node의 주소를 저장)  
* Node의 key(or data)의 순서에 따라서 Left or Right에 연결한다. 이번 예시에서는 key의 값이 "문자열"이기 때문에 알파벳순이 
빠르면 left, 느리면 right로 연결된다.  
#### code>>  
```c  
#define TMAX 20
// Item data  
typedef struct
{
	char character[TMAX];
	char name[TMAX];
} Item;

// Node  
typedef struct node
{
	Item item;
	struct node* left;
	struct node* right;
} Node;  
```  
### 1)  
![Tree](https://user-images.githubusercontent.com/41607872/84231703-ad6f9000-ab29-11ea-9b8e-898cde342065.jpg) 
***  
## 2. Tree 인터페이스 함수  
### 2.1.1 Tree 초기화 함수: void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2))  
* __@ NAME      :__ InitializeTree    
* __@ PARAMETER :__	Tree 구조체 포인터, item.character 문자열 비교함수    
* __@ RETURN	  :__ 없음  	
```c  
void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2))
{
	ptree->root = NULL;
	ptree->size = 0;
	compare_func = comp_func;
}  
```  
***  
### 2.2.1  Tree에 data을 저장하는 함수  
* __@ NAME      :__ AddItem  														 								 
* __@ PARAMETER :__	추가할 Item포인터, Tree 구조체 포인터  							 
* __@ RETURN	  :__ (bool) 성공 true  
#### code>>  
```c 
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

	if (ptree->root == NULL)		    // Tree의 root가 없을 때
		ptree->root = new_node;
	else
		AddNode(new_node, ptree->root); // Node 추가 함수 호출
	
	return true;
}
```  
### 2.2.2 Tree Node추가 함수  
* __@ NAME      :__ AddNode  														 
* __@ FUNCTION  :__ Tree에 Node 추가하기  
                1. 새로 추가할 Node의 위치 정하기(알파벳 순)  
                2. 알파벳 순이 빠르면 left, 느리면 rigt로 이동  
                3. 그 Node(left or right)가 NULL이면 연결, 아니면 2번 반복		 
* __@ PARAMETER :__	추가할 Node포인터, Tree의 Node 포인터  							 
* __@ RETURN	  :__ 없음   
#### code>>  
```c 
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
```  
***  
### 2.3.1 Tree의 data(or key)의 순서에 따라 출력하는 함수(이 예시에서는 알파벳 순으로 한다)  
* __@ NAME      :__ Traverse  													 	 
* __@ PARAMETER :__	Tree 포인터, 함수 포인터  										 
* __@ RETURN	  :__ 없음  
```c  
void Traverse(const Tree* ptree, void (*pfun)(Item item))
{
	if (ptree != NULL)
		InOrder(ptree->root, pfun);
}  
```  
### 2.3.2 순회함수(재귀함수)
#### 중위 순회(Inoreder Traversal)  
* Left 확인 -> Left없으면 visit처리(이 예시는 출력처리) -> right -> left확인 -> left없다면 visit처리 -> leaf면 다시 돌아가기  
* __@ NAME      :__ AddNode  														 
* __@ FUNCTION  :__ Tree에 Node, Item의 character문자열 오름차순으로 출력			 
                1. left로 이동 left가 NULL이면 반환, 아니면 계속 이동  
                2. 1번 반환시 Item 출력  
                3. right로 이동 right가 NULL이면 반환, 아니면 계속 이동  
                4. 1,2,3 반복 (재귀함수)  										 
* __@ PARAMETER :__	Node포인터, 함수 포인터  										 
* __@ RETURN	  :__ 없음  
```c  
static void InOrder(const Node* root, void(*pfun)(Item item))
{
	if (root == NULL)
		return;

	InOrder(root->left, pfun);
	(*pfun)(root->item);
	InOrder(root->right, pfun);
}  
```  
***  
### 2.4.1 Tree의 data삭제 함수  
* __@ NAME      :__ DeleteItem													 
* __@ FUNCTION  :__ 해당 Item 제거												 
                1. SeekItem함수 호출. 삭제할 Item이있는 Node을 child로 받아옴  
                2. Parent Node가 right or left or root에서 왔는지 체크  
                3. DeleteNode함수 호출  
                4. Tree Item개수 -1											 
* __@ PARAMETER :__	제거할 Item 포인터, Tree 포인터    								 
* __@ RETURN	  :__ (bool) 성공 true, 실패 false  
```c 
bool DeleteItem(const Item* pi, Tree* ptree)
{
	Pair look = SeekItem(pi, ptree); // parent-child을 반환하는 함수
	
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
```  
### 2.4.2 Tree의 Node를 제거하는 함수  
* __@ NAME      :__ DeleteNode													 
* __@ FUNCTION  :__ 해당 Item Node 제거(Child Node의 left, right의 존재 여부에 따라서 각각의 경우을 처리해줘야함)    
                1. 삭제할 Node의 Child Node의 left, right가 NULL인지 체크  
                2. 삭제할 Node의 Parent Node와 Child Node 연결  
                3. free함수로 제거											 
* __@ PARAMETER :__	삭제할 Node의 Parent Node의 left or right (Node 이중포인터)  	 
* __@ RETURN	  :__ 없음  
```c  
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
```  

### Ex 1.1-1) Child_Node의 left, right가 둘다 NULL이 아닐 경우(※ child_node가 삭제할 node 입니다.)    
1. Parent_Node의 left or right 와 Child_Node의 left 연결  
2. Child_Node->left의 right 와 Child_Node의 right 연결 (Child_Node->left의 right != null 이라면, left이동후 다시 right확인후 연결)   
![b_node_child_2_1](https://user-images.githubusercontent.com/41607872/84230140-3d133f80-ab26-11ea-9078-37565a0ff072.jpg) 
### Ex 1.1-2)  
![b_node_child_2_2](https://user-images.githubusercontent.com/41607872/84230452-ece8ad00-ab26-11ea-8b61-114bc8ca9031.jpg)
***  
### Ex 1.2-1) ex 1.1의 경우 + Root_Node을 삭제할 경우(※ child_node가 삭제할 node 입니다.)  
![b_node_root_1](https://user-images.githubusercontent.com/41607872/84230327-aabf6b80-ab26-11ea-83e4-d715477abac4.jpg)
### Ex 1.2-2)  
![b_node_root_2](https://user-images.githubusercontent.com/41607872/84229335-469ba800-ab24-11ea-97fb-75c51c424d72.jpg)
***  
### Ex 1.3-1) Child_Node의 left, right가 하나만 NULL일 경우(※ child_node가 삭제할 node 입니다.)  
1. Parent_Node의 left or right 와 Child_Node의 left or right 연결  
![b_node_child_1_1](https://user-images.githubusercontent.com/41607872/84230026-fb829480-ab25-11ea-8042-4866b1d87dc3.jpg)  
### Ex 1.3-2)    
![b_node_child_1_2](https://user-images.githubusercontent.com/41607872/84230027-fcb3c180-ab25-11ea-9acf-c388f4705526.jpg)
***  
### Ex 1.4-1) Leaf_Node일 경우(child_node의 left, right 둘다 NULL) (※ child_node가 삭제할 node 입니다.)     
![leaf제거1jpg](https://user-images.githubusercontent.com/41607872/84230614-59fc4280-ab27-11ea-9af1-d457e562ac96.jpg)
### Ex 1.4-2)  
![leaf제거2jpg](https://user-images.githubusercontent.com/41607872/84230617-5b2d6f80-ab27-11ea-9168-8dfeccbb8675.jpg)
***  
### 2.5.1 Tree의 모든 data삭제 함수  
* __@ NAME      :__ DeleteAll													 
* __@ FUNCTION  :__ 해당 Tree 모든 Node 제거  
                1. DeleteAllNodes호출 모든 Node제거  
                2. Tree의 Item개수 0  
                3. Tree의 root는 NULL  										
* __@ PARAMETER__ :	Tree 포인터													 
* __@ RETURN__	  : 없음  	
```c  
void DeleteAll(Tree* ptree)
{
	if (ptree == NULL)
		return;

	DeleteAllNodes(ptree->root);
	ptree->root = NULL;
	ptree->size = 0;
}
```  
### 2.5.2 Tree의 모든 Node삭제 함수(재귀함수)  
* __@ NAME__      : DeleteAllNodes  												 
* __@ FUNCTION__  : 해당 Tree 모든 Node 제거     
                1. Left이동하기 전, right을 Node* pright에 저장  
                2. Left로 이동->left가 NULL이면 return; NULL이 아니면 다음 Node의 Left로 이동  
                3. left가 NULL로 return;->Left Node는 free 제거  
                4. (pright)right로 이동->right가 NULL이면 return; NULL이 아니면 다음 Node의 Left로 이동  
                5. 1,2,3,4 반복  												 
* __@ PARAMETER__ :	Node 포인터  													 
* __@ RETURN__	  : 없음  
```c  
static void DeleteAllNodes(Node* root)
{
	if (root == NULL)
		return;

	Node* pright = root->right;
	DeleteAllNodes(root->left);
	free(root);
	DeleteAllNodes(pright);
}
```  
***  
### 3. 함수들
```c
void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2));
bool TreeIsEmpty(const Tree* ptree);  // 비어있는지 확인
bool TreeIsFull(const Tree* ptree);   // 차있는지 확인    
int TreeItemCount(const Tree* ptree); // Item(data)개수 반환
bool AddItem(const Item* pi, Tree* ptree);
bool InTree(const Item* pi, const Tree* ptree); // 데이터 검색(찾는 데이터가 있는지 확인)
bool DeleteItem(const Item* pi, Tree* ptree);
void Traverse(const Tree* ptree, void (*pfun)(Item item));
void DeleteAll(Tree* ptree);
Item* TreeSearch(Tree* tree, const Item key); // 데이터 검색(찾는 데이터 반환)
```  
```c  
static void AddNode(Node* new_node, Node* root);
static void InOrder(const Node* root, void(*pfun)(Item item));
static void DeleteNode(Node** ptr);
static Pair SeekItem(const Item* pi, const Tree* ptree);  // 검색한 date에 해당하는 parent-child node 을 반환
static void DeleteAllNodes(Node* root);

static int(*compare_func)(const Item item1, const Item item2) = NULL; // data(key)비교 함수  
```  
***  
[인프런-홍정모의 따라하며 배우는 c언어 부록 참조]  
2020-06-10  
