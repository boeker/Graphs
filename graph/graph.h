#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <QString>
#include <QMap>
#include <QGraphicsScene>
#include <QPointF>
#include <QStringList>
#include "node.h"
#include "exceptions/graphfileerror.h"

namespace graph {
class Graph {
public:
	Graph(QGraphicsScene *);
	~Graph();
	void clear();
	void readFromFile(const QString &) throw (GraphFileError);
	void writeToFile(const QString &);
	void addNode(const QString &, const QPointF &);
	void removeNode(const QString &);
	void moveNodeTo(const QString &, const QPointF &);
	void addEdge(const QString &, const QString &, double);
	void removeEdge(const QString &, const QString &);
	bool edgeExists(const QString &, const QString &);
	QString getNodeAt(const QPointF &);
	void setZValue(const QString &, const qreal &);
	void setNodeColor(const QString &, const QColor &);
	const QColor getNodeColor(const QString &) const;
	void setEdgeColor(const QString &, const QString &, const QColor &);
	const QColor getEdgeColor(const QString &, const QString &);
	void markNodePath(const QString &);
	void clearNodePath();
	QStringList depthFirstSearch();
	QStringList breadthFirstSearch();
private:
	void depthFirstSearch(QStringList &,const QString &);
	QGraphicsScene *scene;
	QMap<QString,Node*> map;
	static const char* const MAGICNUMBER;
	static const char* const RESULTSEPERATOR;
};
}

#endif