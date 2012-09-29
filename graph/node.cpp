#include "node.h"

#include <QLineF>

namespace graph {
Node::Node(QGraphicsScene *scene,
							 const QString &name,
							 const QPointF &pos)
	: name(name),
	graphicsItem(graphicsItem),
	textItem(textItem),
	scene(scene) {
		graphicsItem = scene->addEllipse(0, 0, RADIUS*2, RADIUS*2);
		graphicsItem->setBrush(Qt::white);
		graphicsItem->setPos(pos.x()-RADIUS, pos.y()-RADIUS);
		textItem = scene->addText(name);
		textItem->setPos(pos.x()-TEXTSHIFTX, pos.y()-TEXTSHIFTY);
}

Node::~Node() {
	QSet<Edge*>::iterator it;
	Node *to;
	while (set.size() > 0) {
		it = set.begin();
		to = (*it)->getNode();
		if (getEdgeTo(to) != 0) {
			removeEdge(to);
			to->removeEdge(this, true);
		}
	}
	graphicsItem->scene()->removeItem(graphicsItem);
	textItem->scene()->removeItem(textItem);
	delete graphicsItem;
	delete textItem;
}

QString Node::getName() const {
	return name;
}

bool Node::hasItem(QGraphicsItem *item) {
	if (item == graphicsItem || item == textItem) {
		return true;
	} else {
		return false;
	}
}

void Node::addEdge(Node *to, int quality) {
	QLineF line(graphicsItem->scenePos(), to->graphicsItem->scenePos());
	line.setP1(QPoint(line.p1().x()+RADIUS, line.p1().y()+RADIUS));
	line.setP2(QPoint(line.p2().x()+RADIUS, line.p2().y()+RADIUS));
	Edge *edge = new Edge(scene, to, line, quality);
	set.insert(edge);
	to->set.insert(new Edge(edge, this));
}

Edge* Node::getEdgeTo(Node *node) { //probably slow
	QSet<Edge*>::iterator it;
	for (it = set.begin(); it != set.end(); ++it) {
		if ((*it)->getNode() == node) {
			return *it;
		}
	}
	return 0;
}

void Node::removeEdge(Node *to, bool removeFromScene) {
	Edge *edge = getEdgeTo(to);
	set.remove(edge);
	if (removeFromScene) {
		edge->removeFromScene();
	}
	delete edge;
}

void Node::setZValue(const qreal &val) {
	graphicsItem->setZValue(val);
	textItem->setZValue(val);
}

void Node::moveTo(const QPointF &pos) {
	graphicsItem->setPos(pos.x()-RADIUS, pos.y()-RADIUS);
	textItem->setPos(pos.x()-TEXTSHIFTX, pos.y()-TEXTSHIFTY);
	QSet<Edge*>::iterator it;
	for (it = set.begin(); it != set.end(); ++it) {
		QLineF linepos;
		linepos.setP1(pos);
		QPointF toPos = (*it)->getNode()->graphicsItem->scenePos();
		linepos.setP2(QPointF(toPos.x()+RADIUS, toPos.y()+RADIUS));
		(*it)->moveTo(linepos);
	}
}
}