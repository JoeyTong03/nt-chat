#pragma once

#include "LoginInputPage.h"
#include "LoginWaitPage.h"
#include "LoginChangeKeywordPage.h"

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

	//���õ�ǰ���ڵ�ҳ������һ��ҳ��
	void SetCurrnetPage(PAGENUM _page);

	//��ʾ������Ϣ
	void SetErrorMessage(QString _msg);

private:

	/* QT ���� */
	LoginInputPage* lLoginInputPage;	//��½�����
	LoginWaitPage* lLoginWaitPage;		//��½�ȴ���
	LoginChangeKeywordPage* lLoginChangeKeywordPage; //�״ε�½����ҳ��

	QStackedWidget* lStackWidget;		//��ҳ�洰��

	QVBoxLayout* Layout;				//�ܲ���

	friend class TopWidget;
};
