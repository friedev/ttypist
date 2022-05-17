#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// curses key code for escape, not defined anywhere?
#define KEY_ESCAPE 27

// Color pairs
enum {
	PAIR_DEFAULT,
	PAIR_CORRECT,
	PAIR_INCORRECT,
	PAIR_UNTYPED,
	PAIR_COUNT,
};

// Globals
char *TEST_STR;
char *INPUT_STR;
size_t TEST_STR_LEN;
size_t INPUT_IDX;

// Start curses with desired settings
void init_curses()
{
	initscr();
	cbreak();
	keypad(stdscr, true);
	noecho();
	ESCDELAY = 25;
}

// Initialize curses color pairs
void init_colors()
{
	start_color();
	use_default_colors();
	init_pair(PAIR_CORRECT, COLOR_GREEN, -1);
	init_pair(PAIR_INCORRECT, COLOR_BLACK, COLOR_RED);
	init_pair(PAIR_UNTYPED, COLOR_WHITE, -1);
}

// Draw the test string with the input string overlaid
void draw()
{
	for (size_t i = 0; i < TEST_STR_LEN; i++) {
		int color_pair;
		char display_char;

		if (i >= INPUT_IDX) {
			color_pair = PAIR_UNTYPED;
			display_char = TEST_STR[i];
		} else {
			display_char = INPUT_STR[i];
			if (INPUT_STR[i] == TEST_STR[i]) {
				color_pair = PAIR_CORRECT;
			} else {
				color_pair = PAIR_INCORRECT;
			}
		}

		attron(COLOR_PAIR(color_pair));
		addch(display_char);
		attroff(COLOR_PAIR(color_pair));
	}

	// Move the cursor to INPUT_IDX for a better typing experience
	// addch wraps as needed, but move needs the actual coords
	int max_x = getmaxx(stdscr);
	move(INPUT_IDX / max_x, INPUT_IDX % max_x);
}

// Handle an inputted key code
// Return true if execution should continue, else false
bool handle_input(int input)
{
	// Any printable characters typed are added to the input string
	if (isprint(input)) {
		INPUT_STR[INPUT_IDX] = (char) input;
		INPUT_IDX++;
		return true;
	}

	// Backspace deletes the last character
	if (input == KEY_BACKSPACE && INPUT_IDX > 0) {
		INPUT_IDX--;
		return true;
	}

	if (input == KEY_ESCAPE) {
		// Escape quits the program
		return false;
	}

	// Ignore invalid input
	return true;
}

// Print the results of the typing test to standard output
// Accuracy is determined only by the characters of the final string
// Backspaced errors are not tracked or penalized
// WPM is not reduced by errors
// This behavior is subject to change
void print_results(time_t start_time, time_t end_time)
{
	time_t seconds = end_time - start_time;
	double words = (double) TEST_STR_LEN / 5.0;
	double minutes = (double) seconds / 60.0;
	double wpm = words / minutes;

	int mistakes = 0;
	for (size_t i = 0; i < TEST_STR_LEN; i++) {
		if (TEST_STR[i] != INPUT_STR[i]) {
			mistakes++;
		}
	}

	double accuracy = (double)(TEST_STR_LEN - mistakes)
		/ (double) TEST_STR_LEN
		* 100.0;

	printf(
		"Words Per Minute: %.f\n"
		"Mistakes: %d\n"
		"Accuracy: %.f%%\n",
		wpm,
		mistakes,
		accuracy
	);
}

// Parse arguments and run main loop
int main(int argc, char **argv)
{
	// Do not silently ignore extra arguments
	if (argc != 2) {
		printf("Usage: %s test_string\n", argv[0]);
		return 1;
	}

	TEST_STR = argv[1];
	TEST_STR_LEN = strlen(TEST_STR);

	// Exit if given a 0 length string rather than launching curses
	if (TEST_STR_LEN == 0) {
		return 0;
	}

	INPUT_STR = (char *)malloc(TEST_STR_LEN + 1);
	INPUT_STR[0] = '\0';
	INPUT_IDX = 0;

	init_curses();
	init_colors();

	time_t start_time = time(NULL);
	bool typing = true;
	while (typing && INPUT_IDX < TEST_STR_LEN) {
		draw();
		refresh();
		typing = handle_input(getch());
		erase();
	}
	// Stop the timer as soon as possible
	// Any time spent closing curses will not be counted
	time_t end_time = time(NULL);
	endwin();

	// If the user quit early, don't calculate stats
	if (typing) {
		print_results(start_time, end_time);
	}

	free(INPUT_STR);

	return 0;
}
