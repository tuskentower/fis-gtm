/****************************************************************
 *								*
 *	Copyright 2011 Fidelity Information Services, Inc.*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#ifndef _FORK_INIT_H
#define _FORK_INIT_H

#define DO_FORK(pid)			\
{					\
	pid = fork();			\
	if (0 == pid)			\
		clear_timers();		\
}

#endif
