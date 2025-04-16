#include "rbtree.h"

#include <cassert>

rbtree::rbtree() : m_root(nullptr)
{

}

rbtree::~rbtree()
{
	// destroy TODO
}

void rbtree::add(int key)
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
		(*current)->red = true;

		// 维持平衡，进行调整
		add_modify(parent, *current);
	}
}

void rbtree::del(int key)
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
		auto left_max_node = cur->left;
		while (nullptr != left_max_node->right)
		{
			left_max_node = left_max_node->right;
		}

		std::swap(cur->key, left_max_node->key);
		cur = left_max_node;
		parent = cur->parent;
	}

	del_modify(parent, cur);
}

rbtree::node* rbtree::find(int key)
{
	node** current = nullptr;
	node* parent = nullptr;
	inner_find(&parent, current, key);
	return *current;
}

rbtree::node* rbtree::get()
{
	return m_root;
}

int rbtree::depth()
{
	return inner_depth(m_root);
}

void rbtree::add_modify(node* parent, node* current)
{
	//// 分情况讨论
	//1.根节点，不需要调整
	if (nullptr == parent)
	{
		current->red = false;
		return;
	}

	//2.父节点是黑色节点，不需要调整
	if (!parent->red)
	{
		return;
	}

	//3.父节点红色，叔叔节点黑色/空
	node* uncle = parent->parent->left == parent ? parent->parent->right : parent->parent->left;
	if (nullptr == uncle || !uncle->red)
	{
		//3.1.左左型、右右型
		if ((current == parent->left && parent->parent->left == parent) ||
			(current == parent->right && parent->parent->right == parent))
		{
			rotate(parent->parent, parent);
			parent->red = false;
			parent->left->red = true;
			parent->right->red = true;
		}
		//3.2 左右型、右左型
		else
		{
			rotate(parent, current);
			add_modify(current, parent);	// -> 3.1
		}
		return;
	}

	//4.父节点红色，叔叔节点红色
	parent->parent->red = true;
	parent->red = false;
	uncle->red = false;
	add_modify(parent->parent->parent, parent->parent);	// 递归向上调整
}

void rbtree::del_modify(node* parent, node* current)
{
	//1. 删除的是根节点
	if (current == m_root)
	{
		m_root = (nullptr != current->left ? current->left : current->right);
		if (nullptr != m_root)
		{
			m_root->parent = nullptr;
			m_root->red = false;
		}

		delete current;
		return;
	}

	//2. 删除的是红色节点
	if (current->red)
	{
		auto pcur = (current == parent->left ? &parent->left : &parent->right);
		*pcur = (nullptr != current->left ? current->left : current->right);
		if (*pcur)
		{
			(*pcur)->parent = parent;
		}
		
		delete current;
		return;
	}

	//3. 删除的是黑色节点
	//3.1 删除的节点有左孩子或者右孩子，则必为红色节点（否则不满足路径上黑色节点个数一致的性质）
	auto pcur = (current == parent->left ? &parent->left : &parent->right);
	if (nullptr != current->left || nullptr != current->right)
	{
		*pcur = (nullptr != current->left ? current->left : current->right);
		(*pcur)->parent = parent;
		(*pcur)->red = false;

		delete current;
		return;
	}

	//3.2 删除的节点没有子节点
	//3.2.1 父节点为红色
	auto pother = (current == parent->left ? &parent->right : &parent->left);
	auto other = *pother;
	if (parent->red)
	{
		if ((nullptr != other->left && nullptr != other->right) ||
			(parent->left == other && nullptr != other->left) ||
			(parent->right == other && nullptr != other->right))
		{
			*pcur = nullptr;
			delete current;
			rotate(parent, *pother);
			other->red = true;
			other->left->red = other->right->red = false;
			return;
		}
		else if (nullptr != other->left)
		{
			other->red = true;
			other->left->red = false;
			rotate(other, other->left);
			del_modify(parent, current);
			return;
		}
		else if (nullptr != other->right)
		{
			other->red = true;
			other->right->red = false;
			rotate(other, other->right);
			del_modify(parent, current);
			return;
		}
		else
		{
			*pcur = nullptr;
			delete current;
			rotate(parent, *pother);
			return;
		}
	}
	
	//3.2.2 父节点为黑色
	//3.2.2.1 兄弟节点为红色
	if (other->red)
	{
		// other必定有两个黑色子节点
		other->red = false;
		other == parent->left ? other->right->red = true : other->left->red = true;
		*pcur = nullptr;
		delete current;
		rotate(parent, other);
		return;
	}

	//3.2.2.2 兄弟节点为黑色
	if ((nullptr != other->left && nullptr != other->right) ||
		(parent->left == other && nullptr != other->left) ||
		(parent->right == other && nullptr != other->right))
	{
		*pcur = nullptr;
		delete current;
		other == parent->left ? other->left->red = false : other->right->red = false;
		rotate(parent, *pother);
		return;
	}

	// 需要先旋转一次
	if (nullptr != other->left)
	{
		*pcur = nullptr;
		delete current;
		other->left->red = false;
		rotate(other, other->left);
		rotate(parent, *pother);
		return;
	}

	if (nullptr != other->right)
	{
		*pcur = nullptr;
		delete current;
		other->right->red = false;
		rotate(other, other->right);
		rotate(parent, *pother);
		return;
	}
	
	// 待删节点黑无子节点 父节点黑 兄弟节点黑无子节点
	*pcur = nullptr;
	delete current;

	// 双黑节点处理
	del_modify_double_black(parent, *pcur);
}

void rbtree::del_modify_double_black(node* parent, node* current)
{
	// 0. 父节点为空，调整结束
	if (nullptr == parent)
	{
		return;
	}

	// 1. 刚被删除的节点，将兄弟节点染红
	if (nullptr == current)
	{
		current == parent->left ? parent->right->red = true : parent->left->red = true;
		del_modify_double_black(parent->parent, parent);
		return;
	}

	// 2. 父节点红色，旋转后恢复性质
	if (parent->red)
	{
		auto other = parent->left == current ? parent->right : parent->left;
		rotate(parent, other);
		return;
	}

	// 3. 父节点黑色，兄弟节点红色，旋转后变色，恢复性质
	auto pbrother = parent->left == current ? &parent->right : &parent->left;
	if ((*pbrother)->red)
	{
		(*pbrother)->red = false;
		parent->red = true;
		rotate(parent, *pbrother);
		return;
	}

	// 4. 父节点黑色，兄弟节点黑色，兄弟子节点红色
	auto brother = *pbrother;
	if (brother->left->red || brother->right->red)
	{
		auto other = brother->left->red ? brother->left : brother->right;
		// 4a
		if ((brother->left->red && parent->left == brother) ||
			(brother->right->red && parent->right == brother))
		{
			rotate(parent, brother);
			other->red = false;
			return;
		}

		// 4b
		if ((brother->left->red && parent->right == brother) ||
			(brother->right->red && parent->left == brother))
		{
			rotate(brother, other);
			brother->red = true;
			other->red = false;
		}
		del_modify_double_black(parent, current);	// -> 4a
		return;
	}

	// 5. 父节点黑 兄弟节点黑 兄弟节点子节点都是黑
	// 把兄弟节点染成红色，递归向上调整
	brother->red = true;
	del_modify_double_black(parent->parent, parent);
}

void rbtree::rotate(node* parent, node* current)
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

int rbtree::inner_depth(node* node)
{
	if (nullptr == node)
	{
		return 0;
	}

	int l = inner_depth(node->left);
	int r = inner_depth(node->right);
	
	return l >= r ? l + 1 : r + 1;
}

void rbtree::inner_find(node** parent, node**& current, int key)
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
