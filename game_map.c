#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "game_map.h"

#define LOCATION_SPAWN 1
#define LOCATION_X 2

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

/// Load art from file to 2D array.
/// Returns file "dimensions" into #height & #width respectively.
/// \param height file lines count.
/// \param width file's longest line length.
/// https://stackoverflow.com/a/36833634/8779245
char (*load_art_from(char* file_name, int* height, int* width))[ART_MAX_WIDTH] {
    get_file_dimensions(file_name, height, width);

    // holy shit.
    char (* lines)[ART_MAX_WIDTH] = malloc(sizeof(char[*height][ART_MAX_WIDTH]));

    int i = 0;
    FILE* f = fopen(file_name, "r");
    while (fgets(lines[i], ART_MAX_WIDTH, f)) {
        // get rid of \n
        lines[i][strlen(lines[i]) - 1] = '\0';
        i++;
    }
    fclose(f);
    return lines;
}