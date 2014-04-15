// Copyright (c) 2012 Jan Böker
// MIT License

#include "graphfileerror.h"

namespace graph {
GraphFileError::GraphFileError(const QString &msg)
	: runtime_error(msg.toStdString()) {
}
}