#ifndef MIXER_H
#define MIXER_H

#include <stropts.h>

#include "osscmix.h"

#define OSS_CALL(call, arg)				\
	if (ioctl(infos.mixer_fd, call, arg) == -1) {	\
		perror(#call);				\
  		exit(1); 				\
	}

typedef struct s_dev s_dev;
struct s_dev
{
	int id;
	char *name;
	
	int level;	/* sound level in percent */
	bool muted;
};

struct s_infos
{
	char *version; /* OSS version */
	char *devpath; /* the path to the mixer used, usually /dev/mixer */
	int mixer_fd; /* the mixer file descriptor */

	int n_dev; /* number of controls */
	s_dev *devs; /* controls */
};

struct s_infos infos;
extern struct s_infos infos;

void oss_init(void);
s_dev* list_device (void);
void change_device_level (s_dev dev);

#endif /* MIXER_H */
