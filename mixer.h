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

int mixer_fd;

void oss_init(void);
s_dev* list_device (void);
void change_device_level (s_dev dev);

#endif /* MIXER_H */
