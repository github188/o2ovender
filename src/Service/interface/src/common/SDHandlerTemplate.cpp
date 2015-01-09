#include "SDHandlerTemplate.h"

using namespace std;

IMPL_LOGGER(SDHandlerName, logger);

SDHandlerName::SDHandlerName() : SDThreadPool(1)
{}

SDHandlerName::~SDHandlerName()
{}

void SDHandlerName::doIt()
{}
