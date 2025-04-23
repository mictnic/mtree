#ifndef __BTREE_H__
#define __BTREE_H__

#include <utility>
#include <cstdint>

// 定义类型
using UInt16 = uint16_t;

// B-Tree node
struct bnode
{
	bnode() : size(0), parent(nullptr), pItems(nullptr), pNodes(nullptr)
	{

	}

	// 键数量
	UInt16 size;

	// 父节点
	bnode* parent;

	// 数据项
	int* pItems;

	// 子节点
	bnode* pNodes;
};

using NdPair = std::pair<UInt16, bnode*>;

// B-Tree
class btree
{
public:
	btree();
	~btree();

	// 查找节点
	NdPair find(NdPair& parent, int val);


private:
	bnode* m_root;
};

#endif // __BTREE_H__
