#include "avltreeview.h"

#include <deque>
#include <set>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QLineEdit>
#include <QEventLoop>
#include <QCoreApplication>
#include <QThread>

#define L8B(x) QString::fromLocal8Bit(x)

avltreeview::avltreeview(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
{
	setFixedSize(1000, 800);

	QVBoxLayout* pVLayout = new QVBoxLayout(this);
	pVLayout->addSpacerItem(new QSpacerItem(20, 500, QSizePolicy::Preferred, QSizePolicy::Expanding));

	QHBoxLayout* pLayout = new QHBoxLayout(this);
	//pLayout->setGeometry(QRect(QPoint(0,0), QPoint(1000,30)));
	pVLayout->addLayout(pLayout);
	//pLayout->addSpacerItem(new QSpacerItem(500, 20, QSizePolicy::Expanding));

	m_pLetVal = new QLineEdit(this);
	m_pLetVal->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	pLayout->addWidget(m_pLetVal);

	m_pBtnAdd = new QPushButton(L8B("添加"), this);
	pLayout->addWidget(m_pBtnAdd);
	
	m_pBtnDel = new QPushButton(L8B("删除"), this);
	pLayout->addWidget(m_pBtnDel);

	m_pBtnFind = new QPushButton(L8B("查找"), this);
	pLayout->addWidget(m_pBtnFind);
// 	m_avl_tree.add(2);
// 	m_avl_tree.add(8);
// 	m_avl_tree.add(5);
// 	m_avl_tree.add(3);
//	m_avl_tree.add(4);
//  	
// 	m_avl_tree.add(1);
//  m_avl_tree.add(10);
//  m_avl_tree.add(12);
//  m_avl_tree.add(13);

	connect(m_pBtnAdd, SIGNAL(clicked()), SLOT(onAdd()));
	connect(m_pBtnDel, SIGNAL(clicked()), SLOT(onDel()));
	connect(m_pBtnFind, SIGNAL(clicked()), SLOT(onFind()));
}

void avltreeview::onAdd()
{
	this->setEnabled(false);
	auto v = getNumbers();
	for (auto& r : v)
	{
		m_avl_tree.add(r);
		update();
		qApp->processEvents(QEventLoop::AllEvents);
		QThread::msleep(10);
	}
	this->setEnabled(true);
}

void avltreeview::onDel()
{
	this->setEnabled(false);
	auto v = getNumbers();
	for (auto& r : v)
	{
		m_avl_tree.del(r);
		update();
		qApp->processEvents(QEventLoop::AllEvents);
		QThread::msleep(10);
	}
	this->setEnabled(true);
}

void avltreeview::onFind()
{
	m_nodes.clear();
	auto v = getNumbers();
	for (auto& r : v)
	{
		m_avl_tree.del(r);
		auto nd = m_avl_tree.find(r);
		if (nd)
		{
			m_nodes.insert(nd);
		}
	}
	update();
}

void avltreeview::paintEvent(QPaintEvent* event)
{
	m_node2Rect.clear();

 	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	auto font = painter.font();
	font.setPointSize(15);
	painter.setFont(font);
 
	auto root = m_avl_tree.get();
	if (nullptr == root)
	{
		return;
	}

	std::map<avltree::node*, int> node2Order;
	int order = 0;
	{
		std::deque<avltree::node*> vv;
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

	int r = 30;
	int h = 45;
	int row = 0;
	int col = 0;
	std::deque<avltree::node*> nodes;
	std::deque<avltree::node*> nextNodes;
	int hbeg = 50;
	int wbeg = (width() - r * (order + 1)) / 2;
	nodes.push_back(m_avl_tree.get());
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

		m_node2Rect[node] = QRect(QPoint(cen - r / 2, hig - r / 2), QPoint(cen + r / 2, hig + r / 2));
		col++;
	}

	// 盖住线
	for (auto& r : m_node2Rect)
	{
		font = painter.font();
		font.setPointSize(15);
		painter.setFont(font);

		painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
		painter.drawEllipse(r.second);
		painter.drawText(r.second, Qt::AlignCenter, QString::number(r.first->key));

		// 左侧标记查找节点
		if (m_nodes.find(r.first) != m_nodes.end())
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
		rect.moveTop(r.second.top());
		painter.drawText(rect, Qt::AlignCenter, QString::number(count));
		rect.moveBottom(r.second.bottom());
		painter.drawText(rect, Qt::AlignCenter, QString::number(r.first->depth));
	}
}

std::vector<int> avltreeview::getNumbers()
{
	std::vector<int> v;

	auto s = m_pLetVal->text();
	auto ss = s.split(",", QString::SkipEmptyParts);
	
	for (auto& r : ss)
	{
		bool bOk = false;
		auto val = r.toInt(&bOk);
		if (bOk)
		{
			v.push_back(val);
		}
	}

	return v;
}
