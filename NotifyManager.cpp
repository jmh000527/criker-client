#include "NotifyManager.h"
#include "CommonUtils.h"

NotifyManager::NotifyManager()
	:QObject{ nullptr } {
	
}

NotifyManager::~NotifyManager() = default;

NotifyManager* NotifyManager::getInstance() {
	if(instance == nullptr) {
		instance = new NotifyManager{};
	}

	return instance;
}

void NotifyManager::notifyOtherWindowChangeSkin(const QColor& color) {
	emit signalSkinChanged(color);

	CommonUtils::setDefaultSkinColor(color);
}
