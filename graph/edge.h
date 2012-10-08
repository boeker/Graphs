#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QPointF>

namespace graph {
class Node;
}

namespace graph {
class Edge {
public:
	Edge(QGraphicsScene *, Node *, const QLineF &, int);
	Edge(Edge *, Node *);
	Node* getNode();
	void removeFromScene();
	void moveTo(const QLineF &);
	void setColor(const QColor &);
	QColor getColor() const;
	int getQuality() const;
private:
	QPointF qualityTextPoint(const QLineF &);
	QGraphicsScene *scene;
	Node *node;
	QGraphicsLineItem *lineItem;
	QGraphicsTextItem *textItem;
	int quality;
};
}

#endif