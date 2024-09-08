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
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>

#if(WIN32)
	#include "ClipboardWin.h"
#else
	#include "ClipboardLin.h"	
#endif

#include "Navigation.h"

using namespace std;

namespace QL {

	struct Row {
		string value;
		string display;
		Row(string v, string d) : value(v), display(d) {};
	};

	class QuickLookup {
		//Console Window Size Configuration:
		int windowWidth, windowHeight, windowMargin, maxRows;

	public:
		//Set Window Size Configuration
		QuickLookup(int width, int height, int margin) : windowWidth(width), windowHeight(height), windowMargin(margin) { maxRows = windowHeight - 4; };
		
		//Runs Quick Lookup
		void run() {
			init_screen();
			bool keepRunning = true;
			while (keepRunning) {
				QLNav::NavAction action = search();

				// Navigate through search results
				bool keepNavigating = true;
				while (keepNavigating) {

					//Act according to navigation action
					switch (action.type) {
					case QLNav::MOVE_DOWN:
						navigate_down();
						break;
					case QLNav::MOVE_UP:
						navigate_up();
						break;
					case QLNav::GO_UPMOST:
						navigate_upmost();
						break;
					case QLNav::GO_DOWNMOST:
						navigate_downmost();
						break;
					case QLNav::COPY_AND_QUIT:
						if (rows.size() > selectedRow) { // If valid row is selected, copy to clipboard
							ClipboardUtils::copy_string_to_clipboard(rows[selectedRow].value);
						}
						keepRunning = false; // Stop outer loop
						[[fallthrough]];
					case QLNav::SEARCH: // Go to Search Again
						keepNavigating = false; // Stop inner loop
						break;
					}
					//Get next action if user hasn't quit navigation
					if(keepNavigating) action = QLNav::readAction();
				}
			}
		}

	private:

		vector<Row> rows;
		int selectedRow = -1;

		/**
		* Implement this function in your main file or derived class to populate the rows vector with search results.
		* 
		* @param s The search string entered by the user.
		* 
		* This function must **replace** the contents of the rows vector with the search results.
		* You may want to limit the number of results to the maxRows variable.
		* You may implement the search logic in any way you like.
		* 
		* See: Row struct, rows vector, example implementation in Example.cpp
		*/
		void lookup(string);

		//Enter Search mode, get search string from user and lookup results
		//Returns the first navigation action after search is complete
		QLNav::NavAction search() {
			selectedRow = -1;
			update_search_results();
			set_cursor_to_search();

#if(GETCH_INPUT)

			string s;
			QLNav::NavAction action;
			// Keep getting inputs while they're printable characters or backspace
			while (((action = QLNav::readAction()).type == QLNav::PRINTABLE_CHARACTER) || action.type == QLNav::BACKSPACE) {

				set_cursor_to_search((int)s.length() + 1);
				// Handles backspace
				if (action.type == QLNav::BACKSPACE && !s.empty()) { //BACKSPACE
					s.pop_back();
					set_cursor_to_search((int)s.length() + 1);
					cout << ' ';
				}
				// Prints printable characters as long as string length is less than the available space.
				else if(action.type == QLNav::PRINTABLE_CHARACTER && s.length() < windowWidth - 2 * windowMargin) {
					s += (char)action.navValue;
					cout << (char)action.navValue;
				}
				lookup(s);
				update_search_results();
			}
			return action;

#else

			string s;
			getline(cin, s);
			lookup(s);
			return QLNav::MOVE_DOWN;
#endif
		}

		//Set cursor position to coordinates
		void set_cursor_position(int column, int row) {
			cout << "\033[" << row << ";" << column << "H";
		}

		
        /**
        * Prints a line of text with optional formatting.
		* Centers the text according to the ALIGN_CENTER cmake option.
        * 
        * @param s The string to be printed.
        * @param row The row number where the line should be printed. **1-Based**
        * @param selected Indicates whether the line should be highlighted as selected.
        * @param fillChar The character used to fill the empty space around the string.
		* @param column The column number where the print should start. **1-Based**
        */
        void print_line(string s, int row, bool selected = false, char fillChar = ' ', int column=1) {
		column--;
        //Set cursor to beginning of search bar:
        set_cursor_position(windowMargin + column +1, row);
        //Set color scheme according to selected status:
        cout << (selected ? "\033[30;47m" : "\033[0m");
#if (ALIGN_CENTER) 
			////Calculate spacing to center the string
			int leftSpace = ((windowWidth - 2 * windowMargin - column) - (int)s.length()) / 2;
			int rightSpace = (windowWidth - 2 * windowMargin - column) - leftSpace - (int)s.length();
			cout << string(leftSpace, fillChar) << s << string(rightSpace, fillChar);
#else
			//Print string with fill characters
			cout << s << string(windowWidth - 2 * windowMargin - s.length() - column, fillChar);
#endif
        }

		//Sets window size and clears screen
		void init_screen() {
			//Set terminal size
			cout << "\033[8;" << windowHeight << ";" << windowWidth << "t";
			//Clear Screen
#if(WIN32)
			system("cls");
#else
			system("clear");
#endif
		}

		/**
		* Clears the search bar from the column position to the end
		* Sets cursor to search bar at column position.
		* Column Position is relative to the start of the search bar!
		* 
		* @param column The column number where the cursor should be placed. **1-Based**
		*/
		void set_cursor_to_search(int column = 1) {
			//Print Search Bar
			print_line("", 2, true, ' ', column);
			set_cursor_position(windowMargin + column, 2);
			cout << "\033[30;47m";
		}

		//Updates the result rows according to the current state of the rows vector
		void update_search_results() {
			for (int i = 0; i < maxRows; i++) {
				if (rows.size() > i && !rows[i].display.empty()) print_line(rows[i].display, i+4, i==selectedRow);
				else print_line("", i+4, i==selectedRow, '-');
			}
			set_cursor_position(windowMargin + 1, windowHeight);
		}

		//Select the next row. Cycles back to the first row if the last row is selected.
		void navigate_down() {
			selectedRow++;
			selectedRow %= maxRows;
			update_search_results();
		}

		//Select the last row
		void navigate_downmost() {
			if (selectedRow < maxRows - 1) {
				selectedRow = maxRows - 1;
				update_search_results();
			}
		}

		//Select the previous row. Cycles back to the last row if the first row is selected.
		void navigate_up() {
			if (selectedRow > 0) {
				selectedRow--;
				update_search_results();
			}
			else {
				navigate_downmost();
			}
		}

		//Select the first row
		void navigate_upmost() {
			if (selectedRow != 0) {
				selectedRow=0;
				update_search_results();
			}
		}
	};
}