#pragma once

#include <string>

#include "Core/Base.h"

namespace kepler {
	namespace utility {

		std::wstring StringToWstring(const std::string& str);
		std::string WstringToString(const std::wstring& wstr);
	}
}