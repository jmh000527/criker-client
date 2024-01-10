#pragma once

#include <QMenu>

class CustomMenu : public QMenu {
	Q_OBJECT

public:
	CustomMenu(QWidget* parent = nullptr);
	~CustomMenu() override;

public:
	void addCustomMenu(const QString& text, const QString& icon, const QString& content);
	QAction* getAction(const QString& text);

private:
	QMap<QString, QAction*> m_menuActionMap;
};
