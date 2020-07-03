#ifndef RPG_UI_TOOLS_H
#define RPG_UI_TOOLS_H

#endif //RPG_UI_TOOLS_H

int get_center_x(WINDOW* window);

int get_center_y(WINDOW* window);

/// Prints string centered in specified window,
/// with given y coordinate.
void print_centered_in(WINDOW* parent_window, int y, char* str);

void clear_window(WINDOW* window);

WINDOW* show_window_in(WINDOW* parent_window, int height, int width, int y, int x);

/// Creates a new empty window occupying entire terminal screen.
WINDOW* show_window_fill(WINDOW* parent_window);

/// Creates a new empty window with specified
/// height and width at the center of terminal screen.
WINDOW* show_window_centered_in(WINDOW* parent_window, int height, int width);

/// Creates a new empty window stretched to parent window width
/// at specified y and with specified height.
WINDOW* show_window_stretched_in(WINDOW* parent_window, int height, int y);

/// Shows a menu of options at the center of terminal.
/// Returns 0-based index of an option selected by user.
/// \param [in] options NULL-terminated array of strings.
int show_menu_in(WINDOW* parent_window, char** options, int menu_width);

FIELD* new_edit_box(const char* text, int height, int width, int y, int x);

bool is_edit_box(FIELD* f);

char* get_text(FIELD* edit_box);

FIELD* new_label(const char* text, int height, int width, int y, int x);

FIELD* new_label_autosize(const char* text, int y, int x);

bool is_label(FIELD* f);

bool is_active(FIELD* f);

FORM* show_form(WINDOW* form_window, FIELD** fields, int height, int width, int y, int x);

FORM* show_form_fill(WINDOW* form_window, FIELD** fields);

/// Handles text editing characters in form fields.
void handle_edit_keys(WINDOW* form_window, FORM* form, int key_char);
