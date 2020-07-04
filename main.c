#include <curses.h>
#include <form.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ui_tools.h"
#include "game_map.h"

#define MAX_CMD_LEN 64
#define COMMAND_FIELD_INDEX 1

static WINDOW* main_window;
static WINDOW* view_window;
static WINDOW* command_window;
static int prev_screen = SCREEN_NONE;
static int current_screen = SCREEN_NONE;

void set_screen(int screen_index) {
    show_screen_in(view_window, screen_index);
    prev_screen = current_screen;
    current_screen = screen_index;
}

int main(int argc, char* argv[]) {
    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    // main window
    main_window = show_window_fill(stdscr);

    // command window
    set_boxing(false);
    command_window = show_window_stretched_in(main_window, 2, getmaxy(main_window) - 3);
    set_boxing(true);

    FIELD* fields[] = {
        new_label_autosize("Your action: ", 0, 1),
        new_edit_box("", 1, 30, 0, 14),
        NULL
    };
    FORM* command_form = show_form_fill(command_window, fields);

    // view window
    view_window = show_window_stretched_in(main_window, getbegy(command_window), 1);

    // Welcome!
    print_centered_in(
        view_window,
        get_center_y(view_window),
        "Unnamed RPG v1.0 by @f1uctus. Try 'start' or 'quit'."
    );
    if (getmaxx(stdscr) < 90 || getmaxy(stdscr) < 30) {
        // too small, you won't see some precious arts :(
        print_centered_in(
            view_window,
            get_center_y(view_window) + 1,
            // NOTE: this may break to separate lines, but what's then?
            "For better experience, please adjust your terminal's window (90x30 min.) and re-start."
        );
    } else {
        print_centered_in(
            view_window,
            get_center_y(view_window) + 1,
            "Please, don't shrink your terminal's window."
        );
    }

    // TODO: inventory
    bool has_bottle = false;

    // main game loop
    while (1) {
        // refresh all screens
        refresh();
        wrefresh(main_window);
        wrefresh(view_window);
        // set cursor to command field
        pos_form_cursor(command_form);

        int chr = getch();
        // [ENTER]
        if (chr == 10) {
            // sync current field buffer with what is displayed
            form_driver(command_form, REQ_NEXT_FIELD);
            form_driver(command_form, REQ_PREV_FIELD);

            // get command from field
            char* command = get_text(fields[COMMAND_FIELD_INDEX]);

            // clear windows
            form_driver(command_form, REQ_CLR_FIELD);
            clear_window(view_window);

            // parse command
            bool cmd_valid = true;
            if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
                // exit game
                break;
            }
            if (current_screen == SCREEN_DEATH) {
                cmd_valid = false;
            }
            if (strcmp(command, "start") == 0) {
                if (current_screen == SCREEN_NONE) {
                    set_screen(SCREEN_SPAWN);
                    continue;
                } else {
                    cmd_valid = false;
                }
            }
            if (cmd_valid) {
                const char delimiters[] = " ";

                char** tokens = malloc(MAX_CMD_LEN * sizeof(char));
                char command_buf[MAX_CMD_LEN];
                strcpy(command_buf, command);
                tokens[0] = strtok(command_buf, delimiters);

                if (tokens[0] != NULL) {
                    for (int i = 1; cmd_valid; i++) {
                        tokens[i] = strtok(NULL, delimiters);
                        if (tokens[i] == NULL) break;

                        if (strcmp(tokens[0], "go") == 0 && i > 0) {
                            if (strcmp(tokens[1], "left") == 0) {
                                if (current_screen == SCREEN_LIGHTHOUSE) {
                                    set_screen(SCREEN_MANSION);
                                } else {
                                    cmd_valid = false;
                                }
                            } else if (strcmp(tokens[1], "right") == 0) {
                                if (current_screen == SCREEN_LIGHTHOUSE) {
                                    set_screen(SCREEN_DEATH);
                                } else {
                                    cmd_valid = false;
                                }
                            } else if (strcmp(tokens[1], "forward") == 0) {
                                if (current_screen == SCREEN_SPAWN) {
                                    set_screen(SCREEN_LIGHTHOUSE);
                                    has_bottle = true;
                                } else {
                                    cmd_valid = false;
                                }
                            } else if (strcmp(tokens[1], "back") == 0) {
                                if (current_screen == SCREEN_MANSION) {
                                    set_screen(SCREEN_LIGHTHOUSE);
                                } else if (current_screen == SCREEN_BOTTLE) {
                                    set_screen(prev_screen);
                                } else {
                                    cmd_valid = false;
                                }
                            } else {
                                cmd_valid = false;
                            }
                        } else if (strcmp(tokens[0], "inspect") == 0) {
                            if (strcmp(tokens[1], "bottle") == 0 && has_bottle) {
                                // https://codebeautify.org/encrypt-decrypt
                                set_screen(SCREEN_BOTTLE);
                            } else {
                                cmd_valid = false;
                            }
                        } else {
                            cmd_valid = false;
                        }
                    }
                }
                free(tokens);
            }

            if (!cmd_valid) {
                show_screen_in(view_window, current_screen);
                print_centered_in(
                    view_window,
                    getmaxy(view_window) - 1,
                    "Invalid command!"
                );
            }
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