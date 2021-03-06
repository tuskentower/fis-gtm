/****************************************************************
 *								*
 *	Copyright 2001 Sanchez Computer Associates, Inc.	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#include "mdef.h"
#include "compiler.h"
#include "opcode.h"
#include "toktyp.h"
#include "cmd.h"

GBLREF char window_token;

int m_trestart(void)
{
	triple	*ref;
	error_def(ERR_SPOREOL);

	if (window_token != TK_EOL && window_token != TK_SPACE)
	{
		stx_error(ERR_SPOREOL);
		return FALSE;
	}
	ref = newtriple(OC_TRESTART);
	ref->operand[0] = put_ilit(1);
	return TRUE;
}
