TEMPLATE = app

QT += widgets

CONFIG += debug console

TARGET = mtree

HEADERS = \
	rbtree.h \
	rbtreeview.h \
	avltree.h \
	avltreeview.h \
	btree.h \

SOURCES = \
	rbtree.cpp \
	main.cpp \
	rbtreeview.cpp \
	avltree.cpp \
	avltreeview.cpp \
	btree.cpp \

DESTDIR = ./bin
DLLDESTDIR = ./bin
