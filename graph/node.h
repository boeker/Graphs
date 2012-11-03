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
#include <QStringList>
#include "edge.h"

namespace graph {
class Node {
public:
	Node(QGraphicsScene *, const QString &, const QPointF &);
	~Node();
	QString getName() const;
	bool hasItem(QGraphicsItem *);
	void addEdge(Node *, double);
	Edge* getEdgeTo(Node *) const;
	void removeEdge(Node *, bool = false);
	void setZValue(const qreal &);
	void moveTo(const QPointF &);
	void setColor(const QColor &);
	QColor getColor() const;
	void setEdgeColor(Node *, const QColor &);
	QColor getEdgeColor(Node *) const;
	double getEdgeQuality(Node *) const;
	QString toString() const;
	QStringList getNeighbourNodes() const;
private:
	QSet<Edge*> set;
	QString name;
	QGraphicsEllipseItem *graphicsItem;
	QGraphicsTextItem *textItem;
	QGraphicsScene *scene;
	static const int RADIUS = 20;
#if defined Q_WS_WIN
	static const double TEXTSHIFTY = 10.5f;
	static const double TEXTSHIFTX = 6.25f;
#elif defined Q_WS_MACX
	static const double TEXTSHIFTY = 11.5f;
	static const double TEXTSHIFTX = 8.3f;
#elif defined Q_WS_X11
	static const double TEXTSHIFTY = 11.5f; //Not yet determined
	static const double TEXTSHIFTX = 8.3f; //same here
#endif
};
}

#endif