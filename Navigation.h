#if(WIN32)
#include<conio.h>
#else
#include<stdio.h>
#endif

using namespace std;
namespace QLNav {

	enum NavType {
		MOVE_UP,
		MOVE_DOWN,
		GO_UPMOST,
		GO_DOWNMOST,
		SEARCH,
		COPY_AND_QUIT,
		BACKSPACE,
		UNDEFINED,
		PRINTABLE_CHARACTER,
		CONTROL_SEQUENCE
	};

	struct NavAction {
		NavType type;
		int navValue;
	};

	NavAction processKey(int key, bool control_sequence=false) {
		//If previous input was control sequence, now process second code
		NavType type;
		if (control_sequence) {
			switch (key) { // SECOND ARROW KEY CALL
			case 0x48: // UP ARROW
			case 0x4B: // LEFT ARROW
				type = MOVE_UP;
				break;
			case 0x50: // DOWN ARROW
			case 0x4D: // RIGHT ARROW
				type = MOVE_DOWN;
				break;
			case 0x47: // HOME
			case 0x49: // PG UP
				type = GO_UPMOST;
				break;
			case 0x4F: // END
			case 0x51: // PG DOWN
				type = GO_DOWNMOST;
				break;
			default:
				type = UNDEFINED;
				break;
			}
			return NavAction(type, 0xe000 + key); // Return full key code including control sequence
		} 
		//If this key is not part of a control sequence, process key
		switch (key) {
		case 0x0D: // ENTER; GO Down
			type = MOVE_DOWN;
			break;
		case 0x03: // CTRL+C; Copy and Quit QL 
			type = COPY_AND_QUIT;
			break;
		case 0x1A: // CTRL+Z; Search Again
			type = SEARCH;
			break;
		case 0x08: // BACKSPACE
			type = BACKSPACE;
			break;
		case 0xe0: // First Special key codes
		case 0x00:
			type = CONTROL_SEQUENCE;
			break;
		default:
			if (0x20 <= key && key <= 0x7e) type = PRINTABLE_CHARACTER;
			else type = UNDEFINED;
			break;
		}
		return NavAction(type, key);
	}

	NavAction readAction() {
		NavAction action = processKey(_getch(), false);
		if (action.type == CONTROL_SEQUENCE) {
			//If control sequence, process second code
			action = processKey(_getch(), true);
		}
		return action;
	}
}