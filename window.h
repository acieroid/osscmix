#ifndef WINDOW_H
#define WINDOW_H

#include "osscmix.h"
#include "mixer.h"

typedef struct s_win s_win;
struct s_win
{
	int height;
	int width;

	int selected_ctrl;
};

void curses_init (void);

void curses_exit (void);

void draw_window (s_win win, s_ctrl *dev);

void draw_device (s_win win, s_ctrl dev, int x);

#endif /* WINDOW_H */
