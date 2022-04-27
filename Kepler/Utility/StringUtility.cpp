#include "kepch.h"

#include "StringUtility.h"

std::wstring kepler::utility::StringToWstring(const std::string& str)
{
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
	// std string을 c-style로 캐스팅할 경우 마지막에 '\0'이 추가되어야 하므로 
	// 문자형 단위(wchar_t) 1만큼의 추가 공간이 필요합니다.
	auto buf = new wchar_t[len + sizeof(wchar_t)];
	buf[len] = '\0';
	// 6번째 파라미터는 Byte 수를 전달하는 게 아니라 wchar 배열의 길이를 전달하므로 len + sizeof(wchar_t)가 아니라 len + 1을 전달합니다.
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), buf, len + 1);
	std::wstring wstr{ buf };
	delete[] buf;

	return wstr;
}

std::string kepler::utility::WstringToString(const std::wstring& wstr)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), nullptr, 0, nullptr, nullptr);
	// std wstring을 c_style로 캐스팅할 경우 마지막에 '\0'이 추가되어야 하므로
	// 문자형 단위(char) 1만큼의 추가 공간이 필요합니다.
	auto buf = new char[len + 1];
	buf[len] = '\0';
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), buf, len + 1, nullptr, nullptr);
	std::string str{ buf };
	delete[] buf;
	
	return std::string();
}
