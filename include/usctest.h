/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *               Author: William Roske
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 *
 * http://www.sgi.com
 *
 * For further information regarding this notice, see:
 *
 * http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 */

#ifndef __USCTEST_H__
#define __USCTEST_H__

#include <sys/param.h>

/*
 * Ensure that PATH_MAX is defined
 */
#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX  MAXPATHLEN
#else
#define PATH_MAX  1024
#endif
#endif

/***********************************************************************
 * The following globals are defined in parse_opts.c but must be
 * externed here because they are used in the macros defined below.
 ***********************************************************************/
extern int STD_TIMING_ON,	/* turned on by -t to print timing stats */
           STD_LOOP_COUNT;	/* changed by -in to set loop count to n */

typedef struct {
	char *option;	/* Valid option string (one option only) like "a:"  */
	int  *flag;	/* Pointer to location to set true if option given  */
	char **arg;	/* Pointer to location to place argument, if needed */
} option_t;

/*
 * The parse_opts library routine takes that argc and argv parameters recevied
 * by main() and an array of structures defining user options. It parses the
 * command line setting flag and argument locations associated with the
 * options. The uhf() is a function called to print user defined help.
 *
 * The function returns a pointer to an error message if an error occurs or in
 * case of success NULL.
 */
const char *parse_opts(int ac, char **av, const option_t *user_optarr, void
                       (*uhf)(void));

extern long TEST_RETURN;
extern int TEST_ERRNO;

/***********************************************************************
 * structure for timing accumulator and counters
 ***********************************************************************/
struct tblock {
    long tb_max;
    long tb_min;
    long tb_total;
    long tb_count;
};

/***********************************************************************
 * The following globals are externed here so that they are accessable
 * in the macros that follow.
 ***********************************************************************/
extern struct tblock tblock;

/***********************************************************************
 * TEST: calls a system call
 *
 * parameters:
 *	SCALL = system call and parameters to execute
 *
 ***********************************************************************/
#define TEST(SCALL) \
	do { \
		errno = 0; \
		TEST_RETURN = SCALL; \
		TEST_ERRNO = errno; \
	} while (0)

/***********************************************************************
 * TEST_VOID: calls a system call
 *
 * parameters:
 *	SCALL = system call and parameters to execute
 *
 * Note: This is IDENTICAL to the TEST() macro except that it is intended
 * for use with syscalls returning no values (void syscall()).  The
 * Typecasting nothing (void) into an unsigned integer causes compilation
 * errors.
 *
 ***********************************************************************/
#define TEST_VOID(SCALL) do { errno = 0; SCALL; TEST_ERRNO = errno; } while (0)

/***********************************************************************
 * TEST_PAUSE: Pause for SIGUSR1 if the pause flag is set.
 *	       Just continue when signal comes in.
 *
 * parameters:
 *	none
 *
 ***********************************************************************/
#define TEST_PAUSE usc_global_setup_hook();
int usc_global_setup_hook();

/***********************************************************************
 * TEST_LOOPING now call the usc_test_looping function.
 * The function will return 1 if the test should continue
 * iterating.
 *
 ***********************************************************************/
#define TEST_LOOPING usc_test_looping
int usc_test_looping(int counter);

#endif /* __USCTEST_H__ */
