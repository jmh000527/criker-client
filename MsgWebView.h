#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj : public QObject {
	Q_OBJECT
	//动态属性
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent, const QString& msgLPicPath = "");

private:
	void initHtmlTmpl(); //初始化聊天网页
	QString getMsgTmplHtml(const QString& code);

	QString m_msgLHtmlTmpl; //别人发的消息
	QString m_msgRHtmlTmpl; //我发的消息
	QString m_msgPicPath; //图片路径

signals:
	void signalMsgHtml(const QString& html);
};

class MsgWebPage : public QWebEnginePage {
public:
	MsgWebPage(QObject* parent = nullptr);

protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override;
};

class MsgWebView : public QWebEngineView {
	Q_OBJECT
public:
	MsgWebView(QWidget* parent);
	~MsgWebView() override;

	void appendMsg(const QString& html, const QString& obj = "0");

private:
	QList<QStringList> parseHtml(const QString& html); //解析HTML
	QList<QStringList> parseDocNode(const QDomNode& node); //Qt 中所有dom节点都可以使用DOM进行操作

	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel; //网络通道

signals:
	void signalSendMsg(const QString& strData, int& msgType, QString sFile = "");
};
