/*
** LCLint - annotation-assisted static program checker
** Copyright (C) 1994-2000 University of Virginia,
**         Massachusetts Institute of Technology
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version.
** 
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
** 
** The GNU General Public License is available from http://www.gnu.org/ or
** the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
** MA 02111-1307, USA.
**
** For information on lclint: lclint-request@cs.virginia.edu
** To report a bug: lclint-bug@cs.virginia.edu
** For more information: http://lclint.cs.virginia.edu
*/
/*
** uentry.c
*/

# include "lclintMacros.nf"
# include "basic.h"
# include "structNames.h"
# include "nameChecks.h"

static /*@dependent@*/ uentry posRedeclared = uentry_undefined;
static /*@only@*/ fileloc posLoc = fileloc_undefined;
static int nuentries = 0;
static int totuentries = 0;

static void checkGlobalsModifies (/*@notnull@*/ uentry p_ue, sRefSet p_sr) ;
static void uentry_setDeclDef (uentry p_e, fileloc p_f) /*@modifies p_e@*/ ;
static bool uentry_isRefCounted (uentry p_ue) /*@*/ ;
static bool uentry_isRefsField (uentry p_ue) /*@*/ ;
static bool uentry_isReallySpecified (uentry p_e) /*@*/ ;
static void uentry_checkIterArgs (uentry p_ue);
static cstring uentry_dumpAux (uentry p_v, bool p_isParam);

/*@access ekind@*/
static void checkAliasState (/*@notnull@*/ uentry p_old,
			       /*@notnull@*/ uentry p_unew, 
			       bool p_mustConform, bool p_completeConform) 
   /*@modifies p_old, p_unew@*/ ;
static void checkNullState (/*@notnull@*/ uentry p_old,
			    /*@notnull@*/ uentry p_unew, 
			    bool p_mustConform, bool p_completeConform) 
   /*@modifies p_old, p_unew@*/ ;

static void checkVarConformance (/*@notnull@*/ uentry p_old,
				 /*@notnull@*/ uentry p_unew, 
				 bool p_mustConform, bool p_completeConform) 
   /*@modifies p_old, p_unew@*/;

# ifndef NOLCL
static void uentry_setHasMods (uentry p_ue) /*@modifies p_ue@*/;
static void uentry_setHasGlobs (uentry p_ue) /*@modifies p_ue@*/;
# endif

static void uentry_reallyFree (/*@notnull@*/ /*@only@*/ uentry p_e);

static void uentry_setSpecDef (/*@special@*/ uentry p_e, /*@keep@*/ fileloc p_f)
   /*@defines p_e->whereSpecified, p_e->whereDeclared, p_e->whereDefined@*/
   /*@modifies p_e@*/;

static void returnValueError (/*@notnull@*/ uentry p_old, /*@notnull@*/ uentry p_unew);
static void nargsError (/*@notnull@*/ uentry p_old, /*@notnull@*/ uentry p_unew);
static /*@observer@*/ cstring paramStorageName (uentry p_ue) /*@*/ ;
static /*@observer@*/ cstring fcnErrName (uentry p_ue) /*@*/ ;
static /*@observer@*/ cstring checkedName (chkind p_checked) /*@*/ ;
static void 
  paramTypeError (/*@notnull@*/ uentry p_old, /*@notnull@*/ uentry p_oldCurrent,
		  ctype p_oldType, /*@notnull@*/ uentry p_unew,
		  /*@notnull@*/ uentry p_newCurrent, 
		  ctype p_newType, int p_paramno) /*@modifies g_msgstream@*/ ;

static /*@only@*/ /*@notnull@*/ uentry 
  uentry_makeVariableAux (cstring p_n, ctype p_t, /*@keep@*/ fileloc p_f,
			  /*@exposed@*/ sRef p_s, bool p_priv, vkind p_kind);

static /*@out@*/ /*@notnull@*/ uentry uentry_alloc (void) /*@*/ 
{
  uentry ue = (uentry) dmalloc (sizeof (*ue));
  nuentries++;
  totuentries++;
  
  return ue;
}

static cstring uentry_getOptName (uentry p_e) /*@*/ ;
static void uentry_copyInto (/*@out@*/ /*@unique@*/ uentry p_unew, uentry p_old);
static void uentry_setNullState (/*@notnull@*/ uentry p_ue, nstate p_ns);
static void uentry_setAliasKind (/*@notnull@*/ uentry p_ue, alkind p_ak);
static /*@only@*/ /*@null@*/ uinfo uinfo_copy (uinfo p_u, ekind p_kind);
static void uinfo_free (/*@only@*/ uinfo p_u, ekind p_kind);
static void uvinfo_free (/*@only@*/ uvinfo p_u);

# ifdef DOANNOTS

static /*@only@*/ cstring ancontext_unparse (ancontext an)
{
  switch (an)
    {
    case AN_UNKNOWN: return cstring_makeLiteral ("unknown");
    case AN_FCNRETURN: return cstring_makeLiteral ("return value");
    case AN_FCNPARAM: return cstring_makeLiteral ("function param");
    case AN_SUFIELD: return cstring_makeLiteral ("su field");
    case AN_TDEFN: return cstring_makeLiteral ("type definition");
    case AN_GSVAR: return cstring_makeLiteral ("global/static var");
    case AN_CONST: return cstring_makeLiteral ("constant");
    BADDEFAULT;
    }
  BADEXIT;
}

static int annots[AN_LAST][QU_LAST];
static int decls[AN_LAST];
static int shdecls[AN_LAST];
static int idecls[AN_LAST];

void initAnnots ()
{
  int i, j;

  for (i = AN_UNKNOWN; i < AN_LAST; i++)
    {
      decls[i] = 0;
      shdecls[i] = 0;
      idecls[i] = 0;

      for (j = QU_UNKNOWN; j < QU_LAST; j++)
	{
	  annots[i][j] = 0;
	}
    }
}

static void tallyAnnot (ancontext ac, qual q)
{
  (annots[ac][q])++;
}

void printAnnots ()
{
  int total[QU_LAST];
  int alltotals = 0;
  int totdecls = 0;
  int totshdecls = 0;
  int totidecls = 0;
  int i, j;

  for (j = QU_UNKNOWN; j < QU_LAST; j++)
    {
      total[j] = 0;
    }

  for (i = AN_UNKNOWN; i < AN_LAST; i++)
    {
      int tmptot;

      if (decls[i] > 0)
	{
	  printf ("Context: %s (%d declarations, %d sharable, %d indirect)\n", 
		  ancontext_unparse (i),
		  decls[i], shdecls[i], idecls[i]);
	  
	  totdecls += decls[i];
	  totshdecls += shdecls[i];
	  totidecls += idecls[i];
	  
	  for (j = QU_UNKNOWN; j < QU_LAST; j++)
	    {
	      total[j] += annots[i][j];
	      alltotals += annots[i][j];
	    }
	  
	  printf ("   Allocation:\n");
	  
	  tmptot = 0;
	  
	  for (j = QU_UNKNOWN; j < QU_LAST; j++)
	    {
	      if (qual_isAliasQual (j) && !qual_isUnique (j))
		{
		  if (annots[i][j] > 0)
		    {
		      printf ("\t%10s: %5d (%3.2f%%)\n", qual_unparse (j), annots[i][j],
			      100.0 * (double)annots[i][j] / (double)decls[i]);
		      tmptot += annots[i][j];
		    }
		}
	    }

	  printf ("   Exposure:\n");
	  
	  tmptot = 0;
	  
	  for (j = QU_UNKNOWN; j < QU_LAST; j++)
	    {
	      if (qual_isExQual (j))
		{
		  if (annots[i][j] > 0)
		    {
		      printf ("\t%10s: %5d (%3.2f%%)\n", qual_unparse (j), annots[i][j],
			      100.0 * (double)annots[i][j] / (double)decls[i]);
		      tmptot += annots[i][j];
		    }
		}
	    }
	  
	  printf ("   Definition:\n");
	  
	  for (j = QU_UNKNOWN; j < QU_LAST; j++)
	    {
	      if (qual_isAllocQual (j))
		{
		  if (annots[i][j] > 0)
		    {
		      printf ("\t%10s: %5d (%3.2f%%)\n", qual_unparse (j), annots[i][j],
			      100.0 * (double)annots[i][j] / (double)decls[i]);
		    }
		}
	    }
	  
	  printf ("   Null:\n");
	  
	  for (j = QU_UNKNOWN; j < QU_LAST; j++)
	    {
	      if (qual_isNull (j) || qual_isNotNull (j) || qual_isRelNull (j))
		{
		  if (annots[i][j] > 0)
		    {
		      printf ("\t%10s: %5d (%3.2f%%)\n", qual_unparse (j), annots[i][j],
			      100.0 * (double)annots[i][j] / (double)decls[i]);
		    }
		}
	    }

	  printf ("\n");
	}
    }

  for (j = QU_UNKNOWN; j < QU_LAST; j++)
    {
      bool hasone = FALSE;

      for (i = AN_UNKNOWN; i < AN_LAST; i++)
	{
	  if (annots[i][j] > 0)
	    {
	      hasone = TRUE;
	      break;
	    }
	}

      if (hasone)
	{
	  printf ("Annotation: %s\n", qual_unparse (j));
	  
	  for (i = AN_UNKNOWN; i < AN_LAST; i++)
	    {
	      if (annots[i][j] > 0)
		{
		  printf ("%25s: %5d\n", ancontext_unparse (i), annots[i][j]);
		}
	    }
	  printf ("\n");
	}
    }

  printf ("All Contexts\n");
  
  for (j = QU_UNKNOWN; j < QU_LAST; j++)
    {
      if (total[j] > 0)
	{
	  printf ("%10s: %5d (%3.2f%%)\n", qual_unparse (j), total[j],
		  100.0 * (double)total[j] / (double)(totdecls));
	}
    }
  printf ("\n");

  printf ("Total Annotations: %d (%d decls, %d sharable, %d indirect)\n", alltotals, totdecls, totshdecls, totidecls); }

extern void uentry_tallyAnnots (uentry u, ancontext kind)
{
  alkind ak = sRef_getAliasKind (u->sref);
  exkind ek = sRef_getExKind (u->sref);
  nstate ns = sRef_getNullState (u->sref);
  sstate ss = sRef_getDefState (u->sref);
  bool recordUnknown = FALSE;

  
  if (kind == AN_UNKNOWN)
    {
      ekind e = u->ukind;

      if (e == KENDITER)
	{
	  return;
	}
      else if (e == KCONST || e == KENUMCONST)
	{
	  kind = AN_CONST;
	}
      else if (e == KFCN || e == KITER)
	{
	  uentryList params = uentry_getParams (u);
	  bool hasRet = FALSE;

	  uentryList_elements (params, current)
	    {
	      if (uentry_isReturned (current))
		{
		  hasRet = TRUE;
		}
	      if (!uentry_isElipsisMarker (current))
		{
		  uentry_tallyAnnots (current, AN_FCNPARAM);
		}
	    } end_uentryList_elements;
	  
	  kind = AN_FCNRETURN;
	  
	  if (ctype_isFunction (u->utype)
	      && !hasRet
	      && ctype_isVisiblySharable (ctype_realType (ctype_returnValue (u->utype))))
	    {
	      recordUnknown = TRUE;
	    }
	}
      else if (e == KDATATYPE || e == KSTRUCTTAG || e == KUNIONTAG || e == KENUMTAG)
	{
	  ctype t = ctype_realType (u->utype);

	  if (ctype_isSU (t))
	    {
	      uentryList fields = ctype_getFields (t);

	      uentryList_elements (fields, current)
		{
		  uentry_tallyAnnots (current, AN_SUFIELD);
		}
	    } end_uentryList_elements;
	  
	  kind = AN_TDEFN;

	  if (ctype_isVisiblySharable (u->utype))
	    {
	      recordUnknown = TRUE;
	    }
	}
      else 
	{
	  kind = AN_GSVAR;
	  
	  
	  if (ctype_isVisiblySharable (ctype_realType (u->utype)))
	    {
	      recordUnknown = TRUE;
	    }
	}
    }

  decls[kind]++;

  if (kind == AN_FCNRETURN)
    {
      if (recordUnknown) 
	{
	  shdecls[kind]++;
	  idecls[kind]++;
	}
      else 
	{
	  ;
	}
    }
  else
    {
      if (ctype_isVisiblySharable (ctype_realType (u->utype)))
	{
	  shdecls[kind]++;
	  	}
      
      if (ctype_isRealPointer (ctype_realType (u->utype)))
	{
	  idecls[kind]++;
	}
    }
  









  switch (ss)
    {
    case SS_ALLOCATED: tallyAnnot (kind, QU_OUT); break;
    case SS_PARTIAL:   tallyAnnot (kind, QU_PARTIAL); break;
    case SS_RELDEF:    tallyAnnot (kind, QU_RELDEF); break;
    case SS_SPECIAL:   tallyAnnot (kind, QU_SPECIAL); break;
    default: break;
    }

  if (uentry_isReturned (u))
    {
      tallyAnnot (kind, QU_RETURNED); 
    }

  switch (ak)
    {
    case AK_UNKNOWN:    
      if (ctype_isRefCounted (ctype_realType (u->utype))
	  || (ctype_isFunction (u->utype) &&
	      ctype_isRefCounted (ctype_realType (ctype_returnValue (u->utype)))))
	{
	  ;
	}
      else
	{
	  if (kind == AN_FCNPARAM) 
	    { 
	      tallyAnnot (kind, QU_TEMP); 
	    } 
	  else if (recordUnknown) 
	    { 
	      if (kind == AN_FCNRETURN)
		{
		  		}
	      tallyAnnot (kind, QU_UNKNOWN); 
	    }
	}
      break;
    case AK_ONLY:       tallyAnnot (kind, QU_ONLY); break;
    case AK_IMPONLY:    tallyAnnot (kind, QU_ONLY); break;
    case AK_KEEP:       tallyAnnot (kind, QU_KEEP); break;
    case AK_KEPT:       tallyAnnot (kind, QU_KEPT); break;
    case AK_IMPTEMP:
    case AK_TEMP:       tallyAnnot (kind, QU_TEMP); break;
    case AK_SHARED:     tallyAnnot (kind, QU_SHARED); break;
    case AK_UNIQUE:     tallyAnnot (kind, QU_UNIQUE); break;
    case AK_RETURNED:   tallyAnnot (kind, QU_RETURNED); break;
    case AK_REFCOUNTED: tallyAnnot (kind, QU_UNKNOWN); break;
    case AK_REFS:       tallyAnnot (kind, QU_REFS); break;
    case AK_KILLREF:    tallyAnnot (kind, QU_KILLREF); break;
    case AK_NEWREF:     tallyAnnot (kind, QU_NEWREF); break;
    case AK_OWNED:      tallyAnnot (kind, QU_OWNED); break;
    case AK_IMPDEPENDENT:
    case AK_DEPENDENT:  tallyAnnot (kind, QU_DEPENDENT); break;
    case AK_ERROR:    
    case AK_FRESH:
    case AK_STACK:
    case AK_LOCAL:
      break;
    }

  switch (ek)
    {
    case XO_EXPOSED:    tallyAnnot (kind, QU_EXPOSED); break;
    case XO_OBSERVER:   tallyAnnot (kind, QU_OBSERVER); break;
    default:  break;
    }

  switch (ns)
    {
    case NS_ERROR:   break;
    case NS_UNKNOWN:   break;
    case NS_NOTNULL:   break;
    case NS_MNOTNULL:  tallyAnnot (kind, QU_NOTNULL); break;
    case NS_RELNULL:   tallyAnnot (kind, QU_RELNULL); break;
    case NS_CONSTNULL: tallyAnnot (kind, QU_NULL); break;
    case NS_POSNULL:   tallyAnnot (kind, QU_NULL); break;
    case NS_DEFNULL: 
    case NS_ABSNULL:   break;   
    }
}

# endif

static /*@observer@*/ cstring specCode_unparse (specCode s) /*@*/
{
  switch (s)
    {
    case SPC_NONE: return cstring_makeLiteralTemp ("normal");
    case SPC_PRINTFLIKE: return cstring_makeLiteralTemp ("printflike");
    case SPC_SCANFLIKE: return cstring_makeLiteralTemp ("scanflike");
    case SPC_MESSAGELIKE: return cstring_makeLiteralTemp ("messagelike");
    case SPC_LAST: return cstring_makeLiteralTemp ("<error>");
    }

  BADEXIT;
}

static specCode specCode_fromInt (int i)
{
  /*@+enumint@*/
  llassert (i >= SPC_NONE && i < SPC_LAST);

  return ((specCode) i);
  /*@=enumint@*/
}

/*@observer@*/ cstring uentry_specOrDefName (uentry u) 
{
  if (uentry_isDeclared (u))
    {
      return cstring_makeLiteralTemp ("previously declared");
    }
  else
    {
      return cstring_makeLiteralTemp ("specified");
    }
}

/*@observer@*/ cstring uentry_specDeclName (uentry u) 
{
  if (uentry_isDeclared (u))
    {
      return cstring_makeLiteralTemp ("previous declaration");
    }
  else
    {
      return cstring_makeLiteralTemp ("specification");
    }
}

static /*@observer@*/ cstring uentry_reDefDecl (uentry old, uentry unew)  /*@*/ 
{
  if (uentry_isCodeDefined (old) && uentry_isCodeDefined (unew))
    {
      return cstring_makeLiteralTemp ("redefined");
    }
  else if (uentry_isCodeDefined (unew))
    {
      return cstring_makeLiteralTemp ("defined");
    }
  else if (uentry_isDeclared (old) && uentry_isDeclared (unew))
    {
      return cstring_makeLiteralTemp ("redeclared");
    }
  else
    {
      return cstring_makeLiteralTemp ("declared");
    }
}

static /*@only@*/ fileloc setLocation (void)
{
  fileloc fl = context_getSaveLocation ();

  if (fileloc_isDefined (fl)) 
    {
      return fl;
    }
  else
    {
      return fileloc_copy (g_currentloc);
    }
}

/*@notnull@*/ uentry uentry_makeEnumConstant (cstring n, ctype t)
{
  fileloc loc = setLocation ();
  uentry ue = uentry_makeConstant (n, t, loc);

  ue->ukind = KENUMCONST;
  uentry_setDefined (ue, loc);
  return ue;
}

/*@notnull@*/ uentry uentry_makeEnumInitializedConstant (cstring n, ctype t, exprNode expr)
{
  fileloc loc = setLocation ();
  uentry ue = uentry_makeConstant (n, t, loc);
  ctype etype = exprNode_getType (expr);

  if (!ctype_isRealInt (etype)) {
    voptgenerror 
      (FLG_ENUMMEMBERS,
       message
       ("Value of enum member is not an integeral type (type %s): %s",
	ctype_unparse (etype), exprNode_unparse (expr)),
       exprNode_loc (expr));
  }
  
  ue->ukind = KENUMCONST;
  uentry_setDefined (ue, loc);
  return ue;
}

# ifndef NOLCL
/*@notnull@*/ uentry uentry_makeSpecEnumConstant (cstring n, ctype t, fileloc loc)
{
  uentry ue = uentry_makeConstant (n, t, loc);

  ue->ukind = KENUMCONST;
  return ue;
}
# endif

/*@notnull@*/ uentry uentry_makeVariableLoc (cstring n, ctype t)
{
  return uentry_makeVariable (n, t, setLocation (), FALSE);
}

# ifndef NOLCL
/*@notnull@*/ /*@only@*/ uentry uentry_makeUnnamedVariable (ctype t)
{
  return uentry_makeVariable (cstring_undefined, t, setLocation (), FALSE);
}
# endif

/*@notnull@*/ uentry uentry_makeIdDatatype (idDecl id)
{
  ctype ct = idDecl_getCtype (id);
  uentry ue = uentry_makeDatatype (idDecl_observeId (id), ct, 
				   MAYBE, MAYBE, setLocation ());

  uentry_reflectQualifiers (ue, idDecl_getQuals (id));
  
  if (!ynm_isOn (ue->info->datatype->abs))
    {
      if (ctype_isUnknown (ct))
	{
	  ue->info->datatype->mut = MAYBE;
	}
      else
	{
	  ue->info->datatype->mut = ynm_fromBool (ctype_isMutable (ct));
	}
    }
  
  return ue;
}

void uentry_checkParams (uentry ue)
{
  if (uentry_isValid (ue))
    {
      bool isExt = uentry_isExtern (ue);

      if (uentry_isRealFunction (ue))
	{
	  uentryList params = uentry_getParams (ue);

	  uentryList_elements (params, current)
	    {
	      if (uentry_isValid (current))
		{
		  ctype ct = current->utype;		      
		  
		  if (ctype_isFixedArray (ct))
		    {
		      if (ctype_isArray (ctype_baseArrayPtr (ct))
			  && !ctype_isFixedArray (ctype_baseArrayPtr (ct)))
			{
			  ;
			}
		      else
			{
			  voptgenerror 
			    (FLG_FIXEDFORMALARRAY,
			     message ("Function parameter %q declared as "
				      "manifest array (size constant is meaningless)",
				      uentry_getName (current)),
			     uentry_whereDeclared (current));
			}
		    }
		  else 
		    {
		      if (ctype_isArray (ct))
			{
			  voptgenerror 
			    (FLG_FORMALARRAY,
			     message ("Function parameter %q declared as "
				      "array (treated as pointer)", 
				      uentry_getName (current)),
			     uentry_whereDeclared (current));
			}
		    }

		  if (sRef_getNullState (uentry_getSref (current)) == NS_MNOTNULL)
		    {
		      if (ctype_isAbstract (ct) && 
			  (isExt || (ctype_isAbstract (ctype_realType (ct))
				     && !context_hasFileAccess (ctype_typeId (ct)))))
			{
			  vgenhinterror 
			    (FLG_INCONDEFS,
			     message 
			     ("Function %q declared with notnull parameter %q of abstract "
			      "type %s",
			      uentry_getName (ue),
			      uentry_getName (current),
			      ctype_unparse (ct)),
			     message 
			     ("Since %s is an abstract type, notnull can only be "
			      "used for parameters if the function is static to a "
			      "module where %s is accessible.",
			      ctype_unparse (ct),
			      ctype_unparse (ct)),
			     uentry_whereDeclared (current));
			}
		    }
		}
	    } end_uentryList_elements;
	  
	  if (sRef_getNullState (uentry_getSref (ue)) == NS_MNOTNULL)
	    {
	      ctype ct = ue->utype;
		  
	      if (ctype_isAbstract (ct) 
		  && (isExt || (ctype_isAbstract (ctype_realType (ct))
				&& !context_hasFileAccess (ctype_typeId (ct)))))
		{
		  vgenhinterror 
		    (FLG_INCONDEFS,
		     message 
		     ("%s %q declared %s notnull storage of abstract type %s",
		      ekind_capName (uentry_getKind (ue)),
		      uentry_getName (ue),
		      fcnErrName (ue),
		      ctype_unparse (ct)),
		     message 
		     ("Since %s is an abstract type, notnull can only be used "
		      "if it is static to a module where %s is accessible.",
		      ctype_unparse (ct),
		      ctype_unparse (ct)),
		     uentry_whereDeclared (ue));
		}
	    }
	}
    }
}

static void reflectImplicitFunctionQualifiers (/*@notnull@*/ uentry ue, bool spec)
{
  alkind ak = sRef_getAliasKind (ue->sref);

  if (alkind_isRefCounted (ak))
    {
      sRef_setAliasKind (ue->sref, AK_NEWREF, fileloc_undefined);
    }
  else 
    {
      if (alkind_isUnknown (ak))
	{
	  exkind ek = sRef_getExKind (ue->sref);
	  
	  if (exkind_isKnown (ek))
	    {
	      sRef_setAliasKind (ue->sref, AK_IMPDEPENDENT, fileloc_undefined);
	    }
	  else 
	    {
	      if (context_getFlag (spec ? FLG_SPECRETIMPONLY : FLG_RETIMPONLY))
		{
		  if (ctype_isVisiblySharable 
		      (ctype_realType (ctype_returnValue (ue->utype))))
		    {
		      if (uentryList_hasReturned (uentry_getParams (ue)))
			{
			  ;
			}
		      else
			{
			  sRef_setAliasKind (ue->sref, AK_IMPONLY, 
					     fileloc_undefined);
			  			}
		    }
		}
	    }
	}
    }
}

static /*@notnull@*/ uentry 
uentry_makeFunctionAux (cstring n, ctype t, 
			typeIdSet access,
			/*@only@*/ globSet globs, 
			/*@only@*/ sRefSet mods, 
			/*@keep@*/ fileloc f, bool priv,
			/*@unused@*/ bool isForward)
{
  uentry e = uentry_alloc ();
  ctype ret;

  if (ctype_isFunction (t))
    {
      ret = ctype_returnValue (t);
    }
  else
    {
      if (ctype_isKnown (t))
	{
	  llbug (message ("not function: %s", ctype_unparse (t)));
	}
      ret = ctype_unknown;
    }

  e->ukind = KFCN;

  if (fileloc_isSpec (f) || fileloc_isImport (f))
    {
      e->whereSpecified = f;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereSpecified = fileloc_undefined;
      e->whereDeclared = f;
    }

  /* e->shallowCopy = FALSE; */
  e->uname = cstring_copy (n);
  e->utype = t;
  e->storageclass = SCNONE;

    e->sref = sRef_makeType (ret);

  if (ctype_isUA (ret))
    {
      sRef_setStateFromType (e->sref, ret);
    }
  
  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();
  e->isPrivate = priv;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->fcn = (ufinfo) dmalloc (sizeof (*e->info->fcn));

  e->info->fcn->hasMods = sRefSet_isDefined (mods);
  e->info->fcn->hasGlobs = globSet_isDefined (globs);

  e->info->fcn->exitCode = XK_UNKNOWN;
  e->info->fcn->nullPred = QU_UNKNOWN;
  e->info->fcn->specialCode = SPC_NONE;

  e->info->fcn->access = access;
  e->info->fcn->globs = globs;
  e->info->fcn->defparams = uentryList_undefined;

  sRef_setDefined (e->sref, f);
  e->whereDefined = fileloc_undefined;
  
  e->info->fcn->mods = sRefSet_undefined;
  e->info->fcn->specclauses = NULL;
  checkGlobalsModifies (e, mods);
  e->info->fcn->mods = mods;

  return (e);
}

/*@notnull@*/ uentry uentry_makeIdFunction (idDecl id)
{
  uentry ue = 
    uentry_makeFunction (idDecl_observeId (id), idDecl_getCtype (id), 
			 typeId_invalid, globSet_undefined, 
			 sRefSet_undefined, 
			 setLocation ());
  
  uentry_reflectQualifiers (ue, idDecl_getQuals (id));
  reflectImplicitFunctionQualifiers (ue, FALSE);

  if (!uentry_isStatic (ue)
      && cstring_equalLit (ue->uname, "main"))
    {
      ctype typ = ue->utype;
      ctype retval;
      uentryList args;

      llassert (ctype_isFunction (typ));

      retval = ctype_returnValue (typ);

      if (!ctype_isInt (retval))
	{
	  voptgenerror 
	    (FLG_MAINTYPE,
	     message ("Function main declared to return %s, should return int",
		      ctype_unparse (retval)),
	     uentry_whereDeclared (ue));
	}

      args = ctype_argsFunction (typ);

      if (uentryList_isMissingParams (args) 
	  || uentryList_size (args) == 0)
	{
	  ;
	}
      else
	{
	  if (uentryList_size (args) != 2)
	    {
	      voptgenerror 
		(FLG_MAINTYPE,
		 message ("Function main declared with %d arg%p, "
			  "should have 2 (int argc, char *argv[])",
			  uentryList_size (args)),
		 uentry_whereLast (ue));
	    }
	  else
	    {
	      uentry arg = uentryList_getN (args, 0);
	      ctype ct = uentry_getType (arg);

	      if (!ctype_isInt (ct))
		{
		  voptgenerror 
		    (FLG_MAINTYPE,
		     message ("Parameter 1, %q, of function main declared "
			      "with type %t, should have type int",
			      uentry_getName (arg), ct),
		     uentry_whereDeclared (arg));
		}

	      arg = uentryList_getN (args, 1);
	      ct = uentry_getType (arg);

	      if (ctype_isArrayPtr (ct)
		  && ctype_isArrayPtr (ctype_baseArrayPtr (ct))
		  && ctype_isChar (ctype_baseArrayPtr (ctype_baseArrayPtr (ct))))
		{
		  ;
		}
	      else
		{
		  voptgenerror 
		    (FLG_MAINTYPE,
		     message ("Parameter 2, %q, of function main declared "
			      "with type %t, should have type char **",
			      uentry_getName (arg), ct),
		     uentry_whereDeclared (arg));
		}
	    }
	}
    }

  return ue;
}

static void uentry_implicitParamAnnots (/*@notnull@*/ uentry e)
{
  alkind ak = sRef_getAliasKind (e->sref);

  if ((alkind_isUnknown (ak) || alkind_isImplicit (ak))
      && context_getFlag (FLG_PARAMIMPTEMP))
    {
      exkind ek = sRef_getExKind (e->sref);
      
      if (exkind_isKnown (ek))
	{
	  sRef_setAliasKind (e->sref, AK_IMPDEPENDENT, fileloc_undefined);
	  sRef_setOrigAliasKind (e->sref, AK_IMPDEPENDENT);
	}
      else
	{
	  sRef_setAliasKind (e->sref, AK_IMPTEMP, fileloc_undefined);
	  sRef_setOrigAliasKind (e->sref, AK_IMPTEMP);
	}
    }
}

static /*@only@*/ /*@notnull@*/ uentry 
uentry_makeVariableParamAux (cstring n, ctype t, sRef s, sstate defstate)
{
  cstring pname = makeParam (n);
  uentry e = uentry_makeVariableAux (pname, t, setLocation (), s, FALSE, VKPARAM);

  cstring_free (pname);
  uentry_implicitParamAnnots (e);

  if (!sRef_isAllocated (e->sref) && !sRef_isPartial (e->sref))
    {
      sRef_setDefState (e->sref, defstate, uentry_whereDeclared (e));
      e->info->var->defstate = defstate;
    }

  return (e);
}

# ifndef NOLCL
void
uentry_setRefCounted (uentry e)
{
  if (uentry_isValid (e))
    {
      uentry_setAliasKind (e, AK_REFCOUNTED);
      sRef_storeState (e->sref);
    }
}
# endif

void
uentry_setStatic (uentry c)
{
  if (uentry_isValid (c)) 
    {
      alkind ak = sRef_getAliasKind (c->sref);
      c->storageclass = SCSTATIC;

      if (uentry_isVariable (c) && !ctype_isFunction (uentry_getType (c)))
	{
	  if (!alkind_isUnknown (ak)
	      && !alkind_isStatic (ak))
	    {
	      if  (!(ctype_isRealPointer (uentry_getType (c)))
		   && !(ctype_isAbstract (ctype_realType (uentry_getType (c))))
		   && !alkind_isRefCounted (ak))
		{
		  if (alkind_isImplicit (ak)
		      && alkind_isDependent (ak)
		      && ctype_isArray (uentry_getType (c)))
		    {
		      ; /* no error for observer arrays */
		    }
		  else
		    {
		      voptgenerror 
			(FLG_INCONDEFS,
			 message ("Static storage %q declared as %s",
				  uentry_getName (c),
				  alkind_unparse (ak)),
			 uentry_whereDeclared (c));
		    }
		}
	    }
	  else
	    {
	      if (alkind_isUnknown (ak)
		  || (alkind_isImplicit (sRef_getAliasKind (c->sref))
		      && !alkind_isDependent (sRef_getAliasKind (c->sref))))
		{
		  sRef_setAliasKind (c->sref, AK_STATIC, fileloc_undefined);
		  sRef_setOrigAliasKind (c->sref, AK_STATIC);
		}
	    }
	}
    }
}

void
uentry_setExtern (uentry c)
{
  if (uentry_isValid (c)) 
    c->storageclass = SCEXTERN;
}

void
uentry_setParamNo (uentry ue, int pno)
{
  llassert (uentry_isAnyParam (ue) && sRef_isParam (ue->sref));
  sRef_setParamNo (ue->sref, pno);
}

static
void checkGlobalsModifies (/*@notnull@*/ uentry ue, sRefSet sr)
{
  sRefSet_allElements (sr, el)
    {
      sRef base = sRef_getRootBase (el);
      
      if (sRef_isGlobal (base) || sRef_isInternalState (base)
	  || (sRef_isKindSpecial (base) && !sRef_isNothing (base)))
	{
	  if (!globSet_member (ue->info->fcn->globs, base))
	    {
	      if (uentry_hasGlobs (ue)
		  || context_getFlag (FLG_WARNMISSINGGLOBALSNOGLOBS))
		{
		  if (optgenerror 
		      (FLG_WARNMISSINGGLOBALS,
		       message
		       ("Modifies list for %q uses global %q, "
			"not included in globals list.",
			uentry_getName (ue),
			sRef_unparse (base)),
		       uentry_whereLast (ue)))
		    {
		      uentry_showWhereSpecified (ue);
		    } 
		}
	      
	      ue->info->fcn->globs = globSet_insert (ue->info->fcn->globs, 
						     base);
	      if (sRef_isFileStatic (base))
		{
		  context_recordFileGlobals (ue->info->fcn->globs);
		}
	    }
	}
    } end_sRefSet_allElements;
}

uentry
uentry_makeVariableSrefParam (cstring n, ctype t, sRef s)
{
  return (uentry_makeVariableParamAux (n, t, s, SS_UNKNOWN));
}

void
uentry_fixupSref (uentry ue)
{
  sRef sr;
  
  if (uentry_isUndefined (ue) || uentry_isElipsisMarker (ue)) 
    {
      return;
    }
  
  sr = uentry_getSref (ue);

  sRef_resetState (sr);
  sRef_clearDerived (sr);
  
  llassertprint (uentry_isVariable (ue), ("fixing: %s", uentry_unparseFull (ue)));
  llassert (sRef_isValid (sr)); 
  
  if (uentry_isVariable (ue))
    {
      sRef_setDefState (sr, ue->info->var->defstate, fileloc_undefined);
      sRef_setNullState (sr, ue->info->var->nullstate, fileloc_undefined);
    }
}

void uentry_setSpecialClauses (uentry ue, specialClauses clauses)
{
  llassert (uentry_isFunction (ue));
  llassert (ue->info->fcn->specclauses == NULL);

  ue->info->fcn->specclauses = clauses;
  specialClauses_checkAll (ue);
}

/*
** Used for @modifies@ @endmodifies@ syntax.
**
** If ue is specified, sr must contain *only*:
**
**      o file static globals
**      o sRef's derived from modifies spec (i.e., more specific than
**        what was specified)
**
** Otherwise, if sr has modifies it must match sr.
**
** If it doesn't have modifies, set them to sr.
*/

void
uentry_setModifies (uentry ue, /*@owned@*/ sRefSet sr)
{
  if (sRef_modInFunction ())
    {
      llparseerror
	(message ("Modifies list not in function context.  "
		  "A modifies list can only appear following the parameter list "
		  "in a function declaration or header."));

      /*@-mustfree@*/ return; /*@=mustfree@*/ 
    }

  if (sRefSet_hasStatic (sr))
    {
      context_recordFileModifies (sr);
    }

  if (uentry_isValid (ue))
    {
      if (uentry_isIter (ue))
	{
	  llassert (sRefSet_isUndefined (ue->info->iter->mods));
	  ue->info->iter->mods = sr;
	}
      else
	{
	  if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
	    {
	      uentry_makeVarFunction (ue);
	    }
	  
	  llassertfatal (uentry_isFunction (ue));
	  llassert (sRefSet_isUndefined (ue->info->fcn->mods));
	  
	  ue->info->fcn->mods = sr;
	  ue->info->fcn->hasMods = TRUE;
	  
	  checkGlobalsModifies (ue, sr);
	}
      
      if (context_getFlag (FLG_MODIFIESIMPNOGLOBALS))
	{
	  ue->info->fcn->hasGlobs = TRUE;
	}
    }
  else
    {
      sRefSet_free (sr);
    }
}

/*
** requires: new and old are functions
*/
 
static void
checkGlobalsConformance (/*@notnull@*/ uentry old, 
			 /*@notnull@*/ uentry unew, 
			 bool mustConform, bool completeConform)
{
  bool hasInternalState = FALSE;

  old->info->fcn->hasGlobs |= unew->info->fcn->hasGlobs;

  if (globSet_isDefined (unew->info->fcn->globs))
    {
      globSet_allElements (unew->info->fcn->globs, el)
	{
	  if (sRef_isFileStatic (el))
	    {
	      sRef sr = globSet_lookup (old->info->fcn->globs, el);

	      if (sRef_isInvalid (sr))
		{
		  bool hasError = FALSE;

		  if (!hasInternalState 
		      && sRef_isInvalid (globSet_lookup (old->info->fcn->globs,
							 sRef_makeInternalState ()))
		      && sRef_isInvalid (globSet_lookup (old->info->fcn->globs,
							 sRef_makeSpecState ())))
		    {
		      if (mustConform 
			  && !uentry_isStatic (old)
			  && optgenerror 
			  (FLG_INCONDEFS,
			   message ("Globals list for %q includes internal state, %q, "
				    "but %s without globals internalState.",
				    uentry_getName (old),
				    sRef_unparse (el),
				    uentry_specOrDefName (old)),
			   uentry_whereLast (unew)))
			{
			  uentry_showWhereSpecified (old);
			  hasError = TRUE;
			}
		      
		      old->info->fcn->globs = globSet_insert (old->info->fcn->globs,
							      sRef_makeInternalState ());
		      hasInternalState = TRUE;
		    }

		  if (!hasError
		      && fileloc_sameFile (uentry_whereDeclared (unew),
					   uentry_whereDeclared (old)))
		    {
		      if (mustConform
			  && optgenerror 
			  (FLG_INCONDEFS,
			   message ("Function %q inconsistently %rdeclared (in "
				    "same file) with file static global %q in "
				    "globals list",
				    uentry_getName (unew),
				    uentry_isDeclared (old),
				    sRef_unparse (el)),
			   uentry_whereDeclared (unew)))
			{
			  uentry_showWhereSpecified (old);
			}
		    }
		}

	      old->info->fcn->globs = globSet_insert (old->info->fcn->globs, el);
	      context_recordFileGlobals (old->info->fcn->globs);
	    }
	  else
	    {
	      sRef sr = globSet_lookup (old->info->fcn->globs, el);
	      
	      if (sRef_isInvalid (sr))
		{
		  if (mustConform
		      && optgenerror 
		      (FLG_INCONDEFS,
		       message ("Function %q inconsistently %rdeclared with "
				"%q in globals list",
				uentry_getName (unew),
				uentry_isDeclared (old),
				sRef_unparse (el)),
		       uentry_whereDeclared (unew)))
		    {
		      old->info->fcn->globs = globSet_insert (old->info->fcn->globs, el);
		      uentry_showWhereSpecified (old);
		    }
		}
	      else
		{
		  if (!bool_equal (sRef_isAllocated (el), sRef_isAllocated (sr)))
		    {
		      if (mustConform
			  && optgenerror 
			  (FLG_INCONDEFS,
			   message 
			   ("Function %q global %q inconsistently "
			    "%rdeclared as %qout global",
			    uentry_getName (unew),
			    sRef_unparse (el),
			    uentry_isDeclared (old),
			    cstring_makeLiteral (sRef_isAllocated (el) ? "" : "non-")),
			   uentry_whereDeclared (unew)))
			{
			  uentry_showWhereSpecified (old);
			}
		    }
		}
	    }
	} end_globSet_allElements ;

      if (completeConform)
	{
	  globSet_allElements (old->info->fcn->globs, el)
	    {
	      sRef sr = globSet_lookup (unew->info->fcn->globs, el);
	      
	      if (sRef_isInvalid (sr))
		{
		  if (mustConform
		      && uentry_isReallySpecified (old)
		      && optgenerror 
		      (FLG_NEEDSPEC,
		       message ("Function %q specified with %q in globals list, "
				"but declared without %q",
				uentry_getName (unew),
				sRef_unparse (el),
				sRef_unparse (el)),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereSpecified (old);
		    }
		}
	    } end_globSet_allElements;
	}
    }
  else
    {
      if (completeConform && !globSet_isEmpty (old->info->fcn->globs))
	{
	  if (uentry_isReallySpecified (old)
	      && optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified with globals list, but "
			"declared with no globals",
			ekind_capName (unew->ukind),
			uentry_getName (unew)),
	       uentry_whereDeclared (unew)))
	    {
	      llgenindentmsg 
		(message ("Specification globals: %q", 
			  globSet_unparse (old->info->fcn->globs)),
		 uentry_whereSpecified (old));
	    }
	}
      
      unew->info->fcn->globs = globSet_copy (unew->info->fcn->globs, 
					    old->info->fcn->globs);
    }
}

/*
** new modifies list must be included by old modifies list.
**
** file static state may be added to new, if old has internal.
*/

static void
checkModifiesConformance (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, 
			  bool mustConform, bool completeConform)
{
  sRefSet newMods;
  bool changedMods = FALSE;
  bool modInternal = FALSE;

  llassert (uentry_isFunction (old) && uentry_isFunction (unew));

  old->info->fcn->hasMods |= unew->info->fcn->hasMods;
  newMods = unew->info->fcn->mods;
	    
  if (sRefSet_isEmpty (newMods))
    {
      if (completeConform && !sRefSet_isEmpty (old->info->fcn->mods)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified with modifies clause, "
			"but declared with no modifies clause",
			ekind_capName (unew->ukind),
			uentry_getName (unew)),
	       uentry_whereDeclared (unew)))
	    {
	      llgenindentmsg (message ("Specification has modifies %q", 
				       sRefSet_unparse (old->info->fcn->mods)),
			      uentry_whereSpecified (old));
	    }
	}

      return;
    }

  sRefSet_allElements (newMods, current)
    {
      if (sRef_isValid (current))
	{
	  sRef rb = sRef_getRootBase (current);
	  
	  if (sRef_isFileStatic (rb))
	    {
	      if (!modInternal)
		{
		  if (!sRefSet_isSameMember (old->info->fcn->mods, 
					     sRef_makeInternalState ())
		      && !sRefSet_isSameMember (old->info->fcn->mods, 
						sRef_makeSpecState ()))
		    {
		      if (mustConform 
			  && !uentry_isStatic (old)
			  && optgenerror 
			  (FLG_INCONDEFS,
			   message
			   ("Modifies list for %q includes internal state, "
			    "but %s without modifies internal.",
			    uentry_getName (old),
			    uentry_specOrDefName (old)),
			   uentry_whereLast (unew)))
			{
			  uentry_showWhereSpecified (old);
			}
		      
		      old->info->fcn->mods = 
			sRefSet_insert (old->info->fcn->mods, 
					sRef_makeInternalState ());
		      modInternal = TRUE;
		    }
		}
	      
	      old->info->fcn->mods = sRefSet_insert (old->info->fcn->mods,
						     current);
	      changedMods = TRUE;
	    }
	  else
	    {
	      if (sRef_canModifyVal (current, old->info->fcn->mods))
		{
		  int size = sRefSet_size (old->info->fcn->mods);

		  old->info->fcn->mods = sRefSet_insert (old->info->fcn->mods,
							 current);

		  if (sRefSet_size (old->info->fcn->mods) != size)
		    {
		      changedMods = TRUE;
		    }
		}
	      else
		{
		  if (mustConform
		      && optgenerror 
		      (FLG_INCONDEFS,
		       message 
		       ("Modifies list for %q contains %q, not modifiable "
			"according to %s",
			uentry_getName (old),
			sRef_unparse (current),
			uentry_specDeclName (old)),
		       uentry_whereLast (unew)))
		    {
		      uentry_showWhereSpecified (old);
		    }
		}
	    }
	}
    } end_sRefSet_allElements;

  if (completeConform && uentry_isReallySpecified (old))
    {
      sRefSet_allElements (old->info->fcn->mods, el)
	{
	  if (sRef_canModify (el, newMods))
	    {
	      ; /* okay */
	    }
	  else
	    {
	      if (optgenerror 
		  (FLG_NEEDSPEC,
		   message 
		   ("Specification modifies clause for %q contains %q, "
		    "not included in declaration modifies clause",
		    uentry_getName (old),
		    sRef_unparse (el)),
		   uentry_whereLast (unew)))
		{
		  uentry_showWhereSpecified (old);
		}
	    }
	} end_sRefSet_allElements ;
    } 

  /*
  ** Make sure file static elements will be removed.
  */

  if (changedMods)
    {
      context_recordFileModifies (old->info->fcn->mods);
    }
}

static void 
  uentry_checkMutableType (uentry ue)
{
  ctype ct = uentry_getType (ue);

  if (!ctype_isRealPointer (ct) && !ctype_isRealAbstract (ct))
    {
      voptgenerror (FLG_MUTREP,
		    message ("Mutable abstract type %q declared without pointer "
			     "indirection: %t (violates assignment semantics)",
			     uentry_getName (ue), ct),
		    uentry_whereDeclared (ue));
    }
}

void
uentry_setMutable (uentry e)
{
  llassert (uentry_isDatatype (e));
  e->info->datatype->mut = YES;
}

static void
uentry_checkIterArgs (uentry ue)
{
  bool hasYield = FALSE;
  uentryList args;

  llassert (uentry_isIter (ue));

  args = uentry_getParams (ue);

  uentryList_elements (args, el)
    {
      sstate ds = uentry_getDefState (el);

      if (uentry_isYield (el))
	{
	  hasYield = TRUE;
	}

      if (sstate_isUnknown (ds))
	{
	  uentry_setDefState (el, SS_DEFINED);
	}
      else
	{
	  ;
	}
    } end_uentryList_elements;

  if (!hasYield)
    {
      voptgenerror (FLG_HASYIELD,
		    message ("Iterator %q declared with no yield parameters",
			     uentry_getName (ue)),
		    uentry_whereDeclared (ue));
    }
}

static chkind
chkind_fromQual (qual qel)
{
  if (qual_isChecked (qel))
    {
      return CH_CHECKED;
    }
  else if (qual_isCheckMod (qel))
    {
      return CH_CHECKMOD;
    }
  else if (qual_isCheckedStrict (qel))
    {
      return CH_CHECKEDSTRICT;
    }
  else if (qual_isUnchecked (qel))
    {
      return CH_UNCHECKED;
    }
  else
    {
      BADEXIT;
      /*@notreached@*/ return CH_UNKNOWN;
    }
}

static void
uentry_reflectOtherQualifier (/*@notnull@*/ uentry ue, qual qel)
{
  if (qual_isKillRef (qel) || qual_isNewRef (qel) || qual_isTempRef (qel))
    {
      if (!uentry_isRefCounted (ue))
	{
	  llerror 
	    (FLG_SYNTAX, 
	     message ("Reference counting qualifier %s used on non-reference "
		      "counted storage: %q",
		      qual_unparse (qel), 
		      uentry_unparse (ue)));
	}
      else
	{
	  alkind ak = alkind_fromQual (qel);
	  
	  uentry_setAliasKind (ue, ak);
	}
    }
  else if (qual_isRefCounted (qel))
    {
      ctype ct = ctype_realType (uentry_getType (ue));
      ctype rt;
      
      if (ctype_isPointer (ct) 
	  && (ctype_isStruct (rt = ctype_realType (ctype_baseArrayPtr (ct)))))
	{
	  /* check there is a refs field */
	  uentryList fields = ctype_getFields (rt);
	  uentry refs = uentry_undefined;

	  uentryList_elements (fields, field)
	    {
	      if (uentry_isRefsField (field))
		{
		  if (uentry_isValid (refs))
		    {
		      llerror 
			(FLG_SYNTAX, 
			 message ("Reference counted structure type %s has "
				  "multiple refs fields: %q and %q",
				  ctype_unparse (ct),
				  uentry_getName (refs),
				  uentry_getName (field)));
		    }
		  
		  refs = field;
		}
	    } end_uentryList_elements;

	  if (uentry_isInvalid (refs))
	    {
	      vgenhinterror 
		(FLG_SYNTAX, 
		 message ("Reference counted structure type %s has "
			  "no refs field",
			  ctype_unparse (ct)),
		 cstring_makeLiteral
		 ("To count reference, the structure must have a field named "
		  "refs of type int."),
		 g_currentloc);		  
	    }
	  else if (!ctype_isInt (uentry_getType (refs)))
	    {
	      llerror 
		(FLG_SYNTAX, 
		 message ("Reference counted structure type %s refs field has "
			  "type %s (should be int)", ctype_unparse (ct),
			  ctype_unparse (uentry_getType (refs))));
	    }
	  else
	    {
	      sRef_setAliasKind (ue->sref, alkind_fromQual (qel), 
				 uentry_whereDeclared (ue));
	    }
	}
      else
	{
	  if ((ctype_isPointer (ct) 
	       && ctype_isUnknown (ctype_realType (ctype_baseArrayPtr (ct))))
	      ||ctype_isAbstract (ct) || ctype_isUnknown (ct))
	    {
	      sRef_setAliasKind (ue->sref, alkind_fromQual (qel), 
				 uentry_whereDeclared (ue));
	    }
	  else
	    {
	      llerror 
		(FLG_SYNTAX, 
		 message ("Non-pointer to structure type %s declared with "
			  "refcounted qualifier",
			  ctype_unparse (ct)));
	    }
	}
    }
  else if (qual_isRefs (qel))
    {
      if (uentry_isVariable (ue) && !uentry_isParam (ue))
	{
	  uentry_setAliasKind (ue, AK_REFS);
	}
      else
	{
	  llerror 
	    (FLG_SYNTAX, 
	     message ("Refs qualifier used on non-structure field: %q",
		      uentry_unparse (ue)));
	}
    }
  else if (qual_isAliasQual (qel))
    {
      alkind ak = alkind_fromQual (qel);
      bool okay = TRUE;
      alkind oldak = uentry_getAliasKind (ue);
      ctype ut = uentry_getType (ue);
      
      if (alkind_isImplicit (ak) 
	  && (alkind_isKnown (oldak) && !alkind_isImplicit (oldak)))
	{
	  /* ignore the implied qualifier */
	  okay = FALSE;
	}
      
      if (uentry_isEitherConstant (ue))
	{
	  llerror 
	    (FLG_SYNTAX, 
	     message ("Alias qualifier %s used on constant: %q",
		      alkind_unparse (ak), uentry_unparse (ue)));
	  okay = FALSE;
	}
      
      if (ctype_isFunction (ut))
	{
	  ut = ctype_returnValue (ut);
	}
      
      if (!(ctype_isVisiblySharable (ut) 
	    || ctype_isRealArray (ut)
	    || ctype_isRealSU (ut)))
	{
	  if (!qual_isImplied (qel))
	    {
	      llerror 
		(FLG_SYNTAX, 
		 message ("Alias qualifier %s used on unsharable storage type %t: %q",
			  alkind_unparse (ak), ut, uentry_getName (ue)));
	    }
	  
	  okay = FALSE;
	}
      else
	{
	  if (uentry_isRefCounted (ue))
	    {
	      if (!(qual_isRefQual (qel) || qual_isOnly (qel)
		    || qual_isExposed (qel) 
		    || qual_isObserver (qel)))
		{
		  if (!qual_isImplied (qel))
		    {
		      llerror 
			(FLG_SYNTAX, 
			 message 
			 ("Alias qualifier %s used on reference counted storage: %q",
			  alkind_unparse (ak), 
			  uentry_unparse (ue)));
		    }
		  
		  okay = FALSE;
		}
	    }
	  else
	    {
	      if (qual_isRefQual (qel))
		{
		  llerror 
		    (FLG_SYNTAX, 
		     message ("Qualifier %s used on non-reference counted storage: %q",
			      alkind_unparse (ak), uentry_unparse (ue)));
		  
		  okay = FALSE;
		}
	    }
	}
      
      if (okay)
	{
	  uentry_setAliasKind (ue, ak);
	}
    }
  else if (qual_isNull (qel))
    {
      if (uentry_isConstant (ue))
	{
	  sRef_setNullState 
	    (ue->sref, 
	     ctype_isAbstract (ue->utype) ? NS_CONSTNULL : NS_DEFNULL, 
	     uentry_whereDeclared (ue));
	}
      else
	{
	  uentry_setNullState (ue, NS_POSNULL);
	}
    }
  else if (qual_isRelNull (qel))
    {
      uentry_setNullState (ue, NS_RELNULL);
    }
  else if (qual_isNotNull (qel))
    {
      uentry_setNullState (ue, NS_MNOTNULL);
    }
  else if (qual_isAbstract (qel)
	   || qual_isConcrete (qel))
    {
      if (!uentry_isDatatype (ue))
	{
	  llerror 
	    (FLG_SYNTAX, 
	     message ("Qualifier %s used with non-datatype", 
		      qual_unparse (qel)));
	}
      else
	{
	  ue->info->datatype->abs = ynm_fromBool (qual_isAbstract (qel));
	}
    }
  else if (qual_isMutable (qel))
    {
      if (!uentry_isDatatype (ue))
	{
	  llerror (FLG_SYNTAX,
		   message ("Qualifier %s used with non-datatype", qual_unparse (qel)));
	}
      else
	{
	  if (!ynm_isOn (ue->info->datatype->mut))
	    {
	      uentry_checkMutableType (ue);
	    }
	  
	  ue->info->datatype->mut = YES;
	}
    }
  else if (qual_isImmutable (qel))
    {
      if (!uentry_isDatatype (ue))
	{
	  llerror (FLG_SYNTAX, message ("Qualifier %s used with non-datatype", 
					qual_unparse (qel)));
	}
      else
	{
	  ue->info->datatype->mut = NO;
	}
    }
  else if (qual_isNullPred (qel))
    {
      if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
	{
	  uentry_makeVarFunction (ue);
	}
      
      if (uentry_isFunction (ue))
	{
	  ctype typ = uentry_getType (ue);
	  ctype rtype = ctype_returnValue (uentry_getType (ue));
	  
	  if (ctype_isRealBool (rtype))
	    {
	      uentryList pl = ctype_argsFunction (typ);
	      
	      if (uentryList_size (pl) == 1)
		{
		  ue->info->fcn->nullPred = qel;
		}
	      else
		{
		  llerror (FLG_SYNTAX, 
			   message ("Qualifier %s used with function having %d "
				    "arguments (should have 1)", 
				    qual_unparse (qel),
				    uentryList_size (pl)));
		}
	    }
	  else
	    {
	      llerror (FLG_SYNTAX, 
		       message ("Qualifier %s used with function returning %s "
				"(should return bool)", 
				qual_unparse (qel),
				ctype_unparse (rtype)));
	    }
	}
      else
	{
	  llerror (FLG_SYNTAX, 
		   message ("Qualifier %s used with non-function", 
			    qual_unparse (qel)));
	}
    }
  else if (qual_isExitQual (qel))
    {
      exitkind exk = exitkind_fromQual (qel);
      
      if (uentry_isFunction (ue))
	{
	  if (exitkind_isKnown (ue->info->fcn->exitCode))
	    {
	      llerror (FLG_SYNTAX, 
		       message ("Multiple exit qualifiers used on function %q:  %s, %s", 
				uentry_getName (ue),
				exitkind_unparse (ue->info->fcn->exitCode),
				exitkind_unparse (exk)));
	    }
	  
	  ue->info->fcn->exitCode = exk;
	}
      else
	{
	  if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
	    {
	      uentry_makeVarFunction (ue);
	      ue->info->fcn->exitCode = exk;
	    }
	  else
	    {
	      llerror (FLG_SYNTAX,
		       message ("Exit qualifier %s used with non-function (type %s)", 
				qual_unparse (qel),
				ctype_unparse (uentry_getType (ue))));
	    }
	}
    }
  else
    {
      if (qual_isCQual (qel))
	{
	  ; /* okay */
	}
      else
	{
	  llbug (message ("unhandled qualifier: %s", qual_unparse (qel)));
	}
    }
}

void
uentry_reflectQualifiers (uentry ue, qualList q)
{
  llassert (uentry_isValid (ue)); 

  qualList_elements (q, qel)
    {
      if (qual_isStatic (qel))
	{
	  uentry_setStatic (ue);
	}
      else if (qual_isUnused (qel))
	{
	  uentry_setUsed (ue, fileloc_undefined);	  
	}
      else if (qual_isExternal (qel))
	{
	  fileloc_free (ue->whereDefined);
	  ue->whereDefined = fileloc_createExternal ();
	}
      else if (qual_isSef (qel))
	{
	  if (uentry_isVariable (ue))
	    {
	      vkind vk = ue->info->var->kind;

	      llassert (vk != VKREFPARAM);

	      if (vk == VKYIELDPARAM)
		{
		  llerror
		    (FLG_SYNTAX,
		     message ("Qualifier sef cannot be used with %s: %q",
			      cstring_makeLiteralTemp (vk == VKYIELDPARAM ? "yield" : "returned"),
			      uentry_unparse (ue)));
		}
	      else if (vk == VKRETPARAM)
		{
		  ue->info->var->kind = VKSEFRETPARAM;
		}
	      else
		{
		  ue->info->var->kind = VKSEFPARAM;
		}
	    }
	  else
	    {
	      llerror 
		(FLG_SYNTAX,
		 message ("Qualifier sef is meaningful only on parameters: %q", 
			  uentry_unparse (ue)));
	    }
	}
      else if (qual_isExtern (qel))
	{
	  ue->storageclass = SCEXTERN;
	}
      else if (qual_isGlobalQual (qel)) /* undef, killed */
	{
	  if (uentry_isVariable (ue))
	    {
	      sstate oldstate = ue->info->var->defstate;
	      sstate defstate = sstate_fromQual (qel);


	      if ((oldstate == SS_UNDEFGLOB && defstate == SS_KILLED)
		  || (oldstate == SS_KILLED && defstate == SS_UNDEFGLOB))
		{
		  defstate = SS_UNDEFKILLED;
		}
	      else 
		{
		  ; /* any errors? */
		}

	      sRef_setDefState (ue->sref, defstate, fileloc_undefined);
	      ue->info->var->defstate = defstate;
	    }
	  else
	    {
	      llerror 
		(FLG_SYNTAX, 
		 message ("Qualifier %s used on non-variable: %q",
			  qual_unparse (qel), uentry_unparse (ue)));	      
	    }
	}
      else if (qual_isAllocQual (qel)) /* out, partial, reldef, special, etc. */
	{
	  ctype realType = ctype_realType (ue->utype);
	  sstate defstate = sstate_fromQual (qel);

	  if (ctype_isFunction (realType))
	    {
	      realType = ctype_realType (ctype_returnValue (realType));
	    }

	  if (qual_isRelDef (qel))
	    {
	      ; /* okay anywhere */
	    }
	  else
	    {
	      if (!ctype_isAP (realType) 
		  && !ctype_isSU (realType)
		  && !ctype_isUnknown (realType)
		  && !ctype_isAbstract (ue->utype))
		{
		  llerror 
		    (FLG_SYNTAX, 
		     message ("Qualifier %s used on non-pointer or struct: %q",
			      qual_unparse (qel), uentry_unparse (ue)));
		}
	    }

	  uentry_setDefState (ue, defstate);

	  if (sRef_isStateSpecial (ue->sref)
	      && alkind_isImplicit (sRef_getAliasKind (ue->sref)))
	    {
	      sRef_setAliasKind (ue->sref, AK_ERROR, fileloc_undefined);
	    }
	}
      else if (qual_isYield (qel))
	{
	  if (uentry_isVariable (ue))
	    {
	      ue->info->var->kind = VKYIELDPARAM;
	    }
	  else
	    {
	      llerror 
		(FLG_SYNTAX, 
		 message ("Qualifier %s used on non-iterator parameter: %q",
			  qual_unparse (qel), uentry_unparse (ue)));	      
	    }
	}
      else if (qual_isExQual (qel))
	{
	  exkind ek = exkind_fromQual (qel);
	  ctype ut = uentry_getType (ue);

	  if (ctype_isFunction (ut))
	    {
	      ut = ctype_returnValue (ut);
	    }
	  
	  if (!(ctype_isVisiblySharable (ut))
	      && !(ctype_isArray (ut)) /* can apply to arrays also! */
	      && !(ctype_isStruct (ctype_realType (ut)))) /* applies to structure fields! */
	    {
	      if (!qual_isImplied (qel))
		{
		  llerror 
		    (FLG_SYNTAX, 
		     message ("Qualifier %s used on unsharable storage type %t: %q",
			      exkind_unparse (ek), ut, uentry_getName (ue)));
		}
	    }
	  else
	    {
	      alkind ak = sRef_getAliasKind (ue->sref);

	      sRef_setExKind (ue->sref, ek, uentry_whereDeclared (ue));

	      if (alkind_isUnknown (ak) || alkind_isImplicit (ak) || alkind_isStatic (ak))
		{
		  if (!alkind_isTemp (ak))
		    {
		      uentry_setAliasKind (ue, AK_IMPDEPENDENT);
		    }
		}
	      else if (alkind_isDependent (ak) || alkind_isTemp (ak)
		       || alkind_isOwned (ak))
		{
		  ; /* okay */
		}
	      else
		{
		  llerror 
		    (FLG_SYNTAX, 
		     message ("Exposure qualifier %s used on %s storage (should "
			      "be dependent): %q",
			      qual_unparse (qel), 
			      alkind_unparse (ak),
			      uentry_unparse (ue)));
		}
	    }
	}
      else if (qual_isGlobCheck (qel))
	{
	  if (uentry_isVariable (ue))
	    {
	      chkind ch = chkind_fromQual (qel); 		
		       
	      if (ue->info->var->checked != CH_UNKNOWN)
		{
		  if (ch == ue->info->var->checked)
		    {
		      llerror (FLG_SYNTAX, 
			       message ("Redundant %s qualifier on %q",
					qual_unparse (qel),
					uentry_getName (ue)));
		    }
		  else
		    {
		      llerror (FLG_SYNTAX, 
			       message
			       ("Contradictory %s and %s qualifiers on %q",
				qual_unparse (qel),
				checkedName (ue->info->var->checked),
				uentry_getName (ue)));
		    }
		}

	      ue->info->var->checked = ch;
	    }
	  else
	    {
	      llerror
		(FLG_SYNTAX, 
		 message ("Qualifier %s used with non-variable", 
			  qual_unparse (qel)));
	    }
	}
      else if (qual_isReturned (qel))
	{
	  if (uentry_isVariable (ue))
	    {
	      ue->info->var->kind = VKRETPARAM;
	    }
	  else
	    {
	      llerror (FLG_SYNTAX, message ("Qualifier %s used with non-variable", 
					    qual_unparse (qel)));
	    }
	}
      else
	{
	  uentry_reflectOtherQualifier (ue, qel);
	}

      sRef_storeState (ue->sref);
    } end_qualList_elements;

  qualList_clear (q);
}
	
bool
uentry_isOnly (uentry ue)
{
  return (!uentry_isUndefined (ue) 
	  && uentry_isVariable (ue) 
	  && alkind_isOnly (sRef_getOrigAliasKind (ue->sref)));
}

static void
uentry_setAliasKind (/*@notnull@*/ uentry ue, alkind ak)
{
  sRef_setAliasKind (ue->sref, ak, uentry_whereDeclared (ue));
  sRef_setOrigAliasKind (ue->sref, ak);
}

static void
uentry_setNullState (/*@notnull@*/ uentry ue, nstate ns)
{
  if (uentry_isVariable (ue))
    {
      ue->info->var->nullstate = ns;
    }

  sRef_setNullState (ue->sref, ns, uentry_whereDeclared (ue));
}

bool
uentry_isUnique (uentry ue)
{
  return (!uentry_isUndefined (ue) 
	  && uentry_isVariable (ue) 
	  && alkind_isUnique (sRef_getOrigAliasKind (ue->sref)));
}

bool
uentry_isFileStatic (uentry ue)
{
  return (uentry_isStatic (ue) 
	  && (!uentry_isVariable (ue)
	      || sRef_isFileStatic (uentry_getSref (ue))));
}

bool
uentry_isExported (uentry ue)
{
  if (uentry_isValid (ue))
    {
      if (uentry_isVariable (ue))
	{
	  return (sRef_isRealGlobal (uentry_getSref (ue)));
	}
      else
	{
	  return !uentry_isStatic (ue);
	}
    }

  return FALSE;
}

bool
uentry_isNonLocal (uentry ue)
{
  return (uentry_isValid (ue) && uentry_isVariable (ue)
	  && (sRef_isGlobal (ue->sref) || uentry_isStatic (ue)));
}

bool
uentry_isGlobal (uentry ue)
{
  return (uentry_isValid (ue) && uentry_isVariable (ue) && 
	  sRef_isGlobal (ue->sref));
}

bool
uentry_isPrintfLike (uentry ue)
{
  return (uentry_isFunction (ue) 
	  && (ue->info->fcn->specialCode == SPC_PRINTFLIKE));
}

bool
uentry_isScanfLike (uentry ue)
{
  return (uentry_isFunction (ue) 
	  && (ue->info->fcn->specialCode == SPC_SCANFLIKE));
}

bool
uentry_isMessageLike (uentry ue)
{
  return (uentry_isFunction (ue) 
	  && (ue->info->fcn->specialCode == SPC_MESSAGELIKE));
}

static void checkSpecialFunction (/*@notnull@*/ uentry ue)
{
  uentryList args = uentry_getParams (ue);

  if (!uentryList_isMissingParams (args))
    {
      uentry last = uentry_undefined;

      uentryList_elements (args, current)
	{
	  if (uentry_isElipsisMarker (current))
	    {
	      if (uentry_isUndefined (last))
		{
		  voptgenerror 
		    (FLG_SYNTAX,
		     message ("Function %q is marked %s, but has no format "
			      "string argument before elipsis",
			      uentry_getName (ue),
			      specCode_unparse (ue->info->fcn->specialCode)),
		     uentry_whereLast (ue));
		  ue->info->fcn->specialCode = SPC_NONE;
		}
	      else
		{
		  ctype rt = ctype_realType (uentry_getType (last));

		  if (!ctype_match (rt, ctype_string))
		    {
		      bool okay = FALSE;

		      /* wchar_t * is okay too */
		      if (ctype_isAP (rt))
			{
			  ctype base = ctype_baseArrayPtr (rt);
			  
			  if (ctype_isArbitraryIntegral (base)) 
			    {
			      okay = TRUE;
			    }
			}
		      
		      if (!okay) 
			{
			  voptgenerror
			    (FLG_SYNTAX,
			     message ("Function %q is marked %s, but the argument "
				      "before the elipsis has type %s (should be char *)",
				      uentry_getName (ue),
				      specCode_unparse (ue->info->fcn->specialCode),
				      ctype_unparse (uentry_getType (last))),
			     uentry_whereLast (ue));
			  
			  ue->info->fcn->specialCode = SPC_NONE;
			}
		    }
		}
	      return;
	    }
	  last = current;
	} end_uentryList_elements ;

      voptgenerror 
	(FLG_SYNTAX,
	 message ("Function %q is marked %s, but has no elipsis parameter",
		  uentry_getName (ue),
		  specCode_unparse (ue->info->fcn->specialCode)),
	 uentry_whereLast (ue));

      ue->info->fcn->specialCode = SPC_NONE;
    }
}

void
uentry_setPrintfLike (uentry ue)
{
  if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
    {
      uentry_makeVarFunction (ue);
    }
  
  llassertfatal (uentry_isFunction (ue));
  ue->info->fcn->specialCode = SPC_PRINTFLIKE;
  checkSpecialFunction (ue);
}

void
uentry_setScanfLike (uentry ue)
{
  if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
    {
      uentry_makeVarFunction (ue);
    }
  
  llassertfatal (uentry_isFunction (ue));
  ue->info->fcn->specialCode = SPC_SCANFLIKE;
  checkSpecialFunction (ue);
}

void
uentry_setMessageLike (uentry ue)
{
  if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
    {
      uentry_makeVarFunction (ue);
    }
  
  llassertfatal (uentry_isFunction (ue));
  ue->info->fcn->specialCode = SPC_MESSAGELIKE;
  checkSpecialFunction (ue);
}

bool
uentry_isSpecialFunction (uentry ue)
{
  return (uentry_isFunction (ue) 
	  && (ue->info->fcn->specialCode != SPC_NONE));
}

/*@notnull@*/ uentry uentry_makeParam (idDecl t, int i)
{
  ctype ct = idDecl_getCtype (t);
  ctype base = ct;
  sRef pref = sRef_makeParam (i, ct);
  uentry ue = uentry_makeVariableSrefParam (idDecl_observeId (t), ct, pref);

  uentry_reflectQualifiers (ue, idDecl_getQuals (t));
  uentry_implicitParamAnnots (ue);

  /* Parameter type [][] or [x][] is invalid */

  while (ctype_isFixedArray (base)) {
    base = ctype_baseArrayPtr (base);
  }

  if (ctype_isIncompleteArray (base)) {
    base = ctype_baseArrayPtr (base);

    if (ctype_isArray (base)) {
      (void) optgenerror (FLG_INCOMPLETETYPE, 
			  message ("Function parameter %q is incomplete type (inner array must have bounds): %s",
				   uentry_getName (ue),
				   ctype_unparse (ct)),
			  uentry_whereLast (ue));
    }
  }

  return ue;
}

/*@only@*/ /*@notnull@*/ uentry uentry_makeIdVariable (idDecl t)
{
  ctype ct = idDecl_getCtype (t);

  if (ctype_isFunction (ct))
    {
            return (uentry_makeIdFunction (t));
    }
  else
    {
      fileloc loc = setLocation ();
      uentry ue = uentry_makeVariable (idDecl_observeId (t), ct, loc, FALSE);
      
      uentry_reflectQualifiers (ue, idDecl_getQuals (t));

      if (!uentry_isExtern (ue))
	{
	  uentry_setDefined (ue, loc);
	}

      return ue;
    }
}

# ifndef NOLCL
/*@notnull@*/ uentry uentry_makeVariableParam (cstring n, ctype t)
{
  return (uentry_makeVariableParamAux (n, t, sRef_makeType (t), SS_DEFINED));
}
# endif

/*
** constants
*/

/*@only@*/ /*@notnull@*/ 
uentry uentry_makeConstantAux (cstring n, ctype t, 
			       /*@keep@*/ fileloc f, bool priv,
			       /*@only@*/ multiVal m)
{
  uentry e = uentry_alloc ();

  e->ukind = KCONST;
  e->uname = cstring_copy (n);
  e->utype = t;
  e->storageclass = SCNONE;

  e->sref  = sRef_makeConst (t);

  e->lset = FALSE;
  e->used = FALSE;
  
  e->uses = filelocList_new ();
  e->isPrivate = priv;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->uconst = (ucinfo) dmalloc (sizeof (*e->info->uconst));
  e->info->uconst->val = m;
  e->info->uconst->access = typeIdSet_undefined;

  uentry_setSpecDef (e, f);

  if (multiVal_isInt (m) && (multiVal_forceInt (m) == 0))
    {
      sRef_setDefNull (e->sref, uentry_whereDeclared (e)); 
    }

  return (e);
}

/*@notnull@*/ uentry uentry_makeConstant (cstring n, ctype t, fileloc f)
{
  return (uentry_makeConstantAux (n, t, f, FALSE, multiVal_unknown ()));
}

/*@notnull@*/ uentry uentry_makeIdConstant (idDecl t)
{
  uentry ue = uentry_makeConstant (idDecl_observeId (t), 
				   idDecl_getCtype (t), 
				   fileloc_undefined);

  llassert (fileloc_isUndefined (ue->whereDeclared));
  ue->whereDeclared = setLocation ();

  uentry_reflectQualifiers (ue, idDecl_getQuals (t));

  return ue;
}

/*
** variables
*/

void uentry_setDefState (uentry ue, sstate defstate)
{
  if (uentry_isValid (ue))
    {
      sRef_setDefState (ue->sref, defstate, fileloc_undefined);

      if (uentry_isVariable (ue))
	{
	  ue->info->var->defstate = defstate; /* evs 2000-05-17: fixed bug, was SS_DEFINED! */
	}
    }
}

bool uentry_isCheckedUnknown (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_UNKNOWN));
}

bool uentry_isCheckMod (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_CHECKMOD));
}

bool uentry_isUnchecked (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_UNCHECKED));
}

bool uentry_isChecked (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_CHECKED));
}

bool uentry_isCheckedModify (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_CHECKED
	      || ue->info->var->checked == CH_CHECKMOD
	      || ue->info->var->checked == CH_CHECKEDSTRICT));
}

bool uentry_isCheckedStrict (uentry ue)
{
  return (uentry_isVar (ue) 
	  && (ue->info->var->checked == CH_CHECKEDSTRICT));
}

void uentry_setUnchecked (uentry ue)
{
  llassert (uentry_isVar (ue));

  ue->info->var->checked = CH_UNCHECKED;
}

void uentry_setChecked (uentry ue)
{
  llassert (uentry_isVar (ue));

  ue->info->var->checked = CH_CHECKED;
}

void uentry_setCheckMod (uentry ue)
{
  llassert (uentry_isVar (ue));

  ue->info->var->checked = CH_CHECKMOD;
}

void uentry_setCheckedStrict (uentry ue)
{
  llassert (uentry_isVar (ue));
  
  ue->info->var->checked = CH_CHECKEDSTRICT;
}

static /*@only@*/ /*@notnull@*/ 
  uentry uentry_makeVariableAux (cstring n, ctype t, 
				 fileloc f,
				 /*@exposed@*/ sRef s,
				 bool priv, vkind kind)
{
  uentry e = uentry_alloc ();
  ctype rt = t;

  DPRINTF (("Make variable: %s %s %s", n, ctype_unparse (t), sRef_unparse (s)));

  e->ukind = KVAR;
  e->uname = cstring_copy (n);
  e->utype = t;

  e->storageclass = SCNONE;

  e->sref  = s;

  e->used = FALSE;
  e->lset = FALSE;

  e->uses = filelocList_new ();
  e->isPrivate = priv;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->var = (uvinfo) dmalloc (sizeof (*e->info->var));
  e->info->var->kind = kind;

  e->info->var->checked = CH_UNKNOWN;

  uentry_setSpecDef (e, f);

  if (ctype_isFunction (rt))
    {
      rt = ctype_returnValue (rt);
    }

  if (ctype_isUA (rt))
    {
      sRef_setStateFromType (e->sref, rt);
    }

  e->info->var->defstate = sRef_getDefState (e->sref);  
  e->info->var->nullstate = sRef_getNullState (e->sref);

  return (e);
}

bool
uentry_isYield (uentry ue)
{
  return (uentry_isVariable (ue) 
	  && (ue->info->var->kind == VKYIELDPARAM
	      || ue->info->var->kind == VKREFYIELDPARAM));
}

static bool
uentry_isRefsField (uentry ue)
{
  return (uentry_isVariable (ue) && sRef_isRefsField (ue->sref));
}

/*@only@*/ /*@notnull@*/ 
uentry uentry_makeVariable (cstring n, ctype t, fileloc f, bool isPriv)
{
  return (uentry_makeVariableAux (n, t, f, sRef_makeType (t), isPriv, 
				  fileloc_isSpec (f) ? VKSPEC : VKNORMAL));
}

/*
** functions
*/

void uentry_makeVarFunction (uentry ue)
{
  alkind ak;
  exkind ek;
  uvinfo oldInfo;
  fileloc loc;

  llassert (uentry_isValid (ue));
  llassert (!sRef_modInFunction ());

  ak = sRef_getOrigAliasKind (ue->sref);
  ek = sRef_getOrigExKind (ue->sref);

  oldInfo = ue->info->var;

  llassert (ctype_isUnknown (ue->utype) || ctype_isFunction (ue->utype));

  /*
  ** expanded macro is marked used (until I write a pre-processor)
  */

  ue->used |= (oldInfo->kind == VKEXPMACRO);

  ue->ukind = KFCN;
  ue->info->fcn = (ufinfo) dmalloc (sizeof (*ue->info->fcn));
  ue->info->fcn->exitCode = XK_UNKNOWN;
  ue->info->fcn->nullPred = QU_UNKNOWN;
  ue->info->fcn->specialCode = SPC_NONE;
  ue->info->fcn->access = typeIdSet_undefined;
  ue->info->fcn->hasGlobs = FALSE;
  ue->info->fcn->globs = globSet_undefined;
  ue->info->fcn->hasMods = FALSE;
  ue->info->fcn->mods = sRefSet_undefined;
  ue->info->fcn->specclauses = NULL;
  ue->info->fcn->defparams = uentryList_undefined;

  if (ctype_isFunction (ue->utype))
    {
      ue->sref = sRef_makeType (ctype_returnValue (ue->utype)); 
    }
  else
    {
      ue->sref = sRef_makeType (ctype_unknown); 
    }

  if (sRef_isRefCounted (ue->sref))
    {
      ak = AK_NEWREF;
    }
  else
    {
      if (alkind_isUnknown (ak))
	{
	  if (exkind_isKnown (ek))
	    {
	      ak = AK_IMPDEPENDENT;
	    }
	  else 
	    {
	      if (context_getFlag (FLG_RETIMPONLY))
		{
		  if (ctype_isFunction (ue->utype)
		      && ctype_isVisiblySharable 
		      (ctype_realType (ctype_returnValue (ue->utype))))
		    {
		      if (uentryList_hasReturned (uentry_getParams (ue)))
			{
			  ;
			}
		      else
			{
			  ak = AK_IMPONLY;
			  			}
		    }
		}
	    }
	}
    }

  loc = ue->whereDeclared;

  sRef_setAliasKind (ue->sref, ak, loc);
  sRef_setNullState (ue->sref, oldInfo->nullstate, loc);
  sRef_setDefState (ue->sref, oldInfo->defstate, loc);
  sRef_setExKind (ue->sref, ek, loc);

  if (oldInfo->kind == VKEXPMACRO)
    {
      fileloc_free (loc);
      ue->whereDeclared = fileloc_undefined;
    }
  else
    {
      fileloc_free (ue->whereDefined);
      ue->whereDefined = fileloc_undefined;
    }

  uvinfo_free (oldInfo);
}

void
uentry_setGlobals (uentry ue, /*@owned@*/ globSet globs)
{
  llassert (uentry_isValid (ue));

  if (uentry_isIter (ue))
    {
      llassert (globSet_isUndefined (ue->info->iter->globs));
      ue->info->iter->globs = globs;
    }
  else
    {
      if (uentry_isVariable (ue) && ctype_isFunction (uentry_getType (ue)))
	{
	  uentry_makeVarFunction (ue);
	}
      
      llassert (uentry_isFunction (ue));
      llassert (!ue->info->fcn->hasGlobs 
		&& globSet_isUndefined (ue->info->fcn->globs));
      
      ue->info->fcn->hasGlobs = TRUE;
      /*@-mustfree@*/ ue->info->fcn->globs = globs;
      /*@=mustfree@*/
    }

  if (globSet_hasStatic (globs))
    {
      context_recordFileGlobals (globs);
    }

  if (context_getFlag (FLG_GLOBALSIMPMODIFIESNOTHING))
    {
      ue->info->fcn->hasMods = TRUE;
    }
}

void uentry_addAccessType (uentry ue, typeId tid)
{
  if (uentry_isFunction (ue))
    {
      ue->info->fcn->access = typeIdSet_insert (ue->info->fcn->access, tid);
    }
  else if (uentry_isEitherConstant (ue))
    {
      ue->info->uconst->access = typeIdSet_insert (ue->info->uconst->access, tid);
    }
  else if (uentry_isIter (ue))
    {
      ue->info->iter->access = typeIdSet_insert (ue->info->iter->access, tid);
    }
  else if (uentry_isEndIter (ue))
    {
      ue->info->enditer->access = typeIdSet_insert (ue->info->enditer->access, tid);
    }
  else
    {
      llbug (message ("no access for: %q", uentry_unparse (ue)));
    }
}

/*@only@*/ /*@notnull@*/ uentry 
  uentry_makeFunction (cstring n, ctype t, 
		       typeId access, 
		       /*@only@*/ globSet globs, /*@only@*/ sRefSet mods, 
		       fileloc f)
{
  return (uentry_makeFunctionAux (n, t, 
				  ((typeId_isInvalid (access)) ? typeIdSet_emptySet () 
				   : typeIdSet_single (access)),
				  globs, mods, f,
				  FALSE, FALSE));
}

# ifndef NOLCL
/*@notnull@*/ uentry 
  uentry_makePrivFunction2 (cstring n, ctype t, 
			    typeIdSet access, 
			    globSet globs, sRefSet mods, 
			    fileloc f)
{
  return (uentry_makeFunctionAux (n, t, access, globs, mods, f, TRUE, FALSE));
}


/*@notnull@*/ uentry 
  uentry_makeSpecFunction (cstring n, ctype t, 
			   typeIdSet access,
			   /*@only@*/ globSet globs, 
			   /*@only@*/ sRefSet mods, 
			   fileloc f)
{
  uentry ue = uentry_makeFunctionAux (n, t, access, 
				      globs, mods, f,
				      FALSE, FALSE);

  uentry_setHasGlobs (ue);
  uentry_setHasMods (ue);

  reflectImplicitFunctionQualifiers (ue, TRUE);
  return (ue);
}
# endif

uentry uentry_makeExpandedMacro (cstring s, fileloc f)
{
  uentry ue = uentry_makeVariableAux (s, ctype_unknown, fileloc_undefined, 
				      sRef_undefined, FALSE, VKEXPMACRO);

  uentry_setDefined (ue, f);
  return ue;
}

/*@notnull@*/ /*@notnull@*/ uentry 
  uentry_makeForwardFunction (cstring n, typeId access, fileloc f)
{
  uentry ue = uentry_makeFunctionAux (n, ctype_unknown, 
				      typeIdSet_singleOpt (access),
				      globSet_undefined, sRefSet_undefined, 
				      fileloc_undefined,
				      FALSE, TRUE);

  ue->whereDeclared = fileloc_update (ue->whereDeclared, f);
  return ue;
}

bool uentry_isForward (uentry e)
{
  if (uentry_isValid (e))
    {
      ctype ct = uentry_getType (e);

      return (ctype_isUnknown (ct)
	      || (ctype_isFunction (ct)
		  && ctype_isUnknown (ctype_returnValue (ct))));
    }

  return FALSE;
}

# ifndef NOLCL
/*@notnull@*/ uentry 
uentry_makeTypeListFunction (cstring n, typeIdSet access, fileloc f)
{
  return (uentry_makeFunctionAux (n, ctype_unknown, access,
				  globSet_new (),
				  sRefSet_new (), f,
				  FALSE, TRUE));
}

/*@notnull@*/ uentry 
uentry_makeUnspecFunction (cstring n, ctype t, 
			   typeIdSet access, 
			   fileloc f)
{
  uentry ue = uentry_makeFunctionAux (n, t, access, globSet_new (),
				      sRefSet_new (), f, FALSE, TRUE);

  reflectImplicitFunctionQualifiers (ue, TRUE);
  return ue;
}
# endif

/*
** datatypes
*/

/* is exported for use by usymtab_interface */

/*@notnull@*/ uentry 
  uentry_makeDatatypeAux (cstring n, ctype t, ynm mut, ynm abs, 
			  fileloc f, bool priv)
{
  uentry e = uentry_alloc ();

  /* e->shallowCopy = FALSE; */
  e->ukind = KDATATYPE;
  e->uname = cstring_copy (n);
  e->utype = t;
  e->storageclass = SCNONE;
  e->sref  = sRef_makeUnknown ();

  if (ctype_isUA (t))
    {
      sRef_setStateFromType (e->sref, t);
    }

  uentry_setSpecDef (e, f);

  e->uses = filelocList_new ();
  e->isPrivate = priv;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->datatype = (udinfo) dmalloc (sizeof (*e->info->datatype));
  e->info->datatype->abs = abs;
  e->info->datatype->mut = mut;
  e->info->datatype->type = ctype_undefined;

  if (uentry_isDeclared (e))
    {
      uentry_setDefined (e, f);
    }

  if (ynm_isOn (abs) && !(uentry_isCodeDefined (e)))
    {
      sRef_setNullState (e->sref, NS_ABSNULL, uentry_whereDeclared (e));
    }

  return (e);
}

/*@notnull@*/ uentry
  uentry_makeDatatype (cstring n, ctype t, ynm mut, ynm abs, 
		       fileloc f)
{
  return (uentry_makeDatatypeAux (n, t, mut, abs, f, FALSE));
}

/*@notnull@*/ uentry uentry_makeBoolDatatype (ynm abs)
{
  uentry ret = uentry_makeDatatypeAux (context_getBoolName (),
				       ctype_bool, NO, abs, 
				       fileloc_getBuiltin (),
				       FALSE);
  
  ret->info->datatype->type = ctype_bool;
  return ret;
}

/*
** iters
*/

static /*@only@*/ /*@notnull@*/ uentry 
  uentry_makeIterAux (cstring n, typeIdSet access, ctype ct, 
		      /*@only@*/ fileloc f)
{
  uentry e = uentry_alloc ();

  e->ukind = KITER;
  e->uname = cstring_copy (n);
  e->utype = ct;
  e->sref  = sRef_makeUnknown ();
  e->storageclass = SCNONE;
  e->used = FALSE;
  e->lset = FALSE;

  uentry_setSpecDef (e, f);

  e->uses = filelocList_new ();
  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->iter = (uiinfo) dmalloc (sizeof (*e->info->iter));
  e->info->iter->access = access;
  e->info->iter->mods = sRefSet_undefined;
  e->info->iter->globs = globSet_undefined;

  uentry_checkIterArgs (e);
  return (e);
}

/*@notnull@*/ uentry uentry_makeIter (cstring n, ctype ct, fileloc f)
{
  return (uentry_makeIterAux (n, context_fileAccessTypes (), ct, f));
}

static /*@notnull@*/ uentry
uentry_makeEndIterAux (cstring n, typeIdSet access, /*@only@*/ fileloc f)
{
  uentry e = uentry_alloc ();

  /* e->shallowCopy = FALSE; */
  e->ukind = KENDITER;
  e->storageclass = SCNONE;
  e->uname = message ("end_%s", n);
  e->utype = ctype_unknown;
  e->sref  = sRef_makeUnknown ();

  uentry_setSpecDef (e, f);

  e->used = FALSE;
  e->lset = FALSE;

  e->uses = filelocList_new ();
  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->enditer = (ueinfo) dmalloc (sizeof (*e->info->enditer));

  e->info->enditer->access = access;

  return (e);
}

/*@notnull@*/ /*@only@*/ uentry uentry_makeEndIter (cstring n, fileloc f)
{
  return (uentry_makeEndIterAux (n, context_fileAccessTypes (), f));
}

/*
** tags
*/

static /*@only@*/ /*@notnull@*/ uentry 
  uentry_makeTagAux (cstring n, ctype t, 
		     /*@only@*/ fileloc fl, 
		     bool priv, ekind kind)
{
  uentry e = uentry_alloc ();
  
  if (kind != KSTRUCTTAG && kind != KUNIONTAG && kind != KENUMTAG)
    {
      llbuglit ("uentry_makeTagAux: not a tag type");
    }
  
  e->ukind = kind;
  /* e->shallowCopy = FALSE; */
  e->uname = cstring_copy (n);

  e->utype = t;
  e->sref  = sRef_makeUnknown ();
  e->storageclass = SCNONE;

  uentry_setSpecDef (e, fl);
  
  e->used = FALSE;
  e->lset = FALSE;

  e->uses = filelocList_new ();
  e->isPrivate = priv;
  e->hasNameError = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->datatype = (udinfo) dmalloc (sizeof (*e->info->datatype));
  e->info->datatype->abs = NO;
  e->info->datatype->mut = (kind == KENUMTAG) ? NO : MAYBE;
  e->info->datatype->type = t;

  if (uentry_isDeclared (e))
    {
      uentry_setDefined (e, fl);
    }

  return (e);  
}

uentry uentry_makeStructTagLoc (cstring n, ctype t)
{
  cstring sname = makeStruct (n);
  uentry ret = uentry_makeTagAux (sname, t, setLocation (), FALSE, KSTRUCTTAG);

  cstring_free (sname);
  return (ret);
}

/*@only@*/ uentry
uentry_makeStructTag (cstring n, ctype t, fileloc loc)
{
  cstring sname = makeStruct (n);
  uentry ret = uentry_makeTagAux (sname, t, loc, FALSE, KSTRUCTTAG);

  cstring_free (sname);
  return ret;
}

/*@only@*/ uentry
uentry_makeUnionTag (cstring n, ctype t, fileloc loc)
{
  cstring uname = makeUnion (n);
  uentry ret = uentry_makeTagAux (uname, t, loc, FALSE, KUNIONTAG);

  cstring_free (uname);
  return (ret);
}

# ifndef NOLCL
uentry
uentry_makeEnumTag (cstring n, ctype t, fileloc loc)
{
  cstring ename = makeEnum (n);
  uentry ret = uentry_makeTagAux (ename, t, loc, FALSE, KENUMTAG);

  cstring_free (ename);
  return ret;
}
# endif

uentry
uentry_makeUnionTagLoc (cstring n, ctype t)
{
  cstring uname = makeUnion (n);
  uentry ret = uentry_makeTagAux (uname, t, setLocation (), FALSE, KUNIONTAG);

  cstring_free (uname);
  return ret;
}

uentry
uentry_makeEnumTagLoc (cstring n, ctype t)
{
  cstring ename = makeEnum (n);
  uentry ret = uentry_makeTagAux (ename, t, setLocation (), FALSE, KENUMTAG);

  cstring_free (ename);
  return ret;
}

bool 
uentry_isStructTag (uentry ue) 
{
  return (uentry_isValid (ue) && ue->ukind == KSTRUCTTAG);
}

bool 
uentry_isUnionTag (uentry ue) 
{
  return (uentry_isValid (ue) && ue->ukind == KUNIONTAG);
}

bool 
uentry_isEnumTag (uentry ue) 
{
  return (uentry_isValid (ue) && ue->ukind == KENUMTAG);
}

bool
uentry_isAnyTag (uentry ue)
{
  return (uentry_isStructTag (ue) 
	  || uentry_isUnionTag (ue)
	  || uentry_isEnumTag (ue));
}

static /*@unchecked@*/ /*@only@*/ uentry emarker = NULL;

extern void uentry_destroyMod (void)
   /*@globals killed emarker@*/ /*@modifies emarker@*/
{
  static bool wasDestroyed = FALSE;

  llassert (!wasDestroyed);

  if (emarker != NULL)
    {
      uentry_reallyFree (emarker);
    }

  wasDestroyed = TRUE;
}

uentry
uentry_makeElipsisMarker (void)
{
  if (emarker == NULL)
    {
      emarker = uentry_alloc ();

      emarker->ukind = KELIPSMARKER;
      emarker->uname = cstring_makeLiteral ("...");
      emarker->utype = ctype_elipsMarker;
      emarker->sref  = sRef_undefined;
      emarker->storageclass = SCNONE;
      emarker->used = FALSE;
      emarker->lset = FALSE;
      emarker->info = NULL;

      uentry_setSpecDef (emarker, fileloc_undefined);
      emarker->uses = filelocList_new ();
      emarker->isPrivate = FALSE;
      emarker->hasNameError = FALSE;
    }

  /*@ignore@*/ return (emarker); /*@end@*/
} 

/*
** comparisons
*/

bool
uentry_equiv (uentry p1, uentry p2)
{
  if (uentry_compare (p1, p2) != 0)
    {
      return FALSE;
    }
  else
    {
      return TRUE;
    }
}

int
uentry_xcomparealpha (uentry *p1, uentry *p2)
{
  int res;

  if ((res = uentry_compare (*p1, *p2)) == 0) {
    if ((*p1 != NULL) && (*p2 != NULL)) {
      res = cstring_compare ((*p1)->uname,
			     (*p2)->uname);
    }
  }

  return res;
}

int
uentry_xcompareuses (uentry *p1, uentry *p2)
{
  uentry u1 = *p1;
  uentry u2 = *p2;

  if (uentry_isValid (u1))
    {
      if (uentry_isValid (u2))
	{
	  return (-1 * int_compare (filelocList_size (u1->uses), 
				    filelocList_size (u2->uses)));
	}
      else
	{
	  return 1;
	}
    }
  else
    {
      if (uentry_isValid (u2))
	{
	  return -1;
	}
      else
	{
	  return 0;
	}
    }
}

int 
uentry_compareStrict (uentry v1, uentry v2)
{
  COMPARERETURN (uentry_compare (v1, v2));

  if (v1 != v2 && uentry_isValid (v1) && uentry_isValid (v2))
    {
      COMPARERETURN (fileloc_compare (v1->whereDeclared, v2->whereDeclared));
      COMPARERETURN (fileloc_compare (v1->whereDefined, v2->whereDefined));
      COMPARERETURN (fileloc_compare (v1->whereSpecified, v2->whereSpecified));
    }

  return 0;
}

int
uentry_compare (uentry u1, uentry u2)
{
  if (u1 == u2) return 0;
  
  if (uentry_isInvalid (u1)) return -1;
  if (uentry_isInvalid (u2)) return 1;

  INTCOMPARERETURN (u1->ukind, u2->ukind);
  COMPARERETURN (ctype_compare (u1->utype, u2->utype));
  COMPARERETURN (bool_compare (uentry_isPriv (u1), uentry_isPriv (u2)));

  COMPARERETURN (sRef_compare (u1->sref, u2->sref));

  switch (u1->ukind)
    {
    case KINVALID:
    case KELIPSMARKER:
      /* bug detected by lclint:  
      ** uentry.c:753,14: Return value type bool does not match declared type int: TRUE 
      */
      return 0;
    case KENUMCONST:
    case KCONST:
      return (multiVal_compare (u1->info->uconst->val,
				u2->info->uconst->val));
    case KSTRUCTTAG: 
    case KUNIONTAG: 
    case KENUMTAG: 
      return (ctype_compare (u1->info->datatype->type, u2->info->datatype->type));
    case KITER:
      COMPARERETURN (typeIdSet_compare (uentry_accessType (u1), 
					 uentry_accessType (u2)));
      return (uentryList_compareParams (uentry_getParams (u1), 
					uentry_getParams (u2)));
    case KENDITER:
      return (typeIdSet_compare (uentry_accessType (u1), 
				  uentry_accessType (u2)));
    case KFCN:
      COMPARERETURN (typeIdSet_compare (uentry_accessType (u1), 
					 uentry_accessType (u2)));
      COMPARERETURN (globSet_compare (uentry_getGlobs (u1), 
				      uentry_getGlobs (u2)));
      COMPARERETURN (uentryList_compareParams (uentry_getParams (u1), 
					       uentry_getParams (u2)));
      COMPARERETURN (generic_compare (u1->info->fcn->specialCode,
				       u2->info->fcn->specialCode));
      COMPARERETURN (generic_compare (u1->info->fcn->nullPred,
				       u2->info->fcn->nullPred));

      return (sRefSet_compare (uentry_getMods (u1), uentry_getMods (u2)));
    case KVAR:
      COMPARERETURN (generic_compare (u1->info->var->kind, u2->info->var->kind));
      COMPARERETURN (generic_compare (sRef_getOrigAliasKind (u1->sref),
				      sRef_getOrigAliasKind (u2->sref)));
      COMPARERETURN (generic_compare (sRef_getOrigExKind (u1->sref),
				      sRef_getOrigExKind (u2->sref)));
      COMPARERETURN (generic_compare (u1->info->var->checked,
				      u2->info->var->checked));
      COMPARERETURN (generic_compare (u1->info->var->defstate, 
				      u2->info->var->defstate));
      return        (generic_compare (u1->info->var->nullstate, 
				      u2->info->var->nullstate));
    case KDATATYPE:
      COMPARERETURN (ctype_compare (u1->info->datatype->type,
				    u2->info->datatype->type));
      COMPARERETURN (ynm_compare (u1->info->datatype->mut,
				  u2->info->datatype->mut));
      return (ynm_compare (u1->info->datatype->abs, u2->info->datatype->abs));
    }
  
  BADEXIT;
}

/*
** library format:
**
** all entries are: <type>[@<info>]*#<name>
**
** info depends on kind:
*/

static void
advanceField (char **s)
{
  checkChar (s, '@');
}

static void
advanceName (char **s)
{
  checkChar (s, '#');
}

static vkind
vkind_fromInt (int i)
{
  if /*@+enumint@*/ (i < VKFIRST || i > VKLAST) /*@=enumint@*/
    {
      llbuglit ("vkind_fromInt: out of range");
    }

  return (vkind)i;
}

static uentry  
  uentry_makeConstantBase (/*@only@*/ cstring name, ctype ct, 
			   typeIdSet access, nstate nullstate,
			   /*@keep@*/ fileloc loc, /*@only@*/ multiVal m)
{
  uentry e = uentry_alloc ();
  
  e->ukind = KCONST;
  e->uname = name;
  e->utype = ct;
  e->sref  = sRef_makeConst (ct);

  sRef_setNullState (e->sref, nullstate, loc);
  e->storageclass = SCNONE;

  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereSpecified = fileloc_undefined;
      e->whereDeclared = loc;
    }

  e->whereDefined = fileloc_undefined;
  e->uses = filelocList_new ();
  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->uconst = (ucinfo) dmalloc (sizeof (*e->info->uconst));
  e->info->uconst->val = m;
  e->info->uconst->access = access;

  sRef_storeState (e->sref);

  return (e);
}

static /*@only@*/ uentry  
  uentry_makeVariableBase (/*@only@*/ cstring name, ctype ct, vkind kind, 
			   sstate defstate, nstate isnull, alkind aliased,
			   exkind exp, chkind checked, 
			   /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();

  e->ukind = KVAR;
  e->uname = name;
  e->utype = ct;
  e->storageclass = SCNONE;

  e->sref  = sRef_makeType (ct);
  sRef_setNullState (e->sref, isnull, loc);

  e->whereDefined = fileloc_undefined;

  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereSpecified = fileloc_undefined;
      e->whereDeclared = loc;
    }

  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;

  e->uses = filelocList_new ();

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->var = (uvinfo) dmalloc (sizeof (*e->info->var));
  e->info->var->kind = kind;
  e->info->var->checked = checked;
  e->info->var->defstate = defstate;

  sRef_setDefState (e->sref, defstate, loc);

  e->info->var->nullstate = sRef_getNullState (e->sref);

  sRef_setExKind (e->sref, exp, loc);
  sRef_setAliasKind (e->sref, aliased, loc);

  sRef_storeState (e->sref);
  return (e);
}

static /*@only@*/ uentry  
uentry_makeDatatypeBase (/*@only@*/ cstring name, ctype ct, ynm abs, 
			 ynm mut, ctype rtype, alkind ak, exkind exp, 
			 sstate defstate, nstate isnull,
			 /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();

  e->ukind = KDATATYPE;
  /* e->shallowCopy = FALSE; */
  e->uname = name;
  e->utype = ct;
  e->storageclass = SCNONE;
  e->sref  = sRef_makeUnknown ();

  /*
  ** This is only setting null state.  (I think?)
  */

  if (ctype_isUA (ct))
    {
      uentry te = usymtab_getTypeEntrySafe (ctype_typeId (ct));

      if (uentry_isValid (te))
	{
	  sRef_setStateFromUentry (e->sref, te);
	}
      else
	{
	  /* problem for recursive type definitions */
	}
    }
  
  sRef_setAliasKind (e->sref, ak, loc);
  sRef_setExKind (e->sref, exp, loc);

  sRef_setDefState (e->sref, defstate, loc);

  if (ynm_isOn (abs) && ctype_isUnknown (ct) && isnull == NS_UNKNOWN)
    {
      isnull = NS_ABSNULL;
    }

  sRef_mergeNullState (e->sref, isnull);

  e->whereDefined = fileloc_copy (loc); /*< bogus!  (but necessary for lexer) >*/

  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereSpecified = fileloc_undefined;
      e->whereDeclared = loc;
    }
  
  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->datatype = (udinfo) dmalloc (sizeof (*e->info->datatype));
  e->info->datatype->abs = abs;
  e->info->datatype->mut = mut;
  e->info->datatype->type = rtype;
  
  sRef_storeState (e->sref);

  return (e);
}

# ifndef NOLCL
static void uentry_setHasGlobs (uentry ue)
{
  llassert (uentry_isFunction (ue));

  ue->info->fcn->hasGlobs = TRUE;
}

static void uentry_setHasMods (uentry ue)
{
  llassert (uentry_isFunction (ue));

  ue->info->fcn->hasMods = TRUE;
}
# endif

bool uentry_hasGlobs (uentry ue)
{
  if (uentry_isFunction (ue))
    {
      return (ue->info->fcn->hasGlobs);
    }

  return FALSE;
}

bool uentry_hasSpecialClauses (uentry ue)
{
  return (uentry_isFunction (ue) && (ue->info->fcn->specclauses != NULL));
}

specialClauses uentry_getSpecialClauses (uentry ue)
{
  llassert (uentry_isFunction (ue));
  return ue->info->fcn->specclauses;
}

bool uentry_hasMods (uentry ue)
{
  if (uentry_isFunction (ue))
    {
      return (ue->info->fcn->hasMods);
    }

  return FALSE;
}

static uentry  
  uentry_makeFunctionBase (/*@only@*/ cstring name, ctype ct, 
			   typeIdSet access, 
			   bool hasGlobs, /*@only@*/ globSet globs, 
			   bool hasMods, /*@only@*/ sRefSet mods, 
			   alkind ak, exkind exp, 
			   sstate defstate, nstate isnull,
			   exitkind exitCode,
			   specCode sCode,
			   qual nullPred,
			   /*@only@*/ specialClauses specclauses,
			   /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();
  ctype ret;

  /* e->shallowCopy = FALSE; */
  e->ukind = KFCN;
  e->uname = name;
  e->utype = ct;
  e->storageclass = SCNONE;

  if (ctype_isFunction (ct))
    {
      ret = ctype_returnValue (ct);
    }
  else
    {
      if (ctype_isKnown (ct))
	{
	  llbug (message ("not function: %s", ctype_unparse (ct)));
	}

      ret = ctype_unknown;
    }

  e->sref  = sRef_makeType (ret);

  if (ctype_isUA (ret))
    {
      sRef_setStateFromType (e->sref, ret);
    }

  sRef_setDefined (e->sref, loc);
  sRef_setNullState (e->sref, isnull, loc);

  sRef_setAliasKind (e->sref, ak, loc);
  sRef_setExKind (e->sref, exp, loc);
  sRef_setDefState (e->sref, defstate, loc);

  e->whereSpecified = loc;
  e->whereDefined = fileloc_undefined;

  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();  

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->fcn = (ufinfo) dmalloc (sizeof (*e->info->fcn));
  
  e->info->fcn->exitCode = exitCode;
  e->info->fcn->specialCode = sCode;
  e->info->fcn->nullPred = nullPred;
  e->info->fcn->access = access;

  e->info->fcn->specclauses = specclauses;
  e->info->fcn->hasGlobs = hasGlobs;
  e->info->fcn->globs = globs;

  e->info->fcn->hasMods = hasMods;
  e->info->fcn->mods = mods;

  e->info->fcn->defparams = uentryList_undefined; 
  e->whereDeclared = fileloc_undefined;

  sRef_storeState (e->sref);

  return (e);
}

static /*@only@*/ uentry  
  uentry_makeTagBase (/*@only@*/ cstring name, ekind tagkind, 
		      ctype ct, ctype rtype, /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();
  
  if (tagkind != KSTRUCTTAG && tagkind != KUNIONTAG && tagkind != KENUMTAG)
    {
      llbuglit ("uentry_makeTagBase: not a tag type");
    }

  /* e->shallowCopy = FALSE; */
  e->ukind = tagkind;
  e->uname = name;
  e->utype = ct;
  e->sref  = sRef_makeUnknown ();
  e->storageclass = SCNONE;

  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereDeclared = loc;
      e->whereSpecified = fileloc_undefined;
    }

  e->whereDefined = fileloc_undefined;

  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->datatype = (udinfo) dmalloc (sizeof (*e->info->datatype));
  e->info->datatype->abs  = NO;
  e->info->datatype->mut  = MAYBE;
  e->info->datatype->type = rtype;

  sRef_storeState (e->sref);

  return (e);  
}

static uentry  
  uentry_makeIterBase (/*@only@*/ cstring name, typeIdSet access, 
		       ctype ct, /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();
  
  /* e->shallowCopy = FALSE; */
  e->ukind = KITER;
  e->uname = name;
  e->utype = ct;
  e->sref  = sRef_makeUnknown ();
  e->storageclass = SCNONE;

  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereDeclared = loc;
      e->whereSpecified = fileloc_undefined;
    }

  e->whereDefined = fileloc_undefined;
  
  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->iter = (uiinfo) dmalloc (sizeof (*e->info->iter));
  e->info->iter->access = access;
  e->info->iter->mods = sRefSet_undefined;
  e->info->iter->globs = globSet_undefined;
  
  sRef_storeState (e->sref);
  return (e);
}

static uentry  
  uentry_makeEndIterBase (/*@only@*/ cstring name, typeIdSet access, 
			  /*@only@*/ fileloc loc)
{
  uentry e = uentry_alloc ();

  /* e->shallowCopy = FALSE; */
  e->ukind = KENDITER;
  e->storageclass = SCNONE;
  e->uname = name;
  e->utype = ctype_unknown;
  e->sref  = sRef_makeUnknown ();
  
  if (fileloc_isSpec (loc))
    {
      e->whereSpecified = loc;
      e->whereDeclared = fileloc_undefined;
    }
  else
    {
      e->whereDeclared = loc;
      e->whereSpecified = fileloc_undefined;
    }

  e->whereDefined = fileloc_undefined;

  e->isPrivate = FALSE;
  e->hasNameError = FALSE;

  e->used = FALSE;
  e->lset = FALSE;
  e->uses = filelocList_new ();

  e->info = (uinfo) dmalloc (sizeof (*e->info));
  e->info->enditer = (ueinfo) dmalloc (sizeof (*e->info->enditer));
  e->info->enditer->access = access;
  sRef_storeState (e->sref);

  return (e);
}

void uentry_markFree (/*@unused@*/ /*@owned@*/ uentry u)
{
  /* should save u */
/*@-mustfree@*/
}
/*@=mustfree@*/

/*@only@*/ uentry
uentry_undump (ekind kind, fileloc loc, char **s)
{
  uentry ue;
  
  if (**s == '!')
    {
      checkChar (s, '!');
      checkChar (s, '.');
      ue = uentry_makeElipsisMarker ();
    }
  else
    {
      ctype ct = ctype_undump (s);
      cstring name;

      switch (kind)
	{
	case KVAR:
	  {
	    vkind  tkind;
	    sstate defstate;
	    nstate isnull;
	    alkind aliased;
	    exkind exp;
	    chkind checked;
	    
	    checkChar (s, '|');

	    if (optCheckChar (s, '@'))
	      {
		tkind = vkind_fromInt (getInt (s));
		checkChar (s, '|');
	      }
	    else
	      {
		tkind = VKPARAM;
	      }

	    if (optCheckChar (s, '$'))
	      {
		defstate = SS_UNKNOWN;
		isnull = NS_UNKNOWN;
		aliased = AK_IMPTEMP;
		exp = XO_UNKNOWN;
		checked = CH_UNKNOWN;
	      }		
	    else if (optCheckChar (s, '&'))
	      {
		defstate = SS_DEFINED;
		isnull = NS_UNKNOWN;
		aliased = AK_IMPTEMP;
		exp = XO_UNKNOWN;
		checked = CH_UNKNOWN;
	      }		
	    else if (optCheckChar (s, '^'))
	      {
		defstate = SS_UNKNOWN;
		isnull = NS_UNKNOWN;
		aliased = AK_IMPTEMP;
		exp = XO_UNKNOWN;
		checked = CH_UNKNOWN;
	      }		
	    else
	      {
		defstate = sstate_fromInt (getInt (s));      
		advanceField (s); isnull = nstate_fromInt (getInt (s));      
		advanceField (s); aliased = alkind_fromInt (getInt (s));      

		if (optCheckChar (s, '&'))
		  {
		    exp = XO_UNKNOWN;
		    checked = CH_UNKNOWN;
		  }
		else
		  {
		    advanceField (s); exp = exkind_fromInt (getInt (s));      
		    advanceField (s); checked = (chkind) (getInt (s));      
		  }
	      }

	    advanceName (s);
	    name = getStringWord (s);
	    
	    ue = uentry_makeVariableBase (name, ct, tkind, defstate, 
					  isnull, aliased, exp, 
					  checked, fileloc_copy (loc));
	  }
	  break;
	case KDATATYPE: 
	  {
	    ynm abs;
	    ynm mut;
	    ctype rtype;
	    sstate defstate;
	    nstate isnull;
	    alkind aliased;
	    exkind exp;

	    advanceField (s); abs = ynm_fromCodeChar (loadChar (s));
	    advanceField (s); mut = ynm_fromCodeChar (loadChar (s));
	    advanceField (s); defstate = sstate_fromInt (getInt (s));      
	    advanceField (s); isnull = nstate_fromInt (getInt (s));      
	    advanceField (s); aliased = alkind_fromInt (getInt (s));      
	    advanceField (s); exp = exkind_fromInt (getInt (s));      
	    advanceField (s); rtype = ctype_undump (s);
	    advanceName (s); 
	    name = getStringWord (s);

	    ue = uentry_makeDatatypeBase (name, ct, abs, mut, rtype, 
					  aliased, exp, defstate, isnull, 
					  fileloc_copy (loc));
	  }
	  break;
	case KFCN:
	  {
	    alkind     ak;
	    exkind     exp;
	    sstate     defstate;
	    nstate     isnull;
	    exitkind   exitCode;
	    specCode   specc;
	    qual       nullPred;
	    typeIdSet access;
	    bool       hasGlobs;
	    globSet    globs;
	    bool       hasMods;
	    sRefSet    mods;
	    specialClauses specclauses;

	    if (optCheckChar (s, '$'))
	      {
		defstate = SS_DEFINED;
		isnull = NS_UNKNOWN;
		exitCode = XK_UNKNOWN;
		specc = SPC_NONE;
		nullPred = QU_UNKNOWN;
	      }
	    else
	      {
		advanceField (s); defstate = sstate_fromInt (getInt (s)); 
		advanceField (s); isnull = nstate_fromInt (getInt (s)); 
		advanceField (s); exitCode = exitkind_fromInt (getInt (s)); 
		advanceField (s); specc = specCode_fromInt (getInt (s)); 
		advanceField (s); nullPred = qual_fromInt (getInt (s)); 
	      }

	    if (optCheckChar (s, '$'))
	      {
		hasGlobs = FALSE;
		globs = globSet_undefined;
		hasMods = FALSE;
		mods = sRefSet_undefined;
	      }
	    else if (optCheckChar (s, '^'))
	      {
		hasGlobs = TRUE;
		globs = globSet_undefined;
		hasMods = TRUE;
		mods = sRefSet_undefined;
	      }
	    else
	      {
		advanceField (s); hasGlobs = bool_fromInt (getInt (s));
		advanceField (s); globs  = globSet_undump (s);
		advanceField (s); hasMods = bool_fromInt (getInt (s));
		advanceField (s); mods   = sRefSet_undump (s);	    
	      }

	    if (optCheckChar (s, '$'))
	      {
		ak = AK_UNKNOWN;
		exp = XO_UNKNOWN;
	      }
	    else
	      {
		advanceField (s); ak = alkind_fromInt (getInt (s));
		advanceField (s); exp = exkind_fromInt (getInt (s));      
	      }

	    advanceField (s); access = typeIdSet_undump (s);

	    if (optCheckChar (s, '@'))
	      {
		specclauses = specialClauses_undump (s);
	      }
	    else
	      {
		specclauses = specialClauses_undefined; 
	      }

	    advanceName (s);  name = getStringWord (s);

	    ue = uentry_makeFunctionBase (name, ct, access, 
					  hasGlobs, globs, 
					  hasMods, mods, 
					  ak, exp, defstate, isnull, 
					  exitCode, specc, nullPred,
					  specclauses,
					  fileloc_copy (loc));
	    DPRINTF (("Undump: %s", uentry_unparse (ue)));
	  }
	  break;
	case KITER:
	  {
	    typeIdSet access;
	    
	    advanceField (s); access = typeIdSet_undump (s);
	    advanceName (s);  name = getStringWord (s);
	    
	    ue = uentry_makeIterBase (name, access, ct,
				      fileloc_copy (loc));
	  }
	  break;
	case KENDITER:
	  {
	    typeIdSet access;

	    advanceField (s); access = typeIdSet_undump (s);
	    advanceName (s);  name = getStringWord (s);
	    
	    ue = uentry_makeEndIterBase (name, access, fileloc_copy (loc));
	  }
	  break;
	case KENUMCONST:  
	case KCONST:  
	  {
	    typeIdSet access;
	    multiVal val;
	    nstate nullstate;

	    if (optCheckChar (s, '$'))
	      {
		val = multiVal_undefined;
		access = typeIdSet_undefined;
		nullstate = NS_UNKNOWN;
	      }
	    else
	      {
		advanceField (s); val = multiVal_undump (s);
		advanceField (s); access = typeIdSet_undump (s);
		advanceField (s); nullstate = nstate_fromInt (getInt (s));
	      }

	    advanceName (s);  name = getStringWord (s);
	    
	    ue = uentry_makeConstantBase (name, ct, access,
					  nullstate, fileloc_copy (loc), val);
	    break;
	  }
	case KSTRUCTTAG:
	case KUNIONTAG:
	case KENUMTAG:
	  {
	    ctype rtype;
	    
	    advanceField (s); rtype = ctype_undump (s);
	    advanceName (s);  name = getStringWord (s);
	    ue = uentry_makeTagBase (name, kind, ct, rtype, fileloc_copy (loc));
	  }
	  break;
	case KINVALID:
	  llcontbuglit ("uentry_undump: invalid");
	  ue = uentry_undefined;
	  break;
	case KELIPSMARKER:
	  llcontbuglit ("uentry_undump: elips marker");
	  ue = uentry_undefined;
	  break;
	}
    }
  
  return (ue);
}

cstring
uentry_dump (uentry v)
{
  return (uentry_dumpAux (v, FALSE));
}

cstring
uentry_dumpParam (uentry v)
{
  llassertprint (uentry_isVariable (v) || uentry_isElipsisMarker (v),
		 ("dump: %s", uentry_unparseFull (v)));

  return (uentry_dumpAux (v, TRUE));
}

static cstring
uentry_dumpAux (uentry v, bool isParam)
{
  llassert (uentry_isValid (v));

  switch (v->ukind)
    {
    case KINVALID: 
      llcontbuglit ("uentry_dump: invalid entry"); 
      return cstring_undefined;
    case KELIPSMARKER: 
      return (message ("!."));
    case KVAR:     
      {
	cstring sdump;
	vkind vk  = v->info->var->kind;
	sstate dss = sRef_getDefState (v->sref);
	nstate nst = sRef_getNullState (v->sref);
	alkind alk = sRef_getAliasKind (v->sref);
	exkind exk = sRef_getExKind (v->sref);
	chkind chk = v->info->var->checked;

	if (dss == SS_UNKNOWN
	    && nst == NS_UNKNOWN
	    && alk == AK_IMPTEMP
	    && exk == XO_UNKNOWN
	    && chk == CH_UNKNOWN)
	  {
	    sdump = cstring_makeLiteral ("$");
	  }
	else if (dss == SS_DEFINED
		 && nst == NS_UNKNOWN
		 && alk == AK_IMPTEMP
		 && exk == XO_UNKNOWN
		 && chk == CH_UNKNOWN)
	  {
	    sdump = cstring_makeLiteral ("&");
	  }
	else if (dss == SS_UNKNOWN
		 && nst == NS_UNKNOWN
		 && alk == AK_UNKNOWN
		 && exk == XO_UNKNOWN
		 && chk == CH_UNKNOWN)
	  {
	    sdump = cstring_makeLiteral ("^");
	  }
	else if (exk == XO_UNKNOWN
		 && chk == CH_UNKNOWN)
	  {
	    sdump = message ("%d@%d@%d&",
			     (int) dss,
			     (int) nst,
			     (int) alk);
	  }
	else
	  {
	    sdump = message ("%d@%d@%d@%d@%d",	
		     (int) dss,
			     (int) nst,
			     (int) alk,
			     (int) exk,
			     (int) chk);
	  }
	

	if (vk != VKPARAM)
	  {
	    return (message ("%q|@%d|%q#%s", 
			     ctype_dump (v->utype), 
			     (int) vk,
			     sdump,
			     isParam ? cstring_undefined : v->uname));
	  }
	else
	  {
	    return (message ("%q|%q#%s", 
			     ctype_dump (v->utype), 
			     sdump,
			     isParam ? cstring_undefined : v->uname));
	  }

      }
    case KDATATYPE: 
      return (message ("%q@%s@%s@%d@%d@%d@%d@%q#%s", 
		       ctype_dump (v->utype),
		       ynm_unparseCode (v->info->datatype->abs),
		       ynm_unparseCode (v->info->datatype->mut),
		       (int) sRef_getDefState (v->sref),
		       (int) sRef_getNullState (v->sref),
		       (int) sRef_getAliasKind (v->sref),
		       (int) sRef_getExKind (v->sref),
		       ctype_dump (v->info->datatype->type),
		       v->uname));
    case KFCN:
      {
	cstring sdump, gdump, adump;
	alkind alk = sRef_getAliasKind (v->sref);
	exkind exk = sRef_getExKind (v->sref);

	if (sRef_getDefState (v->sref) == SS_DEFINED
	    && !nstate_isKnown (sRef_getNullState (v->sref))
	    && !exitkind_isKnown (v->info->fcn->exitCode)
	    && v->info->fcn->specialCode == SPC_NONE
	    && v->info->fcn->nullPred == QU_UNKNOWN)
	  {
	    sdump = cstring_makeLiteral ("$");
	  }
	else
	  {
	    sdump = message ("@%d@%d@%d@%d@%d",
			     (int) sRef_getDefState (v->sref),
			     (int) sRef_getNullState (v->sref),
			     (int) v->info->fcn->exitCode,
			     (int) v->info->fcn->specialCode,
			     (int) v->info->fcn->nullPred);
	  }

	if (!uentry_hasGlobs(v) && !uentry_hasMods (v))
	  {
	    gdump = cstring_makeLiteral ("$");
	  }
	else if (uentry_hasGlobs (v) && globSet_isEmpty (uentry_getGlobs (v))
		 && uentry_hasMods (v) && sRefSet_isEmpty (uentry_getMods (v)))
	  {
	    gdump = cstring_makeLiteral ("^");
	  }
	else
	  {
	    gdump = message ("@%s@%q@%s@%q",
			     bool_dump (uentry_hasGlobs (v)),
			     globSet_dump (uentry_getGlobs (v)),
			     bool_dump (uentry_hasMods (v)),
			     sRefSet_dump (uentry_getMods (v)));
	  }

	if (alk == AK_UNKNOWN && exk == XO_UNKNOWN)
	  {
	    adump = cstring_makeLiteral ("$");
	  }
	else
	  {
	    adump = message ("@%d@%d", (int) alk, (int) exk);
	  }

	if (uentry_hasSpecialClauses (v))
	  {
	    return (message ("%q%q%q%q@%q@%q#%s",
			     ctype_dump (v->utype),
			     sdump,
			     gdump,
			     adump,
			     typeIdSet_dump (uentry_accessType (v)),
			     specialClauses_dump (v->info->fcn->specclauses),
			     v->uname));
	  }
	else
	  {
	    return (message ("%q%q%q%q@%q#%s",
			     ctype_dump (v->utype),
			     sdump,
			     gdump,
			     adump,
			     typeIdSet_dump (uentry_accessType (v)),
			     v->uname));
	  }
      }
    case KITER:
      return (message ("%q@%q#%s",
		       ctype_dump (v->utype),
		       typeIdSet_dump (v->info->iter->access),
		       v->uname));
    case KENDITER:
      return (message ("%q@%q#%s",
		       ctype_dump (v->utype),
		       typeIdSet_dump (uentry_accessType (v)),
		       v->uname));
    case KENUMCONST:  
    case KCONST:  
      {
	cstring sdump;

	if (multiVal_isUnknown (v->info->uconst->val)
	    && typeIdSet_isEmpty (uentry_accessType (v))
	    && (sRef_getNullState (v->sref) == NS_UNKNOWN))
	  {
	    sdump = cstring_makeLiteral ("$");
	  }
	else
	  {
	    sdump = message ("@%q@%q@%d",
			     multiVal_dump (v->info->uconst->val),
			     typeIdSet_dump (uentry_accessType (v)),
			     (int) sRef_getNullState (v->sref));
	  }

	return (message ("%q%q#%s", 
			 ctype_dump (v->utype), 
			 sdump,
			 v->uname));
      }
    case KSTRUCTTAG:
    case KUNIONTAG:
    case KENUMTAG:
      return (message ("%q@%q#%s", 
		       ctype_dump (v->utype), 
		       ctype_dump (v->info->datatype->type), v->uname));
    }

  BADEXIT;
}

/*@only@*/ cstring
uentry_unparseAbbrev (uentry v)
{
  if (!uentry_isVariable (v))
    {
      llcontbuglit ("uentry_unparseAbbrev: not variable");
      return uentry_unparse (v);
    }

  return (message ("%s %q", ctype_unparseDeep (v->utype), uentry_getName (v)));
}

/*@only@*/ cstring
uentry_unparse (uentry v)
{
  cstring st;

    if (uentry_isUndefined (v)) return (cstring_makeLiteral ("<undefined>"));
  if (uentry_isElipsisMarker (v)) return (cstring_makeLiteral ("..."));

  st = uentry_getName (v);

  if (cstring_isDefined (st))
    {
      return (ctype_unparseDeclaration (v->utype, st)); 
    }
  else
    {
      cstring_free (st);
      return (cstring_copy (ctype_unparse (v->utype)));
    }
}

/*@only@*/ cstring
uentry_unparseFull (uentry v)
{
  if (uentry_isUndefined (v))
    {
      return (cstring_makeLiteral ("<undefined>"));
    }
  else if (uentry_isDatatype (v))
    {
      return (message ("[%d] [%s] %s %q : %t [%t] %s %s // %q [s: %q; d: %q]",
		       (int) v,
		       ekind_unparse (v->ukind),
		       v->uname,
		       uentry_getName (v),
		       v->utype,
		       ctype_isDefined (v->info->datatype->type) 
		          ? v->info->datatype->type : ctype_unknown,
		       ynm_unparse (v->info->datatype->mut),
		       ynm_unparse (v->info->datatype->abs),
		       sRef_unparseState (v->sref),
		       fileloc_unparse (v->whereSpecified),
		       fileloc_unparse (v->whereDefined)));
    }
  else if (uentry_isFunction (v))
    {
      return (message ("[%w] = [%s] %q : %t / sref: %q / mods: %q / "
		       "globs: %q / [s: %q; decl: %q; def: %q]",
		       (long unsigned) v,
		       ekind_unparse (v->ukind),
		       uentry_getName (v),
		       v->utype,
		       sRef_unparseFull (v->sref),
		       sRefSet_unparse (v->info->fcn->mods),
		       globSet_unparse  (v->info->fcn->globs),
		       fileloc_unparse (v->whereSpecified),
		       fileloc_unparse (v->whereDeclared),
		       fileloc_unparse (v->whereDefined)));
    }
  else if (uentry_isIter (v))
    {
      return (message ("[%s] %q: %t / %q [s: %q; d: %q]",
		       ekind_unparse (v->ukind),
		       uentry_getName (v),
		       v->utype,
		       sRef_unparseFull (v->sref),
		       fileloc_unparse (v->whereSpecified),
		       fileloc_unparse (v->whereDefined)));
    }
  else if (uentry_isVariable (v))
    {
      return 
	(message ("[check: %s] / [%w] = [%s] %s : %t %q [s: %q; def: %q; dec: %q] "
		  "kind <%d> isout <%d> used <%d>",
		  checkedName (v->info->var->checked),
		  (long unsigned) v,
		  ekind_unparse (v->ukind),
		  v->uname,
		  v->utype,
		  sRef_unparseDeep (v->sref),
		  fileloc_unparse (v->whereSpecified),
		  fileloc_unparse (v->whereDefined),
		  fileloc_unparse (v->whereDeclared),
		  (int) v->info->var->kind,
		  (int) v->info->var->defstate,
		  (int) v->used));
    }
  else
    {
      return (message ("[%s] %s : %t %q at [s: %q; d: %q]",
		       ekind_unparse (v->ukind),
		       v->uname,
		       v->utype,
		       sRef_unparseFull (v->sref),
		       fileloc_unparse (v->whereSpecified),
		       fileloc_unparse (v->whereDefined)));
      
    }
}

bool uentry_hasAccessType (uentry e)
{
  if (uentry_isValid (e))
    {
      switch (e->ukind)
	{
	case KITER:
	  return (!typeIdSet_isEmpty (e->info->iter->access));
	case KENDITER:
	  return (!typeIdSet_isEmpty (e->info->enditer->access));
	case KFCN:
	  return (!typeIdSet_isEmpty (e->info->fcn->access));
	case KENUMCONST:
	case KCONST:
	  return (!typeIdSet_isEmpty (e->info->uconst->access));
	default:
	  return FALSE;
	}
    }

  return FALSE;
}
  
typeIdSet uentry_accessType (uentry e)
{
  if (uentry_isValid (e))
    {
      switch (e->ukind)
	{
	case KITER:
	  return (e->info->iter->access);
	case KENDITER:
	  return (e->info->enditer->access);
	case KFCN:
	  return (e->info->fcn->access);
	case KENUMCONST:
	case KCONST:
	  return (e->info->uconst->access);
	default:
	  break;
	}
    }

  return typeIdSet_undefined;
}

bool
uentry_isVariable (uentry e)
{
  return (uentry_isVar (e));
}

bool
uentry_isSpecified (uentry e)
{
  return (uentry_isValid (e) && !fileloc_isUndefined (e->whereSpecified));
}

static bool
uentry_isReallySpecified (uentry e)
{
  return (uentry_isValid (e) 
	  && fileloc_isRealSpec (e->whereSpecified));
}

bool
uentry_isVar (uentry e)
{
  return (!uentry_isUndefined (e) && e->ukind == KVAR);
}

bool 
uentry_isFakeTag (uentry e)
{
  return (uentry_isValid (e) && strchr (cstring_toCharsSafe (e->uname), '!') != 0);
}

bool
uentry_isDatatype (uentry e)
{
  return (!uentry_isUndefined (e) &&
	  (e->ukind == KDATATYPE || e->ukind == KSTRUCTTAG ||
	   e->ukind == KUNIONTAG || e->ukind == KENUMTAG));
}

void
uentry_setAbstract (uentry e)
{
  typeId oldid;

  llassert (uentry_isDatatype (e) 
	    && (ynm_isMaybe (e->info->datatype->abs)));

  oldid = ctype_typeId (e->info->datatype->type);
  e->info->datatype->abs = YES;
  e->info->datatype->type = ctype_createAbstract (oldid);
}

void
uentry_setConcrete (uentry e)
{
  llassert (uentry_isDatatype (e) 
	    && (ynm_isMaybe (e->info->datatype->abs)));

  e->info->datatype->abs = NO;
}

bool
uentry_isAbstractDatatype (uentry e)
{
  return (uentry_isDatatype (e) 
	  && (ynm_isOn (e->info->datatype->abs)));
}

bool
uentry_isMaybeAbstract (uentry e)
{
  return (uentry_isDatatype (e) 
	  && (ynm_isMaybe (e->info->datatype->abs)));
}

bool
uentry_isMutableDatatype (uentry e)
{
  bool res = uentry_isDatatype (e) 
    && (ynm_toBoolRelaxed (e->info->datatype->mut));
  
  return res;
}

bool
uentry_isRefCountedDatatype (uentry e)
{
  return (uentry_isDatatype (e) && (sRef_isRefCounted (uentry_getSref (e))));
}

bool
uentry_isParam (uentry u)
{
  return (uentry_isVariable (u) && (u->info->var->kind == VKPARAM
				    || u->info->var->kind == VKYIELDPARAM));
}

bool
uentry_isExpandedMacro (uentry u)
{
  return (uentry_isVariable (u) && (u->info->var->kind == VKEXPMACRO));
}

bool
uentry_isSefParam (uentry u)
{
  return (uentry_isVariable (u) 
	  && (u->info->var->kind == VKSEFPARAM
	      || u->info->var->kind == VKREFSEFPARAM
	      || u->info->var->kind == VKSEFRETPARAM
	      || u->info->var->kind == VKREFSEFRETPARAM));
}

bool
uentry_isRefParam (uentry u)
{
  return (uentry_isVariable (u) 
	  && (u->info->var->kind == VKREFPARAM
	      || u->info->var->kind == VKREFYIELDPARAM
	      || u->info->var->kind == VKREFSEFPARAM
	      || u->info->var->kind == VKREFSEFRETPARAM));
}

bool
uentry_isAnyParam (uentry u)
{
  return (uentry_isVariable (u) 
	  && ((u->info->var->kind == VKPARAM)
	      || (u->info->var->kind == VKSEFPARAM)
	      || (u->info->var->kind == VKYIELDPARAM)
	      || (u->info->var->kind == VKRETPARAM)
	      || (u->info->var->kind == VKSEFRETPARAM)));
}

sstate 
uentry_getDefState (uentry u)
{
  if (uentry_isValid (u))
    {
      return (sRef_getDefState (u->sref));
    }
  else
    {
      return (SS_UNKNOWN);
    }
}

bool
uentry_isOut (uentry u)
{
  return ((uentry_isVariable (u) && (u->info->var->defstate == SS_ALLOCATED))
	  || (uentry_isDatatype (u) && (sRef_isAllocated (u->sref))));
}

bool
uentry_isPartial (uentry u)
{
  return ((uentry_isVariable (u) && (u->info->var->defstate == SS_PARTIAL))
	  || (uentry_isDatatype (u) && (sRef_isAllocated (u->sref))));
}

bool
uentry_isStateSpecial (uentry u)
{
  return ((uentry_isVariable (u) 
	   && (u->info->var->defstate == SS_SPECIAL))
	  || (uentry_isValid (u) && sRef_isStateSpecial (u->sref)));
}

exitkind uentry_getExitCode (uentry ue) 
{
  if (uentry_isFunction (ue))
    {
      return ue->info->fcn->exitCode;
    }
  else
    {
      return XK_UNKNOWN;
    }
}

qual
uentry_nullPred (uentry u)
{
  llassert (uentry_isRealFunction (u));

  if (uentry_isFunction (u))
    {
      return (u->info->fcn->nullPred);
    }
  else
    {
      return QU_UNKNOWN;
    }
}

bool
uentry_possiblyNull (uentry u)
{
  return ((uentry_isVariable (u) && (nstate_possiblyNull (u->info->var->nullstate)))
	  || (uentry_isDatatype (u) && (sRef_possiblyNull (u->sref))));
}

alkind 
uentry_getAliasKind (uentry u)
{
  if (uentry_isValid (u))
    {
      return (sRef_getAliasKind (uentry_getSref (u)));
    }
  else
    {
      return AK_UNKNOWN;
    }
}

exkind 
uentry_getExpKind (uentry u)
{
  if (uentry_isValid (u))
    {
      return (sRef_getExKind (uentry_getSref (u)));
    }
  else
    {
      return XO_UNKNOWN;
    }
}

bool
uentry_isIter (uentry e)
{
  return (!uentry_isUndefined (e) && e->ukind == KITER);
}

bool
uentry_isEndIter (uentry e)
{
  return (!uentry_isUndefined (e) && e->ukind == KENDITER);
}

bool
uentry_isRealFunction (uentry e)
{
  return (uentry_isFunction (e) ||
	  (uentry_isVariable (e) && ctype_isFunction (uentry_getType (e))));
}

bool
uentry_hasName (uentry e)
{
  if (uentry_isValid (e))
    {
      cstring s = e->uname;
      
      return (!(cstring_isEmpty (s) || cstring_equalLit (s, "...")));
    }
  else
    {
      return FALSE;
    }
}

bool uentry_hasRealName (uentry e)
{
  return (uentry_isValid (e) && cstring_isNonEmpty (e->uname));
}


/*@observer@*/ globSet
uentry_getGlobs (uentry l)
{
  if (uentry_isInvalid (l)) 
    {
      return globSet_undefined;
    }

  if (l->ukind != KFCN)
    {
      if (l->ukind != KITER && l->ukind != KENDITER)
	{
	  if (l->ukind == KVAR)
	    {
	      llbug (message ("Bad call to uentry_getGlobs (var): %q (%s)", 
			      uentry_unparse (l), 
			      ekind_unparse (l->ukind)));
	    }
	  else
	    {
	      llbug (message ("Bad call to uentry_getGlobs: %q (%s)", 
			      uentry_unparse (l), 
			      ekind_unparse (l->ukind)));
	    }
	}
      return globSet_undefined;
    }

  return l->info->fcn->globs;
}

/*@observer@*/ sRefSet
uentry_getMods (uentry l)
{
  llassert (uentry_isValid (l));

  if (l->ukind != KFCN && l->ukind != KITER && l->ukind != KENDITER)
    {
      llcontbug (message ("Bad call to uentry_getMods: %q", uentry_unparse (l)));
      return sRefSet_undefined; 
    }

  return l->info->fcn->mods;
}

ekind
uentry_getKind (uentry e)
{
  llassert (uentry_isValid (e));

  return (e->ukind);
}

/*@observer@*/ multiVal uentry_getConstantValue (uentry e)
{
  llassert (uentry_isEitherConstant (e));

  return (e->info->uconst->val);
}

/*@observer@*/ uentryList
uentry_getParams (uentry l)
{
  if (uentry_isInvalid (l)) return uentryList_undefined;

  switch (l->ukind)
    {
    case KFCN:  
    case KITER:
      {
	ctype ct = l->utype;

	if (ctype_isFunction (ct))
	  {
	    return (ctype_argsFunction (ct));
	  }
	else
	  {
	    return uentryList_undefined;
	  }
      }
    case KVAR:  
      {
	ctype ct = l->utype;

	llassert (ctype_isFunction (ct));
	return (ctype_argsFunction (ct));
      }
    BADDEFAULT;
    }
  BADEXIT;
}

/*@observer@*/ cstring
uentry_rawName (uentry e)
{
  if (uentry_isValid (e))
    {
      return (e->uname);
    }
  else
    {
      return cstring_undefined;
    }
}

static cstring
uentry_getOptName (uentry e)
{
  cstring s = uentry_getName (e);

  if (cstring_isDefined (s))
    {
      s = cstring_appendChar (s, ' ');
    }
  
  return s;
}

/*@only@*/ cstring
uentry_getName (uentry e)
{
  cstring ret = cstring_undefined;

  if (uentry_isValid (e))
    {
      
      if (uentry_isAnyTag (e))
	{
	  ret = fixTagName (e->uname); 
	}
      else if (uentry_isAnyParam (e))
	{
	  ret = cstring_copy (fixParamName (e->uname));
	}
      else
	{
	  ret = cstring_copy (e->uname);
	}
    }

  return ret;
}

cstring uentry_getRealName (uentry e)
{
  if (uentry_isValid (e))
    {
      if (uentry_isAnyTag (e))
	{
	  return (cstring_undefined);
	}
      else
	{
	  return (e->uname);
	}
    }
  return cstring_undefined;
}

ctype uentry_getType (uentry e)
{
  if (uentry_isValid (e))
    {
      return e->utype;
    }
  else
    {
      return ctype_unknown;
    }
}

fileloc uentry_whereLast (uentry e)
{
  fileloc loc;

  if (uentry_isInvalid (e)) 
    {
      return fileloc_undefined;
    }
  
  loc = e->whereDefined;

  if (fileloc_isValid (loc) && !fileloc_isExternal (loc))
    {
      return loc;
    }

  loc = uentry_whereDeclared (e);

  if (fileloc_isValid (loc) && !fileloc_isExternal (loc))
    {
      return loc;
    }

  loc = uentry_whereSpecified (e);
  return loc;
}

fileloc uentry_whereEither (uentry e)
{
  if (uentry_isInvalid (e)) return fileloc_undefined;

  if (fileloc_isDefined (e->whereDefined) 
      && !fileloc_isExternal (e->whereDefined))
    {
      return e->whereDefined;
    }
  else if (fileloc_isDefined (e->whereDeclared))
    {
      return e->whereDeclared;
    }
  else
    {
      return e->whereSpecified;
    }
}

fileloc uentry_whereSpecified (uentry e)
{
  if (uentry_isInvalid (e)) return fileloc_undefined;

  return (e->whereSpecified);
}

fileloc uentry_whereDefined (uentry e)
{
  if (uentry_isInvalid (e)) return fileloc_undefined;

  return (e->whereDefined);
}

fileloc uentry_whereDeclared (uentry e)
{
  if (uentry_isInvalid (e)) return fileloc_undefined;

  return (e->whereDeclared);
}

/*@observer@*/ fileloc
uentry_whereEarliest (uentry e)
{
  if (uentry_isInvalid (e)) return fileloc_undefined;
  
  if (fileloc_isDefined (e->whereSpecified))
    {
      return (e->whereSpecified);
    }
  else if (fileloc_isDefined (e->whereDeclared))
    {
      return (e->whereDeclared);
    }
  else
    {
      return e->whereDefined;
    }
}

void
uentry_setFunctionDefined (uentry e, fileloc loc)
{
  if (uentry_isValid (e))
    {
      llassert (uentry_isFunction (e));

      if (fileloc_isUndefined (e->whereDeclared))
	{
	  e->whereDeclared = fileloc_update (e->whereDeclared, loc);
	}
      
      if (!fileloc_isDefined (e->whereDefined))
	{
	  e->whereDefined = fileloc_update (e->whereDefined, loc);
	}
    }
}

void
uentry_setDeclDef (uentry e, fileloc f)
{
  uentry_setDeclared (e, f);
  
  if (!uentry_isFunction (e)
      && !(uentry_isVariable (e) && uentry_isExtern (e)))
    {
      uentry_setDefined (e, f);
    }
}

void
uentry_setDeclaredForce (uentry e, fileloc f)
{
  llassert (uentry_isValid (e));
  e->whereDeclared = fileloc_update (e->whereDeclared, f);
}

void
uentry_setDeclaredForceOnly (uentry e, fileloc f)
{
  llassert (uentry_isValid (e));
  fileloc_free (e->whereDeclared);
  e->whereDeclared = f;
}

void
uentry_setDeclaredOnly (uentry e, /*@only@*/ fileloc f)
{
  fileloc oldloc;

  llassert (uentry_isValid (e));
  oldloc = e->whereDeclared;  

  if (fileloc_isDefined (oldloc))
    {
      if (fileloc_isLib (oldloc) || fileloc_isImport (oldloc))
	{
	  e->whereDeclared = f;
	  fileloc_free (oldloc);
	}
      else
	{
	  fileloc_free (f);
	}
    }
  else
    {
      e->whereDeclared = f;
      fileloc_free (oldloc);
    }
}
  
void
uentry_setDeclared (uentry e, fileloc f)
{
  fileloc oldloc;

  llassert (uentry_isValid (e));
  oldloc = e->whereDeclared;  

  if (fileloc_isDefined (oldloc))
    {
      if (fileloc_isLib (oldloc) || fileloc_isImport (oldloc))
	{
	  e->whereDeclared = fileloc_update (e->whereDeclared, f);
	}
      else
	{
	  ;
	}
    }
  else
    {
      e->whereDeclared = fileloc_update (e->whereDeclared, f);
    }
}

void
uentry_clearDefined (uentry e)
{
  if (uentry_isValid (e))
    {
      e->whereDefined = fileloc_update (e->whereDefined, fileloc_undefined);
    }
}

void
uentry_setDefined (uentry e, fileloc f)
{
  fileloc oldloc;

  llassert (uentry_isValid (e));
  oldloc = e->whereDefined;

  if (fileloc_isDefined (oldloc))
    {
      if (fileloc_isLib (oldloc) 
	  || fileloc_isImport (oldloc)
	  || fileloc_isBuiltin (oldloc) 
	  || fileloc_isPreproc (oldloc))
	{
	  e->whereDefined = fileloc_update (e->whereDefined, f);
	}
      else
	{
	  if (fileloc_equal (oldloc, f) || context_processingMacros ())
	    {
	      ; /* okay */
	    }
	  else
	    {
	      if (optgenerror (FLG_REDEF,
			       message ("%s %q redefined", 
					ekind_capName (e->ukind),
					uentry_getName (e)),
			       f))
		{
		  llgenindentmsg (message ("Previous definition of %q", 
					   uentry_getName (e)),
				  e->whereDefined);
		}
	    }
	}
    }
  else
    {
      e->whereDefined = fileloc_update (e->whereDefined, f);
    }
}

bool
uentry_isCodeDefined (uentry e)
{
  return (uentry_isValid (e) && fileloc_isDefined (e->whereDefined));
}

bool
uentry_isDeclared (uentry e)
{
  if (uentry_isValid (e))
    {
      return (fileloc_isDefined (e->whereDeclared));
    }

  return FALSE;
}

sRef uentry_getSref (uentry e)
{
  /* not true, used for functions too (but shouldn't be? */
  /* llassertprint (e->ukind == KVAR, ("uentry_getSref: not variable!")); */

  if (uentry_isInvalid (e)) return sRef_undefined;
  
  return (e->sref);
}

sRef uentry_getOrigSref (uentry e)
{
  if (uentry_isValid (e))
    {
      sRef sr = sRef_copy (uentry_getSref (e));
      
      sRef_resetState (sr);
      sRef_clearDerived (sr);
      
      if (uentry_isVariable (e))
	{
	  sRef_setDefState (sr, e->info->var->defstate, fileloc_undefined);
	  sRef_setNullState (sr, e->info->var->nullstate, fileloc_undefined);
	}
      
      return (sr);
    }
  else
    {
      return sRef_undefined;
    }
}

/*
** requires: uentry e is not in a hashed symbol table
*/

void 
uentry_setName (uentry e, /*@only@*/ cstring n)
{
  llassert (uentry_isValid (e));

  cstring_free (e->uname);
  e->uname = n;
}

void
uentry_setType (uentry e, ctype t)
{
  if (uentry_isValid (e)) 
    {
      e->utype = t;
      sRef_setType (e->sref, t);
    }
}

void
uentry_resetParams (uentry ue, /*@only@*/ uentryList pn)
{
  ctype rct;
  ctype rettype = ctype_unknown;
  
  llassert (uentry_isValid (ue));

  rct = ctype_realType (ue->utype);

  if (uentry_isVariable (ue) && (ctype_isFunction (rct) || ctype_isUnknown (rct)))
    {
      uentry_makeVarFunction (ue);
    }

  llassert (uentry_isFunction (ue));

  if (ctype_isFunction (rct))
    {
      rettype = ctype_returnValue (rct);
    }

  ue->utype = ctype_makeNFParamsFunction (rettype, pn);      
}

void
uentry_setRefParam (uentry e)
{
  
  if (!uentry_isVar (e))
    {
      llbug (message ("uentry_setParam: not variable: %q", uentry_unparse (e)));
    }
  else
    {
      if (e->info->var->kind == VKSEFPARAM)
	{
	  e->info->var->kind = VKREFSEFPARAM;
	}
      else if (e->info->var->kind == VKSEFRETPARAM)
	{
	  e->info->var->kind = VKREFSEFRETPARAM;
	}
      else if (e->info->var->kind == VKYIELDPARAM)
	{
	  e->info->var->kind = VKREFYIELDPARAM;
	}
      else
	{
	  e->info->var->kind = VKREFPARAM;
	}
    }
}

void
uentry_setParam (uentry e)
{
  if (!uentry_isVar (e))
    {
      if (uentry_isElipsisMarker (e))
	{

	}
      else
	{
	  llbug (message ("uentry_setParam: not variable: %q", uentry_unparse (e)));
	}
    }
  else
    {
      cstring oldname;

      if (e->info->var->kind == VKYIELDPARAM 
	  || e->info->var->kind == VKSEFPARAM
	  || e->info->var->kind == VKSEFRETPARAM)
	{
	  ;
	}
      else
	{
	  e->info->var->kind = VKPARAM;
	}

      oldname = e->uname;
      e->uname = makeParam (e->uname);
      cstring_free (oldname);
    }
}

void
uentry_setSref (uentry e, sRef s)
{
  if (uentry_isValid (e))
    {
      if (sRef_isValid (e->sref))
	{
	  sRef_mergeStateQuietReverse (e->sref, s);
	}
      else
	{
	  e->sref = sRef_saveCopy (s);
	}
    }
}

ctype
uentry_getAbstractType (uentry e)
{
  llassert (uentry_isDatatype (e));

  /*
  ** This assertion removed.
  ** Okay to have undefined type, for system types
  
  llassertprintret (!ctype_isUndefined (e->info->datatype->type),
		    ("uentry_getAbstractType %q: undefined", uentry_unparseFull (e)),
		    e->utype);
		    
  */

  if (ctype_isUndefined (e->info->datatype->type))
    {
      return ctype_unknown;
    }

  return e->info->datatype->type;
}

ctype uentry_getRealType (uentry e)
{
  ctype   ct;
  typeId uid = USYMIDINVALID;

  if (uentry_isInvalid (e))
    {
      return ctype_unknown;
    }

  llassertprint (uentry_isDatatype (e), ("not datatype: %s", uentry_unparse (e)));

  if (uentry_isAnyTag (e))
    {
      return (e->utype);
    }
  
  if (uentry_isAbstractType (e))
    {
      ct = uentry_getAbstractType (e);      
      llassert (ctype_isUA (ct));
      
      uid = ctype_typeId (ct);
      
      if (!context_hasAccess (uid))
	{
	  return (ct);
	}
    }

  ct = uentry_getType (e);

  if (ctype_isUserBool (ct)) return ct;
  
  if (ctype_isUA (ct))
    {
      usymId iid = ctype_typeId (ct);
      
      if /*@access usymId@*/ (iid == uid) /*@noaccess usymId@*/
	{	  
	  llcontbug (message ("uentry_getRealType: recursive type! %s",
			      ctype_unparse (ct)));
	  return ct;
	}
      else
	{
	  return uentry_getRealType (usymtab_getTypeEntry (iid));
	}
    }
  else
    {
      return ct;
    }
}

ctype uentry_getForceRealType (uentry e)
{
  ctype   ct;
  typeId uid = USYMIDINVALID;

  if (uentry_isInvalid (e))
    {
      return ctype_unknown;
    }

  llassertprint (uentry_isDatatype (e), ("not datatype: %s", uentry_unparse (e)));
  
  if (uentry_isAnyTag (e))
    {
      return (e->utype);
    }
  
  if (uentry_isAbstractType (e))
    {
      ct = uentry_getAbstractType (e);      
      llassert (ctype_isUA (ct));
      
      uid = ctype_typeId (ct);
      /* no check for access! */
    }
  
  ct = uentry_getType (e);
  
  if (ctype_isUserBool (ct)) return ct;
  
  if (ctype_isUA (ct))
    {
      usymId iid = ctype_typeId (ct);
      
      if /*@access usymId@*/ (iid == uid) /*@noaccess usymId@*/
	{	  
	  llcontbug (message ("uentry_getRealType: recursive type! %s",
			      ctype_unparse (ct)));
	  return ct;
	}
      else
	{
	  return uentry_getForceRealType (usymtab_getTypeEntry (iid));
	}
    }
  else
    {
      return ct;
    }
}

uentry uentry_nameCopy (cstring name, uentry e)
{
  uentry enew = uentry_alloc ();

  llassert (uentry_isValid (e));

  /* enew->shallowCopy = FALSE; */
  enew->ukind = e->ukind;
  enew->uname = name;
  enew->utype = e->utype;
  enew->whereSpecified = fileloc_copy (e->whereSpecified);
  enew->whereDefined = fileloc_copy (e->whereDefined);
  enew->whereDeclared = fileloc_copy (e->whereDeclared);
  enew->sref = sRef_copy (e->sref); 
  enew->used = e->used;
  enew->lset = FALSE;
  enew->isPrivate = e->isPrivate;
  enew->hasNameError = FALSE;

  enew->uses = filelocList_new ();

  enew->storageclass = e->storageclass;
  enew->info = uinfo_copy (e->info, e->ukind);

  return enew;
}

void
uentry_setDatatype (uentry e, usymId uid)
{
  llassert (uentry_isDatatype (e));

  if (uentry_isAbstractType (e))
    {
      e->info->datatype->type = ctype_createAbstract (uid);
    }
  else
    {
      e->info->datatype->type = ctype_createUser (uid);
    }
}

static void 
uentry_setSpecDef (/*@special@*/ uentry e, /*@keep@*/ fileloc f)
   /*@defines e->whereSpecified, e->whereDeclared, e->whereDefined@*/
   /*@modifies e@*/
{
  llassert (uentry_isValid (e));

  if (fileloc_isSpec (f) || fileloc_isImport (f))
    {
      e->whereSpecified = f;
      e->whereDeclared  = fileloc_undefined;
      e->whereDefined  = fileloc_undefined;
    }
  else
    {
      e->whereSpecified = fileloc_undefined;
      e->whereDeclared  = f;
      e->whereDefined  = fileloc_undefined;
    }
}

static void
ucinfo_free (/*@only@*/ ucinfo u)
{
  multiVal_free (u->val);
  sfree (u);
}

static void
uvinfo_free (/*@only@*/ uvinfo u)
{
  sfree (u);
}

static void
udinfo_free (/*@only@*/ udinfo u)
{
  sfree (u);
}

static void
ufinfo_free (/*@only@*/ ufinfo u)
{
  globSet_free (u->globs);
  sRefSet_free (u->mods);
  specialClauses_free (u->specclauses);

  sfree (u);
}

static void
uiinfo_free (/*@only@*/ uiinfo u)
{
  sfree (u);
}

static void
ueinfo_free (/*@only@*/ ueinfo u)
{
  sfree (u);
}

static /*@only@*/ ucinfo
ucinfo_copy (ucinfo u)
{
  ucinfo ret = (ucinfo) dmalloc (sizeof (*ret));
  
  ret->val = multiVal_copy (u->val);
  ret->access = u->access;

  return ret;
}

static /*@only@*/ uvinfo
uvinfo_copy (uvinfo u)
{
  uvinfo ret = (uvinfo) dmalloc (sizeof (*ret));
  
  ret->kind = u->kind;
  ret->nullstate = u->nullstate;
  ret->defstate = u->defstate;
  ret->checked = u->checked;

  return ret;
}

static /*@only@*/ udinfo
udinfo_copy (udinfo u)
{
  udinfo ret = (udinfo) dmalloc (sizeof (*ret));

  ret->abs = u->abs;
  ret->mut = u->mut;
  ret->type = u->type;

  return ret;
}

static /*@only@*/ ufinfo
ufinfo_copy (ufinfo u)
{
  ufinfo ret = (ufinfo) dmalloc (sizeof (*ret));

  ret->hasGlobs = u->hasGlobs;
  ret->hasMods = u->hasMods;
  ret->exitCode = u->exitCode;
  ret->specialCode = u->specialCode;
  ret->nullPred = u->nullPred;
  ret->access = u->access;
  ret->globs = globSet_newCopy (u->globs);
  ret->mods = sRefSet_newCopy (u->mods);
  ret->defparams = u->defparams;
  ret->specclauses = specialClauses_copy (u->specclauses);

  return ret;
}

static /*@only@*/ uiinfo
uiinfo_copy (uiinfo u)
{
  uiinfo ret = (uiinfo) dmalloc (sizeof (*ret));

  ret->access = u->access;
  ret->globs = globSet_newCopy (u->globs);
  ret->mods = sRefSet_newCopy (u->mods);

  return (ret);
}

static /*@only@*/ ueinfo
ueinfo_copy (ueinfo u)
{
  ueinfo ret = (ueinfo) dmalloc (sizeof (*ret));

  ret->access = u->access;
  return ret;
}

static void
uinfo_free (uinfo u, ekind kind)
{
  switch (kind)
    {
    case KENUMCONST:
    case KCONST:       ucinfo_free (u->uconst); break;
    case KVAR:         uvinfo_free (u->var); break;
    case KSTRUCTTAG:
    case KUNIONTAG:
    case KENUMTAG:
    case KDATATYPE:    udinfo_free (u->datatype); break;
    case KFCN:         ufinfo_free (u->fcn); break;
    case KITER:        uiinfo_free (u->iter); break;
    case KENDITER:     ueinfo_free (u->enditer); break;
    case KELIPSMARKER: break;
    case KINVALID:     break;
    }
  
    sfree (u);
}

static /*@only@*/ /*@null@*/ uinfo
uinfo_copy (uinfo u, ekind kind)
{
  if (kind == KELIPSMARKER || kind == KINVALID)
    {
      return NULL;
    }
  else
    {
      uinfo ret = (uinfo) dmalloc (sizeof (*ret));
      
      switch (kind)
	{
	case KENUMCONST:
	case KCONST:    ret->uconst = ucinfo_copy (u->uconst); break;
	case KVAR:      ret->var = uvinfo_copy (u->var); break;
	case KSTRUCTTAG:
	case KUNIONTAG:
	case KENUMTAG:
	case KDATATYPE: ret->datatype = udinfo_copy (u->datatype); break;
	case KFCN:      ret->fcn = ufinfo_copy (u->fcn); break;
	case KITER:     ret->iter = uiinfo_copy (u->iter); break;
	case KENDITER:  ret->enditer = ueinfo_copy (u->enditer); break;
	BADDEFAULT;
	}
      return ret;
    }
}

static void
uentry_reallyFree (/*@notnull@*/ /*@only@*/ uentry e)
{
  filelocList_free (e->uses);
  cstring_free (e->uname);
  
  uinfo_free (e->info, e->ukind);
  
  fileloc_free (e->whereSpecified); 
  fileloc_free (e->whereDefined); 
  fileloc_free (e->whereDeclared); 

  nuentries--;
  sfree (e);
  }

extern void uentry_markOwned (/*@owned@*/ uentry u)
{
  sfreeEventually (u);
}

void
uentry_free (/*@only@*/ uentry e)
{
  if (uentry_isValid (e) && !uentry_isElipsisMarker (e))
    {
      uentry_reallyFree (e);
    }
}

/*
** For uentry's in the global or file scope
*/

void
uentry_freeComplete (/*@only@*/ uentry e)
{
  if (uentry_isValid (e) && !uentry_isElipsisMarker (e))
    {
      /*@i@*/ sRef_free (e->sref);
      e->sref = sRef_undefined;
      uentry_reallyFree (e);
    }
}

/*
** requires old->kind != new->kind, old->uname = new->uname
*/

static void
KindConformanceError (/*@unique@*/ uentry old, uentry unew, bool mustConform)
{
  llassert (uentry_isValid (old));
  llassert (uentry_isValid (unew));

  if (uentry_isEitherConstant (unew)
      && (fileloc_isPreproc (uentry_whereDeclared (old))
	  || ctype_isUnknown (old->utype))
      && !uentry_isSpecified (old))
    {
      ; /* no error */
    }
  else 
    {
      if (mustConform)
	{
	  if (!uentry_isDeclared (old))
	    {
	      if (uentry_isSpecified (old))
		{
		  if (uentry_isSpecified (unew))
		    {
		      llbuglit ("Respecification!");
		    }
		  else if (uentry_isDeclared (unew))
		    {
		      if (optgenerror
			  (FLG_INCONDEFS,
			   message ("%s %q inconsistently declared as %s: %t",
				    ekind_capName (old->ukind),
				    uentry_getName (unew),
				    ekind_unparseLong (unew->ukind),
				    unew->utype),
			   uentry_whereDeclared (unew)))
			{
			  uentry_showWhereLast (old);
			}
		    }
		  else
		    {
		      BADEXIT;
		    }
		}
	      else
		{
		  if (optgenerror
		      (FLG_INCONDEFS,
		       message ("%s %q inconsistently declared as %s: %t",
				ekind_capName (old->ukind),
				uentry_getName (unew),
				ekind_unparseLong (unew->ukind),
				unew->utype),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereLast (old);
		    }
		}
	    }
	  else
	    {
	      llassert (uentry_isDeclared (unew));

	      if (optgenerror
		  (FLG_INCONDEFS,
		   message ("%s %q inconsistently redeclared as %s",
			    ekind_capName (old->ukind),
			    uentry_getName (unew),
			    ekind_unparseLong (unew->ukind)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLast (old);
		}
	    }
	}
    }

  uentry_copyInto (old, unew);
}

/*
** def is the definition of spec, modifies spec
**
** reports any inconsistencies
** returns the summary of all available information
** if spec and def are inconsistent, def is returned
*/

void
uentry_showWhereLast (uentry spec)
{
  if (uentry_isValid (spec))
    {
      if (fileloc_isDefined (spec->whereDefined)
	  && !fileloc_isLib (spec->whereDefined)
	  && !fileloc_isPreproc (spec->whereDefined))
	{
	  llgenindentmsg (message ("Previous definition of %q: %t", 
				   uentry_getName (spec),
				   uentry_getType (spec)),
			  uentry_whereDefined (spec));
	}
      else if (uentry_isDeclared (spec))
	{
	  llgenindentmsg (message ("Previous declaration of %q: %t", 
				   uentry_getName (spec),
				   uentry_getType (spec)),
			  uentry_whereDeclared (spec));
	}
      else if (uentry_isSpecified (spec))
	{
	  if (uentry_hasName (spec))
	    {
	      llgenindentmsg (message ("Specification of %q: %t", 
				       uentry_getName (spec),
				       uentry_getType (spec)),
			      uentry_whereSpecified (spec));
	    }
	  else
	    {
	      llgenindentmsg (message ("Specification: %t", uentry_getType (spec)),
			      uentry_whereSpecified (spec));
	    }
	}
      else
	{
	  /* nothing to show */
	}
    }
}

void
uentry_showDefSpecInfo (uentry ce, fileloc fwhere)
{
  fileloc loc = uentry_whereDefined (ce);
  
  if (fileloc_isUser (loc) && !fileloc_equal (loc, fwhere))
    {
      llgenindentmsg (message ("Definition of %q", uentry_getName (ce)),
		      loc);
    }

  loc = uentry_whereSpecified (ce);
  
  if (fileloc_isUser (loc) && !fileloc_equal (loc, fwhere))
    {
      llgenindentmsg (message ("Specification of %q", uentry_getName (ce)),
		      loc);
    }
}

void uentry_showWhereLastExtra (uentry spec, cstring extra)
{
  if (uentry_isDeclared (spec))
    {
      llgenindentmsg (message ("Previous declaration of %q: %q", 
			       uentry_getName (spec), extra),
		      uentry_whereDeclared (spec));
    }
  else if (uentry_isSpecified (spec))
    {
      llgenindentmsg (message ("Specification of %q: %q", 
			       uentry_getName (spec), extra),
		      uentry_whereSpecified (spec));
    }
  else
    {
      cstring_free (extra);
    }
}

void
uentry_showWhereDeclared (uentry spec)
{
  if (uentry_isDeclared (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Declaration of %q", uentry_getName (spec)),
			  uentry_whereDeclared (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Declaration"), uentry_whereDeclared (spec));
	}
    }
  else if (uentry_isSpecified (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Specification of %q", uentry_getName (spec)),
			  uentry_whereSpecified (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Specification"), uentry_whereSpecified (spec));
	}
    }
  else
    {
      /* nothing to show */
    }
    
}

void
uentry_showWhereAny (uentry spec)
{
  if (uentry_isDeclared (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Declaration of %q", uentry_getName (spec)),
			  uentry_whereDeclared (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Declaration"), uentry_whereDeclared (spec));
	}
    }
  else if (uentry_isSpecified (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Specification of %q",
				   uentry_getName (spec)),
			  uentry_whereSpecified (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Specification"), 
			  uentry_whereSpecified (spec));
	}
    }
  else if (fileloc_isDefined (uentry_whereDefined (spec))) 
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Definition of %q", uentry_getName (spec)),
			  uentry_whereDefined (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Definition"), uentry_whereDefined (spec));
	}
    }
  else
    {
      /* nothing to show */
    }
}

void
uentry_showWhereDefined (uentry spec)
{
  if (uentry_isCodeDefined (spec))
    {
      llgenindentmsg (message ("Previous definition of %q", uentry_getName (spec)),
		      uentry_whereDefined (spec));
    }
}

void
uentry_showWhereLastPlain (uentry spec)
{
  if (uentry_isDeclared (spec))
    {
      llgenindentmsg (message ("Previous declaration of %q", uentry_getName (spec)),
		      uentry_whereDeclared (spec));
    }
  else if (uentry_isSpecified (spec))
    {
      llgenindentmsg (message ("Specification of %q", uentry_getName (spec)),
		      uentry_whereSpecified (spec));
    }
  else
    {
          }
}

static void
uentry_showWhereLastVal (uentry spec, cstring val)
{
  if (uentry_isDeclared (spec))
    {
      llgenindentmsg (message ("Previous declaration of %q: %s", 
			       uentry_getName (spec), val),
		      uentry_whereDeclared (spec));
    }
  else if (uentry_isSpecified (spec))
    {
      llgenindentmsg (message ("Specification of %q: %s", 
			       uentry_getName (spec), val),
		      uentry_whereSpecified (spec));
    }
  else
    {
    }
}

void
uentry_showWhereSpecified (uentry spec)
{
  if (uentry_isSpecified (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Specification of %q", uentry_getName (spec)),
			  uentry_whereSpecified (spec));
	}
      else
	{
	  llgenindentmsg (cstring_makeLiteral ("Specification"), 
			  uentry_whereSpecified (spec));
	}
    }
  else if (uentry_isDeclared (spec))
    {
      llgenindentmsg (message ("Declaration of %q", uentry_getName (spec)),
		      uentry_whereDeclared (spec));
    }
  else
    {
      /* nothing to show */
    }
}

void
uentry_showWhereSpecifiedExtra (uentry spec, cstring s)
{
  if (uentry_isSpecified (spec))
    {
      if (uentry_hasName (spec))
	{
	  llgenindentmsg (message ("Specification of %q: %q", 
				   uentry_getName (spec), s),
			  uentry_whereSpecified (spec));
	}
      else
	{
	  llgenindentmsg (message ("Specification: %q", s), 
			  uentry_whereSpecified (spec));
	}
    }
  else if (uentry_isDeclared (spec))
    {
      llgenindentmsg (message ("Declaration of %q: %q", 
			       uentry_getName (spec), s),
		      uentry_whereDeclared (spec));
    }
  else
    {
      llgenindentmsg (message ("Previous: %q", s),
		      uentry_whereLast (spec));
    }
}

/*
**
*/

static void
checkStructConformance (uentry old, uentry unew)
{
  ctype oldr, newr; 
  uentryList fold, fnew;

  /*
  ** requires: types of old and new are structs or unions
  */

  llassert (uentry_isValid (old));
  llassert (uentry_isValid (unew));

  oldr = ctype_realType (old->utype);
  fold =  ctype_getFields (oldr);

  newr = ctype_realType (unew->utype);
  fnew = ctype_getFields (newr);

  if (!uentryList_matchFields (fold, fnew))
    {
      if (fileloc_equal (uentry_whereLast (old),
			 uentry_whereLast (unew)))
	{
	  ; /* cheat! */
	}
      else 
	{
	  if (optgenerror 
	      (FLG_MATCHFIELDS,
	       message ("%q %q %rdeclared with fields { %q }, %s "
			"with fields { %q }",
			cstring_makeLiteral (ctype_isStruct (newr) ? "Structure": "Union"),
			uentry_getName (old), 
			uentry_isDeclared (old),
			uentryList_unparseAbbrev (fnew),
			uentry_specOrDefName (old),
			uentryList_unparseAbbrev (fold)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereLastPlain (old);
	      uentryList_showFieldDifference (fold, fnew);
	    }
	}

      old->utype = unew->utype;
    }
}

static void
checkEnumConformance (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew)
{
  /*
  ** requires old and new are enums
  */
  
  ctype        rold = ctype_realType (old->utype);
  ctype        rnew = ctype_realType (unew->utype);
  enumNameList eold = ctype_elist (rold);
  enumNameList enew = ctype_elist (rnew);
  
  if (!enumNameList_match (eold, enew))
    {
      if (optgenerror 
	  (FLG_MATCHFIELDS,
	   message ("Enum %q declared with members { %q } but "
		    "specified with members { %q }",
		    uentry_getName (old), 
		    enumNameList_unparse (enew),
		    enumNameList_unparse (eold)),
	   uentry_whereDeclared (unew)))
	{
	  uentry_showWhereSpecified (old);
	  old->utype = unew->utype;
	}
    }
}

/*
** either oldCurrent or newCurrent may be undefined!
*/

static void
paramTypeError (uentry old, uentry oldCurrent, ctype oldType,
		uentry unew, uentry newCurrent, ctype newType,
		int paramno)
{
  bool hasError = FALSE;

  if (uentry_isValid (newCurrent) && uentry_isDeclared (newCurrent))
    {
      if (uentry_hasName (newCurrent))
	{
	  hasError = optgenerror 
	    (FLG_TYPE,
	     message ("Parameter %d, %q, of function %q has inconsistent type: "
		      "declared %t, %s %t",
		      paramno + 1, uentry_getName (newCurrent), 
		      uentry_getName (unew),
		      newType, uentry_specOrDefName (old), oldType),
	     uentry_whereDeclared (newCurrent));
	}
      else
	{
	  hasError = optgenerror
	    (FLG_TYPE,
	     message ("Parameter %d of function %q has inconsistent type: "
		      "declared %t, %s %t",
		      paramno + 1, uentry_getName (unew),
		      newType, uentry_specOrDefName (old), oldType),
	     uentry_whereDeclared (newCurrent));

	  DPRINTF (("type: %s / %s",
		    ctype_unparse (newType),
		    ctype_unparse (ctype_realType (newType))));
	}
    }
  else 
    {
      if (uentry_isDeclared (unew))
	{
	  hasError = optgenerror 
	    (FLG_TYPE,
	     message ("Parameter %d of function %s has inconsistent type: "
		      "declared %t, %s %t",
		      paramno + 1, unew->uname, 
		      newType, uentry_specOrDefName (old), oldType),
	     uentry_whereDeclared (unew));
	}
      else
	{
	  hasError = optgenerror
	    (FLG_TYPE,
	     message ("Parameter %d of function %s has inconsistent type: "
		      "declared %t, %s %t",
		      paramno + 1, unew->uname, 
		      newType, uentry_specOrDefName (old), oldType),
	     uentry_whereDeclared (unew));
	}
    }
  
  if (hasError)
    {
      if (!uentry_isUndefined (oldCurrent))
	{
	  if (!uentry_isUndefined (newCurrent) 
	      && cstring_equal (uentry_rawName (newCurrent), uentry_rawName (oldCurrent)))
	    {
	      uentry_showWhereLast (oldCurrent);
	    }
	  else
	    {
	      uentry_showWhereLastPlain (old);
	    }
	  
	  uentry_setType (oldCurrent, newType);
	}
      else
	{
	  uentry_showWhereLastPlain (old);
	}
    }
}

static void
nargsError (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew)
{
  if (optgenerror 
      (FLG_TYPE,
       message ("Function %s %rdeclared with %d arg%p, %s with %d",
		unew->uname, 
		uentry_isDeclared (old),
		uentryList_size (uentry_getParams (unew)),
		uentry_specOrDefName (old),
		uentryList_size (uentry_getParams (old))),
       uentry_whereDeclared (unew)))
    {
      uentry_showWhereLastPlain (old);
    }
}

static void
returnValueError (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew)
{
  if (optgenerror
      (FLG_INCONDEFS,
       message ("Function %s inconsistently %rdeclared to return %t",
		unew->uname,
		uentry_isDeclared (old),
		ctype_returnValue (unew->utype)),
       uentry_whereDeclared (unew)))
    {
      uentry_showWhereLastVal (old, ctype_unparse (ctype_returnValue (old->utype)));
    }
}

static cstring paramStorageName (uentry ue)
{
  return (cstring_makeLiteralTemp (uentry_isParam (ue) ? "param" : "storage"));
}

static cstring fcnErrName (uentry ue)
{
  return (cstring_makeLiteralTemp (uentry_isFunction (ue) ? "to return" : "as"));
}

extern /*@observer@*/ cstring uentry_checkedName (uentry ue)
{
  if (uentry_isVar (ue))
    {
      return (checkedName (ue->info->var->checked));
    }
  else
    {
      return (cstring_makeLiteralTemp ("<checked invalid>"));
    }
}

static cstring checkedName (chkind checked)
{
  switch (checked)
    {
    case CH_UNKNOWN:       return (cstring_makeLiteralTemp ("unknown"));
    case CH_UNCHECKED:     return (cstring_makeLiteralTemp ("unchecked"));
    case CH_CHECKED:       return (cstring_makeLiteralTemp ("checked"));
    case CH_CHECKMOD:      return (cstring_makeLiteralTemp ("checkmod"));
    case CH_CHECKEDSTRICT: return (cstring_makeLiteralTemp ("checkedstrict"));
    }
  BADEXIT;
}

static
void checkNullState (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, bool mustConform, bool completeConform)
{
  nstate oldState;
  nstate newState;
  
  if (uentry_isVar (unew))
    {
      llassert (uentry_isVar (old));
      
      oldState = old->info->var->nullstate;
      newState = unew->info->var->nullstate;
    }
  else
    {
      oldState = sRef_getNullState (old->sref);
      newState = sRef_getNullState (unew->sref);
    }

  if (oldState == NS_ABSNULL)
    {
      if (uentry_isVar (old))
	{
	  old->info->var->nullstate = newState;
	}
      
      sRef_mergeNullState (old->sref, newState);
    }
  else if (newState == NS_UNKNOWN)
    {
      if (completeConform && newState != oldState
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified as %s, but declared without %s qualifier",
			ekind_capName (unew->ukind),
			uentry_getName (unew),
			nstate_unparse (oldState),
			nstate_unparse (oldState)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
      
      if (uentry_isVar (unew))
	{
	  unew->info->var->nullstate = oldState;
	}

      sRef_mergeNullState (unew->sref, oldState);
    }
  else if (newState == NS_POSNULL)
    {
      if (oldState == NS_MNOTNULL 
	  && (ctype_isUA (unew->utype) 
	      || (uentry_isFunction (unew)
		  && ctype_isUA (ctype_returnValue (unew->utype)))))
	{
	  if (uentry_isVar (unew))
	    {
	      unew->info->var->nullstate = oldState;
	    }

	  sRef_mergeNullState (unew->sref, oldState);
	}
      else 
	{
	  if (oldState == NS_NOTNULL || oldState == NS_MNOTNULL 
	      || oldState == NS_UNKNOWN)
	    {
	      if (mustConform)
		{
		  if (optgenerror 
		      (FLG_INCONDEFS,
		       message 
		       ("%s %q inconsistently %rdeclared %s possibly null storage, "
			"%s %q qualifier",
			uentry_ekindName (unew),
			uentry_getName (unew),
			uentry_isDeclared (old),
			fcnErrName (unew),
			uentry_specOrDefName (old),
			cstring_makeLiteral (oldState == NS_MNOTNULL ? "with notnull" : "without null")),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereSpecified (old);
		    }
		}
	    }
	  
	  if (uentry_isVar (old))
	    {
	      old->info->var->nullstate = newState;
	    }

	  sRef_mergeNullState (old->sref, newState);
	}
    }
  else if (newState == NS_MNOTNULL)
    {
      if (oldState != NS_MNOTNULL)
	{
	  if (mustConform)
	    {
	      if (optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q inconsistently %rdeclared %s notnull storage, "
			    "%s without notnull qualifier",
			    uentry_ekindName (unew),
			    uentry_getName (unew),
			    uentry_isDeclared (old),
			    fcnErrName (unew),
			    uentry_specOrDefName (old)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereSpecified (old);
		}
	    }
	  
	  if (uentry_isVar (old))
	    {
	      old->info->var->nullstate = newState;
	    }

	  sRef_mergeNullState (old->sref, newState);
	}
    }
  else
    {
      if (uentry_isVar (unew)) 
	{
	  unew->info->var->nullstate = oldState;
	}

      sRef_mergeNullState (unew->sref, oldState);
    }
}

static
void checkDefState (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, 
		    bool mustConform, bool completeConform)
{
  sstate oldState;
  sstate newState;
  bool vars = FALSE;

  if (uentry_isVar (old) && uentry_isVar (unew))
    {
      oldState = old->info->var->defstate;
      newState = unew->info->var->defstate;
      vars = TRUE;
    }
  else
    {
      oldState = sRef_getDefState (old->sref);
      newState = sRef_getDefState (unew->sref);
    }

  if (newState != oldState && newState != SS_UNKNOWN && newState != SS_DEFINED)
    {
      if (mustConform)
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message ("%s %q inconsistently %rdeclared %s %s %s, "
			"%s %s %s %s",
			uentry_ekindName (unew),
			uentry_getName (unew),
			uentry_isDeclared (old),
			fcnErrName (unew),
			sstate_unparse (newState),
			paramStorageName (unew),
			uentry_specOrDefName (old),
			fcnErrName (unew),
			sstate_unparse (oldState),
			paramStorageName (unew)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
      
      if (vars) old->info->var->defstate = newState;
      sRef_setDefState (old->sref, newState, uentry_whereDeclared (unew));
    }
  else
    {
      if (completeConform
	  && (newState != oldState) && (oldState != SS_DEFINED)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified as %s, but declared without %s qualifier",
			ekind_capName (unew->ukind),
			uentry_getName (unew),
			sstate_unparse (oldState),
			sstate_unparse (oldState)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}

      if (vars) unew->info->var->defstate = oldState;
      sRef_setDefState (unew->sref, oldState, uentry_whereDeclared (unew));
    }
}

static void 
  checkAliasState (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, 
		   bool mustConform, bool completeConform)
{
  alkind newKind;
  alkind oldKind;

  oldKind = sRef_getAliasKind (old->sref);
  newKind = sRef_getAliasKind (unew->sref);

  if (alkind_isImplicit (newKind) 
      || (alkind_isRefCounted (newKind) && !uentry_isDatatype (unew)))
    {
      if (completeConform && !alkind_equal (newKind, oldKind)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified as %s, but declared without "
			"explicit alias qualifier",
			ekind_capName (unew->ukind),
			uentry_getName (unew),
			alkind_unparse (oldKind)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}

      /*  
      ** This really shouldn't be necessary, but it is!
      ** Function params (?) use new here.
      */

      sRef_setAliasKind (unew->sref, oldKind, uentry_whereDeclared (unew));
      return;
    }

  if (alkind_isKnown (newKind))
    {
      if (!alkind_equal (oldKind, newKind))
	{
	  if (alkind_isKnown (oldKind))
	    {
	      if (mustConform && 
		  optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q inconsistently %rdeclared %s %s storage, "
			    "%s as %s storage",
			    uentry_ekindName (unew),
			    uentry_getName (unew),
			    uentry_isDeclared (old),
			    fcnErrName (unew),
			    alkind_unparse (newKind),
			    uentry_specOrDefName (old),
			    alkind_unparse (oldKind)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereSpecified (old);

		  sRef_setAliasKind (old->sref, AK_ERROR, 
				     uentry_whereDeclared (unew));
		}
	      else
		{
		  sRef_setAliasKind (old->sref, newKind, 
				     uentry_whereDeclared (unew));
		}
	    }
	  else
	    {
	      if (!(alkind_isImplicit (newKind)))
		{
		  if (mustConform &&
		      !uentry_isFunction (unew) &&
		      optgenerror 
		      (FLG_INCONDEFS,
		       message ("%s %q inconsistently %rdeclared %s %s storage, "
				"implicitly %s as temp storage",
				uentry_ekindName (unew),
				uentry_getName (unew),
				uentry_isDeclared (old),
				fcnErrName (unew),
				alkind_unparse (newKind),
				uentry_specOrDefName (old)),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereSpecified (old);
		      oldKind = AK_ERROR;
		    }
		  
		  sRef_setAliasKind (old->sref, newKind, 
				     uentry_whereDeclared (unew));
		}
	      else /* newKind is temp or refcounted */
		{
		  ;
		}
	    }
	}
    }
  else /* newKind unknown */
    {
      ;
    }
}

static void 
  checkExpState(/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, 
		bool mustConform, bool completeConform)
{
  exkind newKind;
  exkind oldKind;
  
  oldKind = sRef_getExKind (old->sref);
  newKind = sRef_getExKind (unew->sref);

  if (exkind_isKnown (newKind))
    {
      if (oldKind != newKind)
	{
	  if (exkind_isKnown (oldKind))
	    {
	      if (mustConform && 
		  optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q inconsistently %rdeclared %s %s, %s as %s",
			    uentry_ekindName (unew),
			    uentry_getName (unew),
			    uentry_isDeclared (old),
			    fcnErrName (unew),
			    exkind_unparse (newKind),
			    uentry_specOrDefName (old),
			    exkind_unparse (oldKind)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereSpecified (old);
		}

	      sRef_setExKind (old->sref, newKind, uentry_whereDefined (unew));
	    }
	  else
	    {
	      if (mustConform &&
		  optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q inconsistently %rdeclared %s %s, "
			    "implicitly %s without exposure qualifier",
			    uentry_ekindName (unew),
			    uentry_getName (unew),
			    uentry_isDeclared (old),
			    fcnErrName (unew),
			    exkind_unparse (newKind),
			    uentry_specOrDefName (old)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereSpecified (old);
		}

	      sRef_setExKind (old->sref, newKind, uentry_whereDefined (unew));
	    }
	}
    }
  else
    {
      if (completeConform && exkind_isKnown (oldKind)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified as %s, but declared without "
			"exposure qualifier",
			ekind_capName (unew->ukind),
			uentry_getName (unew),
			exkind_unparse (oldKind)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}

      /* yes, this is necessary! (if its a param) */
      sRef_setExKind (unew->sref, oldKind, fileloc_undefined);
    }
}

static void
uentry_checkStateConformance (/*@notnull@*/ uentry old,
			      /*@notnull@*/ uentry unew,
			      bool mustConform, bool completeConform)
{
  checkDefState (old, unew, mustConform, completeConform);
  checkNullState (old, unew, mustConform, completeConform);
  checkAliasState (old, unew, mustConform, completeConform);
  checkExpState (old, unew, mustConform, completeConform);

  sRef_storeState (old->sref);
  sRef_storeState (unew->sref);
}

static void
checkVarConformance (uentry old, uentry unew, bool mustConform, bool completeConform)
{
  if (uentry_isElipsisMarker (old) || uentry_isElipsisMarker (unew))
    {
      return;
    }

  llassert (uentry_isVar (old));
  llassert (uentry_isVar (unew));

  if (cstring_isEmpty (old->uname)) 
    {
      cstring_free (old->uname);
      old->uname = cstring_copy (unew->uname);
    }

  if (unew->info->var->kind == VKRETPARAM
      || unew->info->var->kind == VKSEFRETPARAM)
    {
      if (old->info->var->kind != VKRETPARAM
	  && old->info->var->kind != VKSEFRETPARAM)
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message ("Parameter %q inconsistently %rdeclared as "
			"returned parameter",
			uentry_getName (unew),
			uentry_isDeclared (old)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	      old->info->var->kind = unew->info->var->kind;
	    }
	}
    }


  if (unew->info->var->kind == VKSEFPARAM || unew->info->var->kind == VKSEFRETPARAM)
    {
      if (old->info->var->kind != VKSEFPARAM 
	  && old->info->var->kind != VKSEFRETPARAM)
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message ("Parameter %qinconsistently %rdeclared as "
			"sef parameter",
			uentry_getOptName (unew),
			uentry_isDeclared (old)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	      old->info->var->kind = unew->info->var->kind;
	    }
	}
    }

  if (old->info->var->kind == VKSPEC)
    {
      old->info->var->kind = unew->info->var->kind;
    }
  else
    {
      unew->info->var->kind = old->info->var->kind;
    }

  if (unew->info->var->checked != CH_UNKNOWN
      && unew->info->var->checked != old->info->var->checked)
    {
      if (old->info->var->checked == CH_UNKNOWN
	  && !fileloc_isUser (uentry_whereLast (old)))
	{
	  ; /* no error */
	}
      else
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message ("Variable %q inconsistently %rdeclared as "
			"%s parameter (was %s)",
			uentry_getName (unew),
			uentry_isDeclared (old),
			checkedName (unew->info->var->checked),
			checkedName (old->info->var->checked)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
      
      old->info->var->checked = unew->info->var->checked;
    }
  else
    {
      if (completeConform 
	  && (old->info->var->checked != CH_UNKNOWN)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message ("%s %q specified as %s, but declared without %s qualifier",
			ekind_capName (unew->ukind),
			uentry_getName (unew),
			checkedName (old->info->var->checked),
			checkedName (old->info->var->checked)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
      
      unew->info->var->checked = old->info->var->checked;
    }

  uentry_checkStateConformance (old, unew, mustConform, completeConform);
}

void uentry_checkMatchParam (uentry u1, uentry u2, int paramno, exprNode e)
{
  if (uentry_isElipsisMarker (u1) || uentry_isElipsisMarker (u2))
    {
      return;
    }

  llassert (uentry_isVar (u1));
  llassert (uentry_isVar (u2));

  if (u1->info->var->kind != u2->info->var->kind) {
    if (u1->info->var->kind == VKSEFRETPARAM) {
      if (u2->info->var->kind == VKRETPARAM) {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "sef parameter, but non-sef parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      } else if (u2->info->var->kind == VKSEFPARAM) {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "returns parameter, but non-returns parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      } else {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "sef returns parameter, but non-sef returns parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      }
    } else if (u1->info->var->kind == VKRETPARAM) {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "returns parameter, but non-returns parameter in "
		  "assigned function: %s",
		  paramno, exprNode_unparse (e)),
	 exprNode_loc (e));
    } else if (u1->info->var->kind == VKSEFPARAM) {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "sef parameter, but non-sef parameter in "
		  "assigned function: %s",
		  paramno, exprNode_unparse (e)),
	 exprNode_loc (e));
    } else {
      if (u2->info->var->kind == VKSEFRETPARAM) {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "normal parameter, but sef returns parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      } else if (u2->info->var->kind == VKSEFPARAM) {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "normal parameter, but sef parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      } else if (u2->info->var->kind == VKRETPARAM) {
	voptgenerror 
	  (FLG_TYPE,
	   message ("Function types are inconsistent. Parameter %d is "
		    "normal parameter, but returns parameter in "
		    "assigned function: %s",
		    paramno, exprNode_unparse (e)),
	   exprNode_loc (e));
      } else {
	BADBRANCH;
      }
    }
  }

  if (u1->info->var->defstate != u2->info->var->defstate) 
    {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "%s, but %s in assigned function: %s",
		  paramno, 
		  sstate_unparse (u1->info->var->defstate),
		  sstate_unparse (u2->info->var->defstate),
		  exprNode_unparse (e)),
	 exprNode_loc (e));
    }

  if (u1->info->var->nullstate != u2->info->var->nullstate) 
    {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "%s, but %s in assigned function: %s",
		  paramno, 
		  nstate_unparse (u1->info->var->nullstate),
		  nstate_unparse (u2->info->var->nullstate),
		  exprNode_unparse (e)),
	 exprNode_loc (e));
    }
      
  if (sRef_getAliasKind (u1->sref) != sRef_getAliasKind (u2->sref))
    {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "%s, but %s in assigned function: %s",
		  paramno, 
		  alkind_unparse (sRef_getAliasKind (u1->sref)),
		  alkind_unparse (sRef_getAliasKind (u2->sref)),
		  exprNode_unparse (e)),
	 exprNode_loc (e));
    }

  if (sRef_getExKind (u1->sref) != sRef_getExKind (u2->sref))
    {
      voptgenerror 
	(FLG_TYPE,
	 message ("Function types are inconsistent. Parameter %d is "
		  "%s, but %s in assigned function: %s",
		  paramno, 
		  exkind_unparse (sRef_getExKind (u1->sref)),
		  exkind_unparse (sRef_getExKind (u2->sref)),
		  exprNode_unparse (e)),
	 exprNode_loc (e));
    }
}

static void
checkFunctionConformance (/*@unique@*/ /*@notnull@*/ uentry old,
			  /*@notnull@*/ uentry unew, 
			  bool mustConform, /*@unused@*/ bool completeConform)
{
  uentryList oldParams  = uentry_getParams (old);
  uentryList newParams  = uentry_getParams (unew);
  ctype      newType    = unew->utype;
  ctype      oldType    = old->utype;
  ctype      oldRetType = ctype_unknown;
  ctype      newRetType = ctype_unknown;

  if (uentry_isForward (old))
    {
      mustConform = FALSE;
      uentry_copyInto (old, unew);
      return;
    }

  /*
  ** check return values
  */
  
  if (ctype_isKnown (oldType))
    {
      llassert (ctype_isFunction (oldType));

      oldRetType = ctype_returnValue (oldType);
    }

  if (ctype_isKnown (newType))
    {
      llassert (ctype_isFunction (newType));

      newRetType = ctype_returnValue (newType);
    }

  if (ctype_isKnown (oldRetType) && ctype_isKnown (newRetType)
      && !ctype_matchDef (newRetType, oldRetType))
    {
      if (mustConform) returnValueError (old, unew);
    }
  else 
    {
      if (ctype_isConj (newRetType))
	{
	  if (ctype_isConj (oldRetType))
	    {
	      if (!ctype_sameAltTypes (newRetType, oldRetType))
		{
		  if (optgenerror 
		      (FLG_INCONDEFS,
		       message ("Function %q inconsistently %rdeclared to "
				"return alternate types %s "
				"(types match, but alternates are not identical, "
				"so checking may not be correct)",
				uentry_getName (unew),
				uentry_isDeclared (old),
				ctype_unparse (newRetType)),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereLastVal (old, ctype_unparse (oldRetType));
		    }
		}
	    }
	  else
	    {
	      old->utype = ctype_makeFunction (oldRetType, uentryList_copy (newParams));
	    }
	}
    }

  uentry_checkStateConformance (old, unew, mustConform, completeConform);

  if (!exitkind_equal (unew->info->fcn->exitCode, old->info->fcn->exitCode))
    {
      if (exitkind_isKnown (unew->info->fcn->exitCode))
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message ("Function %q inconsistently %rdeclared using %s",
			uentry_getName (unew),
			uentry_isDeclared (old),
			exitkind_unparse (unew->info->fcn->exitCode)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
      else
	{
	  unew->info->fcn->exitCode = old->info->fcn->exitCode;
	}
    }

  if (!qual_isUnknown (unew->info->fcn->nullPred))
    {
      if (!qual_equal (old->info->fcn->nullPred, unew->info->fcn->nullPred))
	{
	  if (optgenerror
	      (FLG_INCONDEFS,
	       message ("Function %q inconsistently %rdeclared using %s",
			uentry_getName (unew),
			uentry_isDeclared (old),
			qual_unparse (unew->info->fcn->nullPred)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
    }
  else
    {
      unew->info->fcn->nullPred = old->info->fcn->nullPred;
    }

  if (unew->info->fcn->specialCode != SPC_NONE)
    {
      if (old->info->fcn->specialCode != unew->info->fcn->specialCode)
	{
	  if (optgenerror
	      (FLG_INCONDEFS,
	       message ("Function %q inconsistently %rdeclared using %s",
			uentry_getName (unew),
			uentry_isDeclared (old),
			specCode_unparse (unew->info->fcn->specialCode)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereSpecified (old);
	    }
	}
    }
  else
    {
      unew->info->fcn->specialCode = old->info->fcn->specialCode;
    }
	  
  /*
  ** check parameters
  */
  
  if (!uentryList_sameObject (oldParams, newParams)
      && (!uentryList_isMissingParams (oldParams)))
    {
      if (!uentryList_isMissingParams (newParams))
	{
	  int paramno = 0;
	  int nparams = uentryList_size (oldParams);
	  bool checknames = context_maybeSet (FLG_DECLPARAMMATCH);

	  if (nparams != uentryList_size (newParams))
	    {
	      nargsError (old, unew);
	    }
	  
	  if (uentryList_size (newParams) < nparams) 
	    {
	      nparams = uentryList_size (newParams);
	    }

	  while (paramno < nparams)
	    {
	      uentry oldCurrent = uentryList_getN (oldParams, paramno);
	      uentry newCurrent  = uentryList_getN (newParams, paramno);
	      ctype  oldCurrentType = uentry_getType (oldCurrent);
	      ctype  newCurrentType = uentry_getType (newCurrent);

	      llassert (uentry_isValid (oldCurrent)
			&& uentry_isValid (newCurrent));
	      
	      if (!uentry_isElipsisMarker (oldCurrent)
		  && !uentry_isElipsisMarker (newCurrent))
		{
		  checkVarConformance (oldCurrent, newCurrent, 
				       mustConform, completeConform);
		}

	      if (checknames)
		{
		  if (uentry_hasName (oldCurrent) 
		      && uentry_hasName (newCurrent))
		    {
		      cstring oldname = uentry_getName (oldCurrent);
		      cstring pfx = context_getString (FLG_DECLPARAMPREFIX);
		      cstring oname;
		      cstring nname = uentry_getName (newCurrent);
		      cstring nnamefix;

		      if (cstring_isDefined (pfx)
			  && cstring_equalPrefix (oldname, cstring_toCharsSafe (pfx)))
			{
			  oname = cstring_suffix (oldname, cstring_length (pfx));
			}
		      else
			{
			  oname = oldname;
			/*@-branchstate@*/ } /*@=branchstate@*/

		      if (cstring_isDefined (pfx)
			  && cstring_equalPrefix (nname, cstring_toCharsSafe (pfx)))
			{
			  nnamefix = cstring_suffix (nname, cstring_length (pfx));
			}
		      else
			{
			  nnamefix = nname;
			/*@-branchstate@*/ } /*@=branchstate@*/

		      if (!cstring_equal (oname, nnamefix))
			{
			  if (optgenerror
			      (FLG_DECLPARAMMATCH, 
			       message ("Definition parameter name %s does not match "
					"name of corresponding parameter in "
					"declaration: %s",
					nnamefix, oname),
			       uentry_whereLast (newCurrent)))
			    {
			      uentry_showWhereLastPlain (oldCurrent);
			    }
			}
		      
		      cstring_free (oldname);
		      cstring_free (nname);
		    }
		}

	      if (!ctype_match (oldCurrentType, newCurrentType))
		{
		  paramTypeError (old, oldCurrent, oldCurrentType,
				  unew, newCurrent, newCurrentType, paramno);
		}
	      else
		{
		  if (ctype_isMissingParamsMarker (newCurrentType)
		      || ctype_isElips (newCurrentType)
		      || ctype_isMissingParamsMarker (oldCurrentType)
		      || ctype_isElips (oldCurrentType))
		    {
		      ;
		    }
		  else
		    {
		      if (ctype_isConj (newCurrentType))
			{
			  if (ctype_isConj (oldCurrentType))
			    {
			      if (!ctype_sameAltTypes (newCurrentType, oldCurrentType))
				{
				  if (optgenerror 
				      (FLG_INCONDEFS,
				       message ("Parameter %q inconsistently %rdeclared with "
						"alternate types %s "
						"(types match, but alternates are not identical, "
						"so checking may not be correct)",
						uentry_getName (newCurrent),
						uentry_isDeclared (oldCurrent),
						ctype_unparse (newCurrentType)),
				       uentry_whereDeclared (unew)))
				    {
				      uentry_showWhereLastVal (oldCurrent,
							       ctype_unparse (oldCurrentType));
				    }
				}
			    }
			  else
			    {
			      if (optgenerror 
				  (FLG_INCONDEFS,
				   message ("Parameter %q inconsistently %rdeclared with "
					    "alternate types %s",
					    uentry_getName (newCurrent),
					    uentry_isDeclared (oldCurrent),
					    ctype_unparse (newCurrentType)),
				   uentry_whereDeclared (unew)))
				{
				  uentry_showWhereLastVal (oldCurrent,
							   ctype_unparse (oldCurrentType));
				  
				}
			    }
			}
		      else 
			{
			  if (ctype_isConj (oldCurrentType))
			    {
			      uentry_setType (newCurrent, oldCurrentType);
			    }
			}
		    }
		}

	      paramno++;  
	      /*
	       ** Forgot this!  detected by lclint:
	       ** uentry.c:1257,15: Suspected infinite loop
	       */
	    }
	}
    }

  if (!uentryList_isMissingParams (newParams))
    {
      if (ctype_isConj (oldRetType))
	{
	  old->utype = ctype_makeFunction (oldRetType, 
					   uentryList_copy (newParams));
	}
      else
	{
	  old->utype = unew->utype;
	}
    }

  checkGlobalsConformance (old, unew, mustConform, completeConform);
  checkModifiesConformance (old, unew, mustConform, completeConform);

  if (unew->info->fcn->specclauses != NULL)
    {
      if (old->info->fcn->specclauses == NULL)
	{
	  if (optgenerror
	      (FLG_INCONDEFS,
	       message ("Function %q redeclared using special clauses (can only "
			"be used in first declaration)",
			uentry_getName (unew)),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereLast (old);
	    }
	}
      else
	{
	  specialClauses_checkEqual (old, unew);
	}
    }

  if (fileloc_isUndefined (old->whereDeclared))
    {
      old->whereDeclared = fileloc_copy (unew->whereDeclared);
    }
  else if (fileloc_isUndefined (unew->whereDeclared))
    {
      unew->whereDeclared = fileloc_copy (old->whereDeclared);
    }
  else
    {
      /* no change */
    }
}

void
uentry_mergeConstantValue (uentry ue, /*@only@*/ multiVal m)
{
  multiVal uval;

  llassert (uentry_isValid (ue));
  llassert (uentry_isEitherConstant (ue));

  uval = ue->info->uconst->val;

  if (multiVal_isDefined (uval))
    {
      if (multiVal_isDefined (m))
	{
	  if (!multiVal_equiv (uval, m))
	    {
	      if (optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q defined with inconsistent value: %q",
			    ekind_capName (ue->ukind),
			    uentry_getName (ue), 
			    multiVal_unparse (m)),
		   g_currentloc))
		{
		  uentry_showWhereLastExtra (ue, multiVal_unparse (uval));
		}
	    }
	}
      multiVal_free (m);
    }
  else
    {
      ue->info->uconst->val = m;
      multiVal_free (uval);
    }
}

static
bool checkTypeConformance (/*@notnull@*/ uentry old, /*@notnull@*/ uentry unew, 
			   bool mustConform)
{
  bool typeError = FALSE;

  if (uentry_isStructTag (old) || uentry_isUnionTag (old))
    {
      if (ctype_isSU (old->utype) && ctype_isSU (unew->utype))
	{
	  if (mustConform)
	    {
	      checkStructConformance (old, unew); 
	    }
	}
      else
	{
	  if (!(ctype_isBogus (old->utype) || ctype_isBogus (unew->utype)))
	    {
	      llbug (message ("struct tags: bad types: %t / %t", 
			      old->utype, unew->utype));
	    }
	}
    }
  else if (uentry_isEnumTag (old))
    {
      if (ctype_isEnum (old->utype) && ctype_isEnum (unew->utype))
	{
	  if (mustConform) checkEnumConformance (old, unew);
	}
      else 
	{
	  if (!(ctype_isBogus (old->utype) || ctype_isBogus (unew->utype)))
	    {
	      llbug (message ("enum! bad type: %s / %s", ctype_unparse (old->utype),
			      ctype_unparse (unew->utype)));
	    }
	}
    }
  else if (!ctype_match (old->utype, unew->utype))
    {
      if (cstring_equal (uentry_rawName (old), context_getBoolName ()))
	{
	  ctype realt = ctype_realType (unew->utype);
	  
	  if (ctype_isRealInt (realt) || ctype_isChar (realt))
	    {
	      unew->utype = ctype_bool;
	    }
	  else
	    {
	      if (mustConform)
		{
		  typeError = optgenerror
		    (FLG_INCONDEFS,
		     message ("%q defined as %s", uentry_getName (old), 
			      ctype_unparse (realt)),
		     uentry_whereDeclared (unew));
		}
	    }
	} 
      else 
	{
	  if (mustConform)
	    {
	      ctype oldr = ctype_realType (old->utype);
	      ctype newr = ctype_realType (unew->utype);
	      
	      if (ctype_isStruct (oldr) && ctype_isStruct (newr))
		{
		  checkStructConformance (old, unew);
		}
	      else if (ctype_isUnion (oldr) && ctype_isUnion (newr))
		{
		  checkStructConformance (old, unew);
		}
	      else if (ctype_isEnum (oldr) && ctype_isEnum (newr))
		{
		  checkEnumConformance (old, unew);
		}
	      else if (uentry_isConstant (old) 
		       && (ctype_isAbstract (oldr) && ctype_isEnum (newr)))
		{
		  /* okay...for now! (should check the type is reset later... */
		}
	      else
		{
		  if (optgenerror 
		      (FLG_INCONDEFS,
		       message ("%s %q %rdeclared with inconsistent type: %t",
				ekind_capName (unew->ukind),
				uentry_getName (unew), 
				uentry_isDeclared (old),
				unew->utype),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereLast (old);
		      typeError = TRUE;
		    }
		}
	    }
	}
    }
  else
    {
      /* no error */
    }

  return typeError;
}

static void
uentry_checkDatatypeConformance (/*@notnull@*/ uentry old,
				 /*@notnull@*/ uentry unew,
				 bool mustConform, bool completeConform)
{
  if (ctype_isDefined (unew->info->datatype->type))
    {
      if (ctype_isDirectBool (old->utype) &&
	  cstring_equalLit (unew->uname, "bool"))
	{
	  if (!context_getFlag (FLG_ABSTRACTBOOL))
	    {
	      unew->utype = ctype_bool;
	    }
	}
      
      if (ctype_isUnknown (old->info->datatype->type))
	{
	  old->info->datatype->type = unew->info->datatype->type;
	}
      else
	{
	  if (ctype_matchDef (old->info->datatype->type,
			   unew->info->datatype->type))
	    {
	      ;
	    }
	  else
	    {
	      if (optgenerror 
		  (FLG_INCONDEFS,
		   message
		   ("Type %q %s with inconsistent type: %t",
		    uentry_getName (unew), 
		    uentry_reDefDecl (old, unew),
		    unew->info->datatype->type),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastExtra 
		    (old, cstring_copy (ctype_unparse (old->info->datatype->type)));
		}

	      old->info->datatype->type = unew->info->datatype->type;
	    }
	}
    }
  
  if (unew->info->datatype->abs != MAYBE)
    {
      if (ynm_isOff (old->info->datatype->abs)
	  && ynm_isOn (unew->info->datatype->abs))
	{
	  if (!ctype_isDirectBool (old->utype))
	    {
	      if (optgenerror 
		  (FLG_INCONDEFS,
		   message 
		   ("Datatype %q inconsistently %rdeclared as abstract type",
		    uentry_getName (unew), 
		    uentry_isDeclared (old)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastPlain (old);
		}
	    }
	}
      else if (ynm_isOn (old->info->datatype->abs)
	       && ynm_isOff (unew->info->datatype->abs))
	{
	  if (!ctype_isDirectBool (old->utype))
	    {
	      if (optgenerror 
		  (FLG_INCONDEFS,
		   message 
		   ("Datatype %q inconsistently %rdeclared as concrete type",
		    uentry_getName (unew), 
		    uentry_isDeclared (old)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastPlain (old);
		}
	    }
	}
      else
	{
	  ;
	}
    }
  else 
    {
      if (ynm_isOn (old->info->datatype->abs))
	{
	  old->sref = unew->sref;
	  unew->info->datatype->mut = old->info->datatype->mut;
	  
	  if (completeConform
	      && uentry_isReallySpecified (old))
	    {
	      if (optgenerror 
		  (FLG_NEEDSPEC,
		   message 
		   ("Datatype %q specified as abstract, "
		    "but abstract annotation not used in declaration",
		    uentry_getName (unew)), 
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastPlain (old);
		}
	    }
	}
    }
  
  unew->info->datatype->abs = old->info->datatype->abs;	  
  
  if (ynm_isMaybe (unew->info->datatype->mut))
    {
      if (completeConform && ynm_isOff (old->info->datatype->mut)
	  && uentry_isReallySpecified (old))
	{
	  if (optgenerror 
	      (FLG_NEEDSPEC,
	       message 
	       ("Datatype %q specified as immutable, "
		"but immutable annotation not used in declaration",
		uentry_getName (unew)), 
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereLastPlain (old);
	    }
	}
      
      unew->info->datatype->mut = old->info->datatype->mut;
    }
  else if (ynm_isMaybe (old->info->datatype->mut))
    {
      old->info->datatype->mut = unew->info->datatype->mut;
    }
  else
    {
      if (ynm_isOn (old->info->datatype->abs))
	{
	  if (ynm_isOn (old->info->datatype->mut) && ynm_isOff (unew->info->datatype->mut))
	    {
	      if (optgenerror
		  (FLG_INCONDEFS,
		   message ("Datatype %q inconsistently %rdeclared as immutable",
			    uentry_getName (unew), 
			    uentry_isDeclared (old)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastPlain (old);
		}
	    }
	  else 
	    {
	      if (ynm_isOff (old->info->datatype->mut)
		  && ynm_isOn (unew->info->datatype->mut))
		{
		  if (optgenerror
		      (FLG_INCONDEFS,
		       message ("Datatype %q inconsistently %rdeclared as mutable",
				uentry_getName (unew), 
				uentry_isDeclared (old)),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereLastPlain (old);
		    }
		}
	    }
	}
      old->info->datatype->mut = unew->info->datatype->mut;	  
    }

  uentry_checkStateConformance (old, unew, mustConform, completeConform);
}

static void
uentry_checkConstantConformance (/*@notnull@*/ uentry old,
				 /*@notnull@*/ uentry unew,
				 bool mustConform, 
				 /*@unused@*/ bool completeConform)
{
  multiVal oldVal = old->info->uconst->val;
  multiVal newVal = unew->info->uconst->val;
  
  if (multiVal_isDefined (oldVal))
    {
      if (multiVal_isDefined (newVal))
	{
	  if (!multiVal_equiv (oldVal, newVal))
	    {
	      if (mustConform
		  && optgenerror 
		  (FLG_INCONDEFS,
		   message ("%s %q %rdeclared with inconsistent value: %q",
			    ekind_capName (unew->ukind),
			    uentry_getName (unew), 
			    uentry_isDeclared (old),
			    multiVal_unparse (newVal)),
		   uentry_whereDeclared (unew)))
		{
		  uentry_showWhereLastExtra (old, multiVal_unparse (oldVal));
		}
	    }
	  
	  unew->info->uconst->val = multiVal_copy (oldVal);
	  multiVal_free (newVal);
	}
      else
	{
	  ;
	}
    }
  else
    {
      old->info->uconst->val = multiVal_copy (newVal);
    }
}

static void 
uentry_checkConformance (/*@unique@*/ /*@notnull@*/ uentry old, 
			 /*@notnull@*/ uentry unew, bool mustConform,
			 bool completeConform)
{
  bool typeError = FALSE;
  bool fcnConformance = FALSE;

  if (!ekind_equal (unew->ukind, old->ukind))
    {
      /*
      ** okay, only if one is a function and the other is
      ** a variable of type function.
      */

      if (unew->ukind == KENUMCONST
	  && old->ukind == KCONST)
	{
	  old->ukind = KENUMCONST;
	  goto nokinderror;
	}

      if (unew->ukind == KFCN 
	  && old->ukind == KCONST
	  && ctype_isUnknown (old->utype))
	{
	  /*
	  ** When a function is defined with an unparam macro
	  */

	  uentry_copyInto (old, unew);
	  return;
	}

      if (uentry_isExpandedMacro (old) 
	  && uentry_isEitherConstant (unew))
	{
	  uentry_copyInto (old, unew);
	  return;
	}

      if (uentry_isEndIter (unew))
	{
	  if (ctype_isUnknown (old->utype))
	    {
	      if (!uentry_isSpecified (old)
		  && uentry_isCodeDefined (unew))
		{
		  if (!fileloc_withinLines (uentry_whereDefined (old),
					    uentry_whereDeclared (unew), 2))
		    { /* bogus!  will give errors if there is too much whitespace */
		      voptgenerror
			(FLG_SYNTAX,
			 message
			 ("Iterator finalized name %q does not match name in "
			  "previous iter declaration (should be end_%q).  This iter "
			  "is declared at %q", 
			  uentry_getName (unew),
			  uentry_getName (old),
			  fileloc_unparse (uentry_whereDefined (old))),
			 uentry_whereDeclared (old));
		    }
		}

	      uentry_copyInto (old, unew);
	      return;
	    }
	  else
	    {
	      KindConformanceError (old, unew, mustConform);
	    }
	}

      if (uentry_isFunction (unew))
	{
	  if (uentry_isVariable (old))
	    {
	      if (!ctype_isUnknown (old->utype))
		{
		  if (ctype_isFunction (old->utype))
		    {
		      uentry_makeVarFunction (old);
		      checkFunctionConformance (old, unew, mustConform,
						completeConform);
		      fcnConformance = TRUE;
		    }
		  else
		    {
		      KindConformanceError (old, unew, mustConform);
		    }
		}
	      else
		{
		  if (uentry_isExpandedMacro (old))
		    {
		      if (fileloc_isUndefined (unew->whereDefined))
			{
			  unew->whereDefined = fileloc_update (unew->whereDefined, 
							      old->whereDefined);
			}

		      uentry_copyInto (old, unew);
		      old->used = unew->used = TRUE;
		      return;
		    }
		  else
		    {
		      /* undeclared identifier */
		      old->utype = unew->utype;
		      uentry_makeVarFunction (old);
		      checkFunctionConformance (old, unew, FALSE, FALSE);
		      fcnConformance = TRUE;
		    }
		}
	    }
	  else
	    {
	      KindConformanceError (old, unew, mustConform);
	    }
	}
      else if (uentry_isFunction (old) && uentry_isVariable (unew))
	{
	  if (!ctype_isUnknown (unew->utype))
	    {
	      if (ctype_isFunction (unew->utype))
		{
		  uentry_makeVarFunction (unew);
		  checkFunctionConformance (old, unew, mustConform, completeConform);
		  fcnConformance = TRUE;
		}
	      else
		{
		  KindConformanceError (old, unew, mustConform);
		}
	    }
	  else
	    {
	      KindConformanceError (old, unew, mustConform);
	    }
	}
      else
	{
	  KindConformanceError (old, unew, mustConform);
	}
    }
  else
    {
      /*
      ** check parameter lists for functions 
      ** (before type errors, to get better messages
      */

      if (uentry_isFunction (old))
	{
	  checkFunctionConformance (old, unew, mustConform, completeConform);
	  fcnConformance = TRUE;
	}
      else 
	{
	  if (!ctype_isUndefined (old->utype))
	    {
	      typeError = checkTypeConformance (old, unew, mustConform);
	    }
	}
    }

 nokinderror:

  if (uentry_isEitherConstant (old) && uentry_isEitherConstant (unew))
    {
      uentry_checkConstantConformance (old, unew, mustConform, completeConform);
    }

  if (uentry_isDatatype (old) && uentry_isDatatype (unew))
    {
      uentry_checkDatatypeConformance (old, unew, mustConform, completeConform);
    }

  if (uentry_isVariable (old) && uentry_isVariable (unew))
    {
      if (!typeError && 
	  !ctype_matchDef (old->utype, unew->utype))
	{
	  if (optgenerror 
	      (FLG_INCONDEFS,
	       message
	       ("Variable %q %s with inconsistent type (arrays and pointers are "
		"not identical in variable declarations): %t",
		uentry_getName (unew), 
		uentry_reDefDecl (old, unew),
		unew->utype),
	       uentry_whereDeclared (unew)))
	    {
	      uentry_showWhereLast (old);
	      
	      /*
	      ** Avoid repeated errors.
	      */

	      if (uentry_isCodeDefined (old) && uentry_isCodeDefined (unew))
		{
		  old->whereDefined = fileloc_update (old->whereDefined,
						      fileloc_undefined);
		}

	      typeError = TRUE;
	    }
	}

      checkVarConformance (old, unew, mustConform, completeConform);
    }

  if (fcnConformance)
    {
      /* old->utype = unew->utype; */
    }
  else
    {
      if (ctype_isConj (old->utype))
	{
	  if (ctype_isConj (unew->utype))
	    {
	      if (!ctype_sameAltTypes (old->utype, unew->utype))
		{
		  if (optgenerror 
		      (FLG_INCONDEFS,
		       message ("%s %q inconsistently %rdeclared with "
				"alternate types %s "
				"(types match, but alternates are not identical, "
				"so checking may not be correct)",
				ekind_capName (uentry_getKind (old)),
				uentry_getName (unew),
				uentry_isDeclared (old),
				ctype_unparse (unew->utype)),
		       uentry_whereDeclared (unew)))
		    {
		      uentry_showWhereLastVal (old, ctype_unparse (old->utype));
		    }
		  else
		    {
		      old->utype = unew->utype;
		    }
		}
	    }
	}
      else
	{
	  if (ctype_isUnknown (old->utype))
	    {
	      old->utype = unew->utype;
	    }
	}
    }  

  if (unew->ukind == old->ukind) 
    {
      sfree (unew->info);
      unew->info = uinfo_copy (old->info, old->ukind);
    }

  sRef_storeState (old->sref);
  sRef_storeState (unew->sref);
}

/*
** modifies spec to reflect def, reports any inconsistencies
*/

void
uentry_mergeEntries (uentry spec, /*@only@*/ uentry def)
{
  llassert (uentry_isValid (spec));
  llassert (uentry_isValid (def));
  llassert (cstring_equal (spec->uname, def->uname));

  uentry_checkConformance (spec, def, TRUE, 
			   context_getFlag (FLG_NEEDSPEC));

  /* was: !(fileloc_isImport (uentry_whereSpecified (spec)))); */

  /*
  ** okay, declarations conform.  Propagate extra information.
  */

  uentry_setDefined (spec, uentry_whereDefined (def));
  uentry_setDeclared (spec, uentry_whereDeclared (def));

  if (uentry_isStatic (def))
    {
      if (optgenerror 
	  (FLG_INCONDEFS,
	   message ("%s %q specified, but declared as static",
		    ekind_capName (def->ukind),
		    uentry_getName (def)),
	   uentry_whereDeclared (def)))
	{
	  uentry_showWhereSpecified (spec);
	}
    }
  else 
    {
      spec->storageclass = def->storageclass;
    }

  sRef_storeState (spec->sref);

  spec->used = def->used || spec->used;
  spec->hasNameError |= def->hasNameError;

  uentry_free (def);

  if (!spec->hasNameError)
    {
      uentry_checkName (spec);
    }
  else
    {
      ;
    }
}

/*
** Can't generate function redeclaration errors when the 
** entries are merged, since we don't yet know if its the
** definition of the function.
*/

void
uentry_clearDecl (void)
{
  posRedeclared = uentry_undefined;
  fileloc_free (posLoc);
  posLoc = fileloc_undefined;
}

void
uentry_checkDecl (void)
{
  if (uentry_isValid (posRedeclared))
    {
      llassert (fileloc_isDefined (posLoc));

      if (uentry_isCodeDefined (posRedeclared))
	{
	  if (optgenerror (FLG_REDECL,
			   message ("%s %q declared after definition", 
				    ekind_capName (posRedeclared->ukind),
				    uentry_getName (posRedeclared)),
			   posLoc))
	    {
	      llgenindentmsg (message ("Definition of %q", 
				       uentry_getName (posRedeclared)),
			      posRedeclared->whereDeclared);
	    }
	}
      else
	{
	  if (optgenerror (FLG_REDECL,
			   message ("%s %q declared more than once", 
				    ekind_capName (posRedeclared->ukind),
				    uentry_getName (posRedeclared)),
			   posLoc))
	    {
	      llgenindentmsg (message ("Previous declaration of %q", 
				       uentry_getName (posRedeclared)),
			      posRedeclared->whereDeclared);
	    }
	}
    }

  fileloc_free (posLoc);
  posLoc = fileloc_undefined;
  posRedeclared = uentry_undefined;
}

/*
** Redefinition of old as unew.
** modifies old to reflect unew, reports any inconsistencies
*/

void
uentry_mergeDefinition (uentry old, /*@only@*/ uentry unew)
{
  fileloc olddef = uentry_whereDeclared (old);
  fileloc unewdef = uentry_whereDeclared (unew);
  bool mustConform;
  bool wasForward;
 
  if (uentry_isExtern (unew))
    {
      uentry_setUsed (old, unewdef);
    }

  wasForward = 
    fileloc_isUndefined (olddef) 
      && fileloc_isDefined (uentry_whereDefined (old)) 
	&& !uentry_isExpandedMacro (old);
  
  if (!context_getFlag (FLG_INCONDEFSLIB)
      && (fileloc_isLib (olddef) || fileloc_isImport (olddef)))
    {
      mustConform = FALSE;
    }
  else
    {
      mustConform = TRUE;
    }
  
  llassert (uentry_isValid (old));
  llassert (uentry_isValid (unew));
  llassert (cstring_equal (old->uname, unew->uname));

  /*
  ** should check old one was extern!
  */

  if (uentry_isStatic (old))
    {
      if (!(uentry_isStatic (unew)))
	{
	  if (optgenerror 
	      (FLG_SHADOW,
	       message ("%s %q shadows static declaration",
			ekind_capName (unew->ukind),
			uentry_getName (unew)),
	       unewdef))
	    {
	      uentry_showWhereLast (old);
	    }
	}
      else
	{
	  uentry_setDeclDef (old, unewdef);
	}
    }
  else if (uentry_isStatic (unew))
    {
      uentry_setDeclDef (old, unewdef);
    }
  else if (uentry_isExtern (old))
    {
      uentry_setDeclared (old, unewdef);
    }
  else
    {
      if (!uentry_isExtern (unew) && !uentry_isForward (old)
	  && !fileloc_equal (olddef, unewdef)
	  && !fileloc_isUndefined (olddef)
	  && !fileloc_isUndefined (unewdef)
	  && !fileloc_isBuiltin (olddef)
	  && !fileloc_isBuiltin (unewdef)
	  && !uentry_isYield (old)
	  && !(fileloc_isLib (olddef) || fileloc_isImport (olddef)))
	{
	  if (uentry_isVariable (old) || uentry_isVariable (unew))
	    {
	      ; /* will report redeclaration error later */
	    }
	  else
	    {
	      if (fileloc_isDefined (uentry_whereDefined (old)))
		{
		  if (optgenerror
		      (FLG_REDEF,
		       message ("%s %q defined more than once", 
				ekind_capName (unew->ukind),
				uentry_getName (unew)),
		       uentry_whereLast (unew)))
		    {
		      llgenindentmsg
			(message ("Previous definition of %q", 
				  uentry_getName (old)),
			 uentry_whereLast (old));
		    }
		  /*
		  if (uentry_isDatatype (old) || uentry_isAnyTag (old))
		    {
		      uentry_copyInto (old, unew);
		      old->sref = sRef_saveCopy (old->sref);
		    }
		    */
		}
	    }
	}
      else
	{
	  if (fileloc_isLib (olddef)
	      || fileloc_isUndefined (olddef)
	      || fileloc_isImport (olddef))
	    {
	      if (uentry_isExtern (unew)) 
		{
		  if (uentry_isExtern (old)
		      || (fileloc_isDefined (uentry_whereDeclared (old))
			  && (!fileloc_equal (uentry_whereDeclared (old),
					      uentry_whereDefined (old)))))
		    {
		      if (optgenerror
			  (FLG_REDECL,
			   message ("%s %q declared more than once", 
				    ekind_capName (unew->ukind),
				    uentry_getName (unew)),
			   unew->whereDeclared))
			{
			  llgenindentmsg
			    (message ("Previous declaration of %q", 
				      uentry_getName (old)),
			     old->whereDeclared);
			}
		    }
		  
		  uentry_setExtern (old);
		}
	      else
		{
		  uentry_setDefined (old, unewdef);
		}
	    }
	}
    }

  uentry_checkConformance (old, unew, mustConform, FALSE);

  old->used = old->used || unew->used;
  old->uses = filelocList_append (old->uses, unew->uses);
  unew->uses = filelocList_undefined; 

  sRef_storeState (old->sref); 
  sRef_storeState (unew->sref);

  if (wasForward)
    {
      old->whereDefined = fileloc_update (old->whereDefined,
					  fileloc_undefined);
    }

  /*
  ** No redeclaration errors for functions here, since we
  ** don't know if this is the definition of the function.
  */

  if (fileloc_isUser (old->whereDeclared)
      && fileloc_isUser (unew->whereDeclared)
      && !fileloc_equal (old->whereDeclared, unew->whereDeclared)
      && !fileloc_isDefined (unew->whereDefined))
    {
      if (uentry_isFunction (old))
	{
	  /*@-temptrans@*/ posRedeclared = old; /*@=temptrans@*/
	  posLoc = fileloc_update (posLoc, unew->whereDeclared);
	}
      else
	{
	  if (optgenerror (FLG_REDECL,
			   message ("%s %q declared more than once", 
				    ekind_capName (unew->ukind),
				    uentry_getName (unew)),
			   unew->whereDeclared))
	    {
	      llgenindentmsg (message ("Previous declaration of %q", 
				       uentry_getName (old)),
			      old->whereDeclared);
	    }
	}
    }

  if (fileloc_isUndefined (old->whereDefined))
    {
      old->whereDefined = fileloc_update (old->whereDefined, unew->whereDefined);
    }
  else
    {
      if (!context_processingMacros ()
	  && fileloc_isUser (old->whereDefined) 
	  && fileloc_isUser (unew->whereDefined)
	  && !fileloc_equal (old->whereDefined, unew->whereDefined))
	{
	  if (uentry_isVariable (unew) || uentry_isFunction (unew))
	    {
	      if (uentry_isVariable (unew) 
		  && uentry_isExtern (unew))
		{
		  if (optgenerror (FLG_REDECL,
				   message ("%s %q declared after definition", 
					    ekind_capName (unew->ukind),
					    uentry_getName (unew)),
				   unew->whereDeclared))
		    {
		      llgenindentmsg (message ("Definition of %q", 
					       uentry_getName (old)),
				      old->whereDefined);
		    }
		}
	      else
		{
		  if (optgenerror (FLG_REDEF,
				   message ("%s %q redefined", 
					    ekind_capName (unew->ukind),
					    uentry_getName (unew)),
				   unew->whereDefined))
		    {
		      llgenindentmsg (message ("Previous definition of %q", 
					       uentry_getName (old)),
				      old->whereDefined);
		    }
		}
	    }
	}
    }

  if (uentry_isExternal (unew))
    {
      old->whereDefined = fileloc_createExternal ();
    }

  if (unew->hasNameError)
    {
      old->hasNameError = TRUE;
    }

  uentry_free (unew);

  if (!old->hasNameError)
    {
      uentry_checkName (old);
    }

  llassert (!ctype_isUndefined (old->utype));
}

void
uentry_copyState (uentry res, uentry other)
{
  llassert (uentry_isValid (res));
  llassert (uentry_isValid (other));

  res->used = other->used;

  res->info->var->kind = other->info->var->kind;
  res->info->var->defstate = other->info->var->defstate;
  res->info->var->nullstate = other->info->var->nullstate;
  res->info->var->checked = other->info->var->checked;

  sRef_copyState (res->sref, other->sref);
}

bool
uentry_sameKind (uentry u1, uentry u2)
{
  if (uentry_isValid (u1) && uentry_isValid (u2))
    {
      if (uentry_isVar (u1) && uentry_isVar (u2))
	{
	  ctype c1 = u1->utype;
	  ctype c2 = u2->utype;

	  if (ctype_isUnknown (c1) || ctype_isUnknown (c2)) return FALSE;

	  /*
	  ** both functions, or both not functions
	  */

	  return (bool_equal (ctype_isFunction (c1), ctype_isFunction (c2)));
	}
      else
	{
	  return ((u1->ukind == u2->ukind));
	}
    }
  
  return FALSE;
}
   
static void uentry_copyInto (/*@unique@*/ uentry unew, uentry old)
{
  llassert (uentry_isValid (unew));
  llassert (uentry_isValid (old));

  unew->ukind = old->ukind;
  unew->uname = cstring_copy (old->uname);
  unew->utype = old->utype;

  unew->whereSpecified = fileloc_copy (old->whereSpecified);
  unew->whereDefined = fileloc_copy (old->whereDefined);
  unew->whereDeclared = fileloc_copy (old->whereDeclared);

  unew->sref = sRef_saveCopy (old->sref); /* Memory leak! */
  unew->used = old->used;
  unew->lset = FALSE;
  unew->isPrivate = old->isPrivate;
  unew->hasNameError = old->hasNameError;
  unew->uses = filelocList_undefined;

  unew->storageclass = old->storageclass;
  unew->info = uinfo_copy (old->info, old->ukind);
}


uentry
uentry_copy (uentry e)
{
  if (uentry_isValid (e))
    {
      uentry enew = uentry_alloc ();
      DPRINTF (("copy: %s", uentry_unparseFull (e)));
      uentry_copyInto (enew, e);
      DPRINTF (("Here we are..."));
      DPRINTF (("original: %s", uentry_unparseFull (e)));
      DPRINTF (("copy: %s", uentry_unparse (enew)));
      DPRINTF (("copy: %s", uentry_unparseFull (enew)));
      return enew;
    }
  else
    {
      return uentry_undefined;
    }
}

void
uentry_setState (uentry res, uentry other)
{
  llassert (uentry_isValid (res));
  llassert (uentry_isValid (other));

  llassert (res->ukind == other->ukind);
  llassert (res->ukind == KVAR);

  res->sref = sRef_saveCopy (other->sref);
  res->used = other->used;
  filelocList_free (res->uses); 
  res->uses = other->uses; 
  other->uses = filelocList_undefined; 
  res->lset = other->lset;
}

void
uentry_mergeUses (uentry res, uentry other)
{
  llassert (uentry_isValid (res));
  llassert (uentry_isValid (other));

  res->used = other->used || res->used;
  res->lset = other->lset || res->lset;
  res->uses = filelocList_append (res->uses, other->uses);
  other->uses = filelocList_undefined;
}


/*
** This is a really ugly routine.
**
** gack...fix this one day.
*/

/*
** flip == TRUE
**   >> res is the false branch, other is the true branch (or continuation)
** flip == FALSE
**   >> res is the true branch, other is the false branch (or continutation)
**
** opt == TRUE if,
**
** <other>
** if <res> ;
**
** References not effected by res are propagated from other.
*/

static void
  branchStateError (/*@notnull@*/ uentry res, /*@notnull@*/ uentry other, 
		    bool flip, clause cl, fileloc loc)
{
  if (optgenerror 
      (FLG_BRANCHSTATE,
       message ("%s %q is %s %s, but %s %s.",
		ekind_capName (res->ukind), uentry_getName (res),
		sRef_stateVerb (res->sref), clause_nameFlip (cl, flip),
		sRef_stateAltVerb (res->sref), clause_nameFlip (cl, !flip)),
       loc))
    {
      if (sRef_isDead (res->sref))
	{
	  sRef_showStateInfo (res->sref);
	}
      else if (sRef_isKept (res->sref))
	{
	  sRef_showAliasInfo (res->sref);
	}
      else /* dependent */
	{
	  sRef_showAliasInfo (res->sref);
	  sRef_showAliasInfo (other->sref);
	}
      
      sRef_setAliasKind (res->sref, AK_ERROR, fileloc_undefined);
    }
}

static bool incompatibleStates (sRef rs, sRef os)
{
  alkind rk = sRef_getAliasKind (rs);
  alkind ok = sRef_getAliasKind (os);

  if (alkind_isError (rk) || alkind_isError (ok))
    {
      return FALSE;
    }
  else
    {
      return ((sRef_isDead (rs)
	       || (alkind_isKept (rk) && !alkind_isKept (ok))
	       || (alkind_isDependent (rk) 
		   && !alkind_isDependent (ok) && !alkind_isTemp (ok)))
	      && (sRef_isAllocated (os) || sRef_isStateDefined (os)));
    }
}

static void
  branchStateAltError (/*@notnull@*/ uentry res,
		       /*@notnull@*/ uentry other, bool flip,
		       clause cl, fileloc loc)
{
  if (optgenerror 
      (FLG_BRANCHSTATE,
       message ("%s %q is %s %s, but %s %s.",
		ekind_capName (res->ukind), uentry_getName (res),
		sRef_stateVerb (other->sref), clause_nameFlip (cl, flip),
		sRef_stateAltVerb (other->sref), clause_nameFlip (cl, !flip)),
       loc))
    {
      if (sRef_isDead (other->sref))
	{
	  sRef_showStateInfo (other->sref);
	}
      else /* kept */
	{
	  sRef_showAliasInfo (other->sref);
	}
      
      sRef_setAliasKind (res->sref, AK_ERROR, fileloc_undefined);
      sRef_setDefinedComplete (res->sref, fileloc_undefined);
      
      sRef_setAliasKind (other->sref, AK_ERROR, fileloc_undefined);
      sRef_setDefinedComplete (other->sref, fileloc_undefined);
    }
}

static bool notNull (sRef sr, bool flip)
{
  return (!sRef_definitelyNull (sr)
	  && !(sRef_isKept (sr))
	  && !(sRef_isDependent (sr))
	  && !(flip ? usymtab_isProbableDeepNull (sr)
	       : usymtab_isAltProbablyDeepNull (sr)));
}
    
void
uentry_mergeState (uentry res, uentry other, fileloc loc,
		   bool mustReturn, bool flip, bool opt,
		   clause cl)
{
  llassert (uentry_isValid (res));
  llassert (uentry_isValid (other));

  llassert (res->ukind == other->ukind);
  llassert (res->ukind == KVAR);

  DPRINTF (("Merge state: %s / %s", 
	    uentry_unparse (res),
	    uentry_unparse (other)));

  if (sRef_isValid (res->sref))
    {
      if (!mustReturn)
	{
	  if (incompatibleStates (res->sref, other->sref))
	    {
	      if (sRef_isThroughArrayFetch (res->sref)
		  && !context_getFlag (FLG_STRICTBRANCHSTATE))
		{
		  if (sRef_isKept (res->sref) || sRef_isKept (other->sref))
		    {
		      sRef_maybeKill (res->sref, loc);
		    }
		  else if (sRef_isPossiblyDead (other->sref))
		    {
		      sRef_maybeKill (res->sref, loc);
		    }
		  else
		    {
		      ;
		    }
		}
	      else
		{
		  if (notNull (other->sref, flip))
		    {
		      if (sRef_isLocalParamVar (res->sref) 
			  && (sRef_isLocalState (other->sref) 
			      || sRef_isDependent (other->sref)))
			{
			  if (sRef_isDependent (res->sref))
			    {
			      sRef_setDependent (other->sref, loc);
			    }
			  else
			    {
			      sRef_setDefState (res->sref, SS_UNUSEABLE, loc);
			    }
			}
		      else 
			{
			  branchStateError (res, other, flip, cl, loc);
			}
		    }
		}
	      
	      if (sRef_isKept (res->sref))
		{
		  sRef_setKept (other->sref, loc);
		}
	    }
	  else
	    {
	      if (incompatibleStates (other->sref, res->sref))
		{
		  if (notNull (res->sref, !flip))
		    {
		      if (sRef_isLocalParamVar (res->sref) 
			  && (sRef_isDependent (res->sref)
			      || sRef_isLocalState (res->sref)))
			{
			  if (sRef_isDependent (other->sref))
			    {
			      sRef_setDependent (res->sref, loc);
			    }
			  else
			    {
			      sRef_setDefState (res->sref, SS_UNUSEABLE, loc);
			    }
			}
		      else
			{
			  if (sRef_isParam (other->sref))
			    {
			      /* 
		              ** If the local variable associated
			      ** with the param has the correct state,
			      ** its okay.
			      ** (e.g., free (s); s = new(); ...
			      */

			      uentry uvar = usymtab_lookupSafe (other->uname);

			      if (uentry_isValid (uvar)
				  && ((sRef_isDead (other->sref) 
				       && sRef_isOnly (uvar->sref))
				      || (sRef_isDependent (other->sref)
					  && sRef_isOwned (uvar->sref))))
				{
				  /* no error */
				}
			      else
				{
				  branchStateAltError (res, other,
						       flip, cl, loc);
				}
			    }
			  else
			    {
			      branchStateAltError (res, other, 
						   flip, cl, loc);
			    }
			}
		    }
		}

	      if (sRef_isKept (other->sref))
		{
		  sRef_setKept (res->sref, loc);
		}
	    }
	  
	  if (opt)
	    {
	      DPRINTF (("Merge opt..."));
	      sRef_mergeOptState (res->sref, other->sref, cl, loc);
	      DPRINTF (("Done!"));
	    }
	  else
	    {
	      sRef_mergeState (res->sref, other->sref, cl, loc);
	    }
	}
      else
	{
	  if (sRef_isModified (other->sref))
	    {
	      sRef_setModified (res->sref);
	    }
	}

      if (cl == DOWHILECLAUSE)
	{
	  res->used = other->used || res->used;
	  res->lset = other->lset || res->lset;
	  res->uses = filelocList_append (res->uses, other->uses);
	  other->uses = filelocList_undefined;
	}
      else
	{
	  if (sRef_isMacroParamRef (res->sref)
	      && !uentry_isSefParam (other)
	      && !uentry_isSefParam (res))
	    {
	      bool hasError = FALSE;
	      
	      if (bool_equal (res->used, other->used))
		{
		  res->used = other->used;
		}
	      else
		{
		  if (other->used && !flip)
		    {
		      hasError = 
			optgenerror 
			  (FLG_MACROPARAMS,
			   message ("Macro parameter %q used in true clause, "
				    "but not in false clause",
				    uentry_getName (res)),
			   uentry_whereDeclared (res));
		    }
		  else
		    {	
		      hasError = 
			optgenerror 
			  (FLG_MACROPARAMS,
			   message ("Macro parameter %q used in false clause, "
				    "but not in true clause",
				    uentry_getName (res)),
			   uentry_whereDeclared (res));
		    }
		  res->used = TRUE;
		  
		  if (hasError)
		    {
		      /* make it sef now, prevent more errors */
		      res->info->var->kind = VKREFSEFPARAM;
		    }
		}
	    }
	  else
	    {
	      res->used = other->used || res->used;
	      res->lset = other->lset || res->lset;
	      res->uses = filelocList_append (res->uses, other->uses);
	      other->uses = filelocList_undefined;
	    }
	}
    }
}

void uentry_setUsed (uentry e, fileloc loc)
{
  static bool firstTime = TRUE;
  static bool showUses = FALSE;
  static bool exportLocal = FALSE;

  if (firstTime)
    {
      /* need to track uses is FLG_SHOWUSES or FLG_EXPORTLOCAL is true */

      showUses = context_getFlag (FLG_SHOWUSES); 
      exportLocal = context_maybeSet (FLG_EXPORTLOCAL);

      firstTime = FALSE;
    }

  if (uentry_isValid (e))
    {
      int dp;
      
      if (sRef_isMacroParamRef (e->sref))
	{
	  if (uentry_isYield (e) || uentry_isSefParam (e))
	    {
	      ;
	    }
	  else 
	    {
	      if (context_inConditional ())
		{
		  if (optgenerror
		      (FLG_MACROPARAMS,
		       message ("Macro parameter %q used in conditionally "
				"executed code (may or may not be "
				"evaluated exactly once)", 
				uentry_getName (e)),
		       loc))
		    {
		      e->info->var->kind = VKREFSEFPARAM;
		    }
		}
	      else
		{
		  if ((e)->used)
		    {
		      if (optgenerror
			  (FLG_MACROPARAMS,
			   message ("Macro parameter %q used more than once", 
				    uentry_getName (e)),
			   uentry_whereDeclared (e)))
			{
			  e->info->var->kind = VKREFSEFPARAM;
			}
		    }
		}
	    }
	}
      
      if ((dp = uentry_directParamNo (e)) >= 0)
	{
	  uentry_setUsed (usymtab_getParam (dp), loc);
	}
      
      e->used = TRUE;
      
      if (!sRef_isLocalVar (e->sref))
	{
	  if (showUses)
	    {
	      e->uses = filelocList_add (e->uses, fileloc_copy (loc));
	    }
	  else 
	    {
	      if (exportLocal)
		{
		  if (context_inMacro ())
		    {
		      e->uses = filelocList_addUndefined (e->uses);
		    }
		  else 
		    {
		      e->uses = filelocList_addDifferentFile
			(e->uses, 
			 uentry_whereDeclared (e),
			 loc);
		    }
		}
	    }
	}
    }
}

bool uentry_isReturned (uentry u)
{
  return (uentry_isValid (u) && uentry_isVar (u) 
	  && (u->info->var->kind == VKRETPARAM
	      || u->info->var->kind == VKSEFRETPARAM));
}

/*@exposed@*/ sRef uentry_returnedRef (uentry u, exprNodeList args)
{
  llassert (uentry_isRealFunction (u));

  if (ctype_isFunction (u->utype)
      && sRef_isStateSpecial (uentry_getSref (u)))
    {
      specialClauses clauses = uentry_getSpecialClauses (u);
      sRef res = sRef_makeNew (ctype_returnValue (u->utype), u->sref, u->uname);

      sRef_setAllocated (res, g_currentloc);

      specialClauses_postElements (clauses, cl)
	{
	  sRefSet refs = specialClause_getRefs (cl);
	  sRefMod modf = specialClause_getEffectFunction (cl);

	  sRefSet_elements (refs, el)
	    {
	      sRef base = sRef_getRootBase (el);

	      if (sRef_isResult (base))
		{
		  if (modf != NULL)
		    {
		      sRef sr = sRef_fixBase (el, res);
		      modf (sr, g_currentloc);
		    }
		}
	      else
		{
		  ;
		}
	    } end_sRefSet_elements ;

	} end_specialClauses_postElements ;

      return res;
    }
  else
    {
      uentryList params;
      alkind ak;
      sRefSet prefs = sRefSet_new ();
      sRef res = sRef_undefined;
      int paramno = 0;
      
      params = uentry_getParams (u);
      
      uentryList_elements (params, current)
	{
	  if (uentry_isReturned (current))
	    {
	      if (exprNodeList_size (args) >= paramno)
		{
		  exprNode ecur = exprNodeList_nth (args, paramno);
		  sRef tref = exprNode_getSref (ecur);
		  
		  if (sRef_isValid (tref))
		    {
		      sRef tcref = sRef_copy (tref);
		      
		      if (sRef_isDead (tcref))
			{
			  sRef_setDefined (tcref, g_currentloc);
			  sRef_setOnly (tcref, g_currentloc);
			}
		      
		      if (sRef_isRefCounted (tcref))
			{
			  /* could be a new ref now (but only if its returned) */
			  sRef_setAliasKindComplete (tcref, AK_ERROR, g_currentloc);
			}
		      
		      sRef_makeSafe (tcref);
		      
		      prefs = sRefSet_insert (prefs, tcref);
		    }
		}
	    }
	  
	  paramno++;
	} end_uentryList_elements ;
      
      if (sRefSet_size (prefs) > 0)
	{
	  nstate n = sRef_getNullState (u->sref);
	  
	  if (sRefSet_size (prefs) == 1)
	    {
	      res = sRefSet_choose (prefs);
	    }
	  else
	    {
	      res = sRefSet_mergeIntoOne (prefs);
	    }
	  
	  if (nstate_isKnown (n))
	    {
	      sRef_setNullState (res, n, g_currentloc);
	    }
	}
      else
	{
	  if (ctype_isFunction (u->utype))
	    {
	      res = sRef_makeNew (ctype_returnValue (u->utype), u->sref, u->uname);
	    }
	  else
	    {
	      res = sRef_makeNew (ctype_unknown, u->sref, u->uname);
	    }
	  
	  if (sRef_isRefCounted (res))
	    {
	      sRef_setAliasKind (res, AK_NEWREF, g_currentloc);
	    }
	}
      
      if (sRef_getNullState (res) == NS_ABSNULL)
	{
	  ctype ct = ctype_realType (u->utype);
	  
	  if (ctype_isAbstract (ct))
	    {
	      sRef_setNotNull (res, g_currentloc);
	    }
	  else
	    {
	      if (ctype_isUser (ct))
		{
		  sRef_setStateFromUentry (res, usymtab_getTypeEntry (ctype_typeId (ct)));
		}
	      else
		{
		  sRef_setNotNull (res, g_currentloc);
		}
	    }
	}
      
      if (sRef_isRefCounted (res))
	{
	  sRef_setAliasKind (res, AK_NEWREF, g_currentloc);
	}
      else if (sRef_isKillRef (res))
	{
	  sRef_setAliasKind (res, AK_REFCOUNTED, g_currentloc);
	}
      else
	{
	  ;
	}
      
      ak = sRef_getAliasKind (res);
      
      if (alkind_isImplicit (ak))
	{
	  sRef_setAliasKind (res, 
			     alkind_fixImplicit (ak),
			     g_currentloc);
	}
      
      sRefSet_free (prefs);
      
      return res;
    }
}

static bool uentry_isRefCounted (uentry ue)
{
  ctype ct = uentry_getType (ue);

  if (ctype_isFunction (ct))
    {
      return (ctype_isRefCounted (ctype_returnValue (ct)));
    }
  else
    {
      return (ctype_isRefCounted (ct));
    }
}

/*
** old was declared yield in the specification.  
** new is declared in the iter implementation.
*/

void uentry_checkYieldParam (uentry old, uentry unew)
{
  cstring name;

  llassert (uentry_isVariable (old));
  llassert (uentry_isVariable (unew));

  unew->info->var->kind = VKYIELDPARAM;
  (void) checkTypeConformance (old, unew, TRUE);
  checkVarConformance (old, unew, TRUE, FALSE);

  /* get rid of param marker */

  name = uentry_getName (unew);
  cstring_free (unew->uname);
  unew->uname = name;
  unew->info->var->kind = VKREFYIELDPARAM;

  uentry_setUsed (old, fileloc_undefined);
  uentry_setUsed (unew, fileloc_undefined);
}

/*@observer@*/ cstring
uentry_ekindName (uentry ue)
{
  if (uentry_isValid (ue))
    {
      switch (ue->ukind)
	{
	case KINVALID:
	  return cstring_makeLiteralTemp ("<Error: invalid uentry>");
	case KDATATYPE: 
	  return cstring_makeLiteralTemp ("Datatype");
	case KENUMCONST:
	  return cstring_makeLiteralTemp ("Enum member");
	case KCONST:  
	  return cstring_makeLiteralTemp ("Constant");
	case KVAR:      
	  if (uentry_isParam (ue))
	    {
	      return cstring_makeLiteralTemp ("Parameter");
	    }
	  else if (uentry_isExpandedMacro (ue))
	    {
	      return cstring_makeLiteralTemp ("Expanded macro");
	    }
	  else
	    {
	      return cstring_makeLiteralTemp ("Variable");
	    }
	case KFCN:   
	  return cstring_makeLiteralTemp ("Function");
	case KITER: 
	  return cstring_makeLiteralTemp ("Iterator");
	case KENDITER:
	  return cstring_makeLiteralTemp ("Iterator finalizer");
	case KSTRUCTTAG:
	  return cstring_makeLiteralTemp ("Struct tag");
	case KUNIONTAG:
	  return cstring_makeLiteralTemp ("Union tag");
	case KENUMTAG: 
	  return cstring_makeLiteralTemp ("Enum tag");
	case KELIPSMARKER: 
	  return cstring_makeLiteralTemp ("Optional parameters");
	}
    }
  else
    {
      return cstring_makeLiteralTemp ("<Undefined>");
    }

  BADEXIT;
}

void uentry_setHasNameError (uentry ue)
{
  llassert (uentry_isValid (ue));

  ue->hasNameError = TRUE;
}

void uentry_checkName (uentry ue)
{
  if (uentry_isValid (ue) 
      && !uentry_isElipsisMarker (ue)
      && context_getFlag (FLG_NAMECHECKS)
      && !ue->hasNameError 
      && !uentry_isEndIter (ue)
      && !fileloc_isBuiltin (uentry_whereLast (ue))
      && (uentry_isExpandedMacro (ue) || !uentry_isForward (ue)))
    {
      
      if (uentry_isPriv (ue))
	{
	  ; /* any checks here? */
	}
      else if (fileloc_isExternal (uentry_whereDefined (ue)))
	{
	  ; /* no errors for externals */
	}
      else
	{
	  int scope;
	  
	  if (uentry_isExpandedMacro (ue))
	    {
	      scope = globScope;
	    }
	  else
	    {
	      if (uentry_isExpandedMacro (ue))
		{
		  scope = fileScope;
		}
	      else if (uentry_isVariable (ue))
		{
		  sRef sr = uentry_getSref (ue);

		  if (sRef_isValid (sr))
		    {
		      scope = sRef_getScope (sr);
		    }
		  else
		    {
		      scope = fileScope; 
		    }
		}
	      else if (uentry_isFunction (ue)
		       || uentry_isIter (ue)
		       || uentry_isEndIter (ue)
		       || uentry_isConstant (ue))
		{
		  scope = uentry_isStatic (ue) ? fileScope : globScope;
		}
	      else /* datatypes, etc. must be global */
		{
		  scope = globScope;
		}
	      
	      usymtab_checkDistinctName (ue, scope);
	    }

	  if (context_getFlag (FLG_CPPNAMES)) 
	    {
	      if (checkCppName (uentry_rawName (ue), uentry_whereLast (ue)))
		{
		  uentry_setHasNameError (ue);
		}
	    }

	  if (scope == globScope)
	    {
	      checkGlobalName (ue);

	      if (context_getFlag (FLG_ANSIRESERVED))
		{
		  if (uentry_hasName (ue)
		      && !uentry_isAnyTag (ue))
		    {
		      if (checkAnsiName (uentry_rawName (ue),
					 uentry_whereLast (ue)))
			{
			  uentry_setHasNameError (ue);
			}
		    }
		}
	    }
	  else
	    {
	      checkLocalName (ue);
	      
	      if (context_getFlag (FLG_ANSIRESERVEDLOCAL))
		{
		  if (uentry_hasName (ue)
		      && !uentry_isAnyTag (ue))
		    {
		      if (checkAnsiName (uentry_rawName (ue),
					 uentry_whereLast (ue)))
			{
			  uentry_setHasNameError (ue);
			}
		    }
		}
	    }

	  DPRINTF (("Check prefix: %s", uentry_unparse (ue)));
	  checkPrefix (ue);
	}
    }
}

/*@exposed@*/ uentry uentry_makeUnrecognized (cstring c, /*@keep@*/ fileloc loc)
{
  uentry ue;
  fileloc tloc;

  /*
  ** Can't but unrecognized ids in macros in global scope, because srefs will break! */
  if (!context_inMacro ())
    {
      sRef_setGlobalScopeSafe ();
    }

  ue = uentry_makeVariable (c, ctype_unknown, loc, FALSE);
  uentry_setUsed (ue, loc);		  
  
  tloc = fileloc_createExternal ();
  uentry_setDefined (ue, tloc);
  fileloc_free (tloc);
  uentry_setHasNameError (ue);
  
  if (context_getFlag (FLG_REPEATUNRECOG))
    {
      uentry_markOwned (ue);
    }
  else
    {
      ue = usymtab_supReturnFileEntry (ue);
    }
  
  if (!context_inMacro ())
    {
      sRef_clearGlobalScopeSafe ();
    }

  return ue;
}




