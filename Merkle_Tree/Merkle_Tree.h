#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "../Binary_Tree/Binary_Tree.h"

class Merkle_Tree : public Binary_Tree
{
	private:
		unsigned long long int hash_generator(char *str);
	public:
		Merkle_Tree();
		Merkle_Tree(int);
		unsigned long long int get_master_root() const;
		unsigned long long int* create_merkle_tree(char**, int);
		void generate_master_hash(struct Node*);
		virtual ~Merkle_Tree();
};

#endif