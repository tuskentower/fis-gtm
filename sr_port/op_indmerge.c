/****************************************************************
 *								*
 *	Copyright 2001, 2011 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

/*
 * Description:
 *	This is called, if MERGE parameter has any indirection.
 * Parameters:
 *	glvn_mv: the (mval*) for one of the arguments of merge.
 *	arg1_or_arg2: Is it left hand side or right hand side
 *		MARG1_LCL | MARG1_GBL => glvn1
 *		MARG2_LCL | MARG2_GBL => glvn2
 * Notes:
 *	Unlike op_indset which is called once for a SET, this is called twice for MERGE command,
 *  	if the command parameter is @glvn1=@glvn2. It will be called once with glvn1 and once with glvn2
 */
#include "mdef.h"

#include "toktyp.h"
#include "compiler.h"
#include "opcode.h"
#include "indir_enum.h"
#include "merge_def.h"
#include "op.h"
#include "mvalconv.h"
#include "cache.h"
#include "hashtab_objcode.h"
#include "stringpool.h"

GBLREF char			window_token;

void op_indmerge(mval *glvn_mv, mval *arg1_or_arg2)
{
	error_def(ERR_INDMAXNEST);
	error_def(ERR_VAREXPECTED);
	bool		stat;
	boolean_t       leftarg;
	mstr		object, *obj;
	oprtype		mopr;
	triple		*ref;
	mval		arg_copy;
	icode_str	indir_src;

	assert (((MARG1_LCL | MARG1_GBL) == MV_FORCE_INT(arg1_or_arg2)) ||
		((MARG2_LCL | MARG2_GBL) == MV_FORCE_INT(arg1_or_arg2)));
	leftarg = ((MARG1_LCL | MARG1_GBL) == MV_FORCE_INT(arg1_or_arg2))  ? TRUE:FALSE;
	MV_FORCE_STR(glvn_mv);
	indir_src.str = glvn_mv->str;
	indir_src.code = leftarg ? indir_merge1:indir_merge2;
	if (NULL == (obj = cache_get(&indir_src)))
	{
		comp_init(&glvn_mv->str);
		switch (window_token)
		{
		case TK_IDENT:
			if (stat = lvn(&mopr, leftarg ? OC_PUTINDX : OC_M_SRCHINDX, 0))
			{
				ref = newtriple(OC_MERGE_LVARG);
				ref->operand[0] = put_ilit(leftarg ? MARG1_LCL : MARG2_LCL);
				ref->operand[1] = mopr;
			}
			break;
		case TK_CIRCUMFLEX:
			if (stat = gvn())
			{
				ref = newtriple(OC_MERGE_GVARG);
				ref->operand[0] = put_ilit(leftarg ? MARG1_GBL : MARG2_GBL);
			}
			break;
		case TK_ATSIGN:
			if (stat = indirection(&mopr))
			{
				ref = maketriple(OC_INDMERGE);
				arg_copy = *arg1_or_arg2;
				if (MV_IS_STRING(&arg_copy))
				    s2pool(&arg_copy.str);
				ref->operand[0] = put_lit(&arg_copy);
				ref->operand[1] = mopr;
				ins_triple(ref);
			}
			break;
		default:
			stx_error(ERR_VAREXPECTED);
			break;
		}
		if (comp_fini(stat, &object, OC_RET, 0, glvn_mv->str.len))
		{
			indir_src.str.addr = glvn_mv->str.addr;
			cache_put(&indir_src, &object);
			comp_indr(&object);
		}
	} else
		comp_indr(obj);
}
