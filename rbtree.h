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
		rbnode* next; // 重复值节点，以链表形式记录
		int key;
	};
	using node = rbnode;

public:
	rbtree();
	~rbtree();

	// 添加节点
	void add(int key);
	// 删除节点
	void del(int key);
	// 查找节点
	node* find(int key);

	// 获取根节点
	node* get();

	// 获取深度
	int depth();

protected:
	// 添加节点后调整
	void add_modify(node* parent, node* current);

	// 删除节点后调整
	void del_modify(node* parent, node* current);

	// 双黑节点调整
	void del_modify_double_black(node* parent, node* current);

	// 旋转
	void rotate(node* parent, node* current);

	// 递归获取深度
	int inner_depth(node* node);

	// 查找节点
	void inner_find(node** parent, node**& current, int key);

private:
	node* m_root;
};