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
				search();

				// Navigate through search results
				bool keepNavigating = true;
				while (keepNavigating) {
					switch (_getch()) {
					case 0xE0: //ARROW CONTROL SEQUENCE
						switch (_getch()) { //SECOND ARROW KEY CALL
						case 0x48: // UP ARROW
						case 0x4B: // LEFT ARROW
							navigate_up();
							break;
						case 0x50: // DOWN ARROW
						case 0x4D: // RIGHT ARROW
							navigate_down();
							break;
						case 0x47: // HOME
						case 0x49: // PG UP
							navigate_upmost();
							break;
						case 0x4F: // END
						case 0x51: // PG DOWN
							navigate_downmost();
							break;
						}
						break;
					case 0x0D: // ENTER; GO Down
						navigate_down();
						break;
					case 0x03: // CTRL+C; Copy and Quit QL 
						if (rows.size() > selectedRow) {
							ClipboardUtils::copy_string_to_clipboard(rows[selectedRow].value);
						}
						keepRunning = false;
						[[fallthrough]];
					case 0x1A: // CTRL+Z; Search Again
						keepNavigating = false;
						break;
					}
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

		//Get search string from user and lookup results
		void search() {
			selectedRow = -1;
			update_search_results();
			set_cursor_to_search();
#if(GETCH_INPUT)
			string s;
			char c;
			//Keep getting characters until ENTER or CTRL+C
			while ((c = _getch()) != 0x0D && c != 0x3) {
				set_cursor_to_search((int)s.length() + 1);
				//Handles backspace
				if (c == 0x08) { //BACKSPACE
					if (!s.empty()) {
						s.pop_back();
						set_cursor_to_search((int)s.length() + 1);
						cout << ' ';
					}
				}
				//Stop handling characters after max length has been reached.
				else if (s.length() == windowWidth - 2 * windowMargin) continue;
				//Prints printable characters
				else if(0x20<=c && c<=0x7e) {
					s += c;
					cout << c;
				}
				//Handle Control Sequence
				else if (c == 0x0 || (uint8_t)c == 0xE0) {
					//Consume Scan Code and Do Nothing
					_getch();
					continue;
				}
				lookup(s);
				update_search_results();
		}
#else
			string s;
			getline(cin, s);
			lookup(s);
#endif
			navigate_upmost();
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
			//Set terminal size and clear terminal
			string command = "mode " + to_string(windowWidth) + ", " + to_string(windowHeight) + " && cls";
			system(command.c_str());
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