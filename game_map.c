#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "game_map.h"

#define LOCATION_SPAWN 1
#define LOCATION_X 2

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

char** load_art_from(char* file_name, int* height, int* width) {
#define MAX_WIDTH 100

    get_file_dimensions(file_name, height, width);
    char (*lines)[MAX_WIDTH + 1] = malloc(sizeof(char[*height][MAX_WIDTH + 1]));

    int i = 0;
    FILE* f = fopen(file_name, "r");
    while (fgets(lines[i], MAX_WIDTH, f)) {
        // get rid of ending \n from fgets
        lines[i][strlen(lines[i]) - 1] = '\0';
        i++;
    }
    fclose(f);
    return lines;
}
