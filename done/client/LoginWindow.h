#pragma once

#include "LoginInputPage.h"
#include "LoginWaitPage.h"
#include "LoginChangeKeywordPage.h"

#pragma execution_character_set("utf-8")


#include <QStackedWidget>
#include <QDialog>

class LoginWindow : public QDialog
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = Q_NULLPTR);
	~LoginWindow();

	enum PAGENUM
	{
		pnLOGININPUT,
		pnLOGINWAIT,
		pnLOGINCHANGEKEYWORD
	};

	//设置当前窗口的页面是哪一个页面
	void SetCurrnetPage(PAGENUM _page);

	//显示错误信息
	void SetErrorMessage(QString _msg);

private:

	/* QT 窗口 */
	LoginInputPage* lLoginInputPage;	//登陆输入框
	LoginWaitPage* lLoginWaitPage;		//登陆等待框
	LoginChangeKeywordPage* lLoginChangeKeywordPage; //首次登陆改密页面

	QStackedWidget* lStackWidget;		//多页面窗口

	QVBoxLayout* Layout;				//总布局

	friend class TopWidget;
};
