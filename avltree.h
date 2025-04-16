#ifndef __AVLTREE_H__
#define __AVLTREE_H__

class avltree
{
public:
	struct avlnode
	{
		avlnode() : depth(0), parent(nullptr), left(nullptr), right(nullptr), next(nullptr), key(0) {}

		int depth;
		avlnode* parent;
		avlnode* left;
		avlnode* right;
		avlnode* next;

		int key;
	};
	using node = avlnode;

public:
	avltree();
	~avltree();

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
	// 维持平衡
	void balance(node* parent);

	// 旋转
	void rotate(node* parent, node* current);

	// 查找节点
	void inner_find(node** parent, node**& current, int key);

	// 计算高度
	void calc_depth(node* nd);

	// 获取高度
	int get_depth(node* nd);

	// 高度差
	int get_depth_diff(node* l, node* r);

private:
	avlnode *m_root;
};

#endif // __AVLTREE_H__
