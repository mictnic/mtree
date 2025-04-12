#pragma once

#include <vector>

struct rbnode
{
	rbnode() : red(false), parent(nullptr), left(nullptr), right(nullptr), next(nullptr), key(0) {}

	bool red;
	rbnode* parent;
	rbnode* left;
	rbnode* right;
	rbnode* next;		// �ظ�ֵ�ڵ㣬��������ʽ��¼
	int key;

	// value
};

using rbnode_p = rbnode*;
using rbnode_p_seq = std::vector<rbnode_p>;

class rbtree
{
public:
	rbtree();
	~rbtree();

	// ��ӽڵ�
	void addNode(int key);
	// ɾ���ڵ�
	void delNode(int key);
	// ���ҽڵ�
	rbnode* findNode(int key);

	// ��ȡ���ڵ�
	rbnode* getRoot();

	// ��ȡ���
	int getDepth();

protected:
	// ����
	void modify(rbnode* parent, rbnode* current);

	// ��ת
	void rotate(rbnode* parent, rbnode* current);

	// �ݹ��ȡ���
	int innerGetDepth(rbnode* node);

	// ���ҽڵ�
	void innerfindNode(rbnode** parent, rbnode**& current, int key);

private:
	rbnode* m_root;
};