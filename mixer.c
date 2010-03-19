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

	/* fill the devices in `infos' */
	infos.n_dev = 0;
	/* FIXME: realloc at each iteration ? */
	infos.devs = calloc(10, sizeof(s_dev));
	n = 0; /* we just handle the first mixer, for now */
	OSS_CALL(SNDCTL_MIX_NREXT, &n)
	for (i = 0; i < n; i++) {
		oss_mixext ext;
		ext.dev = 0; /* same comment as for `n' */
		ext.ctrl = i;

		OSS_CALL(SNDCTL_MIX_EXTINFO, &ext)
		
		if (ext.flags &
				(MIXF_MAINVOL | MIXF_PCMVOL | MIXF_RECVOL | MIXF_MONVOL)) {
			infos.devs[infos.n_dev].id = infos.n_dev;
			infos.devs[infos.n_dev].level = 0;
			infos.devs[infos.n_dev].muted = false;

			if (ext.flags & MIXF_MAINVOL) 
				infos.devs[infos.n_dev].name = "Main";
			else if (ext.flags & MIXF_PCMVOL) 
				infos.devs[infos.n_dev].name = "PCM";
			else if (ext.flags & MIXF_RECVOL) 
				infos.devs[infos.n_dev].name = "Record";
			else if (ext.flags & MIXF_MONVOL) 
				infos.devs[infos.n_dev].name = "Monitoring";
			infos.n_dev++;
		}
	}
}

void
change_device_level (s_dev dev)
{
	return;
}
