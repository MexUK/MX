#include "StreamException.h"

using namespace mx;

StreamException::StreamException(EStreamError uiStreamErrorId) :
	m_uiStreamErrorId(uiStreamErrorId)
{
}