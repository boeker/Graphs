#include "graph.h"

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>

const char* const  graph::Graph::MAGICNUMBER = "GRAPH";
const char* const  graph::Graph::RESULTSEPERATOR = "->";

namespace graph {
Graph::Graph(QGraphicsScene *scene)
	: scene(scene) {
}

Graph::~Graph() {
	clear();
}

void Graph::clear() {
	QMap<QString,Node*>::const_iterator it;
	while (map.size() > 0) {
		it = map.constBegin();
		removeNode(it.key());
	}
}

void Graph::readFromFile(const QString &fileName) throw (GraphFileError) {
	QFile file(fileName);
	if (!file.exists()) {
		return;
	}
	clear();
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream stream(&file);
	QString magicnum = stream.readLine();
	if (magicnum != MAGICNUMBER) {
		throw GraphFileError(QWidget::tr("The file is not a valid graph file"));
	}
	QString nodeLine = stream.readLine();
	if (nodeLine.isNull()) {
		throw GraphFileError(QWidget::tr("Unexpected end of file"));
	}
	QStringList nodes = nodeLine.split(";");
	QStringList::iterator it;
	QMap<int,QString> nodeNumNames;
	int counter = 0;
	for (it = nodes.begin();it != nodes.end(); ++it) {
		QString node = *it;
		QString name = node.left(node.indexOf(":"));
		node.remove(0, node.indexOf(":")+1);
		QStringList properties = node.split(",");
		if (name.isEmpty() || properties.size() != 3) {
			throw GraphFileError(QWidget::tr("Error in the node description"));
		}
		QColor color(properties[0]);
		if (!color.isValid()) {
			throw GraphFileError(QWidget::tr("Node \"%1\" has an invalid Color: \"%2\"").arg(name).arg(properties[0]));
		}
		bool ok;
		QPointF pos;
		pos.setX(properties[1].toDouble(&ok));
		if (!ok) {
			throw GraphFileError(QWidget::tr("Node \"%1\" has an invalid X-Coordinate: \"%2\"").arg(name).arg(properties[1]));
		}
		pos.setY(properties[2].toDouble(&ok));
		if (!ok) {
			throw GraphFileError(QWidget::tr("Node \"%1\" has an invalid Y-Coordinate: \"%2\"").arg(name).arg(properties[2]));
		}
		nodeNumNames.insert(counter++, name);
		addNode(name, pos);
		setNodeColor(name, color);
	}
	QString nextLine;
	QStringList edgeDetails;
	while (!(nextLine = stream.readLine()).isNull()) {
		edgeDetails.append(nextLine);
	}
	if (edgeDetails.size() != map.size()) {
		throw GraphFileError(QWidget::tr("Invalid number of lines"));
	}
	counter = 0;
	for (it = edgeDetails.begin(); it != edgeDetails.end(); ++it) {
		QStringList details = it->split(";");
		if (details.size() != map.size()) {
			throw GraphFileError(QWidget::tr("Invalid number of edge details in the following line: \"%1\"").arg(*it));
		}
		int innerCounter = 0;
		QStringList::iterator it2;
		for (it2 = details.begin(); it2 != details.end(); ++it2) {
			if (!it2->isEmpty()) {
				QString detail = *it2;
				QColor color = (detail.left(detail.indexOf(",")));
				if (!color.isValid()) {
					throw GraphFileError(QWidget::tr("An edge has an invalid color: \"%1\"").arg(detail.left(detail.indexOf(","))));
				}
				detail.remove(0, detail.indexOf(",")+1);
				bool ok;
				double quality = detail.toDouble(&ok);
				if (!ok) {
					throw GraphFileError(QWidget::tr("An Edge has an invalid quality: \"%1\"").arg(detail));
				}
				QString from = nodeNumNames.value(counter);
				QString to = nodeNumNames.value(innerCounter);
				if (!edgeExists(from, to)) {
					addEdge(from, to, quality);
					setEdgeColor(from, to, color);
				}
			}
			++innerCounter;
		}
		++counter;
	}
}

void Graph::writeToFile(const QString &fileName) {
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream stream(&file);
	stream << QString(MAGICNUMBER) + "\n";
	const QList<QString> nodeNames = map.keys();
	QList<QString>::const_iterator it;
	for (it = nodeNames.constBegin(); it != nodeNames.constEnd();) {
		stream << map.value(*it)->toString();
		if (++it != nodeNames.constEnd()) {
			stream << ";";
		}
	}
	stream << "\n";
	for (it = nodeNames.constBegin(); it != nodeNames.constEnd();) {
		QList<QString>::const_iterator it2;
		for (it2 = nodeNames.constBegin(); it2 != nodeNames.constEnd();) {
			Node *to = map.value(*it);
			Node *from = map.value(*it2);
			if (from != to && from->getEdgeTo(to) != 0) {
				stream << from->getEdgeColor(to).name() << ",";
				stream << from->getEdgeQuality(to);
			}
			if (++it2 != nodeNames.constEnd()) {
				stream << ";";
			}
		}
		if (++it != nodeNames.constEnd()) {
			stream << "\n";
		}
	}
}

void Graph::addNode(const QString &name, const QPointF &pos) {
	if (!map.contains(name)) {
		map.insert(name, new Node(scene, name, pos));	
	}
}

void Graph::removeNode(const QString &name) {
	if (map.contains(name)) {
		delete map.take(name);
	}
}

void Graph::moveNodeTo(const QString &name, const QPointF &pos) {
	map.value(name)->moveTo(pos);
}

void Graph::addEdge(const QString &l, const QString &r, double quality) {
	if (map.value(l)->getEdgeTo(map.value(r)) == 0) {
		map.value(l)->addEdge(map.value(r), quality);
	}
}

void Graph::removeEdge(const QString &l, const QString &r) {
	if (map.value(l)->getEdgeTo(map.value(r)) != 0) {
		map.value(l)->removeEdge(map.value(r));
		map.value(r)->removeEdge(map.value(l), true);
	}
}

bool Graph::edgeExists(const QString &l, const QString &r) {
	if (map.value(l)->getEdgeTo(map.value(r)) != 0) {
		return true;
	} else {
		return false;
	}
}

QString Graph::getNodeAt(const QPointF &pos) { //probably slow
	QGraphicsItem *item = scene->itemAt(pos);
	const QList<Node*> nodeList = map.values();
	QList<Node*>::const_iterator it;
	for (it = nodeList.constBegin(); it != nodeList.constEnd(); ++it) {
		if ((*it)->hasItem(item)) {
			return (*it)->getName();
			break;
		}
	}
	return QString("");
}

void Graph::setZValue(const QString &name, const qreal &val) {
	map.value(name)->setZValue(val);
}

void Graph::setNodeColor(const QString &name, const QColor &color) {
	map.value(name)->setColor(color);
}

const QColor Graph::getNodeColor(const QString &name) const {
	return map.value(name)->getColor();
}

void Graph::setEdgeColor(const QString &l, const QString &r, const QColor &color) {
	map.value(l)->setEdgeColor(map.value(r), color);
}

const QColor Graph::getEdgeColor(const QString &l, const QString &r) {
	return map.value(l)->getEdgeColor(map.value(r));
}

void Graph::markNodePath(const QString &nodeString) {
	QStringList nodes = nodeString.split(RESULTSEPERATOR);
	QColor color(Qt::red);
	//double increase = 255/(nodes.size()-1);
	while (!nodes.isEmpty()) {
		map.value(nodes.front())->setColor(color);
		nodes.removeFirst();
		//color.setGreenF(color.greenF()+increase);
		color = color.lighter(115);
	}
}

QStringList Graph::depthFirstSearch() {
	QStringList nodes = map.keys();
	nodes.sort();
	QStringList results;
	QStringList::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it) {	
		QStringList visited;
		depthFirstSearch(visited, *it);
		results.append(visited.join(RESULTSEPERATOR));
	}
	return results;
}

void Graph::depthFirstSearch(QStringList &visited, const QString &name) {
	visited.append(name);
	Node *node = map.value(name);
	QStringList neighbourNodes = node->getNeighbourNodes();
	QStringList::iterator it;
	for (it = neighbourNodes.begin(); it != neighbourNodes.end(); ++it) {
		if (!visited.contains(*it)) {
			depthFirstSearch(visited, *it);
		}
	}
}

QStringList Graph::breadthFirstSearch() {
	QStringList nodes = map.keys();
	nodes.sort();
	QStringList results;
	QStringList::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it) {	
		QStringList visited;
		QStringList toDo;
		toDo.append(*it);
		visited.append(*it);
		while (!toDo.isEmpty()) {
			QString node = toDo.front();
			toDo.removeFirst();
			QStringList neighbourNodes = map.value(node)->getNeighbourNodes();
			QStringList::iterator it;
			for (it = neighbourNodes.begin(); it != neighbourNodes.end(); ++it) {
				if (!visited.contains(*it)) {
					toDo.append(*it);
					visited.append(*it);
				}
			}
		}
		results.append(visited.join("->"));
	}
	return results;
}
}