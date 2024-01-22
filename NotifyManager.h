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
    static void showNotification(std::wstring& appName, std::wstring& firstLine, std::wstring& secondLine, std::wstring& imagePath);

private:
	inline static NotifyManager* instance{};
};
