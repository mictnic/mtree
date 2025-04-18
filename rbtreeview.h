#pragma once

#include <QWidget>
#include "rbtree.h"

class QPushButton;
class QSpinBox;

class rbtreeview : public QWidget
{
	Q_OBJECT

public:
	explicit rbtreeview(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected slots:
	void onAdd();
	void onDel();
	void onFind();

protected:
	QPushButton* m_pBtnAdd;
	QPushButton* m_pBtnFind;
	QPushButton* m_pBtnDel;
	QSpinBox* m_pSbxVal;

protected:
	void paintEvent(QPaintEvent* event) override;

	// 鼠标双击
	virtual void mouseDoubleClickEvent(QMouseEvent* event);

private:
	rbtree m_rbTree;

	// 查找的节点
	rbtree::node* m_pFindNode;

	// 记录节点位置
	std::map<rbtree::node*, QRect> m_node2Rect;
};

