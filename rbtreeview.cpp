#include "rbtreeview.h"

#include <deque>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>

rbtreeview::rbtreeview(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
{
	setFixedSize(1000, 800);

	m_pBtn = new QPushButton(this);
	m_pBtn->setGeometry(QRect(QPoint(0,0), QPoint(50,20)));
	m_pSbx = new QSpinBox(this);
	m_pSbx->setGeometry(QRect(QPoint(50, 0), QPoint(100, 20)));

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

	connect(m_pBtn, SIGNAL(clicked()), SLOT(onAdd()));
}

void rbtreeview::onAdd()
{
	m_rbTree.addNode(m_pSbx->value());
	update();
}

void rbtreeview::paintEvent(QPaintEvent* event)
{
 	QPainter painter(this);
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

	// ´ý»æÖÆµÄÔ²ÐÎ
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


	for (auto& r : node2Cen)
	{
		painter.setBrush(QBrush(r.first->red ? Qt::red : Qt::lightGray, Qt::SolidPattern));
		painter.drawEllipse(r.second);
		painter.drawText(r.second, Qt::AlignCenter, QString::number(r.first->val));
	}
}