/*
** Copyright (C) University of Virginia, Massachusetts Institue of Technology 1994-2000.
** See ../LICENSE for license information.
**
*/
/*
** globSet.h
**
** based on set_template.h
*/

# ifndef globSet_H
# define globSet_H

abst_typedef sRefSet globSet;

/*@iter globSet_allElements (sef globSet s, yield exposed sRef el); @*/
# define globSet_allElements(x, m_el)  sRefSet_allElements(x, m_el)
# define end_globSet_allElements           end_sRefSet_allElements

extern int globSet_size (/*@sef@*/ globSet p_s);
# define globSet_size(s) (sRefSet_size (s))

extern bool globSet_isEmpty (/*@sef@*/ globSet p_s);
# define globSet_isEmpty(s) (globSet_size (s) == 0)

extern /*@only@*/ globSet globSet_new (void) /*@*/ ;
extern globSet globSet_insert (/*@returned@*/ globSet p_s, sRef p_el) 
   /*@modifies p_s@*/ ;
extern bool globSet_member (globSet p_s, sRef p_el) /*@*/ ;
extern /*@exposed@*/ sRef globSet_lookup (globSet p_s, sRef p_el) /*@*/ ;
extern void globSet_free (/*@only@*/ /*@only@*/ globSet p_s);
extern /*@only@*/ cstring globSet_unparse (globSet p_ll) /*@*/ ;
extern /*@only@*/ cstring globSet_dump (globSet) /*@*/ ;
extern /*@only@*/ globSet globSet_undump (char **p_s) /*@modifies *p_s@*/ ;
extern globSet 
  globSet_copy (/*@returned@*/ globSet p_s1, /*@exposed@*/ globSet p_s2) 
  /*@modifies p_s1@*/ ;

extern /*@only@*/ globSet globSet_newCopy (globSet p_s) /*@*/ ;
extern bool globSet_hasStatic (globSet p_s) /*@*/ ;

extern int globSet_compare (globSet p_l1, globSet p_l2);
extern void globSet_clear (globSet p_g);

/*@constant null globSet globSet_undefined;@*/
# define globSet_undefined sRefSet_undefined

extern /*@falsenull@*/ bool globSet_isDefined (/*@null@*/ globSet p_s) /*@*/ ;
extern /*@truenull@*/ bool globSet_isUndefined (/*@null@*/ globSet p_s) /*@*/ ;

# define globSet_isDefined(s)   (sRefSet_isDefined (s))
# define globSet_isUndefined(s) (sRefSet_isUndefined (s))

# else
# error "Multiple include"
# endif

