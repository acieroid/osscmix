#ifndef MIXER_H
#define MIXER_H

#include <stropts.h>

#include "osscmix.h"

#define OSS_CALL(call, arg)				\
	if (ioctl(infos.mixer_fd, call, arg) == -1) {	\
		perror(#call);				\
 		exit(1); 				\
	}

enum units { CENTIBEL, DECIBEL, HZ };

typedef struct s_ctrl s_ctrl;
struct s_ctrl
{
	int id;
	char *name;

	int real_id;	/* the id in OSS's internals */
	int dev; 	/* the device of the ctrl, used by OSS */
	int timestamp;	/* timestamp of the ctrl, used by OSS */
	
	int type;	/* control type */
	int units_type; /* the units of the control */

	int max_value;	/* the maximal value the control can have */
	int min_value;	/* the minimal value the control can have */
	int left_val;	/* left value, also used if there's only one value */
	int right_val;	/* right value */

	bool muted;
	bool stereo;
};

struct s_infos
{
	char *version; 	/* OSS version */
	char *devpath; 	/* the path to the mixer used, usually /dev/mixer */
	int mixer_fd; 	/* the mixer file descriptor */

	int n_ctrl; 	/* number of controls */
	s_ctrl *ctrls; 	/* controls */
};

struct s_infos infos;
extern struct s_infos infos;

void oss_init (void);
void oss_close (void);

s_ctrl read_ctrl_infos (oss_mixext ext, int id);
void update_ctrl (s_ctrl ctrl);
void get_values (s_ctrl *ctrl);

void change_values (s_ctrl *ctrl, int left, int right);

#endif /* MIXER_H */
