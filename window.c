#include "window.h"

void
curses_init (void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nonl();
	keypad(stdscr, TRUE);
}

void
curses_exit()
{
	endwin();
}

void
draw_window(s_win win, s_ctrl *dev)
{
	int i;
	
	clear();

	for (i = 0; i < infos.n_ctrl; i++)
	{
		draw_device(win, dev[i], i * 10);
	}
}

void
draw_device (s_win win, s_ctrl dev, int x)
{
	int i;

	move(win.height - 1, x);
	if (dev.id == win.selected_ctrl)
	{
		attron(A_BOLD);
		printw("%s", dev.name);
		attroff(A_BOLD);
	}
	else
	{
		printw("%s", dev.name);
	}

	move(win.height - 2, x);
	if (!dev.muted)
	{
		/*if (dev.units_type == CENTIBEL)
			printw("%f", dev.left_val/10.0);
		else*/
			printw("%d", dev.left_val);
	}
	else
	{
		printw("M");
	}

	/* dev.left_val * (win.height - 5) / dev.max_value + 1 : the size of
	   the level bar on the screen
	*/
	/* FIXME: arithmetic error when using dev.max_value */
	for (i = 0; i < dev.left_val * (win.height - 5) / 100 + 1; i++)
	{
		move(win.height - 3 - i, x);
		printw("--");
	}
}

