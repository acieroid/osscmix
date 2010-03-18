#include "mixer.h"

void
oss_init(void)
{
	int n, i;
	/* open the device */
	if ((infos.devpath = getenv("OSS_MIXERDEV")) == NULL)
		infos.devpath = "/dev/mixer";

	if ((infos.mixer_fd = open(infos.devpath, O_RDWR, 0)) == -1) {
		perror(infos.devpath);
		exit(1);
	}

	/* fetch some informations */
	/* FIXME: handling multiple mixers */
	/*if (ioctl(infos.mixer_fd, SNDCTL_SYSINFO, &si) == -1) {
		perror("SNDCTL_SYSINFO");
		exit(1);
	}
	infos.n_dev = si.nummixers;
	*/

	/* fill the devices in `infos' */
	infos.n_dev = 0;
	n = 0; /* we just handle the first mixer, for now */
	OSS_CALL(SNDCTL_MIX_NREXT, &n)
	//if (ioctl(infos.mixer_fd, SNDCTL_MIX_NREXT, &n) == -1)
	for (i = 0; i < n; i++) {
		oss_mixext ext;
		ext.dev = 0; /* same comment as for `n' */
		ext.ctrl = i;

		OSS_CALL(SNDCTL_MIX_EXTINFO, &ext)
		
		if (ext.flags &
				(MIXF_MAINVOL | MIXF_PCMVOL | MIXF_RECVOL | MIXF_MONVOL))
			infos.n_dev++;
	}
}

s_dev*
list_device (void)
{
	s_dev *dev = NULL;
	
	int i;
	
	dev = calloc(infos.n_dev, sizeof(s_dev));
	
	for (i = 0; i < infos.n_dev; i++)
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
