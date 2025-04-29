#include "btree.h"

btree::btree(uint16 order/* = 5*/) : m_root(nullptr), m_order(order)
{

}

btree::~btree()
{

}

void btree::add(int key, int val)
{
	bindex parent;
	auto bidx = find(parent, key);
	
}

bindex btree::find(int key)
{
	bindex parent;
	return find(parent, key);
}

bindex btree::find(bindex& parent, int key)
{
	bindex curr;

	// 根节点的父
	parent.index = 0;
	parent.p = nullptr;

	auto pos = m_root;
	while (nullptr != pos)
	{
		// 二分查找，第一个大于等于key的位置
		auto index = bsearch(pos->pItems, pos->size, key);

		if (pos->pItems[index].key == key)
		{
			curr.index = index;
			curr.p = pos;
			pos = nullptr;
		}
		else
		{
			parent.index = index;
			parent.p = pos;

			if (nullptr != pos->kids)
			{
				pos = pos->kids[index];
			}
			else
			{
				pos = nullptr;
			}
		}
	}

	return curr;
}

int btree::bsearch(bnode::data_t* vv, int size, int key)
{
	int l(-1), r(size);
	while (l < r)
	{
		auto idx = (l + r) / 2;
		auto& ikey = vv[idx].key;
		if (ikey == key)
		{
			l = r = idx;
		}
		else if (ikey < key)
		{
			l = (l != idx ? idx : r);
		}
		else
		{
			r != idx ? r = idx : l = r;
		}
	}
	return r;
}