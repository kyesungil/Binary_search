#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "tree.h"

#define SPACING 30

int compare(const Item first, const Item second)
{
	return strcmp(first.character, second.character);
}

void print_item(Item item)
{
	printf("%s (%s)\n", item.character, item.name);
}

void printf2DUtil(Node* root, int space)
{
	space += SPACING;

	if (root == NULL)
	{
		for (int i = SPACING; i < space; i++)
			printf(" ");
		printf("NULL");
		return;
	}

	printf2DUtil(root->right, space);

	printf("\n");
	for (int i = SPACING; i < space; i++)
		printf(" ");
	print_item(root->item);

	printf2DUtil(root->left, space);

}

void print_node(Node* root, int level)
{
	if (root == NULL)
		return;

	printf("%s (%s)->", root->item.character, root->item.name);

	if (root->left == NULL)
		printf("NULL, ");
	else
		printf("%s (%s), ", root->left->item.character, root->left->item.name);

	if (root->right == NULL)
		printf("NULL");
	else
		printf("%s (%s)", root->right->item.character, root->right->item.name);

	printf("\n");

	print_node(root->left, level + 1);
	print_node(root->right, level + 1);
}

void print_tree(Tree* ptree)
{
	printf("\n--------------------\n");

	Traverse(ptree, print_item);

	printf("\n--------------------\n");

	printf2DUtil(ptree->root, 0);

	printf("\n--------------------\n");

	print_node(ptree->root, 0);
}
int main()
{
	Item items[] = {
		{"Iron Man", "Tony Stark"},
		{"Thor","Thor Odinson"},
		{"Ant-Man","Hank Pyn"},
		{"Wasp","Janet van Dyne"},
		{"Hulk","Bruce Banner"},
		{"Captin America", "Steve Rogers"},
		{"Scarlet Witch", "Wanda Maximoff"},
		{"Black Widow", "Natasha Romanoff"},
		{"Dr. Strange", "Stephen Strange"},
		{"Daredevil","Matthew Murdock"},
		{"Punisher", "Frank Castle"},
		{"Batman", "Bruce Wayne"}
	};

	int n = sizeof(items) / sizeof(items[0]);

	Tree tree;
	InitializeTree(&tree, &compare);

	for (int i = 0; i < n; ++i)
		AddItem(&items[i], &tree);

	AddItem(&items[2], &tree);
	print_tree(&tree);


	Item key = { "Punisher","" };
	Item* result = TreeSearch(&tree, key);
	if (result == NULL)
		printf("\nSearch failed\n");
	else
		printf("\n%s is the %s\n", result->name, result->character);

	{
		Item key = { "Batman","" };
		DeleteItem(&key, &tree);
		print_tree(&tree);

	}

	DeleteAll(&tree);

	return 0;
}

/*
	Binary Search Tree

	- 하나의 Node가 2개의 포인터를 갖고있다. (Left, Right)
	- 앞쪽이면 Left 연결, 뒤쪽이면 Right 연결.
	Root
	Subtree
	Paren-Child
	Leaf

	1. 이지 탐색 트리가 만들어지는 과정
	2. 탐색(Search)
	3. 순회(Traversal)
		- 전위 순회 (Preorder Traversal)
		- 후위 순회 (Postorder Traversal)
		-*중위 순회 (Inorder Traversal)
		Left 확인 -> Left없으면 visit처리 -> right -> left확인 -> left없다면 visit처리
		-> leaf면 다시 돌아가기
*/