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

	// 添加
	void addNode(int val);
	void delNode(int val);
	bool findNode(int val);

	// 获取根节点
	rbnode* getRoot();

	// 获取深度
	int getDepth();

protected:
	// 调整
	void modify(rbnode* parent, rbnode* current);

	// 旋转
	void rotate(rbnode* parent, rbnode* current);

	// 递归获取深度
	int innerGetDepth(rbnode* node);

private:
	rbnode* m_root;
};