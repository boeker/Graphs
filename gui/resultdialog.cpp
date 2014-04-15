// Copyright (c) 2012 Jan Böker
// MIT License

#include "resultdialog.h"

#include <QHBoxLayout>

ResultDialog::ResultDialog(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f) {
	listWidget = new QListWidget(this);
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onItemClick(QListWidgetItem *)));

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(listWidget);
	setLayout(mainLayout);
}

ResultDialog::~ResultDialog() {
}

void ResultDialog::displayStringList(const QStringList &stringList) {
	listWidget->clear();
	QStringList::const_iterator it;
	for (it = stringList.constBegin(); it != stringList.constEnd(); ++it) {
		listWidget->addItem(*it);
	}
}

void ResultDialog::clearListWidget() {
	listWidget->clear();
}

void ResultDialog::onItemClick(QListWidgetItem *item) {
	emit itemClicked(item->text());
}