#pragma once

#include <QWidget>
#include "rbtree.h"

class rbtreeview : public QWidget
{
	Q_OBJECT

public:
	explicit rbtreeview(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	rbtree m_rbTree;
};

