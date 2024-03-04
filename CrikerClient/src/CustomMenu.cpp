#include "CustomMenu.h"

#include "CommonUtils.h"

CustomMenu::CustomMenu(QWidget* parent)
	: QMenu{ parent } {
	setAttribute(Qt::WA_TranslucentBackground);
	CommonUtils::loadStyleSheet(this, "Menu");
}

CustomMenu::~CustomMenu() = default;

void CustomMenu::addCustomMenu(const QString& text, const QString& icon, const QString& content) {
	QAction* pAction{ addAction(QIcon{icon}, content) };
	m_menuActionMap.insert(text, pAction);
}

QAction* CustomMenu::getAction(const QString& text) {
	return m_menuActionMap[text];
}
