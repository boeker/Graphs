#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <QString>
#include <QMap>
#include <QGraphicsScene>
#include <QPointF>
#include "node.h"

namespace graph {
class Graph {
public:
	Graph(QGraphicsScene *);
	~Graph();
	void addNode(const QString &, const QPointF &);
	void removeNode(const QString &);
	void moveNodeTo(const QString &, const QPointF &);
	void addEdge(const QString &, const QString &, int);
	void removeEdge(const QString &, const QString &);
	QString getNodeAt(const QPointF &);
	void setZValue(const QString &, const qreal &);
	void setNodeColor(const QString &, const QColor &);
	const QColor getNodeColor(const QString &) const;
private:
	QGraphicsScene *scene;
	QMap<QString,Node*> map;
};
}

#endif