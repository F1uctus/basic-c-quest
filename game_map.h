#ifndef RPG_GAME_MAP_H
#define RPG_GAME_MAP_H

#endif //RPG_GAME_MAP_H

/// Returns array of char* used to store
/// an ASCII-art. (max. width of 100).
/// NOTE: don't forget to free it after use!
char** load_art_from(char* file_name, int* height, int* width);