#include "rbtreeview.h"

#include <deque>
#include <set>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QBoxLayout>

#define L8B(x) QString::fromLocal8Bit(x)

rbtreeview::rbtreeview(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
{
	setFixedSize(1000, 800);

	QVBoxLayout* pVLayout = new QVBoxLayout(this);
	pVLayout->addSpacerItem(new QSpacerItem(20, 500, QSizePolicy::Preferred, QSizePolicy::Expanding));

	QHBoxLayout* pLayout = new QHBoxLayout(this);
	//pLayout->setGeometry(QRect(QPoint(0,0), QPoint(1000,30)));
	pVLayout->addLayout(pLayout);
	pLayout->addSpacerItem(new QSpacerItem(500, 20, QSizePolicy::Expanding));

	m_pSbxVal = new QSpinBox(this);
	pLayout->addWidget(m_pSbxVal);

	m_pBtnAdd = new QPushButton(L8B("添加"), this);
	pLayout->addWidget(m_pBtnAdd);
	
	m_pBtnDel = new QPushButton(L8B("删除"), this);
	pLayout->addWidget(m_pBtnDel);

	m_pBtnFind = new QPushButton(L8B("查找"), this);
	pLayout->addWidget(m_pBtnFind);

	m_rbTree.addNode(5);
	m_rbTree.addNode(3);
	m_rbTree.addNode(2);
 	m_rbTree.addNode(8);
// 	m_rbTree.addNode(7);
// 	m_rbTree.addNode(6);
// 	m_rbTree.addNode(9);
// 	m_rbTree.addNode(0);
// 	m_rbTree.addNode(4);
// 	m_rbTree.addNode(1);

	connect(m_pBtnAdd, SIGNAL(clicked()), SLOT(onAdd()));
	connect(m_pBtnDel, SIGNAL(clicked()), SLOT(onDel()));
	connect(m_pBtnFind, SIGNAL(clicked()), SLOT(onFind()));

	m_pFindNode = nullptr;
}

void rbtreeview::onAdd()
{
	m_rbTree.addNode(m_pSbxVal->value());
	update();
}

void rbtreeview::onDel()
{
	m_rbTree.delNode(m_pSbxVal->value());
	update();
}

void rbtreeview::onFind()
{
	m_pFindNode = m_rbTree.findNode(m_pSbxVal->value());
	update();
}

void rbtreeview::paintEvent(QPaintEvent* event)
{
 	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	auto font = painter.font();
	font.setPointSize(15);
	painter.setFont(font);
 
	auto root = m_rbTree.getRoot();
	if (nullptr == root)
	{
		return;
	}

	std::map<rbnode*, int> node2Order;
	int order = 0;
	{
		std::deque<rbnode*> vv;
		if (nullptr != root)
		{
			vv.push_back(root);
		}
		bool bReachRight = true;
		while (!vv.empty())
		{
			auto curr = vv.front();
			if (bReachRight)
			{
				while (nullptr != curr->left)
				{
					vv.push_front(curr->left);
					curr = curr->left;
				}
				bReachRight = false;
			}
			else
			{
				node2Order[curr] = order++;
				vv.pop_front();
				if (nullptr != curr->right)
				{
					vv.push_front(curr->right);
					bReachRight = true;
				}
			}
		}
	}

	// 待绘制的圆形
	std::map<rbnode*, QRect> node2Cen;

	int r = 30;
	int h = 45;
	int row = 0;
	int col = 0;
	std::deque<rbnode*> nodes;
	std::deque<rbnode*> nextNodes;
	int hbeg = 50;
	int wbeg = (width() - r * (order + 1)) / 2;
	nodes.push_back(m_rbTree.getRoot());
	while (!nodes.empty() || !nextNodes.empty())
	{
		if (nodes.empty())
		{
			auto nullNodes = std::count(nextNodes.begin(), nextNodes.end(), nullptr);
			if (nullNodes == nextNodes.size())
			{
				nextNodes.clear();
				break;
			}
			nodes.swap(nextNodes);
			row++;
			col = 0;
		}

		auto node = nodes.front();
		nodes.pop_front();
		if (nullptr == node)
		{
			nextNodes.push_back(nullptr);
			nextNodes.push_back(nullptr);
			continue;
		}

		nextNodes.push_back(node->left);
		nextNodes.push_back(node->right);

		auto cen = wbeg + r/2 + node2Order[node] * r;
		auto hig = row * h + hbeg;

		if (nullptr != node->parent)
		{
			auto pcen = wbeg + r/2 + node2Order[node->parent] * r;
			painter.drawLine(QPoint(cen, hig), QPoint(pcen, hig - h));
		}

		node2Cen[node] = QRect(QPoint(cen - r / 2, hig - r / 2), QPoint(cen + r / 2, hig + r / 2));
		col++;
	}

	// 盖住线
	for (auto& r : node2Cen)
	{
		font = painter.font();
		font.setPointSize(15);
		painter.setFont(font);

		painter.setBrush(QBrush(r.first->red ? Qt::red : Qt::lightGray, Qt::SolidPattern));
		painter.drawEllipse(r.second);
		painter.drawText(r.second, Qt::AlignCenter, QString::number(r.first->key));

		// 左侧标记查找节点
		if (m_pFindNode == r.first)
		{
			QRect rect = r.second;
			auto whalf = rect.width() / 2;
			auto hhalf = rect.height() / 2;
			rect.adjust(whalf - 2, hhalf - 2, -whalf + 2, -whalf + 2);
			rect.moveLeft(r.second.left() - 4);

			painter.setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
			painter.drawRoundedRect(rect, 0.1, 0.1, Qt::RelativeSize);
		}

		// 右侧标记个数
		font = painter.font();
		font.setPointSize(8);
		painter.setFont(font);
		int count = 1;
		auto next = r.first->next;
		while (next)
		{
			++count;
			next = next->next;
		}

		int fz = 8;
		QRect rect = r.second;
		auto whalf = rect.width() / 2;
		auto hhalf = rect.height() / 2;
		rect.adjust(whalf - fz, hhalf - fz, -whalf + fz, -whalf + fz);
		rect.moveRight(r.second.right() + 2 * fz);
		painter.drawText(rect, Qt::AlignCenter, QString::number(count));
	}
}