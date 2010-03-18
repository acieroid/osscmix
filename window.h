#ifndef WINDOW_H
#define WINDOW_H

#include "osscmix.h"
#include "mixer.h"

typedef struct s_win s_win;
struct s_win
{
	int height;
	int width;

	int selected_dev;
};

s_win
init_window (void);

void
exit_window (void);

void
draw_window (s_win win, s_dev *dev);

void
draw_device (s_win win, s_dev dev, int x);

#endif /* WINDOW_H */
