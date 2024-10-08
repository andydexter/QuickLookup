/*
MIT License

Copyright (c) 2024 Andreas Nicolaou

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include<Windows.h>
#include<string>
using namespace std;

namespace ClipboardUtils {
	void copy_string_to_clipboard(string s) {
		UINT format = CF_TEXT;
		for (char c : s) {
			if (c > 127) {
				format = CF_UNICODETEXT;
				break;
			}
		}
		HGLOBAL globMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(s));
		if (globMem==0) throw "Failed to allocate memory for clipboard";
		HGLOBAL writeableMem =  GlobalLock(globMem);
		if (writeableMem == 0) throw "Failed to lock memory for clipboard";
		memcpy(writeableMem, s.c_str(), sizeof(s));
		GlobalUnlock(globMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(format, globMem);
		CloseClipboard();
	}
}