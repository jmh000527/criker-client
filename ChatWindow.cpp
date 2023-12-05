#include "ChatWindow.h"

#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QToolTip>

#include "ChatWindowShell.h"
#include "CommonUtils.h"
#include "ContactItem.h"
#include "RootContactItem.h"
#include "WindowManager.h"

ChatWindow::ChatWindow(QWidget* parent, const QString& uid)
	: QWidget{ parent }, m_chatId{ uid } {
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(this, m_chatId);
	setAttribute(Qt::WA_DeleteOnClose);

	initGroupChatStatus();
	initControl();
}

ChatWindow::~ChatWindow() {
	WindowManager::getInstance()->deleteWindowByName(m_chatId);
}

void ChatWindow::addEmojiImage(int emojiNum) {
	ui.textEdit->setFocus();
	// ui.textEdit->addEmojiUrl(emojiNum);
}

void ChatWindow::setWindowName(const QString& name) {
	ui.nameLabel->setText(name);
}

void ChatWindow::addPeopInfo(QTreeWidgetItem* pRootItem, int employeeID) {
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);

	//获取名 签名 头像路径
	QString strName, strSign, strPicPath, strQuery;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	QModelIndex nameIndex, signIndex, picIndex;
	nameIndex = queryInfoModel.index(0, 0);
	signIndex = queryInfoModel.index(0, 1);
	picIndex = queryInfoModel.index(0, 2);
	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicPath = queryInfoModel.data(picIndex).toString();

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	QImage imageHead;
	imageHead.load(strPicPath);
	pContactItem->setHeadPixmap(CommonUtils::getRoundedImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUsername(strName);
	pContactItem->setSignature(strSign);

	pRootItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUsername();
	m_groupPeopleMap.insert(pChild, str);
}

void ChatWindow::initP2PChat() {
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");
	// ui.widget->setFixedSize(pixSkin.size());

	auto* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());

	ui.widget->hide();
}

int ChatWindow::getCompDepID() {
	QSqlQuery queryDepID(
		QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").
		arg(QString::fromLocal8Bit("公司群")));
	queryDepID.exec();
	queryDepID.next();

	return queryDepID.value(0).toInt();
}

void ChatWindow::initTalkWindow() {
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);
	ui.treeWidget->setFixedHeight(646);

	//当前聊天的群组名或人名
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(m_chatId));
	queryGroupName.exec();
	if (queryGroupName.first()) {
		strGroupName = queryGroupName.value(0).toString();
	}

	int nEmployeeNum = 0;

	//获取员工人数
	QSqlQueryModel queryEmployeeModel;
	//公司群
	if (getCompDepID() == m_chatId.toInt()) {
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE `status`=1");
	} else {
		queryEmployeeModel.setQuery(
			QString("SELECT employeeID FROM tab_employees WHERE `status`=1 AND departmentID=%1").arg(m_chatId));
	}
	nEmployeeNum = queryEmployeeModel.rowCount();
	QString qsGroupName = QString{ "%1 %2/%3" }.arg(strGroupName).arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++) {
		//添加子节点
		int employeeID;
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		employeeID = queryEmployeeModel.data(modelIndex).toInt();

		addPeopInfo(pRootItem, employeeID);
	}
}

void ChatWindow::initControl() {
	loadStyleSheet("TalkWindow");
	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);
	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmojiBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
	        SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	if (m_isGroupChat) {
		initTalkWindow();
	} else {
		initP2PChat();
	}
}

void ChatWindow::initGroupChatStatus() {
	QSqlQueryModel sqlDepModel;
	QString sql = QString("SELECT * FROM tab_department WHERE departmentID=%1").arg(m_chatId);

	sqlDepModel.setQuery(sql);
	int rows = sqlDepModel.rowCount();
	if (rows == 0) {
		m_isGroupChat = false;
	} else {
		m_isGroupChat = true;
	}
}

void ChatWindow::loadStyleSheet(const QString& sheetName) {
	QFile file{ ":/Resources/QSS/" + sheetName + ".css" };
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qStyleSheet{ QLatin1String{ file.readAll() } };

		//获取用户当前皮肤的RGB
		QColor backgroundColor{ CommonUtils::getDefaultSkinColor() };
		const QString r{ QString::number(backgroundColor.red()) };
		const QString g{ QString::number(backgroundColor.green()) };
		const QString b{ QString::number(backgroundColor.blue()) };
		const auto increaseValue{ 230 };

		qStyleSheet += QString("QWidget[titleskin = true] {\
									background-color: rgb(%1, %2, %3);\
									border-top-right-radius: 4px;\
								}\
								QWidget[bottomskin = true] {\
									background-color: rgb(%4, %5, %6);\
									border-top-right-radius: 4px;\
								}").arg(r).arg(g).arg(b)
								   .arg(qMin(r.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(g.toInt() / 10 + increaseValue, 255))
								   .arg(qMin(b.toInt() / 10 + increaseValue, 255));

		qStyleSheet += QString("QToolButton#closeBtn {\
									background-color: rgb(%1, %2, %3);\
									color: #ffffff;\
									font-size: 12px;\
									border-radius: 4px;\
								}\
								QToolButton#closeBtn:hover{\
								   background-color: rgba(%1, %2, %3, 150);\
								}\
								QToolButton#closeBtn:pressed{\
								   background-color: rgba(%1, %2, %3, 150);\
								}").arg(r).arg(g).arg(b);

		qStyleSheet += QString("QToolButton#sendBtn {\
									background-color: rgb(%1, %2, %3);\
									color: #ffffff;\
									font-size: 12px;\
									border-radius: 4px;\
								}\
								QToolButton#sendBtn:hover{\
									background-color: rgba(%1, %2, %3, 150);\
								}\
								QToolButton#sendBtn:pressed{\
									background-color: rgba(%1, %2, %3, 150);\
								}\
								QToolButton#sendBtn::menu-button{\
								    margin-top: 5px;\
								    border-top-right-radius: 4px;\
								    border-bottom-right-radius: 4px;\
								    border-left: 1px solid rgba(255, 255, 255, 100);\
								    background-color: transparent;\
								    width: 24px;\
								    height: 14px;\
								}\
								QToolButton#sendBtn::menu-arrow{\
									image: url(:Resources / MainWindow / aio_setting_white_normal.png);\
								}").arg(r).arg(g).arg(b);


		setStyleSheet(qStyleSheet);
	}

	file.close();
}

void ChatWindow::dealMessage(QNChatMessage* messageW, QListWidgetItem* item, QString text, QString time, QNChatMessage::User_Type type) {
	auto width = ui.listWidget->size().width();
	messageW->setFixedWidth(width);
	QSize size = messageW->fontRect(text);
	item->setSizeHint(QSize{ ui.listWidget->width(), size.height() });
	messageW->setText(text, time, size, type);
	ui.listWidget->setItemWidget(item, messageW);
}

void ChatWindow::dealMessageTime(QString curMsgTime) {
	bool isShowTime = false;
	if (ui.listWidget->count() > 0) {
		QListWidgetItem* lastItem = ui.listWidget->item(ui.listWidget->count() - 1);
		// QNChatMessage* messageW = (QNChatMessage*)ui.listWidget->itemWidget(lastItem);
		QNChatMessage* messageW = dynamic_cast<QNChatMessage*>(ui.listWidget->itemWidget(lastItem));
		int lastTime = messageW->time().toInt();
		int curTime = curMsgTime.toInt();
		// qDebug() << "curTime lastTime:" << curTime - lastTime;
		isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
		//        isShowTime = true;
	}
	else {
		isShowTime = true;
	}
	if (isShowTime) {
		QNChatMessage* messageTime = new QNChatMessage(ui.listWidget->parentWidget());
		QListWidgetItem* itemTime = new QListWidgetItem(ui.listWidget);

		QSize size{ this->width(), 40 };
		messageTime->resize(size);
		itemTime->setSizeHint(QSize{ ui.listWidget->width(), size.height() });
		messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
		ui.listWidget->setItemWidget(itemTime, messageTime);
	}
}

void ChatWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("发送的信息不能为空"),this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	// QString html = ui.textEdit->document()->toHtml();

	// //文本HTML如果没有字体则添加字体
	// if (!html.contains(".png") && !html.contains("</span>")) {
	// 	QString fontHtml;
	// 	QString text = ui.textEdit->toPlainText();
	// 	QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
	// 	if (file.open(QIODevice::ReadOnly)) {
	// 		fontHtml = file.readAll();
	// 		fontHtml.replace("%1", text);
	// 		file.close();
	// 	} else {
	// 		QMessageBox::information(this, QString::fromLocal8Bit("提示"),
	// 		                         QString::fromLocal8Bit("msgFont.txt 不存在"));
	// 		return;
	// 	}
	//
	// 	if (!html.contains(fontHtml)) {
	// 		html.replace(text, fontHtml);
	// 	}
	// }

	QString msg = ui.textEdit->toPlainText();
	QString time = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
	bool isSending = true; // 发送中

	dealMessageTime(time);
	QNChatMessage* messageW = new QNChatMessage(ui.listWidget->parentWidget());
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
	messageW->setTextSuccess();
	ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);

	ui.textEdit->clear();
	// ui.textEdit->deleteAllEmojiImage();
	// ui.msgWidget->appendMsg(html); //收信息窗口
}

void ChatWindow::onItemDoubleClicked(QTreeWidgetItem* item, int colum) {
	if (bool isChild = item->data(0, Qt::UserRole).toBool(); isChild) {
		QString peopleName = m_groupPeopleMap.value(item);
		// QString strEmployeeID = item->data(0, Qt::UserRole + 1).toString();
		// if (strEmployeeID == gLoginEmployeeID)
		// 	return;
		WindowManager::getInstance()->addNewChatWindow(item->data(0, Qt::UserRole + 1).toString());
	}
}