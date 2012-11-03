#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>
#include <QRect>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QKeySequence>
#include <QStringList>
#include "graph/exceptions/graphfileerror.h"


MainWindow::MainWindow() {
	resultDialog = new ResultDialog(this);

	rbInsertNode = new QRadioButton(tr("Insert Node"));
	rbInsertNode->setChecked(true);
	rbRemoveNode = new QRadioButton(tr("Remove Node"));
	rbInsertEdge = new QRadioButton(tr("Insert Edge"));
	rbRemoveEdge = new QRadioButton(tr("Remove Edge"));
	leNextNodeName = new QLineEdit("A");
	leNextEdgeQuality = new QLineEdit("1");
	leSelectedNode = new QLineEdit();
	leSelectedNode->setReadOnly(true);
	leToNode = new QLineEdit();
	leToNode->setReadOnly(true);
	leFromNode = new QLineEdit();
	leFromNode->setReadOnly(true);
	pbSelectNodeColor = new QPushButton(tr("Select Color"));
	pbSelectEdgeColor = new QPushButton(tr("Select Color"));
	graphicsView = new QGraphicsView;
	QRect screen = QApplication::desktop()->screenGeometry();
	graphicsScene = new QGraphicsScene(0, 0, screen.width()/2, screen.height()/2);
	graph = new graph::Graph(graphicsScene);
	graphicsView->setScene(graphicsScene);
	graphicsView->setRenderHint(QPainter::Antialiasing);
	lastClick.clear();
	grabbedNode.clear();

	connect(rbInsertEdge, SIGNAL(pressed()), this, SLOT(resetLastClick()));
	connect(rbRemoveEdge, SIGNAL(pressed()), this, SLOT(resetLastClick()));
	connect(pbSelectNodeColor, SIGNAL(pressed()), this, SLOT(selectNodeColor()));
	connect(pbSelectEdgeColor, SIGNAL(pressed()), this, SLOT(selectEdgeColor()));
	connect(leSelectedNode, SIGNAL(textChanged(const QString &)), this, SLOT(newNodeSelected(const QString &)));
	connect(resultDialog, SIGNAL(itemClicked(const QString &)), this, SLOT(selectedAlgorithmResult(const QString &)));
	connect(resultDialog, SIGNAL(rejected()), this, SLOT(onResultDialogClose()));

	menuBar = new QMenuBar(this);

	fileMenu = menuBar->addMenu(tr("File"));
	newAction = new QAction(tr("New"), this);
	newAction->setShortcuts(QKeySequence::New);
	connect(newAction, SIGNAL(triggered()), this, SLOT(newGraph()));
	fileMenu->addAction(newAction);
	openAction = new QAction(tr("Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(readFromFile()));
	fileMenu->addAction(openAction);
	saveAction = new QAction(tr("Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(writeToLastFile()));
	saveAction->setEnabled(false);
	fileMenu->addAction(saveAction);
	saveAsAction = new QAction(tr("Save As..."), this);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(writeToFile()));
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	saveScreenshotAction = new QAction(tr("Save Screenshot"), this);
	connect(saveScreenshotAction, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
	fileMenu->addAction(saveScreenshotAction);

	algorithmMenu = menuBar->addMenu(tr("Algorithm"));
	depthFirstSearchAction = new QAction(tr("Depth-first Search"), this);
	connect(depthFirstSearchAction, SIGNAL(triggered()), this, SLOT(depthFirstSearch()));
	algorithmMenu->addAction(depthFirstSearchAction);
	breadthFirstSearchAction = new QAction(tr("Breadth-first Search"), this);
	connect(breadthFirstSearchAction, SIGNAL(triggered()), this, SLOT(breadthFirstSearch()));
	algorithmMenu->addAction(breadthFirstSearchAction);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *sideBarLayout = new QVBoxLayout;
	sideBarLayout->setAlignment(Qt::AlignTop);

	QVBoxLayout *actionLayout = new QVBoxLayout;
	actionLayout->addWidget(rbInsertNode);
	actionLayout->addWidget(rbRemoveNode);
	actionLayout->addWidget(rbInsertEdge);
	actionLayout->addWidget(rbRemoveEdge);
	QGroupBox *actionGroupBox = new QGroupBox(tr("Action"));
	actionGroupBox->setLayout(actionLayout);
	sideBarLayout->addWidget(actionGroupBox);

	QVBoxLayout *nextNodeLayout = new QVBoxLayout;
	nextNodeLayout->addWidget(new QLabel(tr("Name:")));
	nextNodeLayout->addWidget(leNextNodeName);
	QGroupBox *nextNodeGroupBox = new QGroupBox(tr("Next Node"));
	nextNodeGroupBox->setLayout(nextNodeLayout);
	sideBarLayout->addWidget(nextNodeGroupBox);

	QVBoxLayout *nextEdgeLayout = new QVBoxLayout;
	nextEdgeLayout->addWidget(new QLabel(tr("Quality:")));
	nextEdgeLayout->addWidget(leNextEdgeQuality);
	QGroupBox *nextEdgeGroupBox = new QGroupBox(tr("Next Edge"));
	nextEdgeGroupBox->setLayout(nextEdgeLayout);
	sideBarLayout->addWidget(nextEdgeGroupBox);

	QVBoxLayout *selectedNodeLayout = new QVBoxLayout;
	selectedNodeLayout->addWidget(new QLabel(tr("Name:")));
	selectedNodeLayout->addWidget(leSelectedNode);
	selectedNodeLayout->addWidget(pbSelectNodeColor);
	QGroupBox *selectedNodeGroupBox = new QGroupBox(tr("Selected Node"));
	selectedNodeGroupBox->setLayout(selectedNodeLayout);
	sideBarLayout->addWidget(selectedNodeGroupBox);

	QVBoxLayout *selectedEdgeLayout = new QVBoxLayout;
	selectedEdgeLayout->addWidget(new QLabel(tr("From:")));
	selectedEdgeLayout->addWidget(leFromNode);
	selectedEdgeLayout->addWidget(new QLabel(tr("To:")));
	selectedEdgeLayout->addWidget(leToNode);
	selectedEdgeLayout->addWidget(pbSelectEdgeColor);
	QGroupBox *selectedEdgeGroupBox = new QGroupBox(tr("Selected Edge"));
	selectedEdgeGroupBox->setLayout(selectedEdgeLayout);
	sideBarLayout->addWidget(selectedEdgeGroupBox);

	mainLayout->setMenuBar(menuBar);
	mainLayout->addLayout(sideBarLayout);
	mainLayout->addWidget(graphicsView);
	graphicsScene->installEventFilter(this);

	setLayout(mainLayout);
	setWindowTitle(tr("Graphs"));
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}

MainWindow::~MainWindow() {
	delete graph;
}

void MainWindow::gvLeftMousePress(const QPointF &pos) {
	grabbedNode = graph->getNodeAt(pos);
	if (!grabbedNode.isEmpty()) {
		leSelectedNode->setText(grabbedNode);
	}
}

void MainWindow::gvLeftMouseRelease() {
	grabbedNode.clear();
}

void MainWindow::gvMouseMove(const QPointF &pos) {
	if (!grabbedNode.isEmpty()) {
		graph->moveNodeTo(grabbedNode, pos);
	}
}

void MainWindow::gvLeftMouseDoubleClick(const QPointF &pos) {
	if (rbInsertNode->isChecked()) {
		QString name = leNextNodeName->text();
		if (!name.isEmpty() && onlyLetters(name)) {
			graph->addNode(name, pos);
			leSelectedNode->setText(name);
			closeAlgorithmView();
			graph->clearNodePath();
			if (name.length() == 1 && name[0].toLower() != 'z') {
				name[0] = QChar(name[0].toAscii()+1);
				leNextNodeName->setText(name);
			}
		}
	} else if (rbRemoveNode->isChecked()) {
		QString name = graph->getNodeAt(pos);
		if (!name.isEmpty()) {
			graph->removeNode(name);
			closeAlgorithmView();
			graph->clearNodePath();
			if (name == leSelectedNode->text()) {
				leSelectedNode->clear();
			}
		}
	} else if (rbInsertEdge->isChecked()) {
		QString newClick = graph->getNodeAt(pos);
		if (lastClick.isEmpty()) {
			lastClick = newClick;
		} else if (!newClick.isEmpty() && newClick != lastClick) {
			bool ok;
			double quality = leNextEdgeQuality->text().toDouble(&ok);
			if (ok) {
				graph->addEdge(lastClick, newClick, quality);
				closeAlgorithmView();
				graph->clearNodePath();
			}
			lastClick.clear();
		}
	} else if (rbRemoveEdge->isChecked()) {
		QString newClick = graph->getNodeAt(pos);
		if (lastClick.isEmpty()) {
			lastClick = newClick;
		} else if (!newClick.isEmpty() && newClick != lastClick) {
			graph->removeEdge(lastClick, newClick);
			closeAlgorithmView();
			graph->clearNodePath();
			lastClick.clear();
		}
	}
}

void MainWindow::closeAlgorithmView() {
	resultDialog->clearListWidget();
	resultDialog->setVisible(false);
}

void MainWindow::resetLastClick() {
	lastClick.clear();
}

void MainWindow::selectNodeColor() {
	QString name = leSelectedNode->text();
	if (!name.isEmpty()) {
		QColor color;
		color = QColorDialog::getColor(graph->getNodeColor(name), this, tr("Node Color"));
		if (color.isValid()) {
			graph->setNodeColor(name, color);
		}
	}
}

void MainWindow::selectEdgeColor() {
	QString lname = leFromNode->text();
	QString rname = leToNode->text();
	if (!lname.isEmpty() && !rname.isEmpty() && graph->edgeExists(lname, rname)) {
		QColor color;
		color = QColorDialog::getColor(graph->getEdgeColor(lname, rname), this, tr("Edge Color"));
		if (color.isValid()) {
			graph->setEdgeColor(lname, rname, color);
		}
	}
}

void MainWindow::newNodeSelected(const QString &name) {
	leFromNode->setText(leToNode->text());
	leToNode->setText(name);
}

void MainWindow::selectedAlgorithmResult(const QString &result) {
	graph->markNodePath(result);
}

void MainWindow::onResultDialogClose() {
	graph->clearNodePath();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
    	QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent*>(event);
    	if (ev->button() == Qt::LeftButton) {
    		gvLeftMousePress(ev->buttonDownScenePos(Qt::LeftButton));
    	}
        return true;
    } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
    	QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent*>(event);
    	if (ev->button() == Qt::LeftButton) {
    		gvLeftMouseRelease();
    	}
        return true;
    } else if (event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
    	QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent*>(event);
    	if (ev->button() == Qt::LeftButton) {
    		gvLeftMouseDoubleClick(ev->buttonDownScenePos(Qt::LeftButton));
    	}
    	return true;
    } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
    	QGraphicsSceneMouseEvent *ev = static_cast<QGraphicsSceneMouseEvent*>(event);
    	gvMouseMove(ev->scenePos());
    	return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

void MainWindow::newGraph() {
	closeAlgorithmView();
	graph->clearNodePath();
	delete graph;
	graph = new graph::Graph(graphicsScene);
	saveAction->setEnabled(false);
}

void MainWindow::saveScreenshot() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", tr("Images (*.png *.jpg)"));
	QPixmap pixMap = QPixmap::grabWidget(graphicsView, 1, 1, graphicsScene->width(), graphicsScene->height());
	pixMap.save(fileName);

}

void MainWindow::readFromFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Graph"), "", tr("Graph (*.gra)"));
	if (!fileName.isNull()) {
		try {
			graph->readFromFile(fileName);
		} catch (graph::GraphFileError &e) {
			graph->clear();
			QMessageBox mb;
			mb.setText(tr("Could not open the file\t\t\t"));
			mb.setInformativeText(tr("There seems to be something wrong with it"));
			mb.setDetailedText(e.what());
			mb.setIcon(QMessageBox::Critical);
			mb.exec();
		}
		closeAlgorithmView();
	}
}

void MainWindow::writeToFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Graph"), "", tr("Graph (*.gra)"));
	if (!fileName.isNull()) {
		graph->writeToFile(fileName);
		lastFilePath = fileName;
		saveAction->setEnabled(true);
	}
}

void MainWindow::writeToLastFile() {
	if (!lastFilePath.isNull()) {
		graph->writeToFile(lastFilePath);
	}
}

void MainWindow::depthFirstSearch() {
	graph->clearNodePath();
	QStringList list = graph->depthFirstSearch();
	if (!resultDialog->isVisible()) {
		resultDialog->show();
	}
	resultDialog->displayStringList(list);
	resultDialog->setWindowTitle(tr("Algorithm: Depth-first search"));
}

void MainWindow::breadthFirstSearch() {
	graph->clearNodePath();
	QStringList list = graph->breadthFirstSearch();
	if (!resultDialog->isVisible()) {
		resultDialog->show();
	}
	resultDialog->displayStringList(list);
	resultDialog->setWindowTitle(tr("Algorithm: Breadth-first search"));
}

bool MainWindow::onlyLetters(const QString &string) {
	bool result = true;
	for (int i = 0; i < string.length(); ++i) {
		if (!string[i].isLetter()) {
			result = false;
		}
	}
	return result;
}