TEMPLATE = app

QT += widgets

CONFIG += debug console

TARGET = mtree

HEADERS = \
	rbtree.h \
	rbtreeview.h

SOURCES = \
	rbtree.cpp \
	main.cpp \
	rbtreeview.cpp

DESTDIR = ./bin
DLLDESTDIR = ./bin