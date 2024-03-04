#pragma once

#include <QSystemTrayIcon>

class SysTray : public QSystemTrayIcon {
	Q_OBJECT

public:
	SysTray(QWidget* parent);
	~SysTray() override;

private:
	void initSystemTray();
	void addSysTrayMenu() const;

	QWidget* m_parent{};

public slots:
	void onIconActivate(QSystemTrayIcon::ActivationReason reason) const;

};
