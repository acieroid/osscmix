#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

typedef struct s_win s_win;
struct s_win
{
	int height;
	int width;

	int selected_dev;
	int nbr_dev;		/* Number of devices */
};

typedef struct s_dev s_dev;
struct s_dev
{
	int id;
	char *name;

	int level;		/* sound level in percent */
	bool muted;
};

/* Window functions */
s_win
init_window (void);

void
exit_window (void);

void
draw_window (s_win win, s_dev *dev);

void
draw_device (s_win win, s_dev dev);


/* OSS functions */
s_dev*
list_device (void);

void
change_device_level(s_dev dev);

int
main (int argc, char **argv)
{
	int event;		/* Keyboard event */
	int i;

	struct s_win window;
	struct s_dev *device;

	window = init_window();
	device = list_device();

	do
	{
		draw_window(window, device);
		
		i = window.selected_dev;

		switch (event = getch())
		{
			/* Sound level change */
			case KEY_UP:
				if (device[i].level < 100)
					device[i].level++;
				break;

			case KEY_DOWN:
				if (device[i].level > 0)
					device[i].level--;
				break;

			case KEY_PPAGE:
				if (device[i].level < 100)
				{
					if ((device[i].level + 5) > 100)
						device[i].level = 100;
					else
						device[i].level += 5;
				}
				break;

			case KEY_NPAGE:
				if (device[i].level > 0)
				{
					if ((device[i].level - 5) < 0)
						device[i].level = 0;
					else
						device[i].level -= 5;
				}
				break;

			/* Muting/unmuting */
			case 'M':
			case 'm':
				if (device[i].muted)
					device[i].muted = FALSE;
				else
					device[i].muted = TRUE;
				break;
			
			/* Selecting device */
			case KEY_LEFT:
				if (window.selected_dev > 0)
					window.selected_dev--;
				break;
			
			case KEY_RIGHT:
				if (window.selected_dev < window.nbr_dev - 1)
					window.selected_dev++;
				break;
		}
	} while (event != 'q' && event != 'Q');

	exit_window();

	return 0;
}

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

	getmaxyx(stdscr, win.height, win.width);
	
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
	int i, n;

	clear();
	
	n = sizeof(*dev) / sizeof(s_dev);

	for (i = 0; i < n; i++)
	{
		draw_device(win, dev[i]);
	}
}

void
draw_device (s_win win, s_dev dev)
{
	int i;
	int x = dev.level * (win.height - 10) / 100 + 1;
	
	if (dev.id == win.selected_dev)
		printw("%s", dev.name);
	else
		printw("%s", dev.name);

	/*
	move(dev.pos_y - 1, dev.pos_x);
	*/

	if (dev.muted)
		printw("%d", dev.level);
	else
		printw("M");
	
	for (i = 0; i < x; i++)
	{
		/*
		move(dev.pos_y - 2 - i, dev.pos_x);
		*/
		printw("--");
	}
}

s_dev*
list_device (void)
{
	s_dev *dev = malloc(sizeof(s_dev));
	
	dev[0].id = 0;
	dev[0].name = "MASTER";
	dev[0].level = 70;
	dev[0].muted = FALSE;

	return dev;
}

void
change_device_level(s_dev dev)
{
	
}
