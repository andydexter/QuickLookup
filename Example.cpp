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

#include "src/QuickLookup.h"
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

//An example data source. Names taken from the [W3Schools SQL Tutorial](https://www.w3schools.com/sql/default.asp).
//In this case, the customer name is what will be searched for and the customer ID is what will be copied to the clipboard.
map<string, string> customerNameToID = { {"Alfreds Futterkiste", "35423431"},
	{"Antonio Moreno", "656236"},
	{"Diego Roel", "76532343"},
	{"Diego Mendel", "2567625"},
	{"Roland Mendel", "7673542347"},
	{"Folies gourmandes", "74235772"},
	{"Jose Pedro Freyre", "579924"},
	{"Patricia McKenna", "743226"},
	{"John Steel", "9261724"},
	{"Rene Phillips", "8924354"},
	{"Paula Wilson", "8325625"} };

//Returns s in lowercase letters.
string to_lower(string s) {
	string result = s;
	transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
	return result;
}

/**
Will replace the rows vector with new search results using the data source above,
*/
void QL::QuickLookup::lookup(string s) {
	// Remove all current rows
	rows.clear();
	// Search for matches in the map key and adds them to the rows vector until maxRows is reached.
	for (auto it = customerNameToID.begin(); it != customerNameToID.end() && rows.size() < maxRows; it++) {
		if (to_lower(it->first).find(to_lower(s)) != string::npos) {
			// Add the map value to the row.value string. This is what will be copied to the clipboard.
			// Add the map key to the row.key string. This is what will be displayed to the user. 
			// The display string can be anything you like, and doesn't necessarily need to be associated with the key or the value.
			rows.push_back(Row(it->second, it->first));
		}
	}
}

int main()
{
	QL::QuickLookup ql(40,10,2);
	ql.run();
	return 0;
}
