#ifndef RPG_GAME_MAP_H
#define RPG_GAME_MAP_H

#endif //RPG_GAME_MAP_H

#define MAPS_DIR "./maps/"
#define LINE_MAX_WIDTH 128

#define SCREEN_NONE 0
#define SCREEN_SPAWN 1
#define SCREEN_LIGHTHOUSE 2
#define SCREEN_MANSION 3
#define SCREEN_BOTTLE 4

#define SCREEN_DEATH 1001

/// Returns array of char* used to store
/// an ASCII-art. (max. width of 100).
/// NOTE: don't forget to free it after use!
char (*string_array_from(char* file_name, int* height, int* width))[LINE_MAX_WIDTH];

void show_screen_in(WINDOW* parent_window, int screen_index);