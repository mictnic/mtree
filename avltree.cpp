#include "avltree.h"

#include <cassert>
#include <vector>

avltree::avltree() : m_root(nullptr)
{

}

avltree::~avltree()
{
	// destroy TODO
}

void avltree::add(int key)
{
	// 查找节点
	node** current = nullptr;
	node* parent = nullptr;
	inner_find(&parent, current, key);
	if (nullptr != (*current))	// 已有关键字
	{
		auto next = &(*current)->next;
		while (*next) next = &(*next)->next;
		*next = new node();
		(*next)->key = key;
	}
	else // 未找到关键字
	{
		*current = new node();
		(*current)->key = key;
		(*current)->parent = parent;
		(*current)->depth = 1;

		// 维持平衡，进行调整
		balance(parent);
	}
}

void avltree::del(int key)
{
	// 找到节点
	node** current = nullptr;
	node* parent = nullptr;
	inner_find(&parent, current, key);

	auto cur = *current;
	if (nullptr == cur)
	{
		return;
	}

	auto next = cur->next;
	if (nullptr != next)
	{
		cur->next = next->next;
		delete next;
		return;
	}

	// 真实需要删除的节点
	if (nullptr != cur->left && nullptr != cur->right)
	{
		// 找到左子树中最大节点
		current = &cur->left;
		while (nullptr != (*current)->right)
		{
			current = &(*current)->right;
		}

		std::swap(cur->key, (*current)->key);
		cur = *current;
		parent = cur->parent;
	}

	(*current) = cur->left ? cur->left : cur->right;
	if (*current)
	{
		(*current)->parent = parent;
	}
	delete cur;

	balance(parent);
}

avltree::node* avltree::find(int key)
{
	node** current = nullptr;
	node* parent = nullptr;
	inner_find(&parent, current, key);
	return *current;
}

avltree::node* avltree::get()
{
	return m_root;
}

int avltree::depth()
{
	return get_depth(m_root);
}

void avltree::balance(node* parent)
{
	if (nullptr == parent)
	{
		return;
	}

	// 记录原子节点高度
	auto depth = parent->depth;
	auto depth_diff = get_depth_diff(parent->left, parent->right);
	if (depth_diff > 1)	// 左高右低
	{
		auto child_depth_diff = get_depth_diff(parent->left->left, parent->left->right);
		if (child_depth_diff >= 0) // 左高右低 a1
		{
			rotate(parent, parent->left);
			parent = parent->parent;
			calc_depth(parent->left);
			calc_depth(parent->right);
			calc_depth(parent);
		}
		else // 右高左低 a2
		{
			rotate(parent->left, parent->left->right);
			calc_depth(parent->left->left);
			calc_depth(parent->left);
			balance(parent); // -> a1
			return;
		}
	}
	else if (depth_diff < -1)	// 右高左低
	{
		auto child_depth_diff = get_depth_diff(parent->right->left, parent->right->right);
		if (child_depth_diff <= 0) // 右高左低 b1
		{
			rotate(parent, parent->right);
			parent = parent->parent;
			calc_depth(parent->left);
			calc_depth(parent->right);
			calc_depth(parent);
		}
		else // 左高右低 b2
		{
			rotate(parent->right, parent->right->left);
			calc_depth(parent->right->right);
			calc_depth(parent->right);
			balance(parent); // -> a1
			return;
		}
	}
	else
	{
		calc_depth(parent);
	}
	
	// 高度没变化，递归结束
	if (depth == parent->depth)
	{
		return;
	}

	// 递归向上调整
	balance(parent->parent);
}

void avltree::rotate(node* parent, node* current)
{
	if (current == parent->left)
	{
		parent->left = current->right;
		if (nullptr != current->right)
		{
			current->right->parent = parent;
		}
		current->parent = parent->parent;
		current->right = parent;

		if (nullptr == parent->parent)
		{
			m_root = current;
		}
		else if (parent->parent->left == parent)
		{
			parent->parent->left = current;
		}
		else
		{
			parent->parent->right = current;
		}
		parent->parent = current;
	}
	else if (current == parent->right)
	{
		parent->right = current->left;
		if (nullptr != current->left)
		{
			current->left->parent = parent;
		}
		current->parent = parent->parent;
		current->left = parent;
		if (nullptr == parent->parent)
		{
			m_root = current;
		}
		else if (parent->parent->left == parent)
		{
			parent->parent->left = current;
		}
		else
		{
			parent->parent->right = current;
		}
		parent->parent = current;
	}
}

void avltree::inner_find(node** parent, node**& current, int key)
{
	current = &m_root;
	*parent = nullptr;
	while (*current && (*current)->key != key)
	{
		if (key < (*current)->key)
		{
			*parent = *current;
			current = &(*current)->left;
		}
		else if (key > (*current)->key)
		{
			*parent = *current;
			current = &(*current)->right;
		}
		else
		{
			assert(false);
		}
	}
}

void avltree::calc_depth(node* nd)
{
	if (nullptr == nd)
	{
		//
	}
	else if (nd->left && nd->right)
	{
		nd->depth = nd->left->depth > nd->right->depth ? nd->left->depth + 1 : nd->right->depth + 1;
	}
	else if (nd->left)
	{
		nd->depth = nd->left->depth + 1;
	}
	else if (nd->right)
	{
		nd->depth = nd->right->depth + 1;
	}
	else
	{
		nd->depth = 1;
	}
}

int avltree::get_depth(node* nd)
{
	return nd ? nd->depth : 0;
}

int avltree::get_depth_diff(node* l, node* r)
{
	return get_depth(l) - get_depth(r);
}

