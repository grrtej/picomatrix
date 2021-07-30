#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

/* Generates a random integer n such that a <= n <= b. */
int randint(int a, int b)
{
    return a + rand() % (b + 1);
}

// Entrypoint!
int main(void)
{
    /* 1. Perform ncurses initialization. */
    // ncurses is what we use to produce colored output and access other advanced features of the terminal.
    initscr(); // Required to use any other ncurses function.
    start_color(); // Request that we want colored output.
    use_default_colors(); // Use our terminal's existing colors.
    init_pair(COLOR_GREEN, COLOR_GREEN, -1); // Green text, transparent background (instead of the default black background).
    init_pair(COLOR_WHITE, COLOR_WHITE, -1); // Same as above but for white text.

    /* 2. Get terminal's current width and height. */
    int width, height;
    getmaxyx(stdscr, height, width);

    /* 3. Create a matrix based on the dimensions we got from above. */
    int matrix[height][width];
    memset(matrix, 0, sizeof(int) * height * width); // Set every element to zero.

    /* 4. Loop till infinity (unless Ctrl-C is pressed). */
    while (true) {
        /* 5. Calculate the next iteration of matrix. */
        for (int x = 0; x < width; x++) { // For each column,
            for (int y = height - 1; y >= 0; y--) { // begin from bottom-most row.
                if (y != 0) { // If not top-most row,
                    matrix[y][x] = matrix[y - 1][x]; // then get current cell's value from the cell above.
                } else { // otherwise it's the top-most row.
                    if (matrix[y][x]) { // if top-most cell is non-zero
                        matrix[y][x]--; // then decrement its value.
                    } else { // otherwise top-most cell is zero.
                        // Roll a 120-faced die to decide whether we want to shoot a rain drop down.
                        // We have a 1/120 probability of doing it.
                        if (randint(1, 120) == 7) { // 7 is arbitrary, you can use anything in range here.
                            // Give the drop and its trial a height of atleast 2 and atmost window's height.
                            matrix[y][x] = randint(2, height);
                        }
                    }
                }
            }
        }

        /* 6. Render the calculated matrix. */
        for (int y = 0; y < height; y++) { // Begin at the top-left cell,
            for (int x = 0; x < width; x++) { // and do the following for each cell till bottom-right cell.
                if (matrix[y][x]) { // If the cell is non-zero,
                    // Check if the cell is tip of rain drop: Cell below it should be zero.
                    // Also make sure we don't check outside the matrix.
                    if (y != height - 1 && matrix[y + 1][x] == 0) { // If cell is the tip,
                        attron(COLOR_PAIR(COLOR_WHITE)); // color it white.
                    } else { // otherwise it is rain drop's trial.
                        attron(COLOR_PAIR(COLOR_GREEN)); // color it green!
                    }
                    mvaddch(y, x, randint(33, 126)); // Place a random printable ASCII character on terminal to represent the cell.
                } else { // otherwise cell is zero.
                    mvaddch(y, x, ' '); // Just place a space to represent 'absence' of rain drop.
                }
            }
            refresh(); // After going through one row, draw it to the screen.
            napms(1); // This loop is too fast to see the effect clearly, so we add a 1ms delay here.
        }
    }

    /* 7. Cleanup */
    endwin();

    return 0;
}
