
#include "UserListWindow.h"
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>
#include <QCloseEvent>

#pragma execution_character_set("utf-8")


UserListWindow::UserListWindow(QString _username, ClientSocket* _socket, QWidget *parent)
	: username(_username), socket(_socket), QDialog(parent)
{
	//Qt �ؼ�
	{
		//��ͷ
		ulUserListHead = new UserListHeadWidget(_username);



		ulUserListListWidget = new QListWidget;

		//ulUserListListWidget->setStyleSheet("background:transparent;");

		ulUserListGroupBox = new QGroupBox("��ϵ��");
		//ulUserListGroupBox->setStyleSheet("background:transparent;");
		ulUserListGroupBoxLayout = new QVBoxLayout(ulUserListGroupBox);
		ulUserListGroupBoxLayout->addWidget(ulUserListListWidget);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(ulUserListHead);
		Layout->addWidget(ulUserListGroupBox);

		this->setFixedWidth(420);

		this->setAutoFillBackground(true);
		QPalette palette = this->palette();
		palette.setBrush(QPalette::Window, QBrush(QPixmap("Resources//9-bg.jpg").scaled(/*this->size()*/QSize(420, 800), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));// ʹ��ƽ�������ŷ�ʽ
		this->setPalette(palette);// ��widget���ϱ���ͼ

		this->resize(200, 800);
	}

	//Qt �źŲ�
	{
		QObject::connect(
			ulUserListListWidget,
			&QListWidget::itemDoubleClicked,
			this,
			&UserListWindow::ItemDoubleClicked
		);

		QObject::connect(
			socket,
			&ClientSocket::Signal_AcceptText,
			this,
			&UserListWindow::GetMessageWithoutDialog
		);

		QObject::connect(
			socket,
			&ClientSocket::Signal_FriendOnline,
			this,
			&UserListWindow::FriendOnline
		);

		QObject::connect(
			socket,
			&ClientSocket::Signal_FriendOffline,
			this,
			&UserListWindow::FriendOffline
		);


	}

	AddOneUser("all1");
}

UserListWindow::~UserListWindow()
{
}

void UserListWindow::AddOneUser(QString _user)
{
	QWidget* item = new QWidget;
	QHBoxLayout* itemlayout = new QHBoxLayout(item);

	//Բ��ͷ�����ʾ
	QPixmap pixmapa(HeadImageAddr);
	QPixmap pixmap(ImageSize, ImageSize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QPainterPath path;
	path.addEllipse(1, 1, ImageSize, ImageSize);
	painter.setClipPath(path);
	painter.drawPixmap(1, 1, ImageSize, ImageSize, pixmapa);

	QLabel* IconLabel = new QLabel;
	IconLabel->setPixmap(pixmap);

	QChar flag = _user[_user.length() - 1];
	QString _username = _user.left(_user.length() - 1);

	QLabel* name = new QLabel(_username);
	QFont SignatureFont("����", SignatureFontSize, 50);
	name->setFont(SignatureFont);


	QLabel* OnOffLabel = new QLabel;
	if (flag == '1') OnOffLabel->setText("(online)");
	else OnOffLabel->setText("(offline)");

	itemlayout->addWidget(IconLabel, 1);
	itemlayout->addWidget(name, 4);
	itemlayout->addWidget(OnOffLabel, 1);
	itemlayout->setAlignment(Qt::AlignLeft);

	OnOfflineLabe.push_back(QPair<QString, QLabel*>(_username,OnOffLabel));

	item->setFixedHeight(60);
	item->setObjectName(_username);
	QListWidgetItem* listitem = new QListWidgetItem();
	listitem->setSizeHint(QSize(100, 60));
	ulUserListListWidget->addItem(listitem);
	ulUserListListWidget->setItemWidget(listitem, item);

	chatwindow.push_back(QPair<QString, bool>(_username, false));
}

void UserListWindow::ItemDoubleClicked(QListWidgetItem * item)
{
	QString itemname = ulUserListListWidget->itemWidget(item)->objectName();

	AddOneDialog(itemname);
}

ChatWidget* UserListWindow::AddOneDialog(QString itemname)
{
	for (int i = 0; i < chatwindow.size(); i++)
	{
		if (chatwindow[i].first == itemname)
		{
			if (chatwindow[i].second == false)
			{
				ChatWidget* onedialog = new ChatWidget(itemname, username, socket);
				onedialog->setObjectName(itemname);
				dialog.push_back(onedialog);
				chatwindow[i].second = true;

				//�źŲ�
				{
					//�رս���
					QObject::connect(
						onedialog,
						&ChatWidget::CloseSignal,
						this,
						&UserListWindow::DelOneDialog
					);
				}

				onedialog->show();
				return onedialog;
			}
		}
	}

	return NULL;
}

void UserListWindow::DelOneDialog(QString _user)
{

	for (int i = 0; i < dialog.size(); i++)
	{
		if (_user == dialog[i]->objectName())
		{
			dialog[i]->close();
			dialog.remove(i);
			break;
		}
	}

	for (int i = 0; i < chatwindow.size(); i++)
	{
		if (_user == chatwindow[i].first)
		{
			chatwindow[i].second = false;
			break;
		}
	}
}

void UserListWindow::GetMessageWithoutDialog(QString _text, QString _from)
{
	AddOneDialog(_from)->RecvMessage(_text, _from);
}

void UserListWindow::FriendOnline(QString _user)
{
	//��������֡
	for (int i = 0; i < OnOfflineLabe.size(); i++)
		if(OnOfflineLabe[i].first==_user)
		OnOfflineLabe[i].second->setText("(online)");

	QString text = _user + "������";
	QMessageBox *box = new QMessageBox(QMessageBox::Information, tr("��ʾ"), text);
	QTimer::singleShot(6000, box, SLOT(accept())); //Ҳ�ɽ�accept��Ϊclose
	box->setWindowFlags(Qt::WindowStaysOnTopHint);
	box->show();

}

void UserListWindow::FriendOffline(QString _user)
{
	//��������֡
	for (int i = 0; i < OnOfflineLabe.size(); i++)
		if (OnOfflineLabe[i].first == _user)
			OnOfflineLabe[i].second->setText("(Offline)");

}

void UserListWindow::closeEvent(QCloseEvent * event)
{
		socket->SendOffLineFrame();

		emit userquit();

		this->hide();

		event->ignore();
}

//#define TEST_USERLISTWINDOW
#ifdef TEST_USERLISTWINDOW

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	UserListWindow test("��С��");

	test.AddOneUser("������1");
	test.AddOneUser("��1");

	test.show();

	return a.exec();
}


#endif // TEST_USERLISTWINDOW
