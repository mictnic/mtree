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

protected:
	QPushButton* m_pBtn;
	QSpinBox* m_pSbx;


protected:
	void paintEvent(QPaintEvent* event) override;

private:
	rbtree m_rbTree;
};

