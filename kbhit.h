/* 
 * File:   kbhit.h
 * Author: nosferatu
 *
 * Created on June 19, 2015, 4:04 PM
 */

#ifndef KBHIT_H
#define	KBHIT_H

#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void);

#endif	/* KBHIT_H */

