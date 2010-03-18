#include "mixer.h"

s_dev*
list_device (void)
{
	s_dev *dev = NULL;
	
	int i;
	
	if (ioctl(mixer_fd, SNDCTL_MIX_NREXT, &nbr_dev) == -1)
		return NULL;
	
	dev = calloc(nbr_dev, sizeof(s_dev));
	
	for (i = 0; i < nbr_dev; i++)
	{
		dev[i].id = i;
		dev[i].name = "Master";
		dev[i].level = i;
		dev[i].muted = FALSE;
	}
	
	return dev;
}

void
change_device_level (s_dev dev)
{
	return;
}    
