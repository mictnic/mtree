#pragma once

#include <vector>

struct rbnode
{
	rbnode() : red(false), parent(nullptr), left(nullptr), right(nullptr), next(nullptr), key(0) {}

	bool red;
	rbnode* parent;
	rbnode* left;
	rbnode* right;
	rbnode* next;		// 重复值节点，以链表形式记录
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

	// 添加节点
	void addNode(int key);
	// 删除节点
	void delNode(int key);
	// 查找节点
	rbnode* findNode(int key);

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

	// 查找节点
	void innerfindNode(rbnode** parent, rbnode**& current, int key);

private:
	rbnode* m_root;
};