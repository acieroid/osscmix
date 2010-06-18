#ifndef MIXER_H
#define MIXER_H

#include <stropts.h>

#include "osscmix.h"

#define OSS_CALL(call, arg)				\
	if (ioctl(infos.mixer_fd, call, arg) == -1) {	\
		perror(#call);				\
 		exit(1); 				\
	}

typedef struct s_ctrl s_ctrl;
struct s_ctrl
{
	int id;
	int real_id;	/* the id in OSS's internals */
	char *name;
	
	int type;	/* control type */

	int level;	/* sound level in percent */
	bool muted;
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
void change_ctrl_level (s_ctrl ctrl, int value);

#endif /* MIXER_H */
