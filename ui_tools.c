#include <curses.h>
#include <string.h>
#include <form.h>
#include <stdbool.h>

#include "ui_tools.h"
#include "string_tools.h"

static bool window_boxing = true;

void set_boxing(bool value) {
    window_boxing = value;
}

int get_center_x(WINDOW* window) {
    return getmaxx(window) / 2;
}

int get_center_y(WINDOW* window) {
    return getmaxy(window) / 2;
}

void print_centered_in(WINDOW* parent_window, int y, char* str) {
    int x = getmaxx(parent_window) / 2 - strlen(str) / 2;
    mvwprintw(parent_window, y, x, "%s", str);
    refresh();
    wrefresh(parent_window->_parent);
    wrefresh(parent_window);
}

void clear_window(WINDOW* window) {
    wclear(window);
    if (window_boxing) box(window, 0, 0);
    wrefresh(window);
}

WINDOW* show_window_in(WINDOW* parent_window, int height, int width, int y, int x) {
    WINDOW* window = derwin(parent_window, height, width, y, x);
    if (window_boxing) box(window, 0, 0);
    wrefresh(parent_window);
    wrefresh(window);
    return window;
}

WINDOW* show_window_fill(WINDOW* parent_window) {
    WINDOW* window = derwin(
        parent_window,
        getmaxy(parent_window), // height
        getmaxx(parent_window), // width
        0,                      // y
        0                       // x
    );
    if (window_boxing) box(window, 0, 0);
    wrefresh(parent_window);
    wrefresh(window);
    return window;
}

WINDOW* show_window_centered_in(WINDOW* parent_window, int height, int width) {
    WINDOW* window = derwin(
        parent_window,
        height + 2,
        width + 4,
        get_center_y(parent_window) - height / 2,
        get_center_x(parent_window) - width / 2
    );
    if (window_boxing) box(window, 0, 0);
    wrefresh(parent_window);
    wrefresh(window);
    return window;
}

WINDOW* show_window_stretched_in(WINDOW* parent_window, int height, int y) {
    int x_margin = 1;
    if (parent_window == stdscr) x_margin = 0;

    WINDOW* w = derwin(
        parent_window,
        height,
        getmaxx(parent_window) - x_margin * 2,
        y,
        x_margin
    );
    if (window_boxing) box(w, 0, 0);
    refresh();
    wrefresh(parent_window);
    wrefresh(w);
    return w;
}

int show_menu_in(WINDOW* parent_window, char** options, int menu_width) {
    int options_count = 0;
    while (options[options_count]) options_count++;

    WINDOW* menu_window = show_window_centered_in(parent_window, options_count, menu_width);
    // draw menu window border
    if (window_boxing) box(menu_window, 0, 0);
    // render options; pre-select first option
    char selected_option[menu_width];
    for (int i = 0; i < options_count; i++) {
        if (i == 0) {
            wattron(menu_window, A_STANDOUT);
        } else {
            wattroff(menu_window, A_STANDOUT);
        }
        sprintf(selected_option, "%-*s", menu_width, options[i]);
        mvwprintw(menu_window, i + 1, 2, "%s", selected_option);
    }
    wrefresh(menu_window);
    // hide user input
    noecho();
    // enabling functional keys
    keypad(menu_window, 1);
    // disable caret
    curs_set(0);
    int ch, i = 0;
    // 10 == ENTER
    while ((ch = wgetch(menu_window)) != 10) {
        // choosing logic
        sprintf(selected_option, "%-*s", menu_width, options[i]);
        mvwprintw(menu_window, i + 1, 2, "%s", selected_option);
        switch (ch) {
            case KEY_UP:
                i--;
                if (i < 0) {
                    i = options_count - 1;
                }
                break;
            case KEY_DOWN:
                i++;
                if (i > options_count - 1) {
                    i = 0;
                }
                break;
            case KEY_HOME:
                i = 0;
                break;
            case KEY_END:
                i = options_count - 1;
                break;
            default:
                break;
        }
        wattron(menu_window, A_STANDOUT);
        sprintf(selected_option, "%-*s", menu_width, options[i]);
        mvwprintw(menu_window, i + 1, 2, "%s", selected_option);
        wattroff(menu_window, A_STANDOUT);
    }
    delwin(menu_window);
    return i;
}

FIELD* new_edit_box(const char* text, int height, int width, int y, int x) {
    FIELD* f = new_field(height, width, y, x, 0, 0);
    set_field_buffer(f, 0, text);
    set_field_opts(f, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_back(f, A_UNDERLINE);
    return f;
}

bool is_edit_box(FIELD* f) {
    return field_opts(f) & O_EDIT;
}

char* get_text(FIELD* edit_box) {
    return trim_whitespaces(field_buffer(edit_box, 0));
}

FIELD* new_label(const char* text, int height, int width, int y, int x) {
    FIELD* f = new_field(height, width, y, x, 0, 0);
    set_field_buffer(f, 0, text);
    set_field_opts(f, O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    return f;
}

FIELD* new_label_autosize(const char* text, int y, int x) {
    int width = (int) strlen(text);
    FIELD* f = new_field(1, width, y, x, 0, 0);
    set_field_buffer(f, 0, text);
    set_field_opts(f, O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    return f;
}

bool is_label(FIELD* f) {
    return field_opts(f) & O_AUTOSKIP;
}

bool is_active(FIELD* f) {
    return field_opts(f) & O_ACTIVE;
}

FORM* show_form(WINDOW* form_window, FIELD** fields, int height, int width, int y, int x) {
    FORM* form = new_form(fields);
    set_form_win(form, form_window);
    set_form_sub(form, derwin(form_window, height, width, y, x));
    post_form(form);
    refresh();
    wrefresh(form_window->_parent);
    wrefresh(form_window);
    return form;
}

FORM* show_form_fill(WINDOW* form_window, FIELD** fields) {
    FORM* form = new_form(fields);
    form_driver(form, REQ_FIRST_FIELD);
    set_form_win(form, form_window);
    set_form_sub(form, derwin(form_window, form->rows, form->cols, 1, 1));
    pos_form_cursor(form);
    post_form(form);
    refresh();
    wrefresh(form_window->_parent);
    wrefresh(form_window);
    return form;
}

void handle_edit_keys(WINDOW* form_window, FORM* form, int key_char) {
    int i;
    switch (key_char) {
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;
        case KEY_BACKSPACE:
        case 127:
            // Delete the char before cursor
            form_driver(form, REQ_DEL_PREV);
            break;
        case KEY_DC:
            // Delete the char under the cursor
            form_driver(form, REQ_DEL_CHAR);
            break;
        case KEY_HOME:
            form_driver(form, REQ_BEG_LINE);
            break;
        case KEY_END:
            form_driver(form, REQ_END_LINE);
            break;
        default:
            form_driver(form, key_char);
            break;
    }
    // refresh required!
    wrefresh(form_window);
}