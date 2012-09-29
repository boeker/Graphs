#include "graph.h"

namespace graph {
Graph::Graph(QGraphicsScene *scene)
	: scene(scene) {
}

Graph::~Graph() {
	QMap<QString,Node*>::const_iterator it;
	while (map.size() > 0) {
		it = map.constBegin();
		removeNode(it.key());
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

void Graph::addEdge(const QString &l, const QString &r, int quality) {
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
}