#include "window.h"

s_win
init_window (void)
{
	s_win win;

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nonl();
	keypad(stdscr, TRUE);
	
	win.height = getmaxy(stdscr);
	win.width = getmaxx(stdscr);
	win.selected_dev = 0;

	return win;
}

void
exit_window (void)
{
	endwin();
}

void
draw_window(s_win win, s_dev *dev)
{
	int i;
	
	clear();

	for (i = 0; i < nbr_dev; i++)
	{
		draw_device(win, dev[i], i * 10);
	}
}

void
draw_device (s_win win, s_dev dev, int x)
{
        int i;

        move(win.height - 1, x);
        if (dev.id == win.selected_dev)
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
                printw("%d", dev.level);
        }
        else
        {
                printw("M");
        }

        /* dev.level * (win.height - 5) / 100 + 1 : the size of
           the level bar on the screen
        */
        for (i = 0; i < dev.level * (win.height - 5) / 100 + 1; i++)
        {
                move(win.height - 3 - i, x);
                printw("--");
        }
}

