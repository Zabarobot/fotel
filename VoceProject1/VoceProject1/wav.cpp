#include <iostream>
#include <string>
#include <vector>
#include <tchar.h>


#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#include <stdio.h>
using namespace std;

vector<string> findXml(wstring path);
std::wstring s2ws(const std::string& s);
/*
int main(){
	vector<string> results;
	results = findXml(L"C:\\Users\\Marek\\Desktop\\pupa\\");

	wstring wstr(results[0].begin(), results[0].end());
	wcout << (LPCTSTR)wstr.c_str();

	PlaySound(L"C:\\Users\\Marek\\Desktop\\pupa\\lean backward.wav", NULL, SND_FILENAME | SND_SYNC);

	cin.get();
	return 0;
}
*/
vector<string> findXml(wstring path){
	WIN32_FIND_DATA searchData;

	wstring::iterator p = path.end() - 1;
	wstring find = L"*.wav";

	while (p != path.begin()){
		if (*p != '\\'){
			path.erase(path.end() - 1);
		}
		else if (*p == '\\'){
			path += find;
			break;
		}
		--p;
	}

	vector<string> result;

	HANDLE sFile = FindFirstFile(path.c_str(), &searchData);
	wstring temp = searchData.cFileName;
	result.push_back(string(temp.begin(), temp.end()));

	while (FindNextFile(sFile, &searchData)){
		temp = searchData.cFileName;
		result.push_back(string(temp.begin(), temp.end()));
	}

	return result;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

