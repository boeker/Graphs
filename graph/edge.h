// Copyright (c) 2012 Jan Böker
// MIT License

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
	Edge(QGraphicsScene *, Node *, const QLineF &, double);
	Edge(Edge *, Node *);

	void moveTo(const QLineF &);
	void setColor(const QColor &);
	QColor getColor() const;
	double getQuality() const;
	
	Node* getNode();
	void removeFromScene();
private:
	QPointF qualityTextPoint(const QLineF &);
private:
	Node *node;
	
	double quality;

	QGraphicsScene *scene;
	QGraphicsLineItem *lineItem;
	QGraphicsTextItem *textItem;
};
}

#endif