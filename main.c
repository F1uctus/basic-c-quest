#include <curses.h>
#include <form.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ui_tools.h"
#include "game_map.h"

#define COMMAND_FIELD_INDEX 1

#define MAP_DIR "./map/"

void print_art_centered_in(WINDOW* parent_window, char* file_name) {
    int width = 0, height = 0;
    char (* art)[ART_MAX_WIDTH] = load_art_from(file_name, &height, &width);
    int x = getmaxx(parent_window) / 2 - width / 2;
    int start_y = getbegy(parent_window) + 1;
    for (int i = 0; i < height; ++i) {
        mvwprintw(parent_window, start_y + i, x, art[i]);
    }
    refresh();
    wrefresh(parent_window);
    free(art);
}

int main(int argc, char* argv[]) {
    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    // main window
    WINDOW* main_window = show_window_fill(stdscr);

    // command window
    WINDOW* command_window = show_window_stretched_in(main_window, 3, getmaxy(main_window) - 4);
    FIELD* fields[] = {
        new_label_autosize("Your action: ", 0, 1),
        new_edit_box("", 1, 30, 0, 14),
        NULL
    };
    FORM* command_form = show_form_fill(command_window, fields);

    // view window
    WINDOW* view_window = show_window_stretched_in(main_window, getbegy(command_window) - 2, 2);

    wrefresh(main_window);
    print_centered_in(
        view_window,
        get_center_y(view_window),
        "Unnamed RPG v1.0 by @f1uctus. Try 'start' or 'quit'."
    );
    print_centered_in(
        view_window,
        get_center_y(view_window) + 1,
        "Please, don't shrink your terminal's window."
    );

    pos_form_cursor(command_form);

    // main game loop
    while (1) {
        int chr = getch();
        // [ENTER]
        if (chr == 10) {
            // sync current field buffer with what is displayed
            form_driver(command_form, REQ_NEXT_FIELD);
            form_driver(command_form, REQ_PREV_FIELD);
            char* command = get_text(fields[COMMAND_FIELD_INDEX]);
            form_driver(command_form, REQ_CLR_FIELD);

            clear_window(view_window);

            if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
                // exit game
                break;
            } else if (strcmp(command, "start") == 0) {
                print_art_centered_in(view_window, MAP_DIR "1-art.txt");
            } else {
                print_centered_in(
                    view_window,
                    1,
                    "Invalid command!"
                );
            }

            // echo input string
            // mvwprintw(view_window, getbegy(view_window), getbegx(view_window), "%*s", getmaxx(view_window) - 4, "");
            // mvwprintw(view_window, getbegy(view_window), getbegx(view_window), "%s", command);

            refresh();
            wrefresh(main_window);
            wrefresh(view_window);
            pos_form_cursor(command_form);
        } else {
            handle_edit_keys(command_window, command_form, chr);
        }
    }

    // clean up garbage
    unpost_form(command_form);
    free_form(command_form);
    for (int i = 0; fields[i]; i++) {
        free_field(fields[i]);
    }
    delwin(command_window);
    delwin(view_window);
    delwin(main_window);
    endwin();
}