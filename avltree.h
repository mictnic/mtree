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
	// ά��ƽ��
	void balance(node* parent);

	// ��ת
	void rotate(node* parent, node* current);

	// ���ҽڵ�
	void inner_find(node** parent, node**& current, int key);

	// ����߶�
	void calc_depth(node* nd);

	// ��ȡ�߶�
	int get_depth(node* nd);

	// �߶Ȳ�
	int get_depth_diff(node* l, node* r);

private:
	avlnode *m_root;
};

#endif // __AVLTREE_H__
