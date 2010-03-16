#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

struct win_t
{
	int height;
	int width;
};

struct dev_t
{
	char *name;

	int level; // sound level in percent
	bool muted;

	/* Position on screen */
	int pos_x;
	int pos_y;
	bool selected;
};

/* Window functions */
struct win_t
init_window (void);

void
exit_window (void);

void
draw_window(struct win_t win, struct dev_t dev[], int n);

void
draw_device_level (struct win_t win, struct dev_t dev);


/* OSS functions */
struct dev_t*
list_device (void);

void
change_device_level(struct dev_t dev);

int
main (int argc, char **argv)
{
	int event;		/* Keyboard event */

	int i = 0, n = 1;	/*	i : selected device
					n : numbre of devices
				*/
	struct win_t window;
	struct dev_t *device;

	int j;

	window = init_window();
	device = list_device();

	n = sizeof(*device) / sizeof(struct dev_t);

	for (j = 0; j < n; j++)
	{
		device[j].pos_x = 0;
		device[j].pos_y = window.height - 1;
	}

	do
	{
		draw_window(window, device, n);
		
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
				if (i > 0)
				{
					device[i].selected = FALSE;
					i--;
					device[i].selected = TRUE;
				}
				break;
			
			case KEY_RIGHT:
				if (i < n - 1)
				{
					device[i].selected = FALSE;
					i++;
					device[i].selected = TRUE;
				}
				break;
		}
		
		/* redrawing window at each iterration */
		draw_window(window, device, n);

	} while (event != 'q' && event != 'Q');

	exit_window();

	return 0;
}

struct win_t
init_window (void)
{
	struct win_t win;
	
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
draw_window(struct win_t win, struct dev_t dev[], int n)
{
	int i;

	clear();

	for (i = 0; i < n; i++)
	{
		move(dev[i].pos_y, dev[i].pos_x);
		
		if (dev[i].selected)
			printw("%s*", dev[i].name);
		else
			printw("%s", dev[i].name);
		
		draw_device_level(win, dev[i]);
	}
}

void
draw_device_level (struct win_t win, struct dev_t dev)
{
	int i;
	int x = dev.level * (win.height - 10) / 100 + 1;
	
	move(dev.pos_y - 1, dev.pos_x);
	if (dev.muted)
		printw("%d", dev.level);
	else
		printw("M");

	for (i = 0; i < x; i++)
	{
		move(dev.pos_y - 2 - i, dev.pos_x);
		printw("--");
	}
}

struct dev_t*
list_device (void)
{
	struct dev_t *dev = malloc(sizeof(struct dev_t));
	
	dev[0].name = "MASTER";
	dev[0].level = 70;
	dev[0].muted = FALSE;
	dev[0].pos_x = 0;
	dev[0].pos_y = 0;
	dev[0].selected = FALSE;

	return dev;
}

void
change_device_level(struct dev_t dev)
{
	
}
