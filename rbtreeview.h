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

private:
	rbtree m_rbTree;

	// 查找的节点
	rbnode* m_pFindNode;
};

