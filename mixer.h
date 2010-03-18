#ifndef MIXER_H
#define MIXER_H

#include "osscmix.h"

typedef struct s_dev s_dev;
struct s_dev
{
	int id;
	char *name;
	
	int level;	/* sound level in percent */
	bool muted;
};

s_dev*
list_device (void);

void
change_device_level (s_dev dev);

#endif /* MIXER_H */
