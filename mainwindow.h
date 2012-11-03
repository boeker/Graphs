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
	void closeAlgorithmView();
public slots:
	void resetLastClick();
	void selectNodeColor();
	void selectEdgeColor();
	void newNodeSelected(const QString &);
	void selectedAlgorithmResult(const QString &);
	void onResultDialogClose();
protected:
	bool eventFilter(QObject *, QEvent *);
private slots:
	void newGraph();
	void saveScreenshot();
	void readFromFile();
	void writeToFile();
	void writeToLastFile();
	void depthFirstSearch();
	void breadthFirstSearch();
private:
	bool onlyLetters(const QString &);

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
	QLineEdit *leFromNode;
	QLineEdit *leToNode;
	QPushButton *pbSelectNodeColor;
	QPushButton *pbSelectEdgeColor;
	QGraphicsView *graphicsView;
	QGraphicsScene *graphicsScene;
	graph::Graph *graph;
	QString lastClick; //For implementing the double click actions
	QString grabbedNode; //For implementing node relocating
	QString lastFilePath;
};

#endif