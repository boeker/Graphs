#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include "resultdialog.h"
#include "../graph/graph.h"

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
protected:
	bool eventFilter(QObject *, QEvent *);
private:
	void gvLeftMousePress(const QPointF &);
	void gvLeftMouseRelease();
	void gvLeftMouseDoubleClick(const QPointF &);
	void gvMouseMove(const QPointF &);

	void closeAlgorithmView();

	bool onlyLetters(const QString &);
public slots:
	void newGraph();
	void saveScreenshot();
	void readFromFile();
	void writeToFile();
	void writeToLastFile();

	void depthFirstSearch();
	void breadthFirstSearch();
private slots:
	void selectNodeColor();
	void selectEdgeColor();

	void resetLastClick();
	void newNodeSelected(const QString &);

	void selectedAlgorithmResult(const QString &);
	void onResultDialogClose();
private:
	ResultDialog *resultDialog;

	QMenuBar *menuBar;

	QMenu *fileMenu;
	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *saveScreenshotAction;

	QMenu *algorithmMenu;
	QAction *depthFirstSearchAction;
	QAction *breadthFirstSearchAction;

	QRadioButton *rbInsertNode;
	QRadioButton *rbRemoveNode;
	QRadioButton *rbInsertEdge;
	QRadioButton *rbRemoveEdge;

	QLineEdit *leNextNodeName;
	QLineEdit *leNextEdgeQuality;

	QLineEdit *leSelectedNode;
	QPushButton *pbSelectNodeColor;

	QLineEdit *leFromNode;
	QLineEdit *leToNode;
	QPushButton *pbSelectEdgeColor;

	QGraphicsView *graphicsView;
	QGraphicsScene *graphicsScene;

	QString lastClick;
	QString grabbedNode;
	QString lastFilePath;

	graph::Graph *graph;
};

#endif