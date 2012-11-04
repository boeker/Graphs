#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>

class ResultDialog : public QDialog {
	Q_OBJECT
public:
	ResultDialog(QWidget * = 0, Qt::WindowFlags = 0);
	~ResultDialog();
	
	void displayStringList(const QStringList &);
	void clearListWidget();
private slots:
	void onItemClick(QListWidgetItem *);
signals:
	void itemClicked(const QString &);
private:
	QListWidget *listWidget;
};

#endif