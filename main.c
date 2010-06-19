#include "osscmix.h"

#include "window.h"
#include "mixer.h"

int
main (int argc, char **argv)
{
	int event;		/* Keyboard event */
	int i, j;

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
		/* update the controls values */
		for (j = 0; j < infos.n_ctrl; j++)
			get_values(&controls[j]);

		i = window.selected_ctrl;

		draw_window(window, controls);

		switch (event = getch())
		{
			/* Sound level change */
			case KEY_UP:
				change_values(&controls[i], 1, 1);
				break;

			case KEY_DOWN:
				change_values(&controls[i], -1, -1);
				break;

			case KEY_PPAGE:
				change_values(&controls[i], 5, 5);
				break;

			case KEY_NPAGE:
				change_values(&controls[i], -5, -5);
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
	oss_close();

	return EXIT_SUCCESS;
}
