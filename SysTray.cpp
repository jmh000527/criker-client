#include "SysTray.h"

#include <QWidget>

#include "CustomMenu.h"

SysTray::SysTray(QWidget* parent)
	: QSystemTrayIcon{ parent }, m_parent{ parent } {
	initSystemTray();
	show();
}

SysTray::~SysTray() = default;

void SysTray::initSystemTray() {
	setToolTip(QStringLiteral("QtQQ"));
	setIcon(QIcon{ ":/Resources/MainWindow/app/logo.ico" });

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
	        SLOT(onIconActivate(QSystemTrayIcon::ActivationReason)));
}

void SysTray::addSysTrayMenu() const {
	auto* customMenu{ new CustomMenu{ m_parent } };
	// const QScopedPointer<CustomMenu> customMenu{new CustomMenu{ m_parent }};
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("显示"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("退出"));

	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));

	customMenu->exec(QCursor::pos());
}

void SysTray::onIconActivate(QSystemTrayIcon::ActivationReason reason) const {
	if (reason == QSystemTrayIcon::Trigger) {
		m_parent->show();
		m_parent->activateWindow();
	}else if(reason == QSystemTrayIcon::Context) {
		addSysTrayMenu();
	}
}
