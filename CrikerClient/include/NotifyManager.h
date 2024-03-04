#pragma once

#include <QObject>

#include "WinToastHandlerCallBack.h"
#include "wintoastlib.h"

class NotifyManager : public QObject {
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager() override;


signals:
	void signalSkinChanged(const QColor& color);

public:
	static NotifyManager* getInstance();
	void notifyOtherWindowChangeSkin(const QColor& color);
    static void showNotification(const std::wstring& appName, const std::wstring& firstLine, const std::wstring& secondLine, const std::wstring&
                                 imagePath);

private:
	inline static NotifyManager* instance{};
};
