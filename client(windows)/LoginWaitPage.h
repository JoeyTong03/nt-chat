#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <Qlabel>
#include <QMovie>

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
