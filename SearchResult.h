#pragma once

#include <QDialog>
#include "ui_SearchResult.h"

class SearchResult : public QDialog {
	Q_OBJECT

public:
	SearchResult(QWidget* parent = nullptr);
	~SearchResult() override;

	void showEvent(QShowEvent* event) override;
	void showTable(); //展示信息

	QStringList strs; //存储需要展示的信息

private slots:
	void onItemDoubleClicked(QTableWidgetItem* item);

private:
	Ui::SearchResultClass ui;
};
