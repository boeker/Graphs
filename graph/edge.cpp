#include "edge.h"

#include <QPen>


namespace graph {
Edge::Edge(QGraphicsScene *scene, Node *node, const QLineF &linePos, double quality)
	: node(node),
	  quality(quality),
	  scene(scene) {
	lineItem = scene->addLine(linePos);
	lineItem->setZValue(-1);
	textItem = scene->addText(QString::number(quality));
	textItem->setPos(qualityTextPoint(linePos));
	textItem->setZValue(-0.5);
}

Edge::Edge(Edge *edge, Node *node)
	: node(node),
	  quality(edge->quality),
	  scene(edge->scene),
	  lineItem(edge->lineItem),
	  textItem(edge->textItem) {
}

void Edge::moveTo(const QLineF &linePos) {
	lineItem->setLine(linePos);
	textItem->setPos(qualityTextPoint(linePos));
}

QPointF Edge::qualityTextPoint(const QLineF &linePos) {
	QPointF middlePoint;
	middlePoint.setX((linePos.x1() + linePos.x2()) / 2);
	middlePoint.setY((linePos.y1() + linePos.y2()) / 2);
	middlePoint.setY(middlePoint.y()-20);
	return middlePoint;
}

void Edge::setColor(const QColor &color) {
	lineItem->setPen(color);
}

QColor Edge::getColor() const {
	return lineItem->pen().color();
}

double Edge::getQuality() const {
	return quality;
}

Node* Edge::getNode() {
	return node;
}

void Edge::removeFromScene() {
	lineItem->scene()->removeItem(lineItem);
	delete lineItem;
	textItem->scene()->removeItem(textItem);
	delete textItem;
}
}