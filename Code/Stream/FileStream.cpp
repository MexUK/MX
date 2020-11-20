#include "FileStream.h"
#include "Flows/FileStreamFlow.h"

using namespace std;
using namespace mx;

FileStream::FileStream(string& strFilePath) :
	Stream(STREAM_DEVICE_TYPE_FILE)
{
	m_pFlow->prepare(strFilePath);
}