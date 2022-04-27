#include "kepch.h"

#include "StringUtility.h"

std::wstring kepler::utility::StringToWstring(const std::string& str)
{
	int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
	// std string�� c-style�� ĳ������ ��� �������� '\0'�� �߰��Ǿ�� �ϹǷ� 
	// ������ ����(wchar_t) 1��ŭ�� �߰� ������ �ʿ��մϴ�.
	auto buf = new wchar_t[len + sizeof(wchar_t)];
	buf[len] = '\0';
	// 6��° �Ķ���ʹ� Byte ���� �����ϴ� �� �ƴ϶� wchar �迭�� ���̸� �����ϹǷ� len + sizeof(wchar_t)�� �ƴ϶� len + 1�� �����մϴ�.
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), buf, len + 1);
	std::wstring wstr{ buf };
	delete[] buf;

	return wstr;
}

std::string kepler::utility::WstringToString(const std::wstring& wstr)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), nullptr, 0, nullptr, nullptr);
	// std wstring�� c_style�� ĳ������ ��� �������� '\0'�� �߰��Ǿ�� �ϹǷ�
	// ������ ����(char) 1��ŭ�� �߰� ������ �ʿ��մϴ�.
	auto buf = new char[len + 1];
	buf[len] = '\0';
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), buf, len + 1, nullptr, nullptr);
	std::string str{ buf };
	delete[] buf;
	
	return std::string();
}
