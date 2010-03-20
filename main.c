#include "osscmix.h"

#include "window.h"
#include "mixer.h"

int
main (int argc, char **argv)
{
	int event;		/* Keyboard event */
	int i;

	s_win window;
	s_ctrl *controls;

	oss_init();
	controls = infos.ctrls;
	if (controls == NULL)
	{
		fprintf(stderr, "Error while listing controls");
		exit(EXIT_FAILURE);
	}  

	curses_init();
	window.height = getmaxy(stdscr);
	window.width = getmaxx(stdscr);
	window.selected_ctrl = 0;

	do
	{
		i = window.selected_ctrl;

		draw_window(window, controls);

		switch (event = getch())
		{
			/* Sound level change */
			case KEY_UP:
				if (controls[i].level < 100)
					controls[i].level++;
				break;

			case KEY_DOWN:
				if (controls[i].level > 0)
					controls[i].level--;
				break;

			case KEY_PPAGE:
				if (controls[i].level < 100)
				{
					if ((controls[i].level + 5) > 100)
						controls[i].level = 100;
					else
						controls[i].level += 5;
				}
				break;

			case KEY_NPAGE:
				if (controls[i].level > 0)
				{
					if ((controls[i].level - 5) < 0)
						controls[i].level = 0;
					else
						controls[i].level -= 5;
				}
				break;

			/* Muting/unmuting */
			case 'M':
			case 'm':
				if (controls[i].muted)
					controls[i].muted = FALSE;
				else
					controls[i].muted = TRUE;
				break;
			
			/* Selecting controls */
			case KEY_LEFT:
				if (window.selected_ctrl > 0)
					window.selected_ctrl--;
				break;
			
			case KEY_RIGHT:
				if (window.selected_ctrl < infos.n_ctrl - 1)
					window.selected_ctrl++;
				break;
		}
	} while (event != 'q' && event != 'Q');

	curses_exit();

	return EXIT_SUCCESS;
}
