#pragma once

#include<Windows.h>
#include<string>
using namespace std;

namespace ClipboardUtils {
	void copy_string_to_clipboard(string s) {
		HGLOBAL globMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(s));
		if (globMem==0) throw "Failed to allocate memory for clipboard";
		HGLOBAL writeableMem =  GlobalLock(globMem);
		memcpy(writeableMem, s.c_str(), sizeof(s));
		GlobalUnlock(globMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, globMem);
		CloseClipboard();
	}
}