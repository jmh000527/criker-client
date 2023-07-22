#pragma once

#include <QObject>

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

private:
	inline static NotifyManager* instance{};
};
