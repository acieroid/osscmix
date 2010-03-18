#ifndef OSSCMIX_H
#define OSSCMIX_H

#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <errno.h>

#include <ncurses.h>
#include <soundcard.h>

int mixer_fd;
int nbr_dev;

extern int mixer_fd;
extern int nbr_dev;

#endif /* OSSCMIX_H */
