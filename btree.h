#ifndef __BTREE_H__
#define __BTREE_H__

#include <utility>
#include <cstdint>

// 定义类型
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

	// 键数量
	uint16 size;

	// 父节点
	bnode* parent;

	// key - value
	data_t* pItems;

	// 子节点
	bnode** kids;
};

// B-Tree node index
struct bindex
{
	bindex() : index(-1), p(nullptr) {}

	// 索引位置
	uint16 index;

	// 节点指针
	bnode* p;
};

// B-Tree
class btree
{
public:
	btree(uint16 order = 5);
	~btree();

	// 添加
	void add(int key, int val);

	// 查找
	bindex find(int key);

protected:
	// 查找节点
	bindex find(bindex& parent, int key);

	// 
	int bsearch(bnode::data_t* vv, int size, int key);

private:
	bnode* m_root;
	const uint16 m_order;
};

#endif // __BTREE_H__
