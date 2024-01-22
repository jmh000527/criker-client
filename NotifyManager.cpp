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

void NotifyManager::showNotification(std::wstring& appName, std::wstring& firstLine, std::wstring& secondLine, std::wstring& imagePath) {
    std::wstring appUserModelID = L"Fucker's APP'ID";
    WinToastLib::WinToast::instance()->setAppName(appName);
    WinToastLib::WinToast::instance()->setAppUserModelId(appUserModelID);

    // 使用 ImageAndText02 模板
    WinToastLib::WinToastTemplate toast(WinToastLib::WinToastTemplate::ImageAndText02);
    toast.setTextField(firstLine, WinToastLib::WinToastTemplate::FirstLine);
    toast.setTextField(secondLine, WinToastLib::WinToastTemplate::SecondLine);

    // 设置图像路径
    toast.setImagePath(imagePath);

    //toast.setAttributionText(std::wstring(L"default"));
    toast.setExpiration(false);

    if (WinToastLib::WinToast::instance()->initialize()) {
        WinToastLib::WinToast::instance()->showToast(toast, new WinToastHandler());
        // Sleep(10000);
    }
}
