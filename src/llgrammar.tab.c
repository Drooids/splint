
/*  A Bison parser, made from llgrammar.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse ylparse
#define yylex yllex
#define yyerror ylerror
#define yylval yllval
#define yychar ylchar
#define yydebug yldebug
#define yynerrs ylnerrs
#define	simpleOp	258
#define	PREFIX_OP	259
#define	POSTFIX_OP	260
#define	LLT_MULOP	261
#define	LLT_SEMI	262
#define	LLT_VERTICALBAR	263
#define	ITERATION_OP	264
#define	LLT_LPAR	265
#define	LLT_LBRACKET	266
#define	selectSym	267
#define	LLT_IF_THEN_ELSE	268
#define	logicalOp	269
#define	eqSepSym	270
#define	equationSym	271
#define	commentSym	272
#define	LLT_WHITESPACE	273
#define	LLT_EOL	274
#define	LLT_TYPEDEF_NAME	275
#define	quantifierSym	276
#define	openSym	277
#define	closeSym	278
#define	sepSym	279
#define	simpleId	280
#define	mapSym	281
#define	markerSym	282
#define	preSym	283
#define	postSym	284
#define	anySym	285
#define	LLT_COLON	286
#define	LLT_COMMA	287
#define	LLT_EQUALS	288
#define	LLT_LBRACE	289
#define	LLT_RBRACE	290
#define	LLT_RBRACKET	291
#define	LLT_RPAR	292
#define	LLT_QUOTE	293
#define	eqOp	294
#define	LLT_CCHAR	295
#define	LLT_CFLOAT	296
#define	LLT_CINTEGER	297
#define	LLT_LCSTRING	298
#define	LLT_ALL	299
#define	LLT_ANYTHING	300
#define	LLT_BE	301
#define	LLT_BODY	302
#define	LLT_CLAIMS	303
#define	LLT_CHECKS	304
#define	LLT_CONSTANT	305
#define	LLT_ELSE	306
#define	LLT_ENSURES	307
#define	LLT_FOR	308
#define	LLT_FRESH	309
#define	LLT_IF	310
#define	LLT_IMMUTABLE	311
#define	LLT_IMPORTS	312
#define	LLT_CONSTRAINT	313
#define	LLT_ISSUB	314
#define	LLT_LET	315
#define	LLT_MODIFIES	316
#define	LLT_MUTABLE	317
#define	LLT_NOTHING	318
#define	LLT_INTERNAL	319
#define	LLT_FILESYS	320
#define	LLT_OBJ	321
#define	LLT_OUT	322
#define	LLT_SEF	323
#define	LLT_ONLY	324
#define	LLT_PARTIAL	325
#define	LLT_OWNED	326
#define	LLT_DEPENDENT	327
#define	LLT_KEEP	328
#define	LLT_KEPT	329
#define	LLT_TEMP	330
#define	LLT_SHARED	331
#define	LLT_UNIQUE	332
#define	LLT_UNUSED	333
#define	LLT_EXITS	334
#define	LLT_MAYEXIT	335
#define	LLT_NEVEREXIT	336
#define	LLT_TRUEEXIT	337
#define	LLT_FALSEEXIT	338
#define	LLT_UNDEF	339
#define	LLT_KILLED	340
#define	LLT_CHECKMOD	341
#define	LLT_CHECKED	342
#define	LLT_UNCHECKED	343
#define	LLT_CHECKEDSTRICT	344
#define	LLT_TRUENULL	345
#define	LLT_FALSENULL	346
#define	LLT_LNULL	347
#define	LLT_LNOTNULL	348
#define	LLT_RETURNED	349
#define	LLT_OBSERVER	350
#define	LLT_EXPOSED	351
#define	LLT_REFCOUNTED	352
#define	LLT_REFS	353
#define	LLT_RELNULL	354
#define	LLT_RELDEF	355
#define	LLT_KILLREF	356
#define	LLT_TEMPREF	357
#define	LLT_NEWREF	358
#define	LLT_PRIVATE	359
#define	LLT_REQUIRES	360
#define	LLT_RESULT	361
#define	LLT_SIZEOF	362
#define	LLT_SPEC	363
#define	LLT_TAGGEDUNION	364
#define	LLT_THEN	365
#define	LLT_TYPE	366
#define	LLT_TYPEDEF	367
#define	LLT_UNCHANGED	368
#define	LLT_USES	369
#define	LLT_CHAR	370
#define	LLT_CONST	371
#define	LLT_DOUBLE	372
#define	LLT_ENUM	373
#define	LLT_FLOAT	374
#define	LLT_INT	375
#define	LLT_ITER	376
#define	LLT_YIELD	377
#define	LLT_LONG	378
#define	LLT_SHORT	379
#define	LLT_SIGNED	380
#define	LLT_UNKNOWN	381
#define	LLT_STRUCT	382
#define	LLT_TELIPSIS	383
#define	LLT_UNION	384
#define	LLT_UNSIGNED	385
#define	LLT_VOID	386
#define	LLT_VOLATILE	387
#define	LLT_PRINTFLIKE	388
#define	LLT_SCANFLIKE	389
#define	LLT_MESSAGELIKE	390

#line 36 "llgrammar.y"


# include "bison.reset"

# include "lclintMacros.nf"
# include "llbasic.h"
# include "lclscan.h"
# include "checking.h"
# include "lslparse.h" 
# include "lh.h"
# include "usymtab_interface.h"

/*@-noparams@*/
static /*@unused@*/ void yyprint ();
/*@=noparams@*/

/*@-redecl@*/
void ylerror (char *) /*@modifies *g_msgstream@*/ ;
/*@=redecl@*/

bool g_inTypeDef = FALSE;

/*@constant int YYDEBUG;@*/
# define YYDEBUG 1

/*@notfunction@*/
# define YYPRINT(file, type, value) yyprint (file, type, value)

# include "bison.head"


#line 68 "llgrammar.y"
typedef union 
{
  ltoken ltok;  /* a leaf is also an ltoken */
  qual typequal;
  unsigned int count;
  /*@only@*/ ltokenList ltokenList;
  /*@only@*/ abstDeclaratorNode abstDecl; 
  /*@only@*/ declaratorNode declare;
  /*@only@*/ declaratorNodeList declarelist;
  /*@only@*/ typeExpr typeexpr;
  /*@only@*/ arrayQualNode array;
  /*@only@*/ quantifierNode quantifier;
  /*@only@*/ quantifierNodeList quantifiers;
  /*@only@*/ varNode var;
  /*@only@*/ varNodeList vars;
  /*@only@*/ storeRefNode storeref;
  /*@only@*/ storeRefNodeList storereflist;
  /*@only@*/ termNode term;
  /*@only@*/ termNodeList termlist;
  /*@only@*/ programNode program; 
  /*@only@*/ stmtNode stmt;
  /*@only@*/ claimNode claim;
  /*@only@*/ typeNode type;
  /*@only@*/ iterNode iter;
  /*@only@*/ fcnNode fcn;
  /*@only@*/ fcnNodeList fcns;
  /*@only@*/ letDeclNode letdecl;
  /*@only@*/ letDeclNodeList letdecls;
  /*@only@*/ lclPredicateNode lclpredicate;
  /*@only@*/ modifyNode modify;
  /*@only@*/ paramNode param;
  /*@only@*/ paramNodeList paramlist;
  /*@only@*/ declaratorInvNodeList declaratorinvs;	
  /*@only@*/ declaratorInvNode declaratorinv;	
  /*@only@*/ abstBodyNode abstbody;
  /*@only@*/ abstractNode abstract;
  /*@only@*/ exposedNode exposed;
  /*    taggedUnionNode taggedunion; */
  /*@only@*/ globalList globals;
  /*@only@*/ constDeclarationNode constdeclaration;
  /*@only@*/ varDeclarationNode vardeclaration;
  /*@only@*/ varDeclarationNodeList vardeclarationlist;
  /*@only@*/ initDeclNodeList initdecls;
  /*@only@*/ initDeclNode initdecl;
  /*@only@*/ stDeclNodeList structdecls;
  /*@only@*/ stDeclNode structdecl;
  /*@only@*/ strOrUnionNode structorunion;
  /*@only@*/ enumSpecNode enumspec; 
  /*@only@*/ lclTypeSpecNode lcltypespec;
  /*@only@*/ typeNameNode typname;
  /*@only@*/ opFormNode opform;
  /*@only@*/ sigNode signature;
  /*@only@*/ nameNode name;
  /*@only@*/ typeNameNodeList namelist;
  /*@only@*/ replaceNode replace;	
  /*@only@*/ replaceNodeList replacelist;
  /*@only@*/ renamingNode renaming;
  /*@only@*/ traitRefNode traitref;
  /*@only@*/ traitRefNodeList traitreflist;
  /*@only@*/ importNode import;
  /*@only@*/ importNodeList importlist;
  /*@only@*/ interfaceNode iface;
  /*@only@*/ interfaceNodeList interfacelist; 
  /*@only@*/ CTypesNode ctypes;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		683
#define	YYFLAG		-32768
#define	YYNTBASE	136

#define YYTRANSLATE(x) ((unsigned)(x) <= 390 ? yytranslate[x] : 290)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
   116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
   126,   127,   128,   129,   130,   131,   132,   133,   134,   135
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     5,     6,     9,    11,    13,    14,    17,    20,
    21,    24,    26,    28,    30,    34,    38,    40,    42,    44,
    46,    48,    50,    57,    58,    60,    62,    66,    69,    71,
    72,    76,    78,    80,    82,    84,    89,    93,    98,   103,
   105,   107,   109,   111,   113,   114,   128,   129,   144,   145,
   159,   164,   165,   166,   173,   174,   175,   183,   184,   185,
   193,   194,   195,   202,   203,   204,   211,   214,   217,   219,
   223,   225,   229,   231,   233,   237,   239,   241,   245,   247,
   252,   256,   263,   265,   269,   271,   273,   277,   279,   283,
   285,   289,   293,   297,   302,   304,   306,   308,   310,   312,
   316,   318,   322,   323,   326,   330,   333,   336,   337,   340,
   345,   346,   350,   352,   356,   362,   366,   368,   369,   373,
   374,   378,   379,   383,   387,   391,   393,   397,   399,   401,
   404,   406,   408,   409,   413,   414,   418,   419,   421,   423,
   425,   429,   431,   435,   436,   441,   447,   449,   453,   456,
   460,   464,   469,   473,   479,   486,   488,   492,   494,   496,
   499,   502,   506,   510,   514,   519,   521,   525,   530,   534,
   535,   538,   539,   541,   542,   549,   551,   555,   556,   560,
   561,   565,   567,   569,   571,   573,   575,   577,   579,   581,
   583,   585,   587,   590,   592,   594,   596,   598,   600,   602,
   604,   606,   608,   610,   612,   614,   616,   618,   620,   622,
   624,   626,   628,   630,   632,   634,   636,   638,   640,   642,
   644,   646,   648,   650,   652,   654,   656,   658,   660,   662,
   664,   666,   668,   670,   673,   679,   681,   684,   686,   689,
   690,   697,   698,   705,   708,   711,   712,   714,   716,   719,
   723,   725,   729,   730,   732,   739,   742,   744,   748,   750,
   752,   754,   758,   761,   764,   768,   773,   775,   779,   782,
   785,   789,   794,   796,   799,   802,   806,   811,   814,   817,
   820,   823,   827,   829,   830,   832,   836,   839,   841,   843,
   846,   850,   854,   859,   862,   866,   873,   875,   878,   881,
   885,   889,   894,   899,   905,   909,   914,   919,   925,   928,
   931,   935,   939,   941,   943,   945,   947,   949,   951,   953,
   954,   956,   958,   962,   964,   966,   971,   972,   974,   976,
   980,   982,   984,   991,   993,   997,   999,  1004,  1008,  1012,
  1014,  1017,  1020,  1022,  1024,  1026,  1029,  1031,  1034,  1037,
  1041,  1043,  1045,  1048,  1051,  1055,  1057,  1060,  1064,  1066,
  1072,  1076,  1081,  1084,  1088,  1091,  1093,  1097,  1101,  1103,
  1108,  1110,  1113,  1117,  1121,  1125,  1130,  1134,  1136,  1140,
  1142,  1144,  1146,  1148,  1150,  1152,  1157,  1162,  1167,  1172,
  1179,  1184,  1186,  1188,  1190,  1192,  1194,  1197,  1198,  1202,
  1204,  1208,  1212,  1217,  1219,  1221,  1223,  1225,  1227,  1229,
  1231,  1233
};

static const short yyrhs[] = {    -1,
   138,   137,   140,     0,     0,   138,   139,     0,   143,     0,
   144,     0,     0,   145,   141,     0,   150,   141,     0,     0,
   141,   142,     0,   145,     0,   150,     0,   144,     0,    57,
   174,     7,     0,   114,   178,     7,     0,   153,     0,   154,
     0,   155,     0,   157,     0,   160,     0,   146,     0,   121,
   284,    10,   147,    37,     7,     0,     0,   148,     0,   149,
     0,   148,    32,   149,     0,   122,   243,     0,   243,     0,
     0,   108,   151,   152,     0,   153,     0,   154,     0,   155,
     0,   157,     0,    50,   226,   188,     7,     0,   226,   188,
     7,     0,   116,   226,   188,     7,     0,   132,   226,   188,
     7,     0,   162,     0,   171,     0,   133,     0,   134,     0,
   135,     0,     0,   226,   239,   190,   158,    34,   192,   194,
   198,   199,   200,   203,   204,    35,     0,     0,   156,   226,
   239,   190,   159,    34,   192,   194,   198,   199,   200,   203,
   204,    35,     0,     0,    48,   286,    10,   205,    37,   190,
   161,    34,   194,   199,   208,   203,    35,     0,    48,   283,
   286,     7,     0,     0,     0,    62,   163,   111,    20,   164,
   213,     0,     0,     0,    62,   165,    97,   111,    20,   166,
   213,     0,     0,     0,    97,    62,   167,   111,    20,   168,
   213,     0,     0,     0,    56,   169,   111,    20,   170,   213,
     0,     0,     0,   112,   226,   172,   218,   173,     7,     0,
   229,     7,     0,   237,     7,     0,   175,     0,   174,    32,
   175,     0,   177,     0,     3,   177,     3,     0,    43,     0,
   177,     0,   176,    32,   177,     0,   282,     0,   179,     0,
   178,    32,   179,     0,   288,     0,   288,    10,   181,    37,
     0,    10,   180,    37,     0,    10,   180,    37,    10,   181,
    37,     0,   288,     0,   180,    32,   288,     0,   183,     0,
   182,     0,   182,    32,   183,     0,   244,     0,   182,    32,
   244,     0,   184,     0,   183,    32,   184,     0,   244,    53,
   222,     0,   244,    53,   185,     0,   244,    53,   185,   255,
     0,   289,     0,   248,     0,   187,     0,   259,     0,   189,
     0,   188,    32,   189,     0,   239,     0,   239,    33,   186,
     0,     0,   190,   191,     0,   226,   188,     7,     0,    64,
     7,     0,    65,     7,     0,     0,   192,   193,     0,   104,
   226,   188,     7,     0,     0,    60,   195,     7,     0,   196,
     0,   195,    32,   196,     0,   284,    31,   197,    46,   259,
     0,   284,    46,   259,     0,   226,     0,     0,    49,   258,
     7,     0,     0,   105,   258,     7,     0,     0,    61,    63,
     7,     0,    61,    45,     7,     0,    61,   201,     7,     0,
   202,     0,   201,    32,   202,     0,   259,     0,   227,     0,
    66,   227,     0,    64,     0,    65,     0,     0,    52,   258,
     7,     0,     0,    48,   258,     7,     0,     0,   206,     0,
   207,     0,   128,     0,   207,    32,   128,     0,   243,     0,
   207,    32,   243,     0,     0,    47,    34,   209,    35,     0,
    47,    34,   209,     7,    35,     0,   210,     0,    10,   209,
    37,     0,   209,     6,     0,   209,     8,   209,     0,   209,
     7,   209,     0,   283,    10,   211,    37,     0,   283,    10,
    37,     0,   284,    33,   283,    10,    37,     0,   284,    33,
   283,    10,   211,    37,     0,   212,     0,   211,    32,   212,
     0,   276,     0,   284,     0,     3,   212,     0,   212,     3,
     0,   212,     3,   212,     0,    10,   212,    37,     0,   283,
    10,    37,     0,   283,    10,   211,    37,     0,     7,     0,
    34,   214,    35,     0,    34,   176,    35,     7,     0,    34,
    35,     7,     0,     0,   214,   157,     0,     0,   216,     0,
     0,    58,   217,   278,    10,   258,    37,     0,   219,     0,
   218,    32,   219,     0,     0,   239,   220,   221,     0,     0,
    34,   215,    35,     0,   131,     0,   115,     0,   117,     0,
   119,     0,   120,     0,   123,     0,   124,     0,   125,     0,
   130,     0,   126,     0,   222,     0,   223,   222,     0,    20,
     0,   223,     0,    67,     0,    78,     0,    68,     0,    69,
     0,    71,     0,    72,     0,    73,     0,    74,     0,    95,
     0,    79,     0,    80,     0,    82,     0,    83,     0,    81,
     0,    75,     0,    76,     0,    77,     0,    87,     0,    88,
     0,    89,     0,    90,     0,    91,     0,    99,     0,   100,
     0,    97,     0,    98,     0,   103,     0,   101,     0,    92,
     0,    93,     0,    94,     0,    96,     0,    70,     0,    84,
     0,    85,     0,   224,     0,   229,     0,   237,     0,   225,
   226,     0,     8,   227,    31,   227,     8,     0,   226,     0,
   226,   228,     0,     6,     0,   228,     6,     0,     0,   127,
   232,   230,    34,   233,    35,     0,     0,   129,   232,   231,
    34,   233,    35,     0,   127,   285,     0,   129,   285,     0,
     0,   285,     0,   234,     0,   233,   234,     0,   226,   235,
     7,     0,   239,     0,   235,    32,   239,     0,     0,    32,
     0,   118,   232,    34,   238,   236,    35,     0,   118,   285,
     0,    25,     0,   238,    32,    25,     0,   241,     0,   240,
     0,   284,     0,    10,   240,    37,     0,     6,   240,     0,
   240,   247,     0,   240,    10,    37,     0,   240,    10,   206,
    37,     0,    20,     0,    10,   241,    37,     0,     6,   241,
     0,   241,   247,     0,   241,    10,    37,     0,   241,    10,
   206,    37,     0,    20,     0,     6,   242,     0,   242,   247,
     0,   242,    10,    37,     0,   242,    10,   206,    37,     0,
   226,   242,     0,   226,   240,     0,   226,   245,     0,   226,
   245,     0,    66,   226,   245,     0,   248,     0,     0,   246,
     0,    10,   246,    37,     0,     6,   246,     0,     6,     0,
   247,     0,   246,   247,     0,   246,    10,    37,     0,    10,
   206,    37,     0,   246,    10,   206,    37,     0,    11,    36,
     0,    11,   187,    36,     0,    55,    27,   110,    27,    51,
    27,     0,   251,     0,    27,   251,     0,   251,    27,     0,
    27,   251,    27,     0,   249,   252,   250,     0,    27,   249,
   252,   250,     0,   249,   252,   250,    27,     0,    27,   249,
   252,   250,    27,     0,    11,   252,    36,     0,    11,   252,
    36,    27,     0,    27,    11,   252,    36,     0,    27,    11,
   252,    36,    27,     0,    12,   282,     0,    26,   282,     0,
    27,    12,   282,     0,    27,    26,   282,     0,    22,     0,
    34,     0,    23,     0,    35,     0,   263,     0,    14,     0,
    39,     0,     0,   253,     0,    27,     0,   253,   254,    27,
     0,    32,     0,    24,     0,    31,   256,    26,   287,     0,
     0,   257,     0,   287,     0,   257,    32,   287,     0,   259,
     0,   260,     0,    55,   260,   110,   260,    51,   260,     0,
   261,     0,   260,    14,   260,     0,   262,     0,   277,    10,
   258,    37,     0,   262,    39,   262,     0,   262,    33,   262,
     0,   264,     0,   267,   265,     0,   267,   266,     0,     3,
     0,     6,     0,   267,     0,   263,   264,     0,   263,     0,
   265,   263,     0,   263,   267,     0,   266,   263,   267,     0,
   272,     0,   268,     0,   268,   272,     0,   272,   268,     0,
   272,   268,   272,     0,   269,     0,   269,   272,     0,   270,
    31,   287,     0,   270,     0,    11,   271,    36,    31,   287,
     0,    11,   271,    36,     0,    11,    36,    31,   287,     0,
    11,    36,     0,   249,   271,   250,     0,   249,   250,     0,
   259,     0,   271,   254,   259,     0,    10,   260,    37,     0,
   284,     0,   289,    10,   273,    37,     0,   275,     0,   272,
   274,     0,   272,    12,   282,     0,   272,    26,   282,     0,
   272,    11,    36,     0,   272,    11,   273,    36,     0,   272,
    31,   287,     0,   260,     0,   273,    32,   260,     0,    28,
     0,    29,     0,    30,     0,    38,     0,   276,     0,   106,
     0,    54,    10,   273,    37,     0,   113,    10,    44,    37,
     0,   113,    10,   201,    37,     0,   107,    10,   259,    37,
     0,    59,    10,   259,    32,   259,    37,     0,   107,    10,
   226,    37,     0,    42,     0,    43,     0,    40,     0,    41,
     0,   278,     0,   277,   278,     0,     0,    21,   279,   280,
     0,   281,     0,   280,    32,   281,     0,   284,    31,   197,
     0,   284,    31,    66,   197,     0,    25,     0,    20,     0,
    25,     0,    25,     0,   282,     0,   282,     0,   282,     0,
   282,     0,   282,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   372,   373,   376,   377,   380,   381,   384,   385,   386,   389,
   390,   393,   394,   395,   398,   404,   408,   410,   412,   414,
   416,   418,   422,   426,   427,   430,   432,   436,   437,   440,
   441,   444,   446,   448,   450,   454,   458,   460,   462,   466,
   467,   470,   471,   472,   475,   476,   483,   485,   496,   498,
   501,   505,   505,   506,   507,   507,   509,   510,   511,   512,
   513,   513,   514,   517,   518,   519,   520,   522,   538,   540,
   544,   545,   547,   550,   551,   554,   558,   560,   564,   566,
   568,   570,   574,   575,   578,   580,   582,   585,   587,   590,
   592,   595,   596,   597,   601,   602,   604,   606,   609,   611,
   615,   616,   619,   621,   625,   626,   627,   630,   631,   634,
   638,   639,   642,   643,   646,   647,   649,   652,   653,   656,
   657,   660,   661,   662,   663,   666,   667,   670,   671,   672,
   673,   674,   677,   678,   681,   682,   685,   686,   689,   690,
   691,   694,   695,   698,   699,   700,   703,   704,   707,   712,
   718,   726,   728,   730,   732,   736,   737,   740,   741,   742,
   743,   744,   745,   746,   748,   752,   753,   754,   755,   758,
   759,   762,   763,   766,   766,   775,   776,   780,   781,   784,
   785,   788,   789,   790,   791,   792,   793,   794,   795,   796,
   797,   805,   806,   811,   813,   819,   820,   821,   822,   823,
   824,   825,   826,   827,   828,   829,   830,   831,   832,   833,
   834,   835,   836,   837,   838,   839,   840,   841,   842,   843,
   844,   845,   846,   847,   848,   849,   850,   851,   852,   853,
   856,   858,   860,   862,   864,   873,   874,   879,   880,   883,
   885,   887,   889,   891,   893,   897,   898,   901,   902,   907,
   910,   912,   916,   917,   920,   922,   926,   927,   937,   938,
   941,   942,   943,   944,   945,   946,   949,   950,   951,   952,
   953,   954,   963,   964,   965,   966,   967,   973,   974,   975,
   985,   986,   987,   992,   993,   996,   997,   998,   999,  1000,
  1001,  1002,  1003,  1006,  1007,  1010,  1012,  1014,  1016,  1018,
  1021,  1023,  1025,  1027,  1029,  1031,  1039,  1041,  1043,  1046,
  1049,  1052,  1057,  1058,  1061,  1062,  1065,  1066,  1067,  1070,
  1071,  1074,  1075,  1078,  1079,  1082,  1085,  1086,  1089,  1090,
  1093,  1096,  1102,  1104,  1105,  1108,  1109,  1114,  1116,  1120,
  1121,  1122,  1125,  1126,  1129,  1130,  1133,  1134,  1137,  1138,
  1141,  1142,  1143,  1144,  1145,  1146,  1147,  1150,  1151,  1154,
  1157,  1159,  1163,  1167,  1168,  1171,  1172,  1175,  1177,  1179,
  1181,  1182,  1184,  1186,  1188,  1191,  1193,  1197,  1198,  1201,
  1202,  1203,  1204,  1207,  1208,  1209,  1210,  1211,  1212,  1217,
  1223,  1233,  1234,  1235,  1236,  1239,  1241,  1245,  1249,  1252,
  1253,  1256,  1257,  1260,  1261,  1264,  1265,  1266,  1267,  1268,
  1269,  1270
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","simpleOp",
"PREFIX_OP","POSTFIX_OP","LLT_MULOP","LLT_SEMI","LLT_VERTICALBAR","ITERATION_OP",
"LLT_LPAR","LLT_LBRACKET","selectSym","LLT_IF_THEN_ELSE","logicalOp","eqSepSym",
"equationSym","commentSym","LLT_WHITESPACE","LLT_EOL","LLT_TYPEDEF_NAME","quantifierSym",
"openSym","closeSym","sepSym","simpleId","mapSym","markerSym","preSym","postSym",
"anySym","LLT_COLON","LLT_COMMA","LLT_EQUALS","LLT_LBRACE","LLT_RBRACE","LLT_RBRACKET",
"LLT_RPAR","LLT_QUOTE","eqOp","LLT_CCHAR","LLT_CFLOAT","LLT_CINTEGER","LLT_LCSTRING",
"LLT_ALL","LLT_ANYTHING","LLT_BE","LLT_BODY","LLT_CLAIMS","LLT_CHECKS","LLT_CONSTANT",
"LLT_ELSE","LLT_ENSURES","LLT_FOR","LLT_FRESH","LLT_IF","LLT_IMMUTABLE","LLT_IMPORTS",
"LLT_CONSTRAINT","LLT_ISSUB","LLT_LET","LLT_MODIFIES","LLT_MUTABLE","LLT_NOTHING",
"LLT_INTERNAL","LLT_FILESYS","LLT_OBJ","LLT_OUT","LLT_SEF","LLT_ONLY","LLT_PARTIAL",
"LLT_OWNED","LLT_DEPENDENT","LLT_KEEP","LLT_KEPT","LLT_TEMP","LLT_SHARED","LLT_UNIQUE",
"LLT_UNUSED","LLT_EXITS","LLT_MAYEXIT","LLT_NEVEREXIT","LLT_TRUEEXIT","LLT_FALSEEXIT",
"LLT_UNDEF","LLT_KILLED","LLT_CHECKMOD","LLT_CHECKED","LLT_UNCHECKED","LLT_CHECKEDSTRICT",
"LLT_TRUENULL","LLT_FALSENULL","LLT_LNULL","LLT_LNOTNULL","LLT_RETURNED","LLT_OBSERVER",
"LLT_EXPOSED","LLT_REFCOUNTED","LLT_REFS","LLT_RELNULL","LLT_RELDEF","LLT_KILLREF",
"LLT_TEMPREF","LLT_NEWREF","LLT_PRIVATE","LLT_REQUIRES","LLT_RESULT","LLT_SIZEOF",
"LLT_SPEC","LLT_TAGGEDUNION","LLT_THEN","LLT_TYPE","LLT_TYPEDEF","LLT_UNCHANGED",
"LLT_USES","LLT_CHAR","LLT_CONST","LLT_DOUBLE","LLT_ENUM","LLT_FLOAT","LLT_INT",
"LLT_ITER","LLT_YIELD","LLT_LONG","LLT_SHORT","LLT_SIGNED","LLT_UNKNOWN","LLT_STRUCT",
"LLT_TELIPSIS","LLT_UNION","LLT_UNSIGNED","LLT_VOID","LLT_VOLATILE","LLT_PRINTFLIKE",
"LLT_SCANFLIKE","LLT_MESSAGELIKE","interface","@1","externals","external","optDeclarations",
"declarations","declaration","imports","uses","export","iter","iterParamList",
"realIterParamList","iterParam","private","@2","private2","constDeclaration",
"varDeclaration","type","special","fcn","@3","@4","claim","@5","abstract","@6",
"@7","@8","@9","@10","@11","@12","@13","exposed","@14","@15","importNameList",
"importName","interfaceNameList","interfaceName","traitRefNodeList","traitRef",
"traitIdList","renaming","nameList","replaceNodeList","replace","name","initializer",
"constLclExpr","initDecls","initDecl","globals","globalDecl","privateInits",
"privateInit","optLetDecl","beDeclList","beDecl","sortSpec","optChecks","optRequire",
"optModify","storeRefList","storeRef","optEnsure","optClaim","optParamList",
"realParamList","paramList","optBody","callExpr","stmt","valueList","value",
"abstBody","fcns","optTypeInv","typeInv","@16","declaratorInvs","declaratorInv",
"@17","optExposedBody","CType","CTypes","typeSpecifier","specialQualifier","lclTypeSpec",
"lclType","pointers","structOrUnionSpec","@18","@19","optTagId","structDecls",
"structDecl","declaratorList","optCOMMA","enumSpec","enumeratorList","declarator",
"notype_decl","after_type_decl","parameter_decl","param","typeName","optAbstDeclarator",
"abstDeclarator","arrayQual","opForm","open","close","anyOp","middle","placeList",
"separator","signature","domain","sortList","lclPredicate","term","term0","equalityTerm",
"simpleOpTerm","simpleOp2","prefixOpTerm","postfixOps","infixOpPart","secondary",
"bracketed","sqBracketed","matched","args","primary","termList","stateFcn","lclPrimary",
"cLiteral","quantifiers","quantifier","@20","quantifiedList","quantified","simpleIdOrTypedefName",
"fcnId","varId","tagId","claimId","sortId","traitId","opId", NULL
};
#endif

static const short yyr1[] = {     0,
   137,   136,   138,   138,   139,   139,   140,   140,   140,   141,
   141,   142,   142,   142,   143,   144,   145,   145,   145,   145,
   145,   145,   146,   147,   147,   148,   148,   149,   149,   151,
   150,   152,   152,   152,   152,   153,   154,   154,   154,   155,
   155,   156,   156,   156,   158,   157,   159,   157,   161,   160,
   160,   163,   164,   162,   165,   166,   162,   167,   168,   162,
   169,   170,   162,   172,   173,   171,   171,   171,   174,   174,
   175,   175,   175,   176,   176,   177,   178,   178,   179,   179,
   179,   179,   180,   180,   181,   181,   181,   182,   182,   183,
   183,   184,   184,   184,   185,   185,   186,   187,   188,   188,
   189,   189,   190,   190,   191,   191,   191,   192,   192,   193,
   194,   194,   195,   195,   196,   196,   197,   198,   198,   199,
   199,   200,   200,   200,   200,   201,   201,   202,   202,   202,
   202,   202,   203,   203,   204,   204,   205,   205,   206,   206,
   206,   207,   207,   208,   208,   208,   209,   209,   209,   209,
   209,   210,   210,   210,   210,   211,   211,   212,   212,   212,
   212,   212,   212,   212,   212,   213,   213,   213,   213,   214,
   214,   215,   215,   217,   216,   218,   218,   220,   219,   221,
   221,   222,   222,   222,   222,   222,   222,   222,   222,   222,
   222,   223,   223,   224,   224,   225,   225,   225,   225,   225,
   225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
   225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
   225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
   226,   226,   226,   226,   226,   227,   227,   228,   228,   230,
   229,   231,   229,   229,   229,   232,   232,   233,   233,   234,
   235,   235,   236,   236,   237,   237,   238,   238,   239,   239,
   240,   240,   240,   240,   240,   240,   241,   241,   241,   241,
   241,   241,   242,   242,   242,   242,   242,   243,   243,   243,
   244,   244,   244,   245,   245,   246,   246,   246,   246,   246,
   246,   246,   246,   247,   247,   248,   248,   248,   248,   248,
   248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
   248,   248,   249,   249,   250,   250,   251,   251,   251,   252,
   252,   253,   253,   254,   254,   255,   256,   256,   257,   257,
   258,   259,   260,   260,   260,   261,   261,   261,   261,   262,
   262,   262,   263,   263,   264,   264,   265,   265,   266,   266,
   267,   267,   267,   267,   267,   267,   267,   268,   268,   269,
   269,   269,   269,   270,   270,   271,   271,   272,   272,   272,
   272,   272,   272,   272,   272,   272,   272,   273,   273,   274,
   274,   274,   274,   275,   275,   275,   275,   275,   275,   275,
   275,   276,   276,   276,   276,   277,   277,   279,   278,   280,
   280,   281,   281,   282,   282,   283,   284,   285,   286,   287,
   288,   289
};

static const short yyr2[] = {     0,
     0,     3,     0,     2,     1,     1,     0,     2,     2,     0,
     2,     1,     1,     1,     3,     3,     1,     1,     1,     1,
     1,     1,     6,     0,     1,     1,     3,     2,     1,     0,
     3,     1,     1,     1,     1,     4,     3,     4,     4,     1,
     1,     1,     1,     1,     0,    13,     0,    14,     0,    13,
     4,     0,     0,     6,     0,     0,     7,     0,     0,     7,
     0,     0,     6,     0,     0,     6,     2,     2,     1,     3,
     1,     3,     1,     1,     3,     1,     1,     3,     1,     4,
     3,     6,     1,     3,     1,     1,     3,     1,     3,     1,
     3,     3,     3,     4,     1,     1,     1,     1,     1,     3,
     1,     3,     0,     2,     3,     2,     2,     0,     2,     4,
     0,     3,     1,     3,     5,     3,     1,     0,     3,     0,
     3,     0,     3,     3,     3,     1,     3,     1,     1,     2,
     1,     1,     0,     3,     0,     3,     0,     1,     1,     1,
     3,     1,     3,     0,     4,     5,     1,     3,     2,     3,
     3,     4,     3,     5,     6,     1,     3,     1,     1,     2,
     2,     3,     3,     3,     4,     1,     3,     4,     3,     0,
     2,     0,     1,     0,     6,     1,     3,     0,     3,     0,
     3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     2,     5,     1,     2,     1,     2,     0,
     6,     0,     6,     2,     2,     0,     1,     1,     2,     3,
     1,     3,     0,     1,     6,     2,     1,     3,     1,     1,
     1,     3,     2,     2,     3,     4,     1,     3,     2,     2,
     3,     4,     1,     2,     2,     3,     4,     2,     2,     2,
     2,     3,     1,     0,     1,     3,     2,     1,     1,     2,
     3,     3,     4,     2,     3,     6,     1,     2,     2,     3,
     3,     4,     4,     5,     3,     4,     4,     5,     2,     2,
     3,     3,     1,     1,     1,     1,     1,     1,     1,     0,
     1,     1,     3,     1,     1,     4,     0,     1,     1,     3,
     1,     1,     6,     1,     3,     1,     4,     3,     3,     1,
     2,     2,     1,     1,     1,     2,     1,     2,     2,     3,
     1,     1,     2,     2,     3,     1,     2,     3,     1,     5,
     3,     4,     2,     3,     2,     1,     3,     3,     1,     4,
     1,     2,     3,     3,     3,     4,     3,     1,     3,     1,
     1,     1,     1,     1,     1,     4,     4,     4,     4,     6,
     4,     1,     1,     1,     1,     1,     2,     0,     3,     1,
     3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
     1,     1
};

static const short yydefact[] = {     3,
     1,     0,     0,     7,     4,     5,     6,     0,   405,   404,
    73,     0,    69,    71,    76,     0,     0,    77,   411,    79,
     0,   194,     0,     0,    61,    52,   196,   198,   199,   228,
   200,   201,   202,   203,   210,   211,   212,   197,   205,   206,
   209,   207,   208,   229,   230,   213,   214,   215,   216,   217,
   224,   225,   226,   204,   227,   220,   221,   218,   219,   223,
   222,    30,     0,   183,     0,   184,   246,   185,   186,     0,
   187,   188,   189,   191,   246,   246,   190,   182,     0,    42,
    43,    44,     2,    10,    22,    10,    17,    18,    19,     0,
    20,    21,    40,    41,   192,   195,   231,     0,     0,   232,
   233,     0,    15,     0,     0,    83,    16,     0,     0,   220,
   236,     0,   232,   233,   406,   409,     0,     0,     0,     0,
     0,     0,    58,     0,    64,     0,     0,   408,   256,   407,
     0,   240,   244,   242,   245,     0,     8,     9,     0,   193,
   234,     0,     0,   267,     0,    99,   103,   260,   259,   261,
    67,    68,    72,    70,     0,    81,    78,   343,   344,   320,
     0,   318,   313,     0,     0,   314,   319,     0,     0,     0,
    86,    85,    90,   284,    88,   283,   320,   297,   317,   238,
   237,     0,     0,   137,     0,   101,     0,     0,     0,     0,
    31,    32,    33,    34,    35,     0,     0,     0,    24,     0,
     0,     0,    11,    14,    12,    13,   103,   263,   269,     0,
     0,    37,     0,     0,    45,     0,     0,   264,     0,   270,
    84,     0,   322,     0,   321,   309,   310,   320,     0,     0,
   320,   298,     0,   284,    80,     0,     0,   288,     0,   281,
   285,   289,     0,     0,   299,   239,     0,    51,   140,     0,
   138,   139,   284,   142,    36,    62,    53,     0,     0,    65,
   176,   178,    38,   257,   253,     0,     0,    25,    26,    29,
     0,     0,    39,    47,   262,   268,   100,     0,     0,   398,
   407,   394,   395,   392,   393,     0,     0,     0,   385,     0,
     0,   102,    97,     0,    98,   332,   334,   336,     0,   340,
   345,   352,   356,   359,   351,   371,   384,     0,   396,   412,
   369,     0,     0,     0,     0,   104,     0,   265,     0,   294,
     0,   271,     0,     0,   305,   325,   324,     0,     0,   311,
   312,     0,   300,     0,   282,    87,    89,    91,     0,   287,
     0,     0,     0,   290,    93,    92,    96,    95,   315,   316,
   301,   235,   103,     0,   288,     0,   273,   279,   278,   280,
     0,     0,    56,    59,     0,     0,   180,   254,     0,    28,
     0,     0,     0,     0,   248,     0,     0,     0,   363,   366,
     0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
     0,     0,   346,   345,   347,   341,   342,   353,   357,     0,
     0,     0,     0,   380,   381,   382,     0,   383,   354,   372,
     0,   397,     0,   106,   107,   108,     0,   266,   295,   272,
    82,   306,   323,   307,   302,     0,   292,   286,   291,     0,
   327,    94,   303,    49,   141,   143,   274,   288,     0,   275,
   166,   170,    63,    54,     0,     0,   177,    66,   172,   179,
   258,   255,    23,    27,     0,   251,   241,   249,   243,   108,
   368,     0,   361,     0,   399,   400,     0,   378,     0,     0,
     0,   194,     0,     0,     0,   131,   132,     0,     0,   126,
   129,   128,   364,   335,   339,   338,   349,   348,     0,   410,
   358,   375,     0,   373,   374,   377,   355,     0,   331,     0,
   111,   105,   308,   304,     0,   293,     0,   328,   329,     0,
   276,     0,     0,     0,    74,     0,    57,    60,   174,     0,
   173,   250,     0,   111,   362,     0,   367,     0,     0,     0,
   386,     0,     0,   391,   389,   387,   130,     0,   388,   350,
   376,   337,   370,     0,     0,   109,   118,   296,     0,     0,
   111,   277,   169,     0,     0,   167,   171,     0,     0,   181,
   252,   118,   360,   401,     0,   402,   117,   379,     0,     0,
   127,     0,   113,     0,     0,     0,   120,   326,   330,   120,
    75,   168,   103,     0,   120,   403,   333,   390,   112,     0,
     0,     0,     0,     0,     0,   122,   144,     0,   122,   114,
     0,   116,   110,   119,     0,     0,   133,     0,   133,     0,
   133,     0,   121,     0,     0,     0,     0,   135,     0,     0,
   175,   135,   115,   124,   123,   125,     0,     0,     0,     0,
   407,     0,   147,     0,     0,    50,     0,   134,     0,    46,
     0,   149,     0,     0,   145,     0,     0,    48,   136,     0,
   148,   146,   151,   150,     0,     0,   153,     0,   156,   158,
     0,   159,   406,     0,   160,     0,     0,   152,   161,     0,
     0,   163,   157,   162,   164,     0,   154,     0,   165,   155,
     0,     0,     0
};

static const short yydefgoto[] = {   681,
     4,     1,     5,    83,   137,   203,     6,   204,   205,    85,
   267,   268,   269,   206,   124,   191,    87,    88,    89,    90,
    91,   315,   377,    92,   510,    93,   121,   362,   122,   445,
   190,   446,   120,   361,    94,   196,   366,    12,    13,   514,
    14,    17,    18,   105,   170,   171,   172,   173,   345,   292,
   293,   145,   146,   215,   316,   501,   546,   547,   572,   573,
   566,   577,   596,   607,   479,   480,   618,   629,   250,   341,
   252,   609,   653,   633,   658,   659,   443,   516,   520,   521,
   559,   260,   261,   367,   450,    95,    96,    97,    98,   253,
   481,   181,   113,   200,   201,   127,   374,   375,   455,   369,
   114,   265,   186,   148,   149,   359,   254,   175,   240,   241,
   242,   176,   294,   351,   178,   224,   225,   464,   432,   507,
   508,   498,   499,   296,   297,   298,   299,   300,   396,   397,
   301,   302,   303,   304,   381,   305,   469,   410,   306,   307,
   308,   309,   382,   465,   466,   310,   661,   311,   129,   118,
   491,    20,   312
};

static const short yypact[] = {-32768,
   -28,   224,   372,  1343,-32768,-32768,-32768,   132,-32768,-32768,
-32768,    48,-32768,-32768,-32768,   132,   105,-32768,-32768,    49,
  2880,-32768,   227,  2880,-32768,    17,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   161,-32768,-32768,-32768,-32768,
-32768,-32768,  2880,-32768,  2880,-32768,   132,-32768,-32768,   221,
-32768,-32768,-32768,-32768,   132,   132,-32768,-32768,  2880,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2880,
-32768,-32768,-32768,-32768,-32768,  1065,-32768,  2880,   396,   247,
   270,   280,-32768,   224,   311,-32768,-32768,   372,  1147,-32768,
   310,   265,-32768,-32768,   343,-32768,   132,   345,   396,   209,
   246,   264,-32768,  1437,-32768,   396,   340,-32768,   346,-32768,
   378,-32768,   346,-32768,   346,   396,  1241,  1241,   396,-32768,
-32768,   396,   396,-32768,   123,-32768,   118,   198,   294,-32768,
-32768,-32768,-32768,-32768,   132,   431,-32768,-32768,-32768,   416,
   132,-32768,-32768,   132,   488,-32768,-32768,   423,  2880,   427,
   442,   444,-32768,   462,   417,-32768,   416,   466,-32768,-32768,
   499,  2880,   502,  2302,   149,   484,   501,   512,   428,   429,
-32768,-32768,-32768,-32768,-32768,   396,   156,   509,  2396,   504,
   507,   165,-32768,-32768,-32768,-32768,-32768,   198,   294,    89,
   112,-32768,   396,  3134,  1824,  1920,  3009,-32768,  2016,-32768,
-32768,  1147,-32768,   500,    33,-32768,-32768,   416,   132,   132,
   416,   516,   437,   462,-32768,  1147,  1147,   462,  1730,-32768,
   329,-32768,   239,    93,-32768,-32768,   538,-32768,-32768,   513,
-32768,   517,   379,-32768,-32768,-32768,-32768,   533,   534,   523,
-32768,-32768,-32768,-32768,   526,  2880,   522,   532,-32768,-32768,
  2880,  2880,-32768,  1824,-32768,-32768,-32768,  3134,  3036,-32768,
   343,-32768,-32768,-32768,-32768,   555,  3134,   556,-32768,   559,
   560,-32768,-32768,   412,-32768,   558,-32768,   121,   557,-32768,
   436,   438,   438,   540,   761,-32768,-32768,   290,-32768,-32768,
-32768,   563,   567,   568,   542,-32768,   396,-32768,   541,-32768,
   544,-32768,   546,   547,   562,-32768,-32768,   566,   549,-32768,
-32768,    93,-32768,   574,-32768,   444,   417,-32768,   417,   329,
   550,   128,  2112,-32768,   564,-32768,-32768,-32768,-32768,-32768,
   575,-32768,-32768,  2490,   379,  1634,-32768,   198,   376,-32768,
    34,    34,-32768,-32768,   396,   579,   570,   565,   578,-32768,
   587,  2396,   396,  2591,-32768,  2692,   576,   159,   583,-32768,
   393,   221,  3134,    21,  3134,  1018,   760,-32768,   463,  3134,
   557,   557,-32768,-32768,   288,   436,   436,   295,   295,   132,
  3077,   132,   132,-32768,-32768,-32768,   132,-32768,   438,-32768,
  3134,-32768,  3134,-32768,-32768,-32768,   211,-32768,-32768,-32768,
-32768,-32768,-32768,   582,   588,   571,-32768,-32768,-32768,   586,
   132,-32768,-32768,  1824,-32768,-32768,   376,   434,  2208,-32768,
-32768,   373,-32768,-32768,    34,    34,-32768,-32768,   561,-32768,
-32768,-32768,-32768,-32768,   249,-32768,-32768,-32768,-32768,-32768,
-32768,   132,   594,  3134,   596,-32768,   595,   558,   419,  3134,
   598,   625,   599,   601,   603,-32768,-32768,  2880,   425,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   288,-32768,
-32768,-32768,    20,-32768,-32768,-32768,   295,   606,-32768,   445,
    -7,-32768,-32768,-32768,   617,-32768,   619,   614,-32768,   613,
-32768,   611,   642,   472,-32768,  1538,-32768,-32768,-32768,   615,
-32768,-32768,   396,    -7,-32768,   132,-32768,   221,  2786,  3134,
-32768,    28,  3134,-32768,-32768,-32768,-32768,   889,-32768,-32768,
-32768,-32768,-32768,   221,  2880,-32768,   605,-32768,   132,   132,
   597,-32768,-32768,   132,   648,-32768,-32768,   396,   637,-32768,
-32768,   605,-32768,-32768,  2880,-32768,-32768,   558,  3134,   622,
-32768,   277,-32768,   115,   396,  3134,   572,-32768,-32768,   572,
-32768,-32768,-32768,   650,   572,-32768,   558,-32768,-32768,   221,
  2880,  3134,   282,   654,  3134,   607,   620,  3134,   607,-32768,
   616,-32768,-32768,-32768,   659,   631,   623,   635,   623,   641,
   623,  3134,-32768,   672,   673,   334,  3134,   633,   192,   647,
-32768,   633,-32768,-32768,-32768,-32768,   676,  3134,   649,   192,
   677,   187,-32768,   678,   656,-32768,   657,-32768,   684,-32768,
    65,-32768,   200,   192,-32768,   245,   668,-32768,-32768,   192,
-32768,-32768,   709,-32768,   189,   189,-32768,   451,   730,-32768,
   725,-32768,-32768,   726,-32768,    47,   189,-32768,   250,   309,
   335,-32768,   730,-32768,-32768,   452,-32768,   464,-32768,-32768,
   739,   740,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,   655,-32768,-32768,   741,   743,-32768,
-32768,-32768,   371,   748,-32768,-32768,   621,   629,   640,-32768,
  -120,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   661,-32768,
    -6,-32768,   651,-32768,   545,-32768,   539,   537,-32768,-32768,
   552,  -114,   573,  -179,-32768,   316,-32768,  -434,-32768,   188,
  -522,   215,   -77,   180,   178,   255,  -420,   166,-32768,  -165,
-32768,-32768,  -406,-32768,  -251,  -288,  -326,-32768,-32768,-32768,
-32768,-32768,   432,-32768,-32768,   -76,-32768,-32768,-32768,    -3,
   -18,-32768,    45,-32768,-32768,   440,   524,   -79,-32768,-32768,
    63,-32768,   -93,  -135,   369,   443,  -174,   287,  -189,  -212,
  -101,   569,   -32,  -257,   643,  -137,-32768,   580,-32768,-32768,
-32768,  -532,  -201,  -254,-32768,   137,   -95,   508,-32768,-32768,
  -282,   505,-32768,-32768,   515,  -264,  -173,-32768,-32768,  -243,
-32768,  -297,-32768,-32768,   278,     7,   -23,   -38,   455,   694,
  -373,    15,   577
};


#define	YYLAST		3247


static const short yytable[] = {   117,
    99,   102,   112,   195,   185,   147,   208,   210,    15,    19,
   412,   197,   295,   179,    15,   295,   394,   111,   251,   140,
   119,   202,    19,   378,   270,   340,   342,   274,     2,   116,
   106,   131,   384,   496,   390,   444,   388,   398,   399,   244,
   441,   390,   586,   594,   335,   207,   218,   220,   100,   669,
   319,   530,   544,   323,   103,   541,   326,   509,   109,   125,
   150,   126,   605,   360,   327,   610,   101,   442,   601,   179,
   642,   650,   644,   128,   425,   136,   177,   380,   569,   104,
   150,   128,   128,   672,   627,     3,   139,   150,   525,   562,
   329,   370,   380,   332,   141,   639,   545,   150,   216,   217,
   150,   651,   262,   150,   150,   174,   218,   220,   218,   220,
    15,   107,   487,   -55,    19,   349,   580,   358,   517,   518,
    99,   219,   217,   116,  -101,   275,   179,   350,   468,   212,
   470,   483,   231,    99,    99,   484,   108,   343,   217,   344,
   179,   179,   340,   342,   497,   591,   468,   179,   276,  -101,
   214,     9,   563,   391,   213,   255,    10,   150,   468,   392,
   592,    19,   263,   247,   428,   234,   346,   226,   100,   221,
   227,   273,   390,   434,   150,   578,   579,   430,   111,   436,
   213,   100,   100,   471,   474,   482,   101,   213,   620,   177,
   622,   655,   642,   643,   644,   461,   213,   270,   656,   101,
   101,   630,   417,   177,   177,   395,   540,   216,   217,   630,
   177,   317,   632,   631,   150,   532,   631,   502,   174,   208,
   210,   645,   123,   641,   631,   340,     8,   493,   282,   283,
   284,   285,   174,   174,   652,   330,   331,   654,   344,   500,
   344,   158,   213,     9,   159,   130,     9,   655,    10,   160,
   161,   115,   162,   151,   656,   522,   218,   440,     9,   656,
   163,   584,   527,    10,   164,   165,    11,   373,   373,   631,
   317,   262,   166,   512,   631,   568,   152,   167,   150,   456,
   523,   657,   153,   589,   282,   283,   284,   285,   603,   282,
   283,   284,   285,   168,   458,   182,   458,   278,   279,   411,
   488,   489,   208,   219,   217,   401,   402,     9,   590,   163,
   280,   655,   281,   213,   587,   180,   150,   150,   656,   187,
   403,   166,   404,   405,   406,   407,   150,   282,   283,   284,
   285,   570,   408,   631,   150,   440,   482,   655,   343,   217,
   626,   286,   155,   467,   656,   675,   288,   156,   282,   283,
   284,   285,  -404,    64,   184,    66,   188,    68,    69,   631,
   189,    71,    72,    73,    74,   538,   665,   666,    77,    78,
   373,   677,   373,   198,   282,   283,   284,   285,   673,  -247,
   674,    16,   473,   111,   355,   439,   217,   199,   356,   217,
   602,     9,     9,   289,   290,   557,    10,    10,   357,   150,
   291,   142,   660,   130,   482,   143,   490,   513,   494,   495,
   623,   660,   660,   490,   158,   144,   326,   159,   676,   678,
   130,   278,   279,   660,   327,   660,   660,   660,   463,   561,
   317,     9,   280,   163,   349,   515,   281,   490,   158,   438,
   222,   159,   223,   356,   217,   166,   350,   278,    15,   233,
   530,   282,   283,   284,   285,   531,   538,     9,   130,   537,
   593,   539,   281,   235,   583,   286,   287,   238,   490,   243,
   288,   239,   217,   236,   111,   237,   530,   282,   283,   284,
   285,   543,   667,   667,   150,   349,   326,   668,   679,   467,
   158,   286,   245,   159,   327,   667,   288,   350,   228,   229,
   680,   162,   597,   554,   246,   574,   555,   599,   248,   163,
   209,   211,   558,   230,   132,   134,   214,   289,   290,   150,
   256,   166,   337,   339,   291,   567,   167,   485,   486,   133,
   135,   257,   490,   264,   111,   325,   150,   271,   258,   259,
   272,   575,   333,   289,   290,   352,   334,   581,   354,   353,
   291,   574,   363,   364,   365,   490,   490,   368,   371,   158,
    15,   567,   159,   372,   383,   385,   278,   279,   386,   387,
   400,   390,   413,   414,   415,   416,     9,   418,   163,   419,
   635,   281,   420,   421,   424,   448,   427,   567,   422,   451,
   166,   635,   423,   453,   431,   634,   282,   283,   284,   285,
   426,   433,   111,   449,   635,   635,   634,   662,   503,   460,
   286,   635,   452,   462,   504,   288,   662,   662,   519,   634,
   634,   505,   506,   664,   526,   529,   634,   528,   662,   533,
   662,   662,   662,   158,  -405,   534,   159,   535,    21,   536,
   278,   279,   542,   548,   549,   550,   551,   552,   553,   560,
   472,   280,   163,   576,   582,   281,   544,   280,   588,   598,
   604,   612,   289,   290,   166,   613,   608,   606,   619,   291,
   282,   283,   284,   285,   617,   614,   595,   621,   624,   625,
   628,   636,   638,   640,   286,   287,  -406,   646,   647,   288,
   649,   648,   663,   615,   476,   477,   478,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    44,    45,   644,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,   110,    57,    58,
    59,    60,   669,    61,   670,   671,   289,   290,   682,   683,
   138,     7,   454,   291,   192,    64,    84,    66,    67,    68,
    69,    86,   193,    71,    72,    73,    74,    75,   157,    76,
    77,    78,   158,   194,   154,   159,   324,    21,   321,   278,
   279,   401,   402,   338,   336,   524,   585,   600,   611,   472,
   280,   163,   163,   616,   281,   277,   403,   637,   404,   405,
   406,   407,   571,   166,   166,   376,   447,   437,   408,   282,
   283,   284,   285,   475,   328,   564,   393,   232,   389,   409,
   183,   347,     0,   286,   287,     0,     0,     0,   288,   348,
     0,     0,     0,   476,   477,   478,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,   110,    57,    58,    59,
    60,     0,    61,     0,     0,   289,   290,     0,     0,     0,
     0,     0,   291,     0,    64,     0,    66,    67,    68,    69,
     0,     0,    71,    72,    73,    74,    75,     0,    76,    77,
    78,   158,     0,     0,   159,     0,    21,     0,   278,   279,
     0,     0,     0,     0,     0,     0,     0,     0,   472,   280,
   163,     0,     0,   281,     0,     0,     0,     0,     0,     0,
     0,     0,   166,     0,     0,     0,     0,     0,   282,   283,
   284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   286,   287,     0,     0,     0,   288,     0,     0,
     0,     0,   476,   477,   478,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,     0,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,   110,    57,    58,    59,    60,
     0,    61,     0,     0,   289,   290,     0,     0,     0,     0,
     0,   291,     0,    64,     0,    66,    67,    68,    69,     0,
     0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
   158,     0,     0,   159,     0,    21,     0,   278,   279,     0,
     0,     0,     0,     0,     0,     0,     0,   472,   280,   163,
     0,     0,   281,     0,     0,     0,     0,     0,     0,     0,
     0,   166,     0,     0,     0,     0,     0,   282,   283,   284,
   285,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   286,   287,     0,     0,     0,   288,     0,     0,     0,
     0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,     0,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,   110,    57,    58,    59,    60,     0,
    61,     0,     0,   289,   290,     0,     0,     0,     0,     0,
   291,     0,    64,     0,    66,    67,    68,    69,     0,     0,
    71,    72,    73,    74,    75,     0,    76,    77,    78,   158,
     0,     0,   159,     0,    21,     0,     0,   160,   161,     0,
   162,     0,     0,     0,     0,     0,    22,     0,   163,     0,
     0,     0,   164,   165,     0,     0,     0,     0,     0,    64,
   166,    66,     0,    68,    69,   167,     0,    71,    72,    73,
    74,     0,     0,     0,    77,    78,     0,     0,     0,     0,
     0,   168,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   169,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,   110,    57,    58,    59,    60,    21,    61,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    22,    64,     0,    66,    67,    68,    69,     0,     0,    71,
    72,    73,    74,    75,     0,    76,    77,    78,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
    24,     0,     0,     0,     0,     0,    25,     0,     0,     0,
     0,     0,    26,     0,     0,     0,     0,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    44,    45,     0,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,     0,    61,     0,     0,     0,     0,    62,     0,
    21,     0,    63,     0,     3,    64,    65,    66,    67,    68,
    69,    70,    22,    71,    72,    73,    74,    75,     0,    76,
    77,    78,    79,    80,    81,    82,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    23,     0,    24,     0,     0,     0,     0,     0,    25,     0,
     0,     0,     0,     0,    26,     0,     0,     0,     0,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    42,    43,    44,    45,     0,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    21,    61,     0,     0,     0,     0,
    62,     0,     0,     0,    63,     0,    22,    64,    65,    66,
    67,    68,    69,    70,     0,    71,    72,    73,    74,    75,
     0,    76,    77,    78,    79,    80,    81,    82,     0,     0,
     0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
     0,     0,    25,     0,     0,     0,     0,     0,    26,     0,
     0,     0,     0,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,     0,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,     0,    61,
     0,     0,     0,     0,     0,    21,     0,     0,    63,     0,
     0,    64,    65,    66,    67,    68,    69,    22,     0,    71,
    72,    73,    74,    75,     0,    76,    77,    78,    79,    80,
    81,    82,   556,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,     0,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,   110,    57,    58,    59,    60,   438,
    61,    21,     0,   356,   217,     0,     0,     0,     0,     0,
     0,     0,    64,    22,    66,    67,    68,    69,   130,     0,
    71,    72,    73,    74,    75,     0,    76,    77,    78,     0,
    80,    81,    82,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,     0,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
   110,    57,    58,    59,    60,   238,    61,    21,     0,   239,
   217,     0,     0,     0,     0,     0,     0,     0,    64,    22,
    66,    67,    68,    69,     0,     0,    71,    72,    73,    74,
    75,   249,    76,    77,    78,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,   110,    57,    58,    59,
    60,    21,    61,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    22,    64,     0,    66,    67,    68,    69,
     0,     0,    71,    72,    73,    74,    75,   249,    76,    77,
    78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   313,   314,     0,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,     0,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
   110,    57,    58,    59,    60,     0,    61,    21,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    64,    22,
    66,    67,    68,    69,     0,     0,    71,    72,    73,    74,
    75,     0,    76,    77,    78,     0,   318,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,   110,    57,    58,    59,
    60,     0,    61,    21,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    64,    22,    66,    67,    68,    69,
     0,     0,    71,    72,    73,    74,    75,   249,    76,    77,
    78,     0,   322,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,     0,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,   110,    57,    58,    59,    60,     0,    61,    21,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    64,    22,    66,    67,    68,    69,     0,     0,    71,    72,
    73,    74,    75,   249,    76,    77,    78,     0,   429,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,     0,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    55,   110,    57,
    58,    59,    60,     0,    61,    21,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    64,    22,    66,    67,
    68,    69,     0,     0,    71,    72,    73,    74,    75,   249,
    76,    77,    78,     0,   511,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    43,    44,    45,     0,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,   110,    57,    58,    59,    60,    21,
    61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    22,    64,     0,    66,    67,    68,    69,     0,     0,
    71,    72,    73,    74,    75,   249,    76,    77,    78,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,     0,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    55,   110,    57,
    58,    59,    60,    21,    61,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    22,    64,     0,    66,    67,
    68,    69,     0,     0,    71,    72,    73,    74,    75,   249,
    76,    77,    78,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,     0,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,   110,    57,    58,    59,    60,    21,    61,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
    64,     0,    66,    67,    68,    69,     0,   266,    71,    72,
    73,    74,    75,     0,    76,    77,    78,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,   110,    57,    58,    59,
    60,     0,    61,     0,     0,     0,     0,     0,    21,     0,
     0,     0,     0,     0,    64,     0,    66,    67,    68,    69,
    22,     0,    71,    72,    73,    74,    75,   435,    76,    77,
    78,     0,     0,     0,     0,   457,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    44,    45,     0,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,   110,    57,    58,
    59,    60,     0,    61,     0,     0,     0,     0,     0,    21,
     0,     0,     0,     0,     0,    64,     0,    66,    67,    68,
    69,    22,     0,    71,    72,    73,    74,    75,     0,    76,
    77,    78,     0,     0,     0,     0,   459,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,     0,    46,    47,
    48,    49,    50,    51,    52,    53,    54,    55,   110,    57,
    58,    59,    60,    21,    61,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    22,    64,     0,    66,    67,
    68,    69,     0,     0,    71,    72,    73,    74,    75,     0,
    76,    77,    78,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   565,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,     0,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,   110,    57,    58,    59,    60,    21,    61,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
    64,     0,    66,    67,    68,    69,     0,     0,    71,    72,
    73,    74,    75,     0,    76,    77,    78,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
    50,    51,    52,    53,    54,    55,   110,    57,    58,    59,
    60,     0,    61,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    64,     0,    66,    67,    68,    69,
     0,     0,    71,    72,    73,    74,    75,     0,    76,    77,
    78,   158,     0,     0,   159,     0,     0,     0,   278,   279,
     0,     0,     0,     0,     0,     0,     0,     0,     9,   280,
   163,     0,     0,   281,     0,     0,     0,     0,   158,     0,
     0,   159,   166,     0,   320,   278,   279,     0,   282,   283,
   284,   285,     0,     0,     0,     9,   280,   163,     0,     0,
   281,     0,   286,   287,     0,     0,     0,   288,     0,   166,
     0,   379,     0,     0,     0,   282,   283,   284,   285,   158,
     0,     0,   159,     0,     0,     0,   278,   279,     0,   286,
   287,     0,     0,     0,   288,     0,     9,   280,   163,     0,
     0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
   166,     0,   492,     0,   289,   290,   282,   283,   284,   285,
     0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
   286,   287,     0,     0,     0,   288,   158,     0,     0,   159,
     0,   289,   290,   278,   279,     0,     0,     0,   291,     0,
     0,     0,     0,     9,   280,   163,     0,     0,   281,     0,
     0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
     0,     0,     0,   282,   283,   284,   285,     0,     0,     0,
     0,     0,   289,   290,     0,     0,     0,   286,   287,   291,
     0,     0,   288,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
   290,     0,     0,     0,     0,     0,   291
};

static const short yycheck[] = {    23,
     4,     8,    21,   124,   119,    99,   142,   143,     2,     3,
   308,   126,   214,   109,     8,   217,   299,    21,   184,    96,
    24,   136,    16,   278,   199,   238,   239,   207,    57,    23,
    16,    70,   287,   407,    14,   362,   294,   302,   303,   177,
     7,    14,   565,   576,   234,   139,   148,   149,     4,     3,
   216,    32,    60,   219,     7,    36,    24,   431,    10,    63,
    99,    65,   595,   253,    32,   598,     4,    34,   591,   165,
     6,     7,     8,    67,   332,    79,   109,   279,    51,    32,
   119,    75,    76,    37,   617,   114,    90,   126,   462,   524,
   228,   266,   294,   231,    98,   628,   104,   136,    10,    11,
   139,    37,   196,   142,   143,   109,   208,   209,   210,   211,
   104,     7,   395,    97,   108,    23,   551,   253,   445,   446,
   124,    10,    11,   117,     7,    37,   222,    35,   383,     7,
   110,   389,   165,   137,   138,   390,    32,    10,    11,   241,
   236,   237,   355,   356,   409,    31,   401,   243,    37,    32,
    33,    20,   526,    33,    32,     7,    25,   196,   413,    39,
    46,   155,     7,   182,    37,   169,   243,   161,   124,   155,
   164,     7,    14,   353,   213,   549,   550,   343,   182,   354,
    32,   137,   138,   385,   386,   387,   124,    32,   609,   222,
   611,     3,     6,     7,     8,    37,    32,   372,    10,   137,
   138,    10,   317,   236,   237,   301,   489,    10,    11,    10,
   243,   215,   619,    25,   253,   470,    25,     7,   222,   355,
   356,    35,    62,   630,    25,   438,     3,   401,    40,    41,
    42,    43,   236,   237,    35,   229,   230,   644,   340,   413,
   342,     3,    32,    20,     6,    25,    20,     3,    25,    11,
    12,    25,    14,     7,    10,     7,   358,   359,    20,    10,
    22,   559,   464,    25,    26,    27,    43,   271,   272,    25,
   274,   365,    34,   439,    25,   530,     7,    39,   317,   373,
    32,    37,     3,     7,    40,    41,    42,    43,     7,    40,
    41,    42,    43,    55,   374,    31,   376,    10,    11,    10,
   396,   397,   438,    10,    11,    11,    12,    20,    32,    22,
    21,     3,    25,    32,   569,     6,   355,   356,    10,   111,
    26,    34,    28,    29,    30,    31,   365,    40,    41,    42,
    43,   533,    38,    25,   373,   437,   538,     3,    10,    11,
     7,    54,    32,   382,    10,    37,    59,    37,    40,    41,
    42,    43,    10,   115,    10,   117,   111,   119,   120,    25,
    97,   123,   124,   125,   126,    32,   655,   656,   130,   131,
   374,    37,   376,    34,    40,    41,    42,    43,   667,    34,
   669,    10,   386,   387,     6,    10,    11,    10,    10,    11,
   592,    20,    20,   106,   107,   516,    25,    25,    20,   438,
   113,     6,   646,    25,   606,    10,   400,    35,   402,   403,
   612,   655,   656,   407,     3,    20,    24,     6,   670,   671,
    25,    10,    11,   667,    32,   669,   670,   671,    36,   523,
   434,    20,    21,    22,    23,   442,    25,   431,     3,     6,
    10,     6,    27,    10,    11,    34,    35,    10,   442,    27,
    32,    40,    41,    42,    43,    37,    32,    20,    25,   478,
   575,    37,    25,    37,   558,    54,    55,     6,   462,    53,
    59,    10,    11,    32,   478,    32,    32,    40,    41,    42,
    43,    37,    32,    32,   523,    23,    24,    37,    37,   528,
     3,    54,    27,     6,    32,    32,    59,    35,    11,    12,
    37,    14,   580,    32,     6,   544,    35,   585,     7,    22,
   142,   143,   516,    26,    75,    76,    33,   106,   107,   558,
    20,    34,   236,   237,   113,   529,    39,   391,   392,    75,
    76,    20,   526,    25,   538,    36,   575,    34,   111,   111,
    34,   545,    27,   106,   107,     8,   110,   554,    32,    37,
   113,   590,    20,    20,    32,   549,   550,    32,    37,     3,
   554,   565,     6,    32,    10,    10,    10,    11,    10,    10,
    31,    14,    10,     7,     7,    34,    20,    37,    22,    36,
   619,    25,    37,    37,    36,     7,    37,   591,    27,    25,
    34,   630,    27,     7,    31,   619,    40,    41,    42,    43,
    27,    27,   606,    34,   643,   644,   630,   646,    27,    34,
    54,   650,    35,    31,    27,    59,   655,   656,    58,   643,
   644,    51,    37,   647,    31,    31,   650,    32,   667,    32,
   669,   670,   671,     3,    10,    37,     6,    37,     8,    37,
    10,    11,    37,    27,    26,    32,    34,    37,     7,    35,
    20,    21,    22,    49,     7,    25,    60,    21,    37,    10,
     7,    46,   106,   107,    34,     7,    47,    61,    34,   113,
    40,    41,    42,    43,    52,    45,   105,    37,     7,     7,
    48,    35,     7,    35,    54,    55,    10,    10,    33,    59,
     7,    35,    25,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,     8,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
   100,   101,     3,   103,    10,    10,   106,   107,     0,     0,
    86,     1,   372,   113,   124,   115,     4,   117,   118,   119,
   120,     4,   124,   123,   124,   125,   126,   127,   108,   129,
   130,   131,     3,   124,   104,     6,   222,     8,   217,    10,
    11,    11,    12,   237,   236,   460,   562,   590,   599,    20,
    21,    22,    22,   606,    25,   213,    26,   622,    28,    29,
    30,    31,   538,    34,    34,   272,   365,   355,    38,    40,
    41,    42,    43,    44,   225,   528,   299,   165,   294,   305,
   117,   243,    -1,    54,    55,    -1,    -1,    -1,    59,   243,
    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    -1,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,    -1,   103,    -1,    -1,   106,   107,    -1,    -1,    -1,
    -1,    -1,   113,    -1,   115,    -1,   117,   118,   119,   120,
    -1,    -1,   123,   124,   125,   126,   127,    -1,   129,   130,
   131,     3,    -1,    -1,     6,    -1,     8,    -1,    10,    11,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    41,
    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,
    -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
    82,    83,    84,    85,    -1,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
    -1,   103,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,
    -1,   113,    -1,   115,    -1,   117,   118,   119,   120,    -1,
    -1,   123,   124,   125,   126,   127,    -1,   129,   130,   131,
     3,    -1,    -1,     6,    -1,     8,    -1,    10,    11,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
   103,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
   113,    -1,   115,    -1,   117,   118,   119,   120,    -1,    -1,
   123,   124,   125,   126,   127,    -1,   129,   130,   131,     3,
    -1,    -1,     6,    -1,     8,    -1,    -1,    11,    12,    -1,
    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,
    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,   115,
    34,   117,    -1,   119,   120,    39,    -1,   123,   124,   125,
   126,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,
    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    66,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
    94,    95,    96,    97,    98,    99,   100,   101,     8,   103,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    20,   115,    -1,   117,   118,   119,   120,    -1,    -1,   123,
   124,   125,   126,   127,    -1,   129,   130,   131,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
   100,   101,    -1,   103,    -1,    -1,    -1,    -1,   108,    -1,
     8,    -1,   112,    -1,   114,   115,   116,   117,   118,   119,
   120,   121,    20,   123,   124,   125,   126,   127,    -1,   129,
   130,   131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    56,    -1,
    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    98,    99,   100,   101,     8,   103,    -1,    -1,    -1,    -1,
   108,    -1,    -1,    -1,   112,    -1,    20,   115,   116,   117,
   118,   119,   120,   121,    -1,   123,   124,   125,   126,   127,
    -1,   129,   130,   131,   132,   133,   134,   135,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,
    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    85,    -1,    87,    88,    89,    90,    91,    92,    93,
    94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
    -1,    -1,    -1,    -1,    -1,     8,    -1,    -1,   112,    -1,
    -1,   115,   116,   117,   118,   119,   120,    20,    -1,   123,
   124,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
   134,   135,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    99,   100,   101,     6,
   103,     8,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   115,    20,   117,   118,   119,   120,    25,    -1,
   123,   124,   125,   126,   127,    -1,   129,   130,   131,    -1,
   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,     6,   103,     8,    -1,    10,
    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    20,
   117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
   127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    -1,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,     8,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    20,   115,    -1,   117,   118,   119,   120,
    -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,    -1,   103,     8,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    20,
   117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
   127,    -1,   129,   130,   131,    -1,    37,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    -1,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,    -1,   103,     8,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   115,    20,   117,   118,   119,   120,
    -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
   131,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,    -1,   103,     8,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   115,    20,   117,   118,   119,   120,    -1,    -1,   123,   124,
   125,   126,   127,   128,   129,   130,   131,    -1,    37,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
    99,   100,   101,    -1,   103,     8,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   115,    20,   117,   118,
   119,   120,    -1,    -1,   123,   124,   125,   126,   127,   128,
   129,   130,   131,    -1,    37,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    99,   100,   101,     8,
   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    20,   115,    -1,   117,   118,   119,   120,    -1,    -1,
   123,   124,   125,   126,   127,   128,   129,   130,   131,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
    99,   100,   101,     8,   103,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    20,   115,    -1,   117,   118,
   119,   120,    -1,    -1,   123,   124,   125,   126,   127,   128,
   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,     8,   103,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
   115,    -1,   117,   118,   119,   120,    -1,   122,   123,   124,
   125,   126,   127,    -1,   129,   130,   131,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    -1,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,     8,    -1,
    -1,    -1,    -1,    -1,   115,    -1,   117,   118,   119,   120,
    20,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
   131,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
   100,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,     8,
    -1,    -1,    -1,    -1,    -1,   115,    -1,   117,   118,   119,
   120,    20,    -1,   123,   124,   125,   126,   127,    -1,   129,
   130,   131,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
    99,   100,   101,     8,   103,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    20,   115,    -1,   117,   118,
   119,   120,    -1,    -1,   123,   124,   125,   126,   127,    -1,
   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,     8,   103,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
   115,    -1,   117,   118,   119,   120,    -1,    -1,   123,   124,
   125,   126,   127,    -1,   129,   130,   131,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    -1,    87,    88,    89,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   115,    -1,   117,   118,   119,   120,
    -1,    -1,   123,   124,   125,   126,   127,    -1,   129,   130,
   131,     3,    -1,    -1,     6,    -1,    -1,    -1,    10,    11,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,     3,    -1,
    -1,     6,    34,    -1,    36,    10,    11,    -1,    40,    41,
    42,    43,    -1,    -1,    -1,    20,    21,    22,    -1,    -1,
    25,    -1,    54,    55,    -1,    -1,    -1,    59,    -1,    34,
    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,     3,
    -1,    -1,     6,    -1,    -1,    -1,    10,    11,    -1,    54,
    55,    -1,    -1,    -1,    59,    -1,    20,    21,    22,    -1,
    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    34,    -1,    36,    -1,   106,   107,    40,    41,    42,    43,
    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    54,    55,    -1,    -1,    -1,    59,     3,    -1,    -1,     6,
    -1,   106,   107,    10,    11,    -1,    -1,    -1,   113,    -1,
    -1,    -1,    -1,    20,    21,    22,    -1,    -1,    25,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
    -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
    -1,    -1,   106,   107,    -1,    -1,    -1,    54,    55,   113,
    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
   107,    -1,    -1,    -1,    -1,    -1,   113
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 372 "llgrammar.y"
{ lhExternals (yyvsp[0].interfacelist); ;
    break;}
case 2:
#line 373 "llgrammar.y"
{ interfaceNodeList_free (yyvsp[-2].interfacelist); interfaceNodeList_free (yyvsp[0].interfacelist); ;
    break;}
case 3:
#line 376 "llgrammar.y"
{ yyval.interfacelist = interfaceNodeList_new (); ;
    break;}
case 4:
#line 377 "llgrammar.y"
{ yyval.interfacelist = interfaceNodeList_addh (yyvsp[-1].interfacelist, yyvsp[0].iface);;
    break;}
case 7:
#line 384 "llgrammar.y"
{ yyval.interfacelist = interfaceNodeList_new (); ;
    break;}
case 8:
#line 385 "llgrammar.y"
{ yyval.interfacelist = consInterfaceNode (yyvsp[-1].iface, yyvsp[0].interfacelist);;
    break;}
case 9:
#line 386 "llgrammar.y"
{ yyval.interfacelist = consInterfaceNode (yyvsp[-1].iface, yyvsp[0].interfacelist);;
    break;}
case 10:
#line 389 "llgrammar.y"
{ yyval.interfacelist = interfaceNodeList_new (); ;
    break;}
case 11:
#line 390 "llgrammar.y"
{ yyval.interfacelist = interfaceNodeList_addh (yyvsp[-1].interfacelist, yyvsp[0].iface);;
    break;}
case 15:
#line 399 "llgrammar.y"
{ yyval.iface = makeInterfaceNodeImports (yyvsp[-1].importlist);
     /* assume subspecs are already processed, symbol table info in external file */
   ;
    break;}
case 16:
#line 405 "llgrammar.y"
{ yyval.iface = makeInterfaceNodeUses (yyvsp[-1].traitreflist); readlsignatures (yyval.iface);;
    break;}
case 17:
#line 409 "llgrammar.y"
{ declareConstant (yyvsp[0].constdeclaration); yyval.iface = interfaceNode_makeConst (yyvsp[0].constdeclaration); ;
    break;}
case 18:
#line 411 "llgrammar.y"
{ declareVar (yyvsp[0].vardeclaration); yyval.iface = interfaceNode_makeVar (yyvsp[0].vardeclaration); ;
    break;}
case 19:
#line 413 "llgrammar.y"
{ declareType (yyvsp[0].type); yyval.iface = interfaceNode_makeType (yyvsp[0].type); ;
    break;}
case 20:
#line 415 "llgrammar.y"
{ declareFcn (yyvsp[0].fcn, typeId_invalid); yyval.iface = interfaceNode_makeFcn (yyvsp[0].fcn); ;
    break;}
case 21:
#line 417 "llgrammar.y"
{ yyval.iface = interfaceNode_makeClaim (yyvsp[0].claim); ;
    break;}
case 22:
#line 419 "llgrammar.y"
{ declareIter (yyvsp[0].iter); yyval.iface = interfaceNode_makeIter (yyvsp[0].iter); ;
    break;}
case 23:
#line 423 "llgrammar.y"
{ yyval.iter = makeIterNode (yyvsp[-4].ltok, yyvsp[-2].paramlist); ;
    break;}
case 24:
#line 426 "llgrammar.y"
{ yyval.paramlist = paramNodeList_new (); ;
    break;}
case 25:
#line 427 "llgrammar.y"
{ yyval.paramlist = yyvsp[0].paramlist; ;
    break;}
case 26:
#line 431 "llgrammar.y"
{ yyval.paramlist = paramNodeList_add (paramNodeList_new (), yyvsp[0].param); ;
    break;}
case 27:
#line 433 "llgrammar.y"
{ yyval.paramlist = paramNodeList_add (yyvsp[-2].paramlist,yyvsp[0].param); ;
    break;}
case 28:
#line 436 "llgrammar.y"
{ yyval.param = markYieldParamNode (yyvsp[0].param); ;
    break;}
case 29:
#line 437 "llgrammar.y"
{ yyval.param = yyvsp[0].param; ;
    break;}
case 30:
#line 440 "llgrammar.y"
{ symtable_export (g_symtab, FALSE); ;
    break;}
case 31:
#line 441 "llgrammar.y"
{ yyval.iface = yyvsp[0].iface; symtable_export (g_symtab, TRUE); ;
    break;}
case 32:
#line 445 "llgrammar.y"
{ declarePrivConstant (yyvsp[0].constdeclaration); yyval.iface =  interfaceNode_makePrivConst (yyvsp[0].constdeclaration); ;
    break;}
case 33:
#line 447 "llgrammar.y"
{ declarePrivVar (yyvsp[0].vardeclaration); yyval.iface = interfaceNode_makePrivVar (yyvsp[0].vardeclaration); ;
    break;}
case 34:
#line 449 "llgrammar.y"
{ declarePrivType (yyvsp[0].type); yyval.iface = interfaceNode_makePrivType (yyvsp[0].type); ;
    break;}
case 35:
#line 451 "llgrammar.y"
{ declarePrivFcn (yyvsp[0].fcn, typeId_invalid); yyval.iface = interfaceNode_makePrivFcn (yyvsp[0].fcn); ;
    break;}
case 36:
#line 455 "llgrammar.y"
{ yyval.constdeclaration = makeConstDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls); ;
    break;}
case 37:
#line 459 "llgrammar.y"
{ yyval.vardeclaration = makeVarDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls, FALSE, FALSE); yyval.vardeclaration->qualifier = QLF_NONE; ;
    break;}
case 38:
#line 461 "llgrammar.y"
{ yyval.vardeclaration = makeVarDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls, FALSE, FALSE); yyval.vardeclaration->qualifier = QLF_CONST; ;
    break;}
case 39:
#line 463 "llgrammar.y"
{ yyval.vardeclaration = makeVarDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls, FALSE, FALSE); yyval.vardeclaration->qualifier = QLF_VOLATILE; ;
    break;}
case 40:
#line 466 "llgrammar.y"
{ yyval.type = makeAbstractTypeNode (yyvsp[0].abstract); ;
    break;}
case 41:
#line 467 "llgrammar.y"
{ yyval.type = makeExposedTypeNode (yyvsp[0].exposed); ;
    break;}
case 42:
#line 470 "llgrammar.y"
{ yyval.typequal = qual_createPrintfLike (); ;
    break;}
case 43:
#line 471 "llgrammar.y"
{ yyval.typequal = qual_createScanfLike (); ;
    break;}
case 44:
#line 472 "llgrammar.y"
{ yyval.typequal = qual_createMessageLike (); ;
    break;}
case 45:
#line 475 "llgrammar.y"
{ enteringFcnScope (yyvsp[-2].lcltypespec, yyvsp[-1].declare, yyvsp[0].globals); ;
    break;}
case 46:
#line 477 "llgrammar.y"
{ yyval.fcn = makeFcnNode (qual_createUnknown (), yyvsp[-12].lcltypespec, yyvsp[-11].declare, yyvsp[-10].globals, yyvsp[-7].vardeclarationlist, yyvsp[-6].letdecls, 
		       yyvsp[-5].lclpredicate, yyvsp[-4].lclpredicate, yyvsp[-3].modify, yyvsp[-2].lclpredicate, yyvsp[-1].lclpredicate); 
     /* type, declarator, glovbls, privateinits,
	lets, checks, requires, modifies, ensures, claims */
     symtable_exitScope (g_symtab);
   ;
    break;}
case 47:
#line 483 "llgrammar.y"
{ enteringFcnScope (yyvsp[-2].lcltypespec, yyvsp[-1].declare, yyvsp[0].globals); ;
    break;}
case 48:
#line 487 "llgrammar.y"
{ yyval.fcn = makeFcnNode (yyvsp[-13].typequal, yyvsp[-12].lcltypespec, yyvsp[-11].declare, yyvsp[-10].globals, yyvsp[-7].vardeclarationlist, 
		       yyvsp[-6].letdecls, yyvsp[-5].lclpredicate, yyvsp[-4].lclpredicate, yyvsp[-3].modify, yyvsp[-2].lclpredicate, yyvsp[-1].lclpredicate); 
     /* type, declarator, glovbls, privateinits,
	lets, checks, requires, modifies, ensures, claims */
     symtable_exitScope (g_symtab);
   ;
    break;}
case 49:
#line 497 "llgrammar.y"
{ enteringClaimScope (yyvsp[-2].paramlist, yyvsp[0].globals); ;
    break;}
case 50:
#line 499 "llgrammar.y"
{      yyval.claim = makeClaimNode (yyvsp[-11].ltok, yyvsp[-9].paramlist, yyvsp[-7].globals, yyvsp[-4].letdecls, yyvsp[-3].lclpredicate, yyvsp[-2].program, yyvsp[-1].lclpredicate); 
     symtable_exitScope (g_symtab); ;
    break;}
case 51:
#line 502 "llgrammar.y"
{ yyval.claim = (claimNode) 0; ;
    break;}
case 52:
#line 505 "llgrammar.y"
{g_inTypeDef = TRUE; ;
    break;}
case 53:
#line 505 "llgrammar.y"
{g_inTypeDef = FALSE; ;
    break;}
case 54:
#line 506 "llgrammar.y"
{ yyval.abstract = makeAbstractNode (yyvsp[-5].ltok, yyvsp[-2].ltok, TRUE, FALSE, yyvsp[0].abstbody); ;
    break;}
case 55:
#line 507 "llgrammar.y"
{g_inTypeDef = TRUE; ;
    break;}
case 56:
#line 508 "llgrammar.y"
{g_inTypeDef = FALSE; ;
    break;}
case 57:
#line 509 "llgrammar.y"
{ yyval.abstract = makeAbstractNode (yyvsp[-6].ltok, yyvsp[-2].ltok, TRUE, TRUE, yyvsp[0].abstbody); ;
    break;}
case 58:
#line 510 "llgrammar.y"
{g_inTypeDef = TRUE; ;
    break;}
case 59:
#line 511 "llgrammar.y"
{g_inTypeDef = FALSE; ;
    break;}
case 60:
#line 512 "llgrammar.y"
{ yyval.abstract = makeAbstractNode (yyvsp[-5].ltok, yyvsp[-2].ltok, TRUE, TRUE, yyvsp[0].abstbody); ;
    break;}
case 61:
#line 513 "llgrammar.y"
{g_inTypeDef = TRUE; ;
    break;}
case 62:
#line 513 "llgrammar.y"
{g_inTypeDef = FALSE; ;
    break;}
case 63:
#line 514 "llgrammar.y"
{ yyval.abstract = makeAbstractNode (yyvsp[-5].ltok, yyvsp[-2].ltok, FALSE, FALSE, yyvsp[0].abstbody); ;
    break;}
case 64:
#line 517 "llgrammar.y"
{ g_inTypeDef = TRUE; setExposedType (yyvsp[0].lcltypespec); ;
    break;}
case 65:
#line 518 "llgrammar.y"
{ g_inTypeDef = FALSE; ;
    break;}
case 66:
#line 519 "llgrammar.y"
{ yyval.exposed = makeExposedNode (yyvsp[-5].ltok, yyvsp[-4].lcltypespec, yyvsp[-2].declaratorinvs); /* to support mutually recursive types */ ;
    break;}
case 67:
#line 521 "llgrammar.y"
{ yyval.exposed = makeExposedNode (yyvsp[0].ltok, makeLclTypeSpecNodeSU (yyvsp[-1].structorunion), declaratorInvNodeList_new ()); ;
    break;}
case 68:
#line 523 "llgrammar.y"
{ yyval.exposed = makeExposedNode (yyvsp[0].ltok, makeLclTypeSpecNodeEnum (yyvsp[-1].enumspec), declaratorInvNodeList_new ()); ;
    break;}
case 69:
#line 539 "llgrammar.y"
{ yyval.importlist = importNodeList_add (importNodeList_new (), yyvsp[0].import); ;
    break;}
case 70:
#line 541 "llgrammar.y"
{ yyval.importlist = importNodeList_add (yyvsp[-2].importlist, yyvsp[0].import); ;
    break;}
case 71:
#line 544 "llgrammar.y"
{ yyval.import = importNode_makePlain (yyvsp[0].ltok); ;
    break;}
case 72:
#line 546 "llgrammar.y"
{ checkBrackets (yyvsp[-2].ltok, yyvsp[0].ltok); yyval.import = importNode_makeBracketed (yyvsp[-1].ltok); ;
    break;}
case 73:
#line 547 "llgrammar.y"
{ yyval.import = importNode_makeQuoted (yyvsp[0].ltok); ;
    break;}
case 74:
#line 550 "llgrammar.y"
{ yyval.ltokenList = ltokenList_singleton (yyvsp[0].ltok); ;
    break;}
case 75:
#line 551 "llgrammar.y"
{ yyval.ltokenList = ltokenList_push (yyvsp[-2].ltokenList, yyvsp[0].ltok); ;
    break;}
case 77:
#line 559 "llgrammar.y"
{ yyval.traitreflist = traitRefNodeList_add (traitRefNodeList_new (), yyvsp[0].traitref); ;
    break;}
case 78:
#line 561 "llgrammar.y"
{ yyval.traitreflist = traitRefNodeList_add (yyvsp[-2].traitreflist, yyvsp[0].traitref); ;
    break;}
case 79:
#line 565 "llgrammar.y"
{ yyval.traitref = makeTraitRefNode (ltokenList_singleton (yyvsp[0].ltok), (renamingNode)0); ;
    break;}
case 80:
#line 567 "llgrammar.y"
{ yyval.traitref = makeTraitRefNode (ltokenList_singleton (yyvsp[-3].ltok), yyvsp[-1].renaming); ;
    break;}
case 81:
#line 569 "llgrammar.y"
{ yyval.traitref = makeTraitRefNode (yyvsp[-1].ltokenList, (renamingNode)0); ;
    break;}
case 82:
#line 571 "llgrammar.y"
{ yyval.traitref = makeTraitRefNode (yyvsp[-4].ltokenList, yyvsp[-1].renaming); ;
    break;}
case 83:
#line 574 "llgrammar.y"
{ yyval.ltokenList = ltokenList_singleton (yyvsp[0].ltok); ;
    break;}
case 84:
#line 575 "llgrammar.y"
{ yyval.ltokenList = ltokenList_push (yyvsp[-2].ltokenList, yyvsp[0].ltok); ;
    break;}
case 85:
#line 579 "llgrammar.y"
{ yyval.renaming = makeRenamingNode (typeNameNodeList_new (), yyvsp[0].replacelist); ;
    break;}
case 86:
#line 581 "llgrammar.y"
{ yyval.renaming = makeRenamingNode (yyvsp[0].namelist, replaceNodeList_new ()); ;
    break;}
case 87:
#line 582 "llgrammar.y"
{ yyval.renaming = makeRenamingNode (yyvsp[-2].namelist, yyvsp[0].replacelist); ;
    break;}
case 88:
#line 586 "llgrammar.y"
{ yyval.namelist = typeNameNodeList_add (typeNameNodeList_new (), yyvsp[0].typname); ;
    break;}
case 89:
#line 587 "llgrammar.y"
{ yyval.namelist = typeNameNodeList_add (yyvsp[-2].namelist, yyvsp[0].typname); ;
    break;}
case 90:
#line 591 "llgrammar.y"
{ yyval.replacelist = replaceNodeList_add (replaceNodeList_new (), yyvsp[0].replace); ;
    break;}
case 91:
#line 592 "llgrammar.y"
{ yyval.replacelist = replaceNodeList_add (yyvsp[-2].replacelist, yyvsp[0].replace); ;
    break;}
case 92:
#line 595 "llgrammar.y"
{ yyval.replace = makeReplaceNode (yyvsp[-1].ltok, yyvsp[-2].typname, TRUE, yyvsp[0].ltok, NULL, NULL); ;
    break;}
case 93:
#line 596 "llgrammar.y"
{ yyval.replace = makeReplaceNameNode (yyvsp[-1].ltok, yyvsp[-2].typname, yyvsp[0].name); ;
    break;}
case 94:
#line 597 "llgrammar.y"
{ yyval.replace = makeReplaceNode (yyvsp[-2].ltok, yyvsp[-3].typname, FALSE, ltoken_undefined,
							 yyvsp[-1].name, yyvsp[0].signature); ;
    break;}
case 95:
#line 601 "llgrammar.y"
{ yyval.name = makeNameNodeId (yyvsp[0].ltok); ;
    break;}
case 96:
#line 602 "llgrammar.y"
{ yyval.name = makeNameNodeForm (yyvsp[0].opform); ;
    break;}
case 99:
#line 610 "llgrammar.y"
{ yyval.initdecls = initDeclNodeList_add (initDeclNodeList_new (), yyvsp[0].initdecl); ;
    break;}
case 100:
#line 612 "llgrammar.y"
{ yyval.initdecls = initDeclNodeList_add (yyvsp[-2].initdecls, yyvsp[0].initdecl); ;
    break;}
case 101:
#line 615 "llgrammar.y"
{ yyval.initdecl = makeInitDeclNode (yyvsp[0].declare, (termNode)0); ;
    break;}
case 102:
#line 616 "llgrammar.y"
{ yyval.initdecl = makeInitDeclNode (yyvsp[-2].declare, yyvsp[0].term); ;
    break;}
case 103:
#line 620 "llgrammar.y"
{ yyval.globals = varDeclarationNodeList_new (); ;
    break;}
case 104:
#line 622 "llgrammar.y"
{ varDeclarationNodeList_addh (yyvsp[-1].globals, yyvsp[0].vardeclaration); yyval.globals = yyvsp[-1].globals; ;
    break;}
case 105:
#line 625 "llgrammar.y"
{ yyval.vardeclaration = makeVarDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls, TRUE, FALSE); ;
    break;}
case 106:
#line 626 "llgrammar.y"
{ yyval.vardeclaration = makeInternalStateNode (); ;
    break;}
case 107:
#line 627 "llgrammar.y"
{ yyval.vardeclaration = makeFileSystemNode (); ;
    break;}
case 108:
#line 630 "llgrammar.y"
{ yyval.vardeclarationlist = varDeclarationNodeList_new (); ;
    break;}
case 109:
#line 631 "llgrammar.y"
{ varDeclarationNodeList_addh (yyvsp[-1].vardeclarationlist, yyvsp[0].vardeclaration); yyval.vardeclarationlist = yyvsp[-1].vardeclarationlist; ;
    break;}
case 110:
#line 635 "llgrammar.y"
{ yyval.vardeclaration = makeVarDeclarationNode (yyvsp[-2].lcltypespec, yyvsp[-1].initdecls, FALSE, TRUE); ;
    break;}
case 111:
#line 638 "llgrammar.y"
{ yyval.letdecls = letDeclNodeList_new (); ;
    break;}
case 112:
#line 639 "llgrammar.y"
{ yyval.letdecls = yyvsp[-1].letdecls; ;
    break;}
case 113:
#line 642 "llgrammar.y"
{ yyval.letdecls = letDeclNodeList_add (letDeclNodeList_new (), yyvsp[0].letdecl); ;
    break;}
case 114:
#line 643 "llgrammar.y"
{ yyval.letdecls = letDeclNodeList_add (yyvsp[-2].letdecls, yyvsp[0].letdecl); ;
    break;}
case 115:
#line 646 "llgrammar.y"
{ yyval.letdecl = makeLetDeclNode (yyvsp[-4].ltok, yyvsp[-2].lcltypespec, yyvsp[0].term); ;
    break;}
case 116:
#line 647 "llgrammar.y"
{ yyval.letdecl = makeLetDeclNode (yyvsp[-2].ltok, (lclTypeSpecNode)0, yyvsp[0].term); ;
    break;}
case 118:
#line 652 "llgrammar.y"
{ yyval.lclpredicate = (lclPredicateNode)0; ;
    break;}
case 119:
#line 653 "llgrammar.y"
{ checkLclPredicate (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); yyval.lclpredicate = makeChecksNode (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); ;
    break;}
case 120:
#line 656 "llgrammar.y"
{ yyval.lclpredicate = (lclPredicateNode)0; ;
    break;}
case 121:
#line 657 "llgrammar.y"
{ checkLclPredicate (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); yyval.lclpredicate = makeRequiresNode (yyvsp[-2].ltok, yyvsp[-1].lclpredicate);;
    break;}
case 122:
#line 660 "llgrammar.y"
{ yyval.modify = (modifyNode)0; ;
    break;}
case 123:
#line 661 "llgrammar.y"
{ yyval.modify = makeModifyNodeSpecial (yyvsp[-2].ltok, TRUE); ;
    break;}
case 124:
#line 662 "llgrammar.y"
{ yyval.modify = makeModifyNodeSpecial (yyvsp[-2].ltok, FALSE); ;
    break;}
case 125:
#line 663 "llgrammar.y"
{ yyval.modify = makeModifyNodeRef (yyvsp[-2].ltok, yyvsp[-1].storereflist); ;
    break;}
case 126:
#line 666 "llgrammar.y"
{ yyval.storereflist = storeRefNodeList_add (storeRefNodeList_new (), yyvsp[0].storeref); ;
    break;}
case 127:
#line 667 "llgrammar.y"
{ yyval.storereflist = storeRefNodeList_add (yyvsp[-2].storereflist, yyvsp[0].storeref); ;
    break;}
case 128:
#line 670 "llgrammar.y"
{ yyval.storeref = makeStoreRefNodeTerm (yyvsp[0].term); ;
    break;}
case 129:
#line 671 "llgrammar.y"
{ yyval.storeref = makeStoreRefNodeType (yyvsp[0].lcltypespec, FALSE); ;
    break;}
case 130:
#line 672 "llgrammar.y"
{ yyval.storeref = makeStoreRefNodeType (yyvsp[0].lcltypespec, TRUE); ;
    break;}
case 131:
#line 673 "llgrammar.y"
{ yyval.storeref = makeStoreRefNodeInternal (); ;
    break;}
case 132:
#line 674 "llgrammar.y"
{ yyval.storeref = makeStoreRefNodeSystem (); ;
    break;}
case 133:
#line 677 "llgrammar.y"
{ yyval.lclpredicate = (lclPredicateNode)0; ;
    break;}
case 134:
#line 678 "llgrammar.y"
{ checkLclPredicate (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); yyval.lclpredicate = makeEnsuresNode (yyvsp[-2].ltok, yyvsp[-1].lclpredicate);;
    break;}
case 135:
#line 681 "llgrammar.y"
{ yyval.lclpredicate = (lclPredicateNode)0; ;
    break;}
case 136:
#line 682 "llgrammar.y"
{ checkLclPredicate (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); yyval.lclpredicate = makeIntraClaimNode (yyvsp[-2].ltok, yyvsp[-1].lclpredicate);;
    break;}
case 137:
#line 685 "llgrammar.y"
{ yyval.paramlist = paramNodeList_new (); ;
    break;}
case 138:
#line 686 "llgrammar.y"
{ yyval.paramlist = yyvsp[0].paramlist; ;
    break;}
case 140:
#line 690 "llgrammar.y"
{ yyval.paramlist = paramNodeList_add (paramNodeList_new (), paramNode_elipsis ()); ;
    break;}
case 141:
#line 691 "llgrammar.y"
{ yyval.paramlist = paramNodeList_add (yyvsp[-2].paramlist, paramNode_elipsis ()); ;
    break;}
case 142:
#line 694 "llgrammar.y"
{ yyval.paramlist = paramNodeList_single (yyvsp[0].param); ;
    break;}
case 143:
#line 695 "llgrammar.y"
{ yyval.paramlist = paramNodeList_add (yyvsp[-2].paramlist, yyvsp[0].param); ;
    break;}
case 144:
#line 698 "llgrammar.y"
{ yyval.program = (programNode)0; ;
    break;}
case 145:
#line 699 "llgrammar.y"
{ yyval.program = yyvsp[-1].program; ;
    break;}
case 146:
#line 700 "llgrammar.y"
{ yyval.program = yyvsp[-2].program; ;
    break;}
case 147:
#line 703 "llgrammar.y"
{ yyval.program = makeProgramNode (yyvsp[0].stmt); ;
    break;}
case 148:
#line 706 "llgrammar.y"
{ yyval.program = yyvsp[-1].program; yyval.program->wrapped = yyval.program->wrapped + 1; ;
    break;}
case 149:
#line 708 "llgrammar.y"
{ programNodeList x = programNodeList_new ();
     programNodeList_addh (x, yyvsp[-1].program);
     yyval.program = makeProgramNodeAction (x, ACT_ITER); 
   ;
    break;}
case 150:
#line 713 "llgrammar.y"
{ programNodeList x = programNodeList_new ();
     programNodeList_addh (x, yyvsp[-2].program);
     programNodeList_addh (x, yyvsp[0].program);
     yyval.program = makeProgramNodeAction (x, ACT_ALTERNATE); 
   ;
    break;}
case 151:
#line 719 "llgrammar.y"
{ programNodeList x = programNodeList_new ();
     programNodeList_addh (x, yyvsp[-2].program);
     programNodeList_addh (x, yyvsp[0].program);
     yyval.program = makeProgramNodeAction (x, ACT_SEQUENCE); 
   ;
    break;}
case 152:
#line 727 "llgrammar.y"
{ yyval.stmt = makeStmtNode (ltoken_undefined, yyvsp[-3].ltok, yyvsp[-1].termlist); ;
    break;}
case 153:
#line 729 "llgrammar.y"
{ yyval.stmt = makeStmtNode (ltoken_undefined, yyvsp[-2].ltok, termNodeList_new ()); ;
    break;}
case 154:
#line 731 "llgrammar.y"
{ yyval.stmt = makeStmtNode (yyvsp[-4].ltok, yyvsp[-2].ltok, termNodeList_new ()); ;
    break;}
case 155:
#line 733 "llgrammar.y"
{ yyval.stmt = makeStmtNode (yyvsp[-5].ltok, yyvsp[-3].ltok, yyvsp[-1].termlist); ;
    break;}
case 156:
#line 736 "llgrammar.y"
{ yyval.termlist = termNodeList_push (termNodeList_new (), yyvsp[0].term); ;
    break;}
case 157:
#line 737 "llgrammar.y"
{ yyval.termlist = termNodeList_push (yyvsp[-2].termlist, yyvsp[0].term); ;
    break;}
case 159:
#line 741 "llgrammar.y"
{ yyval.term = makeSimpleTermNode (yyvsp[0].ltok); ;
    break;}
case 160:
#line 742 "llgrammar.y"
{ yyval.term = makePrefixTermNode (yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 161:
#line 743 "llgrammar.y"
{ yyval.term = makePostfixTermNode2 (yyvsp[-1].term, yyvsp[0].ltok); ;
    break;}
case 162:
#line 744 "llgrammar.y"
{ yyval.term = makeInfixTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 163:
#line 745 "llgrammar.y"
{ yyval.term = yyvsp[-1].term; yyval.term->wrapped = yyval.term->wrapped + 1; ;
    break;}
case 164:
#line 747 "llgrammar.y"
{ yyval.term = makeOpCallTermNode (yyvsp[-2].ltok, yyvsp[-1].ltok, termNodeList_new (), yyvsp[0].ltok); ;
    break;}
case 165:
#line 749 "llgrammar.y"
{ yyval.term = makeOpCallTermNode (yyvsp[-3].ltok, yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok); ;
    break;}
case 166:
#line 752 "llgrammar.y"
{ yyval.abstbody = (abstBodyNode)0; ;
    break;}
case 167:
#line 753 "llgrammar.y"
{ yyval.abstbody = makeAbstBodyNode (yyvsp[-2].ltok, yyvsp[-1].fcns); ;
    break;}
case 168:
#line 754 "llgrammar.y"
{ yyval.abstbody = makeAbstBodyNode2 (yyvsp[-3].ltok, yyvsp[-2].ltokenList); ;
    break;}
case 169:
#line 755 "llgrammar.y"
{ yyval.abstbody = (abstBodyNode)0; ;
    break;}
case 170:
#line 758 "llgrammar.y"
{ yyval.fcns = fcnNodeList_new (); ;
    break;}
case 171:
#line 759 "llgrammar.y"
{ yyval.fcns = fcnNodeList_add (yyvsp[-1].fcns, yyvsp[0].fcn); ;
    break;}
case 172:
#line 762 "llgrammar.y"
{ yyval.lclpredicate = (lclPredicateNode)0; ;
    break;}
case 174:
#line 766 "llgrammar.y"
{ g_inTypeDef = FALSE; ;
    break;}
case 175:
#line 767 "llgrammar.y"
{ yyvsp[-1].lclpredicate->tok = yyvsp[-5].ltok; yyvsp[-1].lclpredicate->kind = LPD_CONSTRAINT;
     checkLclPredicate (yyvsp[-5].ltok, yyvsp[-1].lclpredicate);
     yyval.lclpredicate = yyvsp[-1].lclpredicate;
     symtable_exitScope (g_symtab); 
     g_inTypeDef = TRUE;
   ;
    break;}
case 176:
#line 775 "llgrammar.y"
{ yyval.declaratorinvs = declaratorInvNodeList_add (declaratorInvNodeList_new (), yyvsp[0].declaratorinv); ;
    break;}
case 177:
#line 777 "llgrammar.y"
{ yyval.declaratorinvs = declaratorInvNodeList_add (yyvsp[-2].declaratorinvs, yyvsp[0].declaratorinv); ;
    break;}
case 178:
#line 780 "llgrammar.y"
{ declareForwardType (yyvsp[0].declare); ;
    break;}
case 179:
#line 781 "llgrammar.y"
{ yyval.declaratorinv = makeDeclaratorInvNode (yyvsp[-2].declare, yyvsp[0].abstbody); ;
    break;}
case 180:
#line 784 "llgrammar.y"
{ yyval.abstbody = (abstBodyNode)0; ;
    break;}
case 181:
#line 785 "llgrammar.y"
{ yyval.abstbody = makeExposedBodyNode (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); ;
    break;}
case 182:
#line 788 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_VOID, 0)); ;
    break;}
case 183:
#line 789 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_CHAR, 0)); ;
    break;}
case 184:
#line 790 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_DOUBLE, 0)); ;
    break;}
case 185:
#line 791 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_FLOAT, 0)); ;
    break;}
case 186:
#line 792 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_INT, 0)); ;
    break;}
case 187:
#line 793 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_LONG, 0)); ;
    break;}
case 188:
#line 794 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_SHORT, 0)); ;
    break;}
case 189:
#line 795 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_SIGNED, 0)); ;
    break;}
case 190:
#line 796 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_UNSIGNED, 0)); ;
    break;}
case 191:
#line 797 "llgrammar.y"
{ yyval.ltok = yyvsp[0].ltok; ltoken_setIntField (yyval.ltok, fixBits (TS_UNKNOWN, 0)); ;
    break;}
case 192:
#line 805 "llgrammar.y"
{ yyval.ctypes = makeCTypesNode ((CTypesNode)0, yyvsp[0].ltok); ;
    break;}
case 193:
#line 806 "llgrammar.y"
{ yyval.ctypes = makeCTypesNode (yyvsp[-1].ctypes, yyvsp[0].ltok); ;
    break;}
case 194:
#line 812 "llgrammar.y"
{ yyval.ctypes = makeTypeSpecifier (yyvsp[0].ltok); ;
    break;}
case 195:
#line 814 "llgrammar.y"
{ yyval.ctypes = yyvsp[0].ctypes; yyval.ctypes->sort = sort_lookupName (lclctype_toSort (yyvsp[0].ctypes->intfield)); ;
    break;}
case 196:
#line 819 "llgrammar.y"
{ yyval.typequal = qual_createOut (); ;
    break;}
case 197:
#line 820 "llgrammar.y"
{ yyval.typequal = qual_createUnused (); ;
    break;}
case 198:
#line 821 "llgrammar.y"
{ yyval.typequal = qual_createSef (); ;
    break;}
case 199:
#line 822 "llgrammar.y"
{ yyval.typequal = qual_createOnly (); ;
    break;}
case 200:
#line 823 "llgrammar.y"
{ yyval.typequal = qual_createOwned (); ;
    break;}
case 201:
#line 824 "llgrammar.y"
{ yyval.typequal = qual_createDependent (); ;
    break;}
case 202:
#line 825 "llgrammar.y"
{ yyval.typequal = qual_createKeep (); ;
    break;}
case 203:
#line 826 "llgrammar.y"
{ yyval.typequal = qual_createKept (); ;
    break;}
case 204:
#line 827 "llgrammar.y"
{ yyval.typequal = qual_createObserver (); ;
    break;}
case 205:
#line 828 "llgrammar.y"
{ yyval.typequal = qual_createExits (); ;
    break;}
case 206:
#line 829 "llgrammar.y"
{ yyval.typequal = qual_createMayExit (); ;
    break;}
case 207:
#line 830 "llgrammar.y"
{ yyval.typequal = qual_createTrueExit (); ;
    break;}
case 208:
#line 831 "llgrammar.y"
{ yyval.typequal = qual_createFalseExit (); ;
    break;}
case 209:
#line 832 "llgrammar.y"
{ yyval.typequal = qual_createNeverExit (); ;
    break;}
case 210:
#line 833 "llgrammar.y"
{ yyval.typequal = qual_createOnly (); ;
    break;}
case 211:
#line 834 "llgrammar.y"
{ yyval.typequal = qual_createShared (); ;
    break;}
case 212:
#line 835 "llgrammar.y"
{ yyval.typequal = qual_createUnique (); ;
    break;}
case 213:
#line 836 "llgrammar.y"
{ yyval.typequal = qual_createChecked (); ;
    break;}
case 214:
#line 837 "llgrammar.y"
{ yyval.typequal = qual_createUnchecked (); ;
    break;}
case 215:
#line 838 "llgrammar.y"
{ yyval.typequal = qual_createCheckedStrict (); ;
    break;}
case 216:
#line 839 "llgrammar.y"
{ yyval.typequal = qual_createTrueNull (); ;
    break;}
case 217:
#line 840 "llgrammar.y"
{ yyval.typequal = qual_createFalseNull (); ;
    break;}
case 218:
#line 841 "llgrammar.y"
{ yyval.typequal = qual_createRelNull (); ;
    break;}
case 219:
#line 842 "llgrammar.y"
{ yyval.typequal = qual_createRelDef (); ;
    break;}
case 220:
#line 843 "llgrammar.y"
{ yyval.typequal = qual_createRefCounted (); ;
    break;}
case 221:
#line 844 "llgrammar.y"
{ yyval.typequal = qual_createRefs (); ;
    break;}
case 222:
#line 845 "llgrammar.y"
{ yyval.typequal = qual_createNewRef (); ;
    break;}
case 223:
#line 846 "llgrammar.y"
{ yyval.typequal = qual_createKillRef (); ;
    break;}
case 224:
#line 847 "llgrammar.y"
{ yyval.typequal = qual_createNull (); ;
    break;}
case 225:
#line 848 "llgrammar.y"
{ yyval.typequal = qual_createNotNull (); ;
    break;}
case 226:
#line 849 "llgrammar.y"
{ yyval.typequal = qual_createReturned (); ;
    break;}
case 227:
#line 850 "llgrammar.y"
{ yyval.typequal = qual_createExposed (); ;
    break;}
case 228:
#line 851 "llgrammar.y"
{ yyval.typequal = qual_createPartial (); ;
    break;}
case 229:
#line 852 "llgrammar.y"
{ yyval.typequal = qual_createUndef (); ;
    break;}
case 230:
#line 853 "llgrammar.y"
{ yyval.typequal = qual_createKilled (); ;
    break;}
case 231:
#line 857 "llgrammar.y"
{ yyval.lcltypespec = makeLclTypeSpecNodeType (yyvsp[0].ctypes); ;
    break;}
case 232:
#line 859 "llgrammar.y"
{ yyval.lcltypespec = makeLclTypeSpecNodeSU (yyvsp[0].structorunion); ;
    break;}
case 233:
#line 861 "llgrammar.y"
{ yyval.lcltypespec = makeLclTypeSpecNodeEnum (yyvsp[0].enumspec); ;
    break;}
case 234:
#line 863 "llgrammar.y"
{ yyval.lcltypespec = lclTypeSpecNode_addQual (yyvsp[0].lcltypespec, yyvsp[-1].typequal); ;
    break;}
case 235:
#line 865 "llgrammar.y"
{ yyval.lcltypespec = makeLclTypeSpecNodeConj (yyvsp[-3].lcltypespec, yyvsp[-1].lcltypespec); ;
    break;}
case 237:
#line 875 "llgrammar.y"
{ llassert (lclTypeSpecNode_isDefined (yyvsp[-1].lcltypespec));
     yyvsp[-1].lcltypespec->pointers = yyvsp[0].count; yyval.lcltypespec = yyvsp[-1].lcltypespec; ;
    break;}
case 238:
#line 879 "llgrammar.y"
{ yyval.count = 1; ;
    break;}
case 239:
#line 880 "llgrammar.y"
{ yyval.count = yyvsp[-1].count + 1; ;
    break;}
case 240:
#line 884 "llgrammar.y"
{ (void) checkAndEnterTag (TAG_FWDSTRUCT, ltoken_copy (yyvsp[0].ltok)); ;
    break;}
case 241:
#line 886 "llgrammar.y"
{ yyval.structorunion = makestrOrUnionNode (yyvsp[-5].ltok, SU_STRUCT, yyvsp[-4].ltok, yyvsp[-1].structdecls); ;
    break;}
case 242:
#line 888 "llgrammar.y"
{ (void) checkAndEnterTag (TAG_FWDUNION, ltoken_copy (yyvsp[0].ltok)); ;
    break;}
case 243:
#line 890 "llgrammar.y"
{ yyval.structorunion = makestrOrUnionNode (yyvsp[-5].ltok, SU_UNION, yyvsp[-4].ltok, yyvsp[-1].structdecls); ;
    break;}
case 244:
#line 892 "llgrammar.y"
{ yyval.structorunion = makeForwardstrOrUnionNode (yyvsp[-1].ltok, SU_STRUCT, yyvsp[0].ltok); ;
    break;}
case 245:
#line 894 "llgrammar.y"
{ yyval.structorunion = makeForwardstrOrUnionNode (yyvsp[-1].ltok, SU_UNION, yyvsp[0].ltok); ;
    break;}
case 246:
#line 897 "llgrammar.y"
{ yyval.ltok = ltoken_undefined; ;
    break;}
case 248:
#line 901 "llgrammar.y"
{ yyval.structdecls = stDeclNodeList_add (stDeclNodeList_new (), yyvsp[0].structdecl); ;
    break;}
case 249:
#line 902 "llgrammar.y"
{ yyval.structdecls = stDeclNodeList_add (yyvsp[-1].structdecls, yyvsp[0].structdecl); ;
    break;}
case 250:
#line 907 "llgrammar.y"
{ yyval.structdecl = makestDeclNode (yyvsp[-2].lcltypespec, yyvsp[-1].declarelist); ;
    break;}
case 251:
#line 911 "llgrammar.y"
{ yyval.declarelist = declaratorNodeList_add (declaratorNodeList_new (), yyvsp[0].declare); ;
    break;}
case 252:
#line 913 "llgrammar.y"
{ yyval.declarelist = declaratorNodeList_add (yyvsp[-2].declarelist, yyvsp[0].declare); ;
    break;}
case 253:
#line 916 "llgrammar.y"
{ ; ;
    break;}
case 254:
#line 917 "llgrammar.y"
{ ; ;
    break;}
case 255:
#line 921 "llgrammar.y"
{ yyval.enumspec = makeEnumSpecNode (yyvsp[-5].ltok, yyvsp[-4].ltok, yyvsp[-2].ltokenList); ;
    break;}
case 256:
#line 923 "llgrammar.y"
{ yyval.enumspec = makeEnumSpecNode2 (yyvsp[-1].ltok, yyvsp[0].ltok); ;
    break;}
case 257:
#line 926 "llgrammar.y"
{ yyval.ltokenList = ltokenList_singleton (yyvsp[0].ltok); ;
    break;}
case 258:
#line 927 "llgrammar.y"
{ yyval.ltokenList = ltokenList_push (yyvsp[-2].ltokenList, yyvsp[0].ltok); ;
    break;}
case 259:
#line 937 "llgrammar.y"
{ yyval.declare = makeDeclaratorNode (yyvsp[0].typeexpr); ;
    break;}
case 260:
#line 938 "llgrammar.y"
{ yyval.declare = makeDeclaratorNode (yyvsp[0].typeexpr); ;
    break;}
case 261:
#line 941 "llgrammar.y"
{ yyval.typeexpr = makeTypeExpr (yyvsp[0].ltok); ;
    break;}
case 262:
#line 942 "llgrammar.y"
{ yyval.typeexpr = yyvsp[-1].typeexpr; yyval.typeexpr->wrapped = yyval.typeexpr->wrapped + 1; ;
    break;}
case 263:
#line 943 "llgrammar.y"
{ yyval.typeexpr = makePointerNode (yyvsp[-1].ltok, yyvsp[0].typeexpr); ;
    break;}
case 264:
#line 944 "llgrammar.y"
{ yyval.typeexpr = makeArrayNode (yyvsp[-1].typeexpr, yyvsp[0].array); ;
    break;}
case 265:
#line 945 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-2].typeexpr, paramNodeList_new ()); ;
    break;}
case 266:
#line 946 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-3].typeexpr, yyvsp[-1].paramlist); ;
    break;}
case 267:
#line 949 "llgrammar.y"
{ yyval.typeexpr = makeTypeExpr (yyvsp[0].ltok); ;
    break;}
case 268:
#line 950 "llgrammar.y"
{ yyval.typeexpr = yyvsp[-1].typeexpr; yyval.typeexpr->wrapped = yyval.typeexpr->wrapped + 1; ;
    break;}
case 269:
#line 951 "llgrammar.y"
{ yyval.typeexpr = makePointerNode (yyvsp[-1].ltok, yyvsp[0].typeexpr); ;
    break;}
case 270:
#line 952 "llgrammar.y"
{ yyval.typeexpr = makeArrayNode (yyvsp[-1].typeexpr, yyvsp[0].array); ;
    break;}
case 271:
#line 953 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-2].typeexpr, paramNodeList_new ()); ;
    break;}
case 272:
#line 954 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-3].typeexpr, yyvsp[-1].paramlist); ;
    break;}
case 273:
#line 963 "llgrammar.y"
{ yyval.typeexpr = makeTypeExpr (yyvsp[0].ltok); ;
    break;}
case 274:
#line 964 "llgrammar.y"
{ yyval.typeexpr = makePointerNode (yyvsp[-1].ltok, yyvsp[0].typeexpr); ;
    break;}
case 275:
#line 965 "llgrammar.y"
{ yyval.typeexpr = makeArrayNode (yyvsp[-1].typeexpr, yyvsp[0].array); ;
    break;}
case 276:
#line 966 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-2].typeexpr, paramNodeList_new ()); ;
    break;}
case 277:
#line 967 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-3].typeexpr, yyvsp[-1].paramlist); ;
    break;}
case 278:
#line 973 "llgrammar.y"
{ yyval.param = makeParamNode (yyvsp[-1].lcltypespec, yyvsp[0].typeexpr); ;
    break;}
case 279:
#line 974 "llgrammar.y"
{ yyval.param = makeParamNode (yyvsp[-1].lcltypespec, yyvsp[0].typeexpr); ;
    break;}
case 280:
#line 975 "llgrammar.y"
{ yyval.param = makeParamNode (yyvsp[-1].lcltypespec, yyvsp[0].abstDecl); ;
    break;}
case 281:
#line 985 "llgrammar.y"
{ yyval.typname = makeTypeNameNode (FALSE, yyvsp[-1].lcltypespec, yyvsp[0].abstDecl); ;
    break;}
case 282:
#line 986 "llgrammar.y"
{ yyval.typname = makeTypeNameNode (TRUE, yyvsp[-1].lcltypespec, yyvsp[0].abstDecl); ;
    break;}
case 283:
#line 987 "llgrammar.y"
{ yyval.typname = makeTypeNameNodeOp (yyvsp[0].opform); ;
    break;}
case 284:
#line 992 "llgrammar.y"
{ yyval.abstDecl = (abstDeclaratorNode)0; ;
    break;}
case 285:
#line 993 "llgrammar.y"
{ yyval.abstDecl = (abstDeclaratorNode)yyvsp[0].typeexpr; ;
    break;}
case 286:
#line 996 "llgrammar.y"
{ yyval.typeexpr = yyvsp[-1].typeexpr; yyval.typeexpr->wrapped = yyval.typeexpr->wrapped + 1; ;
    break;}
case 287:
#line 997 "llgrammar.y"
{ yyval.typeexpr = makePointerNode (yyvsp[-1].ltok, yyvsp[0].typeexpr); ;
    break;}
case 288:
#line 998 "llgrammar.y"
{ yyval.typeexpr = makePointerNode (yyvsp[0].ltok, (typeExpr)0); ;
    break;}
case 289:
#line 999 "llgrammar.y"
{ yyval.typeexpr = makeArrayNode ((typeExpr)0, yyvsp[0].array); ;
    break;}
case 290:
#line 1000 "llgrammar.y"
{ yyval.typeexpr = makeArrayNode (yyvsp[-1].typeexpr, yyvsp[0].array); ;
    break;}
case 291:
#line 1001 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-2].typeexpr, paramNodeList_new ()); ;
    break;}
case 292:
#line 1002 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode ((typeExpr)0, yyvsp[-1].paramlist); ;
    break;}
case 293:
#line 1003 "llgrammar.y"
{ yyval.typeexpr = makeFunctionNode (yyvsp[-3].typeexpr, yyvsp[-1].paramlist); ;
    break;}
case 294:
#line 1006 "llgrammar.y"
{ yyval.array = makeArrayQualNode (yyvsp[-1].ltok, (termNode)0); ;
    break;}
case 295:
#line 1007 "llgrammar.y"
{ yyval.array = makeArrayQualNode (yyvsp[-2].ltok, yyvsp[-1].term); ;
    break;}
case 296:
#line 1011 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-5].ltok, OPF_IF, opFormUnion_createMiddle (0), ltoken_undefined); ;
    break;}
case 297:
#line 1013 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[0].ltok, OPF_ANYOP, opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 298:
#line 1015 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-1].ltok, OPF_MANYOP, opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 299:
#line 1017 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-1].ltok, OPF_ANYOPM, opFormUnion_createAnyOp (yyvsp[-1].ltok), ltoken_undefined); ;
    break;}
case 300:
#line 1019 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_MANYOPM, 
			  opFormUnion_createAnyOp (yyvsp[-1].ltok), ltoken_undefined); ;
    break;}
case 301:
#line 1022 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_MIDDLE, opFormUnion_createMiddle (yyvsp[-1].count), yyvsp[0].ltok); ;
    break;}
case 302:
#line 1024 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-3].ltok, OPF_MMIDDLE, opFormUnion_createMiddle (yyvsp[-1].count), yyvsp[0].ltok); ;
    break;}
case 303:
#line 1026 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-3].ltok, OPF_MIDDLEM, opFormUnion_createMiddle (yyvsp[-2].count), yyvsp[-1].ltok); ;
    break;}
case 304:
#line 1028 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-3].ltok, OPF_MMIDDLEM, opFormUnion_createMiddle (yyvsp[-2].count), yyvsp[-1].ltok); ;
    break;}
case 305:
#line 1030 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_BMIDDLE, opFormUnion_createMiddle (yyvsp[-1].count), yyvsp[0].ltok); ;
    break;}
case 306:
#line 1032 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-3].ltok, OPF_BMIDDLEM, opFormUnion_createMiddle (yyvsp[-2].count), yyvsp[-1].ltok); ;
    break;}
case 307:
#line 1040 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_BMMIDDLE, opFormUnion_createMiddle (yyvsp[-1].count), yyvsp[0].ltok); ;
    break;}
case 308:
#line 1042 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-3].ltok, OPF_BMMIDDLEM, opFormUnion_createMiddle (yyvsp[-2].count), yyvsp[-1].ltok); ;
    break;}
case 309:
#line 1044 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-1].ltok, OPF_SELECT, 
			  opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 310:
#line 1047 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-1].ltok, OPF_MAP, 
			  opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 311:
#line 1050 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_MSELECT, 
			  opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 312:
#line 1053 "llgrammar.y"
{ yyval.opform = makeOpFormNode (yyvsp[-2].ltok, OPF_MMAP, 
			  opFormUnion_createAnyOp (yyvsp[0].ltok), ltoken_undefined); ;
    break;}
case 320:
#line 1070 "llgrammar.y"
{ yyval.count = 0; ;
    break;}
case 322:
#line 1074 "llgrammar.y"
{ yyval.count = 1; ;
    break;}
case 323:
#line 1075 "llgrammar.y"
{ yyval.count = yyvsp[-2].count + 1; ;
    break;}
case 326:
#line 1082 "llgrammar.y"
{ yyval.signature = makesigNode (yyvsp[-3].ltok, yyvsp[-2].ltokenList, yyvsp[0].ltok); ;
    break;}
case 327:
#line 1085 "llgrammar.y"
{ yyval.ltokenList = ltokenList_new (); ;
    break;}
case 329:
#line 1089 "llgrammar.y"
{ yyval.ltokenList = ltokenList_singleton (yyvsp[0].ltok); ;
    break;}
case 330:
#line 1090 "llgrammar.y"
{ yyval.ltokenList = ltokenList_push (yyvsp[-2].ltokenList, yyvsp[0].ltok); ;
    break;}
case 331:
#line 1093 "llgrammar.y"
{ yyval.lclpredicate = makeLclPredicateNode (ltoken_undefined, yyvsp[0].term, LPD_PLAIN);;
    break;}
case 332:
#line 1096 "llgrammar.y"
{ yyval.term = checkSort (yyvsp[0].term); ;
    break;}
case 333:
#line 1103 "llgrammar.y"
{ yyval.term = makeIfTermNode (yyvsp[-5].ltok,yyvsp[-4].term,yyvsp[-3].ltok,yyvsp[-2].term,yyvsp[-1].ltok,yyvsp[0].term); ;
    break;}
case 335:
#line 1105 "llgrammar.y"
{ yyval.term = makeInfixTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 337:
#line 1111 "llgrammar.y"
{ checkLclPredicate (yyvsp[-2].ltok, yyvsp[-1].lclpredicate); yyval.term = makeQuantifiedTermNode (yyvsp[-3].quantifiers, yyvsp[-2].ltok, yyvsp[-1].lclpredicate->predicate, yyvsp[0].ltok);
     symtable_exitScope (g_symtab); 
   ;
    break;}
case 338:
#line 1115 "llgrammar.y"
{ yyval.term = makeInfixTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].term);;
    break;}
case 339:
#line 1117 "llgrammar.y"
{ yyval.term = makeInfixTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].term);;
    break;}
case 341:
#line 1121 "llgrammar.y"
{ yyval.term = makePostfixTermNode (yyvsp[-1].term, yyvsp[0].ltokenList); ;
    break;}
case 342:
#line 1122 "llgrammar.y"
{ yyval.term = CollapseInfixTermNode (yyvsp[-1].term, yyvsp[0].termlist); ;
    break;}
case 346:
#line 1130 "llgrammar.y"
{ yyval.term = makePrefixTermNode (yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 347:
#line 1133 "llgrammar.y"
{ yyval.ltokenList = ltokenList_singleton (yyvsp[0].ltok); ;
    break;}
case 348:
#line 1134 "llgrammar.y"
{ yyval.ltokenList = ltokenList_push (yyvsp[-1].ltokenList, yyvsp[0].ltok); ;
    break;}
case 349:
#line 1137 "llgrammar.y"
{ yyval.termlist = pushInfixOpPartNode (termNodeList_new (), yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 350:
#line 1138 "llgrammar.y"
{ yyval.termlist = pushInfixOpPartNode (yyvsp[-2].termlist, yyvsp[-1].ltok, yyvsp[0].term); ;
    break;}
case 352:
#line 1142 "llgrammar.y"
{ yyval.term = computePossibleSorts (yyvsp[0].term); ;
    break;}
case 353:
#line 1143 "llgrammar.y"
{ yyval.term = updateMatchedNode ((termNode)0, yyvsp[-1].term, yyvsp[0].term); ;
    break;}
case 354:
#line 1144 "llgrammar.y"
{ yyval.term = updateMatchedNode (yyvsp[-1].term, yyvsp[0].term, (termNode)0); ;
    break;}
case 355:
#line 1145 "llgrammar.y"
{ yyval.term = updateMatchedNode (yyvsp[-2].term, yyvsp[-1].term, yyvsp[0].term); ;
    break;}
case 356:
#line 1146 "llgrammar.y"
{ yyval.term = computePossibleSorts (yyvsp[0].term); ;
    break;}
case 357:
#line 1147 "llgrammar.y"
{ yyval.term = updateSqBracketedNode ((termNode)0, yyvsp[-1].term, yyvsp[0].term); ;
    break;}
case 358:
#line 1150 "llgrammar.y"
{ yyval.term = yyvsp[-2].term; yyval.term->sort = sort_lookupName (ltoken_getText (yyvsp[0].ltok)); ;
    break;}
case 360:
#line 1155 "llgrammar.y"
{ yyval.term = makeSqBracketedNode (yyvsp[-4].ltok, yyvsp[-3].termlist, yyvsp[-2].ltok); 
     yyval.term->given = sort_lookupName (ltoken_getText (yyvsp[0].ltok)); ;
    break;}
case 361:
#line 1158 "llgrammar.y"
{ yyval.term = makeSqBracketedNode (yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok); ;
    break;}
case 362:
#line 1160 "llgrammar.y"
{ yyval.term = makeSqBracketedNode (yyvsp[-3].ltok, termNodeList_new (), yyvsp[-2].ltok); 
     yyval.term->given = sort_lookupName (ltoken_getText (yyvsp[0].ltok)); 
   ;
    break;}
case 363:
#line 1164 "llgrammar.y"
{ yyval.term = makeSqBracketedNode (yyvsp[-1].ltok, termNodeList_new (), yyvsp[0].ltok); ;
    break;}
case 364:
#line 1167 "llgrammar.y"
{ yyval.term = makeMatchedNode (yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok); ;
    break;}
case 365:
#line 1168 "llgrammar.y"
{ yyval.term = makeMatchedNode (yyvsp[-1].ltok, termNodeList_new (), yyvsp[0].ltok); ;
    break;}
case 366:
#line 1171 "llgrammar.y"
{ yyval.termlist = termNodeList_push (termNodeList_new (), yyvsp[0].term); ;
    break;}
case 367:
#line 1172 "llgrammar.y"
{ yyval.termlist = termNodeList_push (yyvsp[-2].termlist, yyvsp[0].term); ;
    break;}
case 368:
#line 1176 "llgrammar.y"
{ yyval.term = yyvsp[-1].term; yyval.term->wrapped = yyval.term->wrapped + 1; ;
    break;}
case 369:
#line 1178 "llgrammar.y"
{ yyval.term = makeSimpleTermNode (yyvsp[0].ltok); ;
    break;}
case 370:
#line 1180 "llgrammar.y"
{ yyval.term = makeOpCallTermNode (yyvsp[-3].ltok, yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok); ;
    break;}
case 372:
#line 1183 "llgrammar.y"
{ yyval.term = makePostfixTermNode2 (yyvsp[-1].term, yyvsp[0].ltok); ;
    break;}
case 373:
#line 1185 "llgrammar.y"
{ ltoken_markOwned (yyvsp[0].ltok); yyval.term = makeSelectTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].ltok); ;
    break;}
case 374:
#line 1187 "llgrammar.y"
{ ltoken_markOwned (yyvsp[0].ltok); yyval.term = makeMapTermNode (yyvsp[-2].term, yyvsp[-1].ltok, yyvsp[0].ltok); ;
    break;}
case 375:
#line 1189 "llgrammar.y"
{ yyval.term = updateSqBracketedNode (yyvsp[-2].term, makeSqBracketedNode (yyvsp[-1].ltok, termNodeList_new (), yyvsp[0].ltok), 
				(termNode)0); ;
    break;}
case 376:
#line 1192 "llgrammar.y"
{ yyval.term = updateSqBracketedNode (yyvsp[-3].term, makeSqBracketedNode (yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok), (termNode)0); ;
    break;}
case 377:
#line 1194 "llgrammar.y"
{ yyval.term = yyvsp[-2].term; yyval.term->given = sort_lookupName (ltoken_getText (yyvsp[0].ltok)); ;
    break;}
case 378:
#line 1197 "llgrammar.y"
{ yyval.termlist = termNodeList_push (termNodeList_new (), yyvsp[0].term); ;
    break;}
case 379:
#line 1198 "llgrammar.y"
{ yyval.termlist = termNodeList_push (yyvsp[-2].termlist, yyvsp[0].term); ;
    break;}
case 385:
#line 1208 "llgrammar.y"
{ yyval.term = makeSimpleTermNode (yyvsp[0].ltok); ;
    break;}
case 386:
#line 1209 "llgrammar.y"
{ yyval.term = makeOpCallTermNode (yyvsp[-3].ltok, yyvsp[-2].ltok, yyvsp[-1].termlist, yyvsp[0].ltok); ;
    break;}
case 387:
#line 1210 "llgrammar.y"
{ yyval.term = makeUnchangedTermNode1 (yyvsp[-3].ltok, yyvsp[-1].ltok); ;
    break;}
case 388:
#line 1211 "llgrammar.y"
{ yyval.term = makeUnchangedTermNode2 (yyvsp[-3].ltok, yyvsp[-1].storereflist); ;
    break;}
case 389:
#line 1213 "llgrammar.y"
{ termNodeList x = termNodeList_new (); 
     termNodeList_addh (x, yyvsp[-1].term);
     yyval.term = makeOpCallTermNode (yyvsp[-3].ltok, yyvsp[-2].ltok, x, yyvsp[0].ltok); 
   ;
    break;}
case 390:
#line 1218 "llgrammar.y"
{ termNodeList x = termNodeList_new ();
     termNodeList_addh (x, yyvsp[-3].term);
     termNodeList_addh (x, yyvsp[-1].term);
     yyval.term = makeOpCallTermNode (yyvsp[-5].ltok, yyvsp[-4].ltok, x, yyvsp[0].ltok); 
   ;
    break;}
case 391:
#line 1223 "llgrammar.y"
{ yyval.term = makeSizeofTermNode (yyvsp[-3].ltok, yyvsp[-1].lcltypespec); ;
    break;}
case 392:
#line 1233 "llgrammar.y"
{ yyval.term = makeLiteralTermNode (yyvsp[0].ltok, sort_int); ;
    break;}
case 393:
#line 1234 "llgrammar.y"
{ yyval.term = makeLiteralTermNode (yyvsp[0].ltok, sort_cstring); ;
    break;}
case 394:
#line 1235 "llgrammar.y"
{ yyval.term = makeLiteralTermNode (yyvsp[0].ltok, sort_char); ;
    break;}
case 395:
#line 1236 "llgrammar.y"
{ yyval.term = makeLiteralTermNode (yyvsp[0].ltok, sort_double); ;
    break;}
case 396:
#line 1240 "llgrammar.y"
{ yyval.quantifiers = quantifierNodeList_add (quantifierNodeList_new (), yyvsp[0].quantifier); ;
    break;}
case 397:
#line 1242 "llgrammar.y"
{ yyval.quantifiers = quantifierNodeList_add (yyvsp[-1].quantifiers, yyvsp[0].quantifier); ;
    break;}
case 398:
#line 1245 "llgrammar.y"
{ scopeInfo si = (scopeInfo) dmalloc (sizeof (*si));
		   si->kind = SPE_QUANT;
		   symtable_enterScope (g_symtab, si); ;
    break;}
case 399:
#line 1249 "llgrammar.y"
{ yyval.quantifier = makeQuantifierNode (yyvsp[0].vars, yyvsp[-2].ltok); ;
    break;}
case 400:
#line 1252 "llgrammar.y"
{ yyval.vars = varNodeList_add (varNodeList_new (), yyvsp[0].var); ;
    break;}
case 401:
#line 1253 "llgrammar.y"
{ yyval.vars = varNodeList_add (yyvsp[-2].vars, yyvsp[0].var); ;
    break;}
case 402:
#line 1256 "llgrammar.y"
{ yyval.var = makeVarNode (yyvsp[-2].ltok, FALSE, yyvsp[0].lcltypespec); ;
    break;}
case 403:
#line 1257 "llgrammar.y"
{ yyval.var = makeVarNode (yyvsp[-3].ltok, TRUE, yyvsp[0].lcltypespec); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 1272 "llgrammar.y"


# include "bison.reset"

/*
** yytext is set in lex scanner 
** extern YYSTYPE yylval;  
** yylval is defined by code generated by bison 
*/

void ylerror (char *s) 
{
  /* 
  ** This resetting of the left context is very important when multiple
  ** files are to be parsed.  We do not want failures to propagate.
  ** Lex/Yacc does not reset the flags when the parsing goes bad.  
  ** BEGIN 0;        
  **/

  /*@-mustfree@*/
  lclfatalerror (yllval.ltok,
		 message ("%s: Token code: %s, Token String: %s", 
			  cstring_fromChars (s), 
			  ltoken_unparseCodeName (yllval.ltok), 
			  ltoken_getRawString (yllval.ltok)));
  /*@=mustfree@*/
}

static void yyprint (FILE *f, int t, YYSTYPE value) 
{
  fprintf (f, " type: %d (%s)", t, 
	   cstring_toCharsSafe (ltoken_getRawString (value.ltok)));
}







