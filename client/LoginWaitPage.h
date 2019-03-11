#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <Qlabel>
#include <QMovie>

#pragma execution_character_set("utf-8")


class LoginWaitPage : public QWidget
{
	Q_OBJECT

public:
	LoginWaitPage(QWidget *parent = Q_NULLPTR);
	~LoginWaitPage();

private:

	QLabel* WaitImageLabel;
	QVBoxLayout* Layout;

	const char* ImageAddr = "Resources//6-wait.gif";
};
