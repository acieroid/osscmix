#include "mixer.h"

void
oss_init (void)
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
	infos.n_ctrl = 0;
	/* FIXME: realloc at each iteration ? */
	infos.ctrls = calloc(100, sizeof(s_ctrl));
	n = 0; /* we just handle the first mixer, for now */
	OSS_CALL(SNDCTL_MIX_NREXT, &n)
	for (i = 0; i < n; i++) {
		oss_mixext ext;
		ext.dev = 0; /* same comment as for `n' */
		ext.ctrl = i;

		OSS_CALL(SNDCTL_MIX_EXTINFO, &ext)
		
		if (ext.flags &
			(MIXF_MAINVOL | MIXF_PCMVOL )) {
			infos.ctrls[infos.n_ctrl] = read_ctrl_infos(ext, infos.n_ctrl);
			infos.n_ctrl++;
		}
	}
}

s_ctrl
read_ctrl_infos (oss_mixext ext, int id)
{
	oss_mixer_value val;
	s_ctrl ctrl;

	val.dev = ext.dev;
	val.ctrl = ext.ctrl;
	val.timestamp = ext.timestamp;

	ctrl.id = id;
	ctrl.muted = false;
	ctrl.name = ext.extname;

	OSS_CALL(SNDCTL_MIX_READ, &val);
	/* FIXME: handle multiple value types */
	ctrl.level = val.value & 0xff;

	return ctrl;
}

void
change_device_level (s_ctrl ctrl)
{
	return;
}
