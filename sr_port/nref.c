/****************************************************************
 *								*
 *	Copyright 2001, 2003 Sanchez Computer Associates, Inc.	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#include "mdef.h"
#include "compiler.h"
#include "toktyp.h"
#include "opcode.h"
#include "indir_enum.h"
#include "advancewindow.h"

GBLREF char window_token;

int nref(void)
{

	triple *ref;
	bool	gbl;
	oprtype tmparg;
	error_def(ERR_VAREXPECTED);

	gbl = FALSE;
	switch(window_token)
	{
	case TK_CIRCUMFLEX:
		gbl = TRUE;
		advancewindow();
		/* caution fall through */
	case TK_LBRACKET:
	case TK_IDENT:
	case TK_VBAR:
		if (!lkglvn(gbl))
			return EXPR_FAIL;
		return TRUE;
	case TK_ATSIGN:
		if (!indirection(&tmparg))
			return EXPR_FAIL;
		ref = maketriple(OC_COMMARG);
		ref->operand[0] = tmparg;
		ref->operand[1] = put_ilit((mint) indir_nref);
		ins_triple(ref);
		return EXPR_INDR;
	default:
		stx_error(ERR_VAREXPECTED);
		return EXPR_FAIL;
	}
}
