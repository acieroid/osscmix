#include "osscmix.h"

#include "window.h"
#include "mixer.h"

int
main (int argc, char **argv)
{
	int event;		/* Keyboard event */
	int i;

	s_win window;
	s_dev *device;

	mixer_fd = open("/dev/mixer", O_RDWR);
	if (mixer_fd == -1)
	{
		fprintf(stderr,
			"Error while opening mixer : %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	window = init_window();
	device = list_device();

	if (device == NULL)
	{
		fprintf(stderr,
			"Error while listing devices : %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

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
				if (window.selected_dev < nbr_dev - 1)
					window.selected_dev++;
				break;
		}
	} while (event != 'q' && event != 'Q');

	exit_window();

	return 0;
}
