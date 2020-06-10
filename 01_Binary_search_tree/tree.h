#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define TMAX 20

typedef struct
{
	char character[TMAX];
	char name[TMAX];
} Item;

#define MAXITEMS 20

typedef struct node
{
	Item item;
	struct node* left;
	struct node* right;
} Node;

typedef struct tree
{
	Node* root;
	int size;
} Tree;


void InitializeTree(Tree* ptree, int(comp_func)(const Item item1, const Item item2));
bool TreeIsEmpty(const Tree* ptree);
bool TreeIsFull(const Tree* ptree);
int TreeItemCount(const Tree* ptree);
bool AddItem(const Item* pi, Tree* ptree);
bool InTree(const Item* pi, const Tree* ptree);
bool DeleteItem(const Item* pi, Tree* ptree);
void Traverse(const Tree* ptree, void (*pfun)(Item item));
void DeleteAll(Tree* ptree);
Item* TreeSearch(Tree* tree, const Item key);







