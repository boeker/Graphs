#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>
#include <QRect>
#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>


MainWindow::MainWindow() {
	rbInsertNode = new QRadioButton("Insert Node");
	rbInsertNode->setChecked(true);
	rbRemoveNode = new QRadioButton("Remove Node");
	rbInsertEdge = new QRadioButton("Insert Edge");
	rbRemoveEdge = new QRadioButton("Remove Edge");
	leNextNodeName = new QLineEdit("A");
	leNextEdgeQuality = new QLineEdit("1");
	leSelectedNode = new QLineEdit();
	leSelectedNode->setReadOnly(true);
	pbSelectedNodeColor = new QPushButton("Select Color");
	graphicsView = new QGraphicsView;
	QRect screen = QApplication::desktop()->screenGeometry();
	graphicsScene = new QGraphicsScene(0, 0, screen.width()/2, screen.height()/2);
	graph = new graph::Graph(graphicsScene);
	graphicsView->setScene(graphicsScene);
	lastClick.clear();
	grabbedNode.clear();

	connect(rbInsertEdge, SIGNAL(pressed()), this, SLOT(resetLastClick()));
	connect(rbRemoveEdge, SIGNAL(pressed()), this, SLOT(resetLastClick()));
	connect(pbSelectedNodeColor, SIGNAL(pressed()), this, SLOT(selectNodeColor()));

	menuBar = new QMenuBar(this);
	fileMenu = menuBar->addMenu("File");
	newAction = new QAction("New", this);
	connect(newAction, SIGNAL(triggered()), this, SLOT(newAct()));
	fileMenu->addAction(newAction);
	saveScreenshotAction = new QAction("Save Screenshot", this);
	connect(saveScreenshotAction, SIGNAL(triggered()), this, SLOT(saveScreenshotAct()));
	fileMenu->addAction(saveScreenshotAction);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *sideBarLayout = new QVBoxLayout;
	sideBarLayout->setAlignment(Qt::AlignTop);

	QVBoxLayout *actionLayout = new QVBoxLayout;
	actionLayout->addWidget(rbInsertNode);
	actionLayout->addWidget(rbRemoveNode);
	actionLayout->addWidget(rbInsertEdge);
	actionLayout->addWidget(rbRemoveEdge);
	QGroupBox *actionGroupBox = new QGroupBox("Action");
	actionGroupBox->setLayout(actionLayout);
	sideBarLayout->addWidget(actionGroupBox);

	QVBoxLayout *nextNodeLayout = new QVBoxLayout;
	nextNodeLayout->addWidget(new QLabel("Name:"));
	nextNodeLayout->addWidget(leNextNodeName);
	QGroupBox *nextNodeGroupBox = new QGroupBox("Next Node");
	nextNodeGroupBox->setLayout(nextNodeLayout);
	sideBarLayout->addWidget(nextNodeGroupBox);

	QVBoxLayout *nextEdgeLayout = new QVBoxLayout;
	nextEdgeLayout->addWidget(new QLabel("Quality:"));
	nextEdgeLayout->addWidget(leNextEdgeQuality);
	QGroupBox *nextEdgeGroupBox = new QGroupBox("Next Edge");
	nextEdgeGroupBox->setLayout(nextEdgeLayout);
	sideBarLayout->addWidget(nextEdgeGroupBox);

	QVBoxLayout *selectedNodeLayout = new QVBoxLayout;
	selectedNodeLayout->addWidget(new QLabel("Name:"));
	selectedNodeLayout->addWidget(leSelectedNode);
	selectedNodeLayout->addWidget(pbSelectedNodeColor);
	QGroupBox *selectedNodeGroupBox = new QGroupBox("Selected Node");
	selectedNodeGroupBox->setLayout(selectedNodeLayout);
	sideBarLayout->addWidget(selectedNodeGroupBox);

	mainLayout->setMenuBar(menuBar);
	mainLayout->addLayout(sideBarLayout);
	mainLayout->addWidget(graphicsView);
	graphicsScene->installEventFilter(this);

	setLayout(mainLayout);
	setWindowTitle("Graphs");
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}

MainWindow::~MainWindow() {
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
			if (name.length() == 1 && name[0].toLower() != 'z') {
				name[0] = QChar(name[0].toAscii()+1);
				leNextNodeName->setText(name);
			}
		}
	} else if (rbRemoveNode->isChecked()) {
		QString name = graph->getNodeAt(pos);
		if (!name.isEmpty()) {
			graph->removeNode(name);
			if (name == leSelectedNode->text()) {
				leSelectedNode->setText("");
			}
		}
	} else if (rbInsertEdge->isChecked()) {
		QString newClick = graph->getNodeAt(pos);
		if (lastClick.isEmpty()) {
			lastClick = newClick;
		} else if (!newClick.isEmpty() && newClick != lastClick) {
			bool ok;
			int quality = leNextEdgeQuality->text().toInt(&ok, 10);
			if (ok) {
				graph->addEdge(lastClick, newClick, quality);
				leNextEdgeQuality->setText(QString::number(++quality));
			}
			lastClick.clear();
		}
	} else if (rbRemoveEdge->isChecked()) {
		QString newClick = graph->getNodeAt(pos);
		if (lastClick.isEmpty()) {
			lastClick = newClick;
		} else if (!newClick.isEmpty() && newClick != lastClick) {
			graph->removeEdge(lastClick, newClick);
			lastClick.clear();
		}
	}
}

void MainWindow::resetLastClick() {
	lastClick.clear();
}

void MainWindow::selectNodeColor() {
	QString name = leSelectedNode->text();
	if (!name.isEmpty()) {
		QColor color;
		color = QColorDialog::getColor(graph->getNodeColor(name), this, "Node Color");
		graph->setNodeColor(name, color);
	}
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

void MainWindow::newAct() {
	delete graph;
	graph = new graph::Graph(graphicsScene);
}

void MainWindow::saveScreenshotAct() {
	QString fileName = QFileDialog::getSaveFileName(this, "Save Screenshot", "", "Images (*.png *.jpg)");
	QPixmap pixMap = QPixmap::grabWidget(graphicsView, 1, 1, graphicsScene->width(), graphicsScene->height());
	pixMap.save(fileName);

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