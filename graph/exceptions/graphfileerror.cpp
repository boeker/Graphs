#include "graphfileerror.h"

namespace graph {
GraphFileError::GraphFileError(const QString &msg)
	: runtime_error(msg.toStdString()) {
}
}