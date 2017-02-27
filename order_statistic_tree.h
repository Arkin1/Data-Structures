#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include<iostream>
using namespace std;

template<typename T>
class order_statistic_tree
{
public:

	struct Node
	{
		Node *l, *r;
		int key;
		T val;
		int height_l, height_r;
		int sz_l, sz_r;
		int lz;


		Node(Node *l, Node*r, int key, T val)
		{
			this->l = l;
			this->r = r;
			this->key = key;
			this->val = val;
			this->height_l = 0;
			this->height_r = 0;
			this->sz_l = this->sz_r = this->lz = 0;
		}

		Node()
		{
			l = r = NULL;
			key = 0;
			memset(&val, 0, sizeof(val));
			this->height_l = 0;
			this->height_r = 0;

			this->sz_l = this->sz_r = 0;
			this->lz = 0;
		}


	};

	order_statistic_tree()
	{
		root = NULL;
	}

	void insert(int key, T val)
	{
		insert(key, val, root);
	}

	void remove(int key)
	{
		remove(key, root);
	}

	bool find(int key)
	{
		return find(key, root);
	}

	int count(int key)
	{
		return count(key, root);
	}

	Node* find_kth_element(int k)
	{
		return find_kth_element(k, root);
	}

	void update_greater_k(int k, int val)
	{
		update_greater_k(k, val, root);
	}

	Node* find_small_close_k(int k)
	{
		return find_small_close_k(k, root);
	}

	void print()
	{
		print(root);
	}

	int size()
	{
		if (root)
			return root->sz_l + root->sz_r + 1;
		else
			return 0;
	}

private:
	Node *root;

	void update_lazy(Node *&node)
	{
		node->key += node->lz;

		if (node->l != NULL)
			node->l->lz += node->lz;

		if (node->r != NULL)
			node->r->lz += node->lz;

		node->lz = 0;

	}

	void rotate_left(Node *&node)
	{
		update_lazy(node);
		
		Node *temp = node->r;
		node->height_r = node->r->height_l;
		node->sz_r = node->r->sz_l;

		update_lazy(node->r);

		node->r = node->r->l;

		temp->height_l = max(node->height_r, node->height_l) + 1;
		temp->sz_l = node->sz_l + node->sz_r + 1;

		temp->l = node;
		node = temp;
	}

	void rotate_right(Node *&node)
	{
		update_lazy(node);
		
		Node *temp = node->l;
		node->height_l = node->l->height_r;
		node->sz_l = node->l->sz_r;

		update_lazy(node->l);

		node->l = node->l->r;

		temp->height_r = max(node->height_r, node->height_l) + 1;
		temp->sz_r = node->sz_r + node->sz_l + 1;

		temp->r = node;

		node = temp;
	}

	int count(int key, Node *&node)
	{
		if (node != NULL)
		{
			update_lazy(node);

			if (key < node->key)
				return count(key, node->l);
			else if (key > node->key)
			{
				return node->sz_l + count(key, node->r) + 1;
			}
			else
				return node->sz_l;
		}
		else
			return 0;
	}

	void insert(int key, T val, Node *&node)
	{
		if (node == NULL)
		{
			node = new Node(NULL, NULL, key, val);
		}
		else
		{
			update_lazy(node);

			if (key < node->key)
			{
				insert(key, val, node->l);

				node->height_l = max(node->l->height_l, node->l->height_r) + 1;
				node->sz_l = node->l->sz_l + node->l->sz_r + 1;

			}
			else
			{
				insert(key, val, node->r);

				node->height_r = max(node->r->height_l, node->r->height_r) + 1;
				node->sz_r = node->r->sz_l + node->r->sz_r + 1;

			}

			if (node->height_l - node->height_r > 1)
			{
				if (node->l->height_l - node->l->height_r >= 0)
				{
					rotate_right(node);
				}
				else
				{
					rotate_left(node->l);
					rotate_right(node);
				}
			}
			else if (node->height_l - node->height_r < -1)
			{

				if (node->r->height_l - node->r->height_r <= 0)
				{
					rotate_left(node);
				}
				else
				{
					rotate_right(node->r);
					rotate_left(node);
				}
			}

		}
	}

	void remove(int key, Node *&node)
	{
		if (node)
		{
			update_lazy(node);

			if (key < node->key)
			{
				remove(key, node->l);

				if (node->l)
				{
					node->height_l = max(node->l->height_l, node->l->height_r) + 1;
					node->sz_l = node->l->sz_l + node->l->sz_r + 1;
				}
				else
				{
					node->height_l = 0;
					node->sz_l = 0;
				}

			}
			else if (key > node->key)
			{
				remove(key, node->r);

				if (node->r)
				{
					node->height_r = max(node->r->height_l, node->r->height_r) + 1;
					node->sz_r = node->r->sz_l + node->r->sz_r + 1;
				}
				else
				{
					node->height_r = 0;
					node->sz_r = 0;
				}
			}
			else if (key == node->key)
			{
				if (node->l == node->r && node->l == NULL)
				{
					delete node;
					node = NULL;
				}
				else if (node->l == NULL && node->r != NULL)
				{
					Node *p = node;
					node = node->r;

					delete p;
				}
				else if (node->l != NULL && node->r == NULL)
				{
					Node *p = node;
					node = node->l;

					delete p;
				}
				else
				{
					Node *tempn = find_kth_element(node->sz_l + 2, node);
					int key = tempn->key;

					node->key = key;
					node->val = tempn->val;

					remove(key, node->r);

					if (node->r)
					{
						node->height_r = max(node->r->height_l, node->r->height_r) + 1;
						node->sz_r = node->r->sz_l + node->r->sz_r + 1;
					}
					else
					{
						node->height_r = 0;
						node->sz_r = 0;
					}

				}
			}
			if (node)
			{
				if (node->height_l - node->height_r > 1)
				{
					if (node->l->height_l - node->l->height_r >= 0)
					{
						rotate_right(node);
					}
					else
					{
						rotate_left(node->l);
						rotate_right(node);
					}
				}
				else if (node->height_l - node->height_r < -1)
				{

					if (node->r->height_l - node->r->height_r <= 0)
					{
						rotate_left(node);
					}
					else
					{
						rotate_right(node->r);
						rotate_left(node);
					}
				}
			}
		}

	}

	bool find(int key, Node *&node)
	{
		if (node != NULL)
		{
			update_lazy(node);

			if (key < node->key)
				return find(key, node->l);
			else if (key > node->key)
				return find(key, node->r);
			else
				return true;
		}
		else
			return false;
	}

	Node* find_kth_element(int k, Node *&node)
	{
		if (node != NULL)
		{
			update_lazy(node);

			if (k < node->sz_l + 1)
				return find_kth_element(k, node->l);
			else if (k > node->sz_l + 1)
				return find_kth_element(k - (node->sz_l + 1), node->r);
			else
				return node;
		}
		else
			return NULL;
	}

	Node* find_small_close_k(int key, Node *&node)
	{
		if (node != NULL)
		{
			update_lazy(node);

			if (key < node->key)
				return find_small_close_k(key, node->l);
			else if (key > node->key)
			{
				Node* k = find_small_close_k(key, node->r);

				if (k == NULL)
					return node;
				else
					return k;

			}
			else
				return node;

		}
		else
			return NULL;
	}

	void update_greater_k(int key, int val, Node *&node)
	{
		if (node != NULL)
		{
			update_lazy(node);

			if (key < node->key)
			{
				node->key += val;
				if (node->r)
					node->r->lz += val;
				update_greater_k(key, val, node->l);
			}
			else if (key > node->key)
			{
				update_greater_k(key, val, node->r);
			}
			else
			{
				node->key += val;
				if (node->r)
					node->r->lz += val;
			}
		}
	}

	void print(Node *&node)
	{
		if (node)
		{
			update_lazy(node);

			print(node->l);
			cout << node->key << " ";
			print(node->r);
		}
	}


};

#endif
