#include "CCMainWindow.h"

#include "CommonUtils.h"

CCMainWindow::CCMainWindow(QWidget* parent)
	: BasicWindow{ parent } {
	ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");

	initColtrol();
}

CCMainWindow::~CCMainWindow() {}

void CCMainWindow::initColtrol() {
	ui.treeWidget->setStyle(new CustomProxyStyle{ this });
}

void CCMainWindow::setUserName(const QString& username) {}

void CCMainWindow::setUserLevel(int level) {
	QPixmap levePixmap{ ui.levelBtn->size() };
	levePixmap.fill(Qt::transparent);

}

void CCMainWindow::setHeadPixmap(const QString& headPath) {}
