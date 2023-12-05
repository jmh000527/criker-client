#include "ChatWindowShell.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>

#include "CommonUtils.h"

ChatWindowShell::ChatWindowShell(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();

	QFile file(":/Resources/MainWindow/MsgHtml/msgtmpl.js");
	//文件大小为零则创建文件
	// if (!file.size()) {
	QStringList employeesIDList = getEmployeesID();
	if (!createJSFile(employeesIDList)) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("js文件写入数据失败"));
	}
	// }

	//获取用户当前皮肤的RGB
	QColor backgroundColor{ CommonUtils::getDefaultSkinColor() };
	const QString r{ QString::number(backgroundColor.red()) };
	const QString g{ QString::number(backgroundColor.green()) };
	const QString b{ QString::number(backgroundColor.blue()) };
	const auto increaseValue{ 230 };

	ui.listWidget->setStyleSheet(QString{ "QListView {\
											background-color: rgb(%1, %2, %3);\
										 }" }.arg(r).arg(g).arg(b));
	ui.listWidget->setStyleSheet(QString{ "QListView {\
											padding: 4px; \
											border: none;\
										 }\
										 QListView:item {\
											border-radius:4px;\
											background-color: transparent;\
											height: 60px;\
										 }\
										 QListView::item:selected {\
											border-radius:4px;\
											background-color: rgba(%1, %2, %3, %4);\
										 }\
										 QListView::item:selected:!active{\
											border-radius:4px;\
											background-color: rgba(%1, %2, %3, %4);\
										 }\
										 QListView::item:selected:active{\
											border-radius:4px;\
											background-color: rgba(%1, %2, %3, %4);\
										 }\
										 QListView::item:hover{\
											background-color: rgba(%1, %2, %3, %5);\
										 }" }.arg(qMin(r.toInt() / 10 + increaseValue, 255))
										     .arg(qMin(g.toInt() / 10 + increaseValue, 255))
										     .arg(qMin(b.toInt() / 10 + increaseValue, 255))
										     .arg(150).arg(100)
	);
}

ChatWindowShell::~ChatWindowShell() {}

void ChatWindowShell::addTalkWindow(ChatWindow* chatWindow, ChatWindowItem* chatWindowItem, const QString& uid) {
	ui.rightStackedWidget->addWidget(chatWindow);

	connect(m_emojiWindow, SIGNAL(signalEmojiWindowHide()), chatWindow, SLOT(onSetEmojiBtnFocusStatus()));

	QListWidgetItem* item{ new QListWidgetItem{ ui.listWidget } };
	m_chatWindowItemMap.insert(item, chatWindow);
	item->setSelected(true);

	//判断是单聊还是群聊，获取头像
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(uid);
	sqlDepModel.setQuery(strSql);
	int rows = sqlDepModel.rowCount();

	//单聊
	if (rows == 0) {
		QString sql = QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(uid);
		sqlDepModel.setQuery(sql);
	}
	QModelIndex index = sqlDepModel.index(0, 0);

	QImage img;
	img.load(sqlDepModel.data(index).toString());
	chatWindowItem->setHeadPixmap(QPixmap::fromImage(img)); //设置头像

	ui.listWidget->addItem(item);
	ui.listWidget->setItemWidget(item, chatWindowItem);

	onChatWindowItemClicked(item);
	connect(chatWindowItem, &ChatWindowItem::signalCloseClicked,
	        [chatWindowItem, chatWindow, item, this]() {
		        m_chatWindowItemMap.remove(item);
		        chatWindow->close();
		        ui.listWidget->takeItem(ui.listWidget->row(item));
		        delete chatWindowItem;
		        ui.rightStackedWidget->removeWidget(chatWindow);

		        if (ui.rightStackedWidget->count() < 1) {
			        close();
		        }
	        });
}

void ChatWindowShell::setCurrentWidget(QWidget* widget) const {
	ui.rightStackedWidget->setCurrentWidget(widget);
}

QMap<QListWidgetItem*, QWidget*> ChatWindowShell::getChatWindowItemMap() const {
	return m_chatWindowItemMap;
}

bool ChatWindowShell::eventFilter(QObject* obj, QEvent* event) {
	if (obj == m_emojiWindow && event->type() == QEvent::Leave) {
		// 鼠标离开Emoji面板时隐藏面板
		m_emojiWindow->hide();
		return true;
	}
	return QObject::eventFilter(obj, event);
}

void ChatWindowShell::onEmojiBtnClicked(bool) {
	m_emojiWindow->setVisible(!m_emojiWindow->isVisible());
	QPoint emojiPoint = this->mapToGlobal(QPoint{ 0, 0 }); //将当前控件的相对位置转换为屏幕的绝对位置

	emojiPoint.setX(emojiPoint.x() + 170);
	emojiPoint.setY(emojiPoint.y() + 220);

	m_emojiWindow->move(emojiPoint);

	// 安装事件过滤器，以捕获鼠标离开事件
	m_emojiWindow->installEventFilter(this);
}

void ChatWindowShell::updateSendTcpMsg(const QString& data, int msgType, QString file) {}

void ChatWindowShell::onChatWindowItemClicked(QListWidgetItem* item) {
	QWidget* chatWindowWidget{ m_chatWindowItemMap.find(item).value() };
	ui.rightStackedWidget->setCurrentWidget(chatWindowWidget);
}

void ChatWindowShell::onEmojiItemClicked(int emojiNum) {
	ChatWindow* currentChatWindow{ dynamic_cast<ChatWindow*>(ui.rightStackedWidget->currentWidget()) };
	if (currentChatWindow != nullptr) {
		currentChatWindow->addEmojiImage(emojiNum);
	}
}

void ChatWindowShell::initControl() {
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString{ "聊天窗口" });

	m_emojiWindow = new EmojiWindow;
	m_emojiWindow->hide();

	QList<int> leftWidgetSize;
	leftWidgetSize.append(180);
	leftWidgetSize.append(width() - 180);
	ui.splitter->setSizes(leftWidgetSize);

	ui.listWidget->setStyle(new CustomProxyStyle{ this });

	connect(ui.listWidget, &QListWidget::itemClicked, this, &ChatWindowShell::onChatWindowItemClicked);
	connect(m_emojiWindow, SIGNAL(signalEmojiItemClicked(int)), this, SLOT(onEmojiItemClicked(int)));
}

void ChatWindowShell::initTcpSocket() {
	m_tcpClientSocket = new QTcpSocket{ this };
	m_tcpClientSocket->connectToHost("127.0.0.1", 6666);
}

QStringList ChatWindowShell::getEmployeesID() {
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE `status`=1");
	QStringList employeeIDList;

	//员工总数
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++){
		index = queryModel.index(i, 0);
		employeeIDList << queryModel.data(index).toString();
	}
	return employeeIDList;
}

bool ChatWindowShell::createJSFile(const QStringList& employeesList) {
	//读取文件数据
	QString strFileTxt("Resources/MainWindow/MsgHtml/msgtmpl.txt");
	// QString strFileTxt(qApp->applicationDirPath() + "/msgtmpl.txt");
	QFile fileRead(strFileTxt);
	QString strFile;
	if (fileRead.open(QIODevice::ReadOnly)) {
		strFile = fileRead.readAll();
		fileRead.close();
	} else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("msgtmpl.txt读取数据失败"));
		return false;
	}
	//替换(extern0,append10用作自己发信息使用)
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	// QFile fileWrite(qApp->applicationDirPath() + "/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		//更新空值
		QString strSourceInitNull = "var external = null;";
		//更新初始化
		QString strSourceInit = "external = channel.objects.external;";
		//更新newChannel
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); ";

		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		// QFile fileRecvHtml(qApp->applicationDirPath() + "/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) {
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		} else {
			QMessageBox::information(this,
			                         QString::fromLocal8Bit("提示"),
			                         QString::fromLocal8Bit("recvHtml.txt读取失败"));
			return false;
		}

		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		for (int i = 0; i < employeesList.length(); i++) {
			// 编辑 替换后的空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//编辑替换后的 newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//编辑替换后的recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
		}
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		QTextStream stream(&fileWrite);
		stream << strFile;
		return true;
	} else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("写msgtmpl.js失败"));
		return false;
	}
}
