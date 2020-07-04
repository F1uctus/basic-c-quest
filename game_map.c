#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <curses.h>

#include "game_map.h"

/// \param height file lines count.
/// \param width file's longest line length.
static void get_file_dimensions(char* file_name, int* height, int* width) {
    *height = 0;
    *width = 0;
    FILE* f = fopen(file_name, "r");

    // non-empty file has 1 line.
    if (!feof(f)) {
        (*height)++;
    }
    int cur_width = 0;
    while (!feof(f)) {
        int chr = fgetc(f);
        if (chr == '\n') {
            (*height)++;
            if (cur_width > *width) {
                *width = cur_width;
            }
            cur_width = 0;
            continue;
        }
        cur_width++;
    }

    fclose(f);
}

/// Load text from file to 2D array.
/// Returns file "dimensions" into #height & #width respectively.
/// \param height file lines count.
/// \param width file's longest line length.
/// https://stackoverflow.com/a/36833634/8779245
char (* string_array_from(char* file_name, int* height, int* width))[LINE_MAX_WIDTH] {
    get_file_dimensions(file_name, height, width);

    // holy shit.
    char (* lines)[LINE_MAX_WIDTH] = malloc(sizeof(char[*height][LINE_MAX_WIDTH]));

    int i = 0;
    FILE* f = fopen(file_name, "r");
    while (fgets(lines[i], LINE_MAX_WIDTH, f)) {
        // get rid of \n-s
        char* c = &lines[i][strlen(lines[i]) - 1];
        if (*c == '\n') {
            *c = '\0';
        }
        i++;
    }
    fclose(f);
    return lines;
}

void show_screen_in(WINDOW* parent_window, int screen_index) {
    // print art
    int width = 0, height = 0;
    char file_path[64];
    sprintf(file_path, "%s%i-art.txt", MAPS_DIR, screen_index);
    char (* art)[LINE_MAX_WIDTH] = string_array_from(file_path, &height, &width);

    int x = getmaxx(parent_window) / 2 - width / 2;
    for (int i = 0; i < height; ++i) {
        mvwprintw(parent_window, 1 + i, x, art[i]);
    }

    // then print description
    sprintf(file_path, "%s%i-description.txt", MAPS_DIR, screen_index);
    char (* description)[LINE_MAX_WIDTH] = string_array_from(file_path, &height, &width);

    int y = getmaxy(parent_window) - height - 1;
    x = 2;
    for (int i = 0; i < height; ++i) {
        mvwprintw(parent_window, y + i, x, description[i]);
    }

    refresh();
    wrefresh(parent_window);

    // clean up
    free(art);
    free(description);
}