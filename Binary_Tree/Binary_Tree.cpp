#include "Binary_Tree.h"
#include <iostream>
#include <queue>
#include <stdio.h>

using namespace std;

Binary_Tree::Binary_Tree()
{
	this->root = new_node(1);
	this->count = 1;
}

Binary_Tree::Binary_Tree(int value)
{
	this->root = new_node(value);
	this->count = 1;
	node_queue.push(root);
	current_root = node_queue.front();
}

struct Node* Binary_Tree::new_node(unsigned long long int item)
{
	struct Node *ptr = new struct Node;
	ptr->key = item;
	ptr->left_child = ptr->right_child = NULL;
	return ptr;
}

struct Node* Binary_Tree::get_root() const
{
	return this->root;
}

void Binary_Tree::insert(unsigned long long int key)
{
	insert_key(key);
}

void Binary_Tree::insert_key(unsigned long long int key)
{
	if(count%2)
	{
		current_root->left_child = new_node(key);
		node_queue.push(current_root->left_child);
	}
	else
	{
		current_root->right_child = new_node(key);
		node_queue.push(current_root->right_child);
		node_queue.pop();
		current_root = node_queue.front();
	}
	count++;
}

void Binary_Tree::level_order_traversal(struct Node* root) const
{
	queue <struct Node*> node_queue;
	struct Node *ptr = root;
	int node_count = 1;
	while(ptr)
	{
		cout << endl;
		cout << node_count++ << " " << ptr->key << " ";
		if (ptr->left_child != NULL)
		{
			node_queue.push(ptr->left_child);
		}
		if (ptr->right_child != NULL)
		{
			node_queue.push(ptr->right_child);
		}
		ptr = node_queue.front();
		node_queue.pop();
	}
}

Binary_Tree::~Binary_Tree()
{

}