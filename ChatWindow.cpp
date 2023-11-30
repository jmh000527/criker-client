#include "ChatWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QToolTip>

#include "ChatWindowShell.h"
#include "CommonUtils.h"
#include "ContactItem.h"
#include "RootContactItem.h"
#include "WindowManager.h"

ChatWindow::ChatWindow(QWidget* parent, const QString& uid, GroupType groupType)
	: QWidget{ parent }, m_chatId{ uid }, m_groupType{ groupType } {
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(this, m_chatId);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

ChatWindow::~ChatWindow() {
	WindowManager::getInstance()->deleteWindowByName(m_chatId);
}

void ChatWindow::addEmojiImage(int emojiNum) {
	ui.textEdit->setFocus();
	ui.textEdit->addEmojiUrl(emojiNum);
}

void ChatWindow::setWindowName(const QString& name) {
	ui.nameLabel->setText(name);
}

void ChatWindow::addPeopInfo(QTreeWidgetItem* pRootItem) {
	auto* pChild = new QTreeWidgetItem();

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));
	auto* pContactItem = new ContactItem(ui.treeWidget);

	static int i = 0;
	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	QImage imageHead;
	imageHead.load(":/Resources/MainWindow/girl.png");
	pContactItem->setHeadPixmap(
		CommonUtils::getRoundedImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUsername(QString::fromLocal8Bit("test%1").arg(i++));
	pContactItem->setSignature("hhhhh");

	pRootItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUsername();
	m_groupPeopleMap.insert(pChild, str);
}

void ChatWindow::initCompanyTalk() {
	QTreeWidgetItem* pRootItem{ new QTreeWidgetItem{} };
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	//设置data，区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName{ new RootContactItem{ false, ui.treeWidget } };
	ui.treeWidget->setFixedHeight(646);
	int nEmployeeNum = 50;
	QString qsGroupName{ QString::fromLocal8Bit("公司群 %1%2").arg(0).arg(nEmployeeNum) };
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i{}; i < nEmployeeNum; ++i) {
		addPeopInfo(pRootItem);
	}
}

void ChatWindow::initPersonalTalk() {
	QTreeWidgetItem* pRootItem{ new QTreeWidgetItem{} };
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	//设置data，区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName{ new RootContactItem{ false, ui.treeWidget } };
	ui.treeWidget->setFixedHeight(646);
	int nEmployeeNum = 10;
	QString qsGroupName{ QString::fromLocal8Bit("人事部 %1/%2").arg(0).arg(nEmployeeNum) };
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i{}; i < nEmployeeNum; ++i) {
		addPeopInfo(pRootItem);
	}
}

void ChatWindow::initMarketingTalk() {
	QTreeWidgetItem* pRootItem{ new QTreeWidgetItem{} };
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	//设置data，区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName{ new RootContactItem{ false, ui.treeWidget } };
	ui.treeWidget->setFixedHeight(646);
	int nEmployeeNum = 5;
	QString qsGroupName{ QString::fromLocal8Bit("市场部 %1%2").arg(0).arg(nEmployeeNum) };
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i{}; i < nEmployeeNum; ++i) {
		addPeopInfo(pRootItem);
	}
}

void ChatWindow::initDevelopmentTalk() {
	QTreeWidgetItem* pRootItem{ new QTreeWidgetItem{} };
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	//设置data，区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactItem* pItemName{ new RootContactItem{ false, ui.treeWidget } };
	ui.treeWidget->setFixedHeight(646);
	int nEmployeeNum = 50;
	QString qsGroupName{ QString::fromLocal8Bit("研发部 %1%2").arg(0).arg(nEmployeeNum) };
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i{}; i < nEmployeeNum; ++i) {
		addPeopInfo(pRootItem);
	}
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

	switch (m_groupType) {
		case GroupType::Company: {
			initCompanyTalk();
			break;
		}
		case GroupType::Personal: {
			initPersonalTalk();
			break;
		}
		case GroupType::Marketing: {
			initMarketingTalk();
			break;
		}
		case GroupType::Development: {
			initDevelopmentTalk();
			break;
		}
		default: {
			initP2PChat();
			break;
		}
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

void ChatWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), QString::fromLocal8Bit("发送的信息不能为空"),
		                   this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	QString html = ui.textEdit->document()->toHtml();

	//文本HTML如果没有字体则添加字体
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		} else {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
			                         QString::fromLocal8Bit("msgFont.txt 不存在"));
			return;
		}
		if (!html.contains(fontHtml)) {
			html.replace(text, fontHtml);
		}
	}

	ui.textEdit->clear();
	ui.textEdit->deleteAllEmojiImage();
	ui.msgWidget->appendMsg(html); //收信息窗口
}

void ChatWindow::onItemDoubleClicked(QTreeWidgetItem* item, int colum) {
	if (bool isChild = item->data(0, Qt::UserRole).toBool(); isChild) {
		QString peopleName = m_groupPeopleMap.value(item);
		// QString strEmployeeID = item->data(0, Qt::UserRole + 1).toString();
		// if (strEmployeeID == gLoginEmployeeID)
		// 	return;
		WindowManager::getInstance()->addNewChatWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::P2P,
		                                               peopleName);
	}
}
