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
				if (controls[i].left_val <
						controls[i].max_value) {
					controls[i].left_val++;
					update_ctrl(controls[i]);
				}
				break;

			case KEY_DOWN:
				if (controls[i].left_val > 0) {
					controls[i].left_val--;
					update_ctrl(controls[i]);
				}
				break;

			case KEY_PPAGE:
				if (controls[i].left_val <
						controls[i].max_value)
				{
					if ((controls[i].left_val + 5) >
							controls[i].max_value)
						controls[i].left_val =
							controls[i].max_value;
					else
						controls[i].left_val += 5;
					update_ctrl(controls[i]);
				}
				break;

			case KEY_NPAGE:
				if (controls[i].left_val > 0)
				{
					if ((controls[i].left_val - 5) < 0)
						controls[i].left_val = 0;
					else
						controls[i].left_val -= 5;
					update_ctrl(controls[i]);
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
	oss_close();

	return EXIT_SUCCESS;
}
