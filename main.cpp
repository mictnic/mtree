#include <iostream>
#include <QApplication>
#include "rbtreeview.h"
#include "rbtree.h"
#include "avltreeview.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	//rbtreeview view;
	avltreeview view;
	app.setActiveWindow(&view);
	app.activeWindow();
	
	view.show();
	
	return app.exec();
}