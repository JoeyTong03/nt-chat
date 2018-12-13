#pragma once

#include <QFile>
#include <QTextEdit>
#include <QFileInfo>
#include <QMimeData>
#include <QImageReader>

class Image_TextEdit :public QTextEdit
{
	Q_OBJECT

public:
	Image_TextEdit(QWidget *parent = Q_NULLPTR);
	~Image_TextEdit() {};


};

Image_TextEdit::Image_TextEdit(QWidget *parent)
	: QTextEdit(parent)
{
	
}

