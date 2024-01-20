#include "SearchResult.h"

#include <QHeaderView>
#include <QModelIndex>

#include "CCMainWindow.h"

SearchResult::SearchResult(QWidget* parent)
	: QDialog(parent) {
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint); //设置窗口无边框

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //延伸显示(水平铺满)
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //只读 不允许编辑
	ui.tableWidget->setShowGrid(false); //网格线不显示

	// 连接双击信号
	connect(ui.tableWidget, &QTableWidget::itemDoubleClicked, this, &SearchResult::onItemDoubleClicked);
}

SearchResult::~SearchResult() {}

void SearchResult::showEvent(QShowEvent* event) {
	showTable(); //展示信息
}

void SearchResult::showTable() {
	ui.tableWidget->clear(); //表格先清除

	ui.tableWidget->setRowCount(strs.size()); //创建行 有多少信息 创建多少行
	ui.tableWidget->setColumnCount(1);

	ui.tableWidget->horizontalHeader()->setVisible(false);
	ui.tableWidget->verticalHeader()->setVisible(false);

	for (int i = 0; i < strs.size(); i++) {
		QString str = strs.at(i);
		QTableWidgetItem* item = new QTableWidgetItem(str); //创建项 显示字符串几个内的元素内容

		ui.tableWidget->setItem(i, 0, item); //添加项
	}
}

void SearchResult::onItemDoubleClicked(QTableWidgetItem* item) {
	if (item) {
		// 获取双击项的文本
		QString itemText = item->text();

		if (auto* parentObj = qobject_cast<CCMainWindow*>(parent())) {
			parentObj->setLineEditText(itemText);
		}
		else {
			qDebug() << "Failed to cast to CCMainWindow";
		}

		this->hide();
	}
}
