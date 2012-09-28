#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "graph/graph.h"

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
	void gvLeftMousePress(const QPointF &);
	void gvLeftMouseRelease();
	void gvMouseMove(const QPointF &);
	void gvLeftMouseDoubleClick(const QPointF &);
public slots:
	void resetLastClick();
protected:
	bool eventFilter(QObject *, QEvent *);
private slots:
	void newAct();
	void saveScreenshotAct();
private:
	bool onlyLetters(const QString &);
	QMenuBar *menuBar;
	QMenu *fileMenu;
	QAction *newAction;
	QAction *saveScreenshotAction;
	QRadioButton *rbInsertNode;
	QRadioButton *rbRemoveNode;
	QRadioButton *rbInsertEdge;
	QRadioButton *rbRemoveEdge;
	QLineEdit *leNextNodeName;
	QLineEdit *leNextEdgeQuality;
	QGraphicsView *graphicsView;
	QGraphicsScene *graphicsScene;
	graph::Graph *graph;
	QString lastClick; //For implementing the double click actions
	QString grabbedNode; //For implementing node relocating
};

#endif