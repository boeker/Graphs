#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <QSet>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPointF>
#include <QColor>
#include "edge.h"

namespace graph {
class Node {
public:
	Node(QGraphicsScene *, const QString &, const QPointF &);
	~Node();
	QString getName() const;
	bool hasItem(QGraphicsItem *);
	void addEdge(Node *, int);
	Edge* getEdgeTo(Node*);
	void removeEdge(Node *, bool = false);
	void setZValue(const qreal &);
	void moveTo(const QPointF &);
	void setColor(const QColor &);
	const QColor getColor();
private:
	QSet<Edge*> set;
	QString name;
	QGraphicsEllipseItem *graphicsItem;
	QGraphicsTextItem *textItem;
	QGraphicsScene *scene;
	static const int RADIUS = 20;
	static const int TEXTSHIFTY = 10;
	static const int TEXTSHIFTX = 7;
};
}

#endif