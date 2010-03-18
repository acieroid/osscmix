#ifndef MIXER_H
#define MIXER_H

#include <stropts.h>

#include "osscmix.h"

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

	int n_dev; /* number of mixer devices */
	s_dev devs[]; /* mixer devices */
};

struct s_infos infos;
extern struct s_infos infos;

void oss_init(void);
s_dev* list_device (void);
void change_device_level (s_dev dev);

#endif /* MIXER_H */
