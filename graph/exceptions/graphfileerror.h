// Copyright (c) 2012 Jan Böker
// MIT License

#ifndef GRAPH_EXCEPTIONS_GRAPHFILERROR
#define GRAPH_EXCEPTIONS_GRAPHFILERROR

#include <stdexcept>
#include <QString>

namespace graph {
class GraphFileError : public std::runtime_error {
public:
	GraphFileError(const QString &);
};
}

#endif