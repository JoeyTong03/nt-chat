#pragma once

#include "Qt.h"


class GroupChat_GroupMemberWidget : public QWidget
{
	Q_OBJECT

public:
	GroupChat_GroupMemberWidget(QWidget *parent = Q_NULLPTR);
	~GroupChat_GroupMemberWidget();

private:

	QGroupBox* gGroupMember_GroupBox;
	QVBoxLayout* gGroupMember_VLayout;
	QTableWidget* gGroupMember_TableWidget;

	QVBoxLayout* Layout;
};
