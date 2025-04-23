#ifndef __BTREE_H__
#define __BTREE_H__

#include <utility>
#include <cstdint>

// ��������
using UInt16 = uint16_t;

// B-Tree node
struct bnode
{
	bnode() : size(0), parent(nullptr), pItems(nullptr), pNodes(nullptr)
	{

	}

	// ������
	UInt16 size;

	// ���ڵ�
	bnode* parent;

	// ������
	int* pItems;

	// �ӽڵ�
	bnode* pNodes;
};

using NdPair = std::pair<UInt16, bnode*>;

// B-Tree
class btree
{
public:
	btree();
	~btree();

	// ���ҽڵ�
	NdPair find(NdPair& parent, int val);


private:
	bnode* m_root;
};

#endif // __BTREE_H__
