#pragma once

#include <vector>

class rbtree
{
public:
	struct rbnode
	{
		rbnode() : red(false), parent(nullptr), left(nullptr), right(nullptr), next(nullptr), key(0) {}

		bool red;
		rbnode* parent;
		rbnode* left;
		rbnode* right;
		rbnode* next; // �ظ�ֵ�ڵ㣬��������ʽ��¼
		int key;
	};
	using node = rbnode;

public:
	rbtree();
	~rbtree();

	// ��ӽڵ�
	void add(int key);
	// ɾ���ڵ�
	void del(int key);
	// ���ҽڵ�
	node* find(int key);

	// ��ȡ���ڵ�
	node* get();

	// ��ȡ���
	int depth();

protected:
	// ��ӽڵ�����
	void add_modify(node* parent, node* current);

	// ɾ���ڵ�����
	void del_modify(node* parent, node* current);

	// ˫�ڽڵ����
	void del_modify_double_black(node* parent, node* current);

	// ��ת
	void rotate(node* parent, node* current);

	// �ݹ��ȡ���
	int inner_depth(node* node);

	// ���ҽڵ�
	void inner_find(node** parent, node**& current, int key);

private:
	node* m_root;
};