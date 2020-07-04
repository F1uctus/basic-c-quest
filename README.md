# unnamed-rpg
I've decided to make a small 1-person "hackathon" for myself to improve my C skills in 3 days, and the result is this "game".

## Requirements
Project is written with C11 standard. (GCC 4.7+)
You'll need `ncurses` library to build it.

## Pitfalls
- No support for terminal resizing (minimal window size is 90 by 30)
- Segfaults on misspelled commands
- No actual 'inventory', just a set of flags
- Probably, some other small bugs I didn't noticed