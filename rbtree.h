#pragma once

struct rbnode
{
	rbnode() : red(false), parent(nullptr), left(nullptr), right(nullptr), val(0) {}

	bool red;
	rbnode* parent;
	rbnode* left;
	rbnode* right;
	int val;
};

class rbtree
{
public:
	rbtree();
	~rbtree();

	// ���
	void addNode(int val);
	void delNode(int val);
	bool findNode(int val);

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

private:
	rbnode* m_root;
};