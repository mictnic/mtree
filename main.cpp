#include <iostream>
#include <QApplication>
#include "rbtreeview.h"
#include "rbtree.h"
#include "avltreeview.h"

int bsearch(const std::vector<int>& vv, int key) {
	int l(-1), r(vv.size());
	while (l < r)
	{
		auto idx = (l + r) / 2;
		auto& ikey = vv[idx];
		if (ikey == key)
		{
			l = r = idx;
		}
		else if (ikey < key)
		{
			l = (l != idx ? idx : r);
		}
		else
		{
			r != idx ? r = idx : l = r;
		}
	}
	return r;
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	std::vector<int> vv = { 1 };
	auto index = bsearch(vv, 0);
	std::cout << index << std::endl;
	index = bsearch(vv, 1);
	std::cout << index << std::endl;
	index = bsearch(vv, 2);
	std::cout << index << std::endl;
	index = bsearch(vv, 3);
	std::cout << index << std::endl;
	index = bsearch(vv, 4);
	std::cout << index << std::endl;
	index = bsearch(vv, 5);
	std::cout << index << std::endl;
	index = bsearch(vv, 6);
	std::cout << index << std::endl;
	index = bsearch(vv, 7);
	std::cout << index << std::endl;

	//rbtreeview view;
	avltreeview view;
	app.setActiveWindow(&view);
	app.activeWindow();
	
	view.show();
	
	return app.exec();
}