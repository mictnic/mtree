#pragma once

#include <set>
#include <QWidget>
#include "avltree.h"

class QPushButton;
class QSpinBox;
class QLineEdit;

class avltreeview : public QWidget
{
	Q_OBJECT

public:
	explicit avltreeview(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected slots:
	void onAdd();
	void onDel();
	void onFind();

protected:
	QPushButton* m_pBtnAdd;
	QPushButton* m_pBtnFind;
	QPushButton* m_pBtnDel;
	QLineEdit* m_pLetVal;

protected:
	void paintEvent(QPaintEvent* event) override;

	std::vector<int> getNumbers();

private:
	avltree m_avl_tree;

	// ���ҵĽڵ�
	std::set<avltree::node*> m_nodes;

	// ��¼�ڵ�λ��
	std::map<avltree::node*, QRect> m_node2Rect;
};

