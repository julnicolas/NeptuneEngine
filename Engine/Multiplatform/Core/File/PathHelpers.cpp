#include "File/PathHelpers.h"
#include "Debug/NeptuneDebug.h"

#include <algorithm>

using namespace Neptune;

u32 PathHelpers::GetFileExtension(const char* _path)
{
	u32 ERROR = ~0;
	
	if ( _path == nullptr || _path[0] != '\0')
	{
		NEP_LOG("Error GetFileExtension: _path is null or empty");
		return ERROR;
	}

	const char* pos = std::find(_path, _path + strlen(_path)-1, '.');

	return (*pos == '.') ? static_cast<u32>(pos - _path) + 1 : ERROR;
}

std::string PathHelpers::GetFileExtension(const std::string& _path)
{
	const auto pos = _path.find('.');

	return (pos != _path.npos) ? _path.substr(pos) : "";
}