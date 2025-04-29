#ifndef __BTREE_H__
#define __BTREE_H__

#include <utility>
#include <cstdint>

// ��������
using uint16 = uint16_t;

// B-Tree node
struct bnode
{
	struct data_t 
	{
		data_t() : key(0), val(0) {}

		int key;
		int val;
	};

	bnode() : size(0), parent(nullptr), pItems(nullptr), kids(nullptr) {}

	// ������
	uint16 size;

	// ���ڵ�
	bnode* parent;

	// key - value
	data_t* pItems;

	// �ӽڵ�
	bnode** kids;
};

// B-Tree node index
struct bindex
{
	bindex() : index(-1), p(nullptr) {}

	// ����λ��
	uint16 index;

	// �ڵ�ָ��
	bnode* p;
};

// B-Tree
class btree
{
public:
	btree(uint16 order = 5);
	~btree();

	// ���
	void add(int key, int val);

	// ����
	bindex find(int key);

protected:
	// ���ҽڵ�
	bindex find(bindex& parent, int key);

	// 
	int bsearch(bnode::data_t* vv, int size, int key);

private:
	bnode* m_root;
	const uint16 m_order;
};

#endif // __BTREE_H__
