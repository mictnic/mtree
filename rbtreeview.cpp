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
// 	m_rbTree.addNode(8);
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
 
// 	painter.drawLine(QPoint(0,0), QPoint(100,100));
// 
// 	painter.setBackground(QBrush(Qt::red));
// 	painter.setBrush(QBrush(Qt::red));
// 	painter.drawEllipse(QRect(QPoint(100, 100), QPoint(120, 120)));

	// 获取最大深度
	auto depth = m_rbTree.getDepth();
	if (depth == 0)
	{
		return;
	}

	std::map<rbnode*, std::pair<int, int> > node2Fid;

	int row = 0;
	int col = 0;
	std::deque<rbnode*> nodes;
	std::deque<rbnode*> nextNodes;
	int hbeg = 50;
	int woff = width() / 2 - (pow(2, depth) - 1) * 20 / 2;
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

		int nFid = 0;
		int pcen = 0;
		if (nullptr == node->parent)
		{
			nFid = pow(2, depth - 1) / 2;
		}
		else
		{
			auto ite = node2Fid.find(node->parent);
			assert(ite != node2Fid.end());
			auto nPFid = ite->second.first;
			nFid = nPFid + (node == node->parent->left ? -1 : 1) * ((nPFid + 1) / 2);
			pcen = ite->second.second;
		}

		nextNodes.push_back(node->left);
		nextNodes.push_back(node->right);

		auto cen = woff + 10 + nFid * 20;
		auto hig = row * 40 + hbeg;

		node2Fid[node] = std::make_pair(nFid, cen);

		painter.setBrush(QBrush(node->red ? Qt::red : Qt::lightGray));
		painter.drawEllipse(QRect(QPoint(cen - 10, hig - 10), QPoint(cen + 10, hig + 10)));
		painter.drawText(QRect(QPoint(cen - 10, hig - 10), QPoint(cen + 10, hig + 10)), Qt::AlignCenter, QString::number(node->val));
		if (nullptr != node->parent)
		{
			painter.drawLine(QPoint(cen, hig), QPoint(pcen, hig - 40));
		}
		col++;
	}
}