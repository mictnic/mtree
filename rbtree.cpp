#include "rbtree.h"

#include <deque>
#include <QApplication>
#include <QWidget>
#include <QEvent>

rbtree::rbtree() : m_root(nullptr)
{

}

rbtree::~rbtree()
{

}

void rbtree::addNode(int key)
{
	// 查找节点
	rbnode** current = nullptr;
	rbnode* parent = nullptr;
	innerfindNode(&parent, current, key);
	if (nullptr != (*current))	// 已有关键字
	{
		auto next = &(*current)->next;
		while (*next) next = &(*next)->next;
		*next = new rbnode();
		(*next)->key = key;
	}
	else // 未找到关键字
	{
		*current = new rbnode();
		(*current)->key = key;
		(*current)->parent = parent;
		(*current)->red = true;

		// 维持平衡，进行调整
		modify(parent, *current);
	}
}

void rbtree::delNode(int key)
{
	// 找到节点
	rbnode** current = nullptr;
	rbnode* parent = nullptr;
	innerfindNode(&parent, current, key);

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


}

rbnode* rbtree::findNode(int key)
{
	rbnode** current = nullptr;
	rbnode* parent = nullptr;
	innerfindNode(&parent, current, key);
	return *current;
}

rbnode* rbtree::getRoot()
{
	return m_root;
}

int rbtree::getDepth()
{
	return innerGetDepth(m_root);
}

void rbtree::modify(rbnode* parent, rbnode* current)
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
	rbnode* uncle = parent->parent->left == parent ? parent->parent->right : parent->parent->left;
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
			modify(current, parent);
		}
		return;
	}

	//4.父节点红色，叔叔节点红色
	parent->parent->red = true;
	parent->red = false;
	uncle->red = false;
	modify(parent->parent->parent, parent->parent);	// 递归向上调整
}

void rbtree::rotate(rbnode* parent, rbnode* current)
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

int rbtree::innerGetDepth(rbnode* node)
{
	if (nullptr == node)
	{
		return 0;
	}

	int l = innerGetDepth(node->left);
	int r = innerGetDepth(node->right);
	
	return l >= r ? l + 1 : r + 1;
}

void rbtree::innerfindNode(rbnode** parent, rbnode**& current, int key)
{
	current = &m_root;
	*parent = m_root;
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
