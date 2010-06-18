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

void
oss_close (void)
{
	close(infos.mixer_fd);
}

s_ctrl
read_ctrl_infos (oss_mixext ext, int id)
{
	oss_mixer_value val;
	s_ctrl ctrl;
	int mask = 0xff, shift = 8;

	val.dev = ext.dev;
	val.ctrl = ext.ctrl;
	val.timestamp = ext.timestamp;

	ctrl.id = id;
	ctrl.real_id = ext.ctrl;
	ctrl.type = ext.type;

	/* FIXME: you *can't* mute a ctrl which is not a MIXT_MUTE */
	ctrl.muted = false; 
	ctrl.name = strdup(ext.extname);

	OSS_CALL(SNDCTL_MIX_READ, &val);

	if (ext.flags & MIXF_DECIBEL)
		ctrl.units_type = DECIBEL;
	else if (ext.flags & MIXF_CENTIBEL)
		ctrl.units_type = CENTIBEL;
	else if (ext.flags & MIXF_HZ)
		ctrl.units_type = HZ;

	switch (ext.type) {
		case MIXT_STEREOSLIDER16:
			shift = 16; mask = 0xffff;
		case MIXT_STEREOSLIDER:
			ctrl.stereo = TRUE;
			ctrl.max_value = ext.maxvalue;
			ctrl.left_val = val.value & mask;
			ctrl.right_val = (val.value >> shift) & mask;
			break;
		case MIXT_SLIDER:
			mask = ~0;
		case MIXT_MONOSLIDER16:
			mask = 0xffff;
		case MIXT_MONOSLIDER:
		case MIXT_MONODB:
			ctrl.stereo = FALSE;
			ctrl.max_value = ext.maxvalue;
			ctrl.left_val = val.value & mask;
		default:
			ctrl.stereo = FALSE;
			ctrl.max_value = 1;
			ctrl.left_val = 0;
	}
	
	return ctrl;
}

void
update_ctrl (s_ctrl ctrl)
{
	oss_mixext ext;
	oss_mixer_value val;
	int left, right;

	left = ctrl.left_val;
	right = ctrl.right_val;

	ext.dev = 0;
	ext.ctrl = ctrl.real_id;

	OSS_CALL(SNDCTL_MIX_EXTINFO, &ext)

	/* if we can't change the values, we just do nothing */
	if (!(ext.flags & MIXF_WRITEABLE))
		return;

	val.dev = ext.dev;
	val.ctrl = ext.ctrl;
	val.timestamp = ext.timestamp;
	switch (ext.type) {
		case MIXT_STEREOSLIDER16:
		case MIXT_MONOSLIDER16:
			if (left > 0xffff) left = 0xffff;
			if (right > 0xffff) right = 0xffff;
			val.value = (left & 0xffff) | ((right & 0xffff) << 16);
			break;
		case MIXT_MONOSLIDER:
		case MIXT_SLIDER:
			val.value = left;
			break;
		default:
			if (left > 255) left = 255;
			if (right > 255) right = 255;
			val.value = (left & 255) | ((right & 255) << 8);
			break;
	}
	OSS_CALL(SNDCTL_MIX_WRITE, &val);
}
