/*
 * $XConsortium: imakemdep.h,v 1.64 94/01/18 19:23:07 rws Exp $
 * 
 * This file contains machine-dependent constants for the imake utility.
 * When porting imake, read each of the steps below and add in any necessary
 * definitions.  In general you should *not* edit ccimake.c or imake.c!
 */

#ifdef CCIMAKE
/*
 * Step 1:  imake_ccflags
 *     Define any special flags that will be needed to get imake.c to compile.
 *     These will be passed to the compile along with the contents of the
 *     make variable BOOTSTRAPCFLAGS.
 */
#ifdef hpux
#ifdef hp9000s800
#define imake_ccflags "-DSYSV"
#else
#define imake_ccflags "-Wc,-Nd4000,-Ns3000 -DSYSV"
#endif
#endif

#if defined(macII) || defined(_AUX_SOURCE)
#define imake_ccflags "-DmacII -DSYSV"
#endif

#ifdef stellar
#define imake_ccflags "-DSYSV"
#endif

#if defined(USL) || defined(Oki) || defined(NCR)
#define imake_ccflags "-Xc -DSVR4"
#endif

#ifdef sony
#if defined(SYSTYPE_SYSV) || defined(_SYSTYPE_SYSV)
#define imake_ccflags "-DSVR4"
#else
#include <sys/param.h>
#if NEWSOS < 41
#define imake_ccflags "-Dbsd43 -DNOSTDHDRS"
#else
#if NEWSOS < 42
#define imake_ccflags "-Dbsd43"
#endif
#endif
#endif
#endif

#ifdef CRAY
#define imake_ccflags "-DSYSV -DUSG"
#endif

#if defined(_IBMR2) || defined(aix)
#define imake_ccflags "-Daix -DSYSV"
#endif

#ifdef Mips
#  if defined(SYSTYPE_BSD) || defined(BSD) || defined(BSD43)
#    define imake_ccflags "-DBSD43"
#  else 
#    define imake_ccflags "-DSYSV"
#  endif
#endif 

#ifdef is68k
#define imake_ccflags "-Dluna -Duniosb"
#endif

#ifdef SYSV386
# ifdef SVR4
#  define imake_ccflags "-Xc -DSVR4"
# else
#  define imake_ccflags "-DSYSV"
# endif
#endif

#ifdef __convex__
#define imake_ccflags "-fn -tm c1"
#endif

#ifdef apollo
#define imake_ccflags "-DX_NOT_POSIX"
#endif

#ifdef WIN32
#define imake_ccflags "-nologo -batch -D__STDC__"
#endif
#else /* not CCIMAKE */
#ifndef MAKEDEPEND
/*
 * Step 2:  dup2
 *     If your OS doesn't have a dup2() system call to duplicate one file
 *     descriptor onto another, define such a mechanism here (if you don't
 *     already fall under the existing category(ies).
 */
#if defined(SYSV) && !defined(CRAY) && !defined(Mips)
#define	dup2(fd1,fd2)	((fd1 == fd2) ? fd1 : (close(fd2), \
					       fcntl(fd1, F_DUPFD, fd2)))
#endif


/*
 * Step 3:  FIXUP_CPP_WHITESPACE
 *     If your cpp collapses tabs macro expansions into a single space and
 *     replaces escaped newlines with a space, define this symbol.  This will
 *     cause imake to attempt to patch up the generated Makefile by looking
 *     for lines that have colons in them (this is why the rules file escapes
 *     all colons).  One way to tell if you need this is to see whether or not
 *     your Makefiles have no tabs in them and lots of @@ strings.
 */
#if defined(sun) || defined(SYSV) || defined(SVR4) || defined(hcx) || defined(WIN32)
#define FIXUP_CPP_WHITESPACE
#endif
#ifdef WIN32
#define REMOVE_CPP_LEADSPACE
#define INLINE_SYNTAX
#define MAGIC_MAKE_VARS
#endif

/*
 * Step 4:  USE_CC_E, DEFAULT_CC, DEFAULT_CPP
 *     If you want to use cc -E instead of cpp, define USE_CC_E.
 *     If use cc -E but want a different compiler, define DEFAULT_CC.
 *     If the cpp you need is not in /lib/cpp, define DEFAULT_CPP.
 */
#ifdef hpux
#define USE_CC_E
#endif
#ifdef WIN32
#define USE_CC_E
#define DEFAULT_CC "cl"
#endif
#ifdef apollo
#define DEFAULT_CPP "/usr/lib/cpp"
#endif
#if defined(_IBMR2) && !defined(DEFAULT_CPP)
#define DEFAULT_CPP "/usr/lpp/X11/Xamples/util/cpp/cpp"
#endif
#if defined(sun) && defined(SVR4)
#define DEFAULT_CPP "/usr/ccs/lib/cpp"
#endif
#ifdef bsdi
#define DEFAULT_CPP "/usr/bin/cpp"
#endif

/*
 * Step 5:  cpp_argv
 *     The following table contains the flags that should be passed
 *     whenever a Makefile is being generated.  If your preprocessor 
 *     doesn't predefine any unique symbols, choose one and add it to the
 *     end of this table.  Then, do the following:
 * 
 *         a.  Use this symbol in Imake.tmpl when setting MacroFile.
 *         b.  Put this symbol in the definition of BootstrapCFlags in your
 *             <platform>.cf file.
 *         c.  When doing a make World, always add "BOOTSTRAPCFLAGS=-Dsymbol" 
 *             to the end of the command line.
 * 
 *     Note that you may define more than one symbol (useful for platforms 
 *     that support multiple operating systems).
 */

#define	ARGUMENTS 50	/* number of arguments in various arrays */
char *cpp_argv[ARGUMENTS] = {
	"cc",		/* replaced by the actual program to exec */
	"-I.",		/* add current directory to include path */
#ifdef unix
	"-Uunix",	/* remove unix symbol so that filename unix.c okay */
#endif
#ifdef M4330
	"-DM4330",	/* Tektronix */
#endif
#ifdef M4310
	"-DM4310",	/* Tektronix */
#endif
#if defined(macII) || defined(_AUX_SOURCE)
	"-DmacII",	/* Apple A/UX */
#endif
#ifdef USL
	"-DUSL",	/* USL */
#endif
#ifdef sony
	"-Dsony",	/* Sony */
#if !defined(SYSTYPE_SYSV) && !defined(_SYSTYPE_SYSV) && NEWSOS < 42
	"-Dbsd43",
#endif
#endif
#ifdef _IBMR2
	"-D_IBMR2",	/* IBM RS-6000 (we ensured that aix is defined above */
#ifndef aix
#define aix		/* allow BOOTSTRAPCFLAGS="-D_IBMR2" */
#endif
#endif /* _IBMR2 */
#ifdef aix
	"-Daix",	/* AIX instead of AOS */
#ifndef ibm
#define ibm		/* allow BOOTSTRAPCFLAGS="-Daix" */
#endif
#endif /* aix */
#ifdef ibm
	"-Dibm",	/* IBM PS/2 and RT under both AOS and AIX */
#endif
#ifdef luna
	"-Dluna",	/* OMRON luna 68K and 88K */
#ifdef luna1
	"-Dluna1",
#endif
#ifdef luna88k		/* need not on UniOS-Mach Vers. 1.13 */
	"-traditional", /* for some older version            */
#endif			/* instead of "-DXCOMM=\\#"          */
#ifdef uniosb
	"-Duniosb",
#endif
#ifdef uniosu
	"-Duniosu",
#endif
#endif /* luna */
#ifdef CRAY		/* Cray */
	"-Ucray",
#endif
#ifdef Mips
	"-DMips",	/* Define and use Mips for Mips Co. OS/mach. */
# if defined(SYSTYPE_BSD) || defined(BSD) || defined(BSD43)
	"-DBSD43",	/* Mips RISCOS supports two environments */
# else
	"-DSYSV",	/* System V environment is the default */
# endif
#endif /* Mips */
#ifdef MOTOROLA
	"-DMOTOROLA",    /* Motorola Delta Systems */
# ifdef SYSV
	"-DSYSV", 
# endif
# ifdef SVR4
	"-DSVR4",
# endif
#endif /* MOTOROLA */
#ifdef SYSV386           /* System V/386 folks */
	"-DSYSV386",
# ifdef SVR4
	"-DSVR4",
# endif
# ifdef ISC
	"-DISC",         /* ISC 2.2.1 */
# endif
# ifdef SCO
	"-DSCO",
# endif
# ifdef ESIX
	"-DESIX",
# endif
# ifdef ATT
	"-DATT",
# endif
# ifdef DELL
	"-DDELL",
#endif
#endif
#ifdef __osf__
	"-D__osf__",
# ifdef __mips__
	"-D__mips__",
# endif
# ifdef __alpha
	"-D__alpha",
# endif
# ifdef __i386__
	"-D__i386__",
# endif
# ifdef __GNUC__
	"-traditional"
# endif
#endif
#ifdef Oki
	"-DOki",
#endif
#ifdef sun
#ifdef SVR4
	"-DSVR4",
#endif
#endif
#ifdef WIN32
	"-DWIN32",
	"-nologo",
	"-batch",
	"-D__STDC__",
#endif
#ifdef NCR
	"-DNCR",	/* NCR */
#endif
};
#else /* else MAKEDEPEND */
/*
 * Step 6:  predefs
 *     If your compiler and/or preprocessor define any specific symbols, add
 *     them to the the following table.  The definition of struct symtab is
 *     in util/makedepend/def.h.
 */
struct symtab	predefs[] = {
#ifdef apollo
	{"apollo", "1"},
#endif
#ifdef ibm032
	{"ibm032", "1"},
#endif
#ifdef ibm
	{"ibm", "1"},
#endif
#ifdef aix
	{"aix", "1"},
#endif
#ifdef sun
	{"sun", "1"},
#endif
#ifdef sun2
	{"sun2", "1"},
#endif
#ifdef sun3
	{"sun3", "1"},
#endif
#ifdef sun4
	{"sun4", "1"},
#endif
#ifdef sparc
	{"sparc", "1"},
#endif
#ifdef __sparc__
	{"__sparc__", "1"},
#endif
#ifdef hpux
	{"hpux", "1"},
#endif
#ifdef __hpux
	{"__hpux", "1"},
#endif
#ifdef __hp9000s800
	{"__hp9000s800", "1"},
#endif
#ifdef __hp9000s700
	{"__hp9000s700", "1"},
#endif
#ifdef vax
	{"vax", "1"},
#endif
#ifdef VMS
	{"VMS", "1"},
#endif
#ifdef cray
	{"cray", "1"},
#endif
#ifdef CRAY
	{"CRAY", "1"},
#endif
#ifdef att
	{"att", "1"},
#endif
#ifdef mips
	{"mips", "1"},
#endif
#ifdef __mips__
	{"__mips__", "1"},
#endif
#ifdef ultrix
	{"ultrix", "1"},
#endif
#ifdef stellar
	{"stellar", "1"},
#endif
#ifdef mc68000
	{"mc68000", "1"},
#endif
#ifdef mc68020
	{"mc68020", "1"},
#endif
#ifdef __GNUC__
	{"__GNUC__", "1"},
#endif
#if __STDC__
	{"__STDC__", "1"},
#endif
#ifdef __HIGHC__
	{"__HIGHC__", "1"},
#endif
#ifdef CMU
	{"CMU", "1"},
#endif
#ifdef luna
	{"luna", "1"},
#ifdef luna1
	{"luna1", "1"},
#endif
#ifdef luna2
	{"luna2", "1"},
#endif
#ifdef luna88k
	{"luna88k", "1"},
#endif
#ifdef uniosb
	{"uniosb", "1"},
#endif
#ifdef uniosu
	{"uniosu", "1"},
#endif
#endif
#ifdef ieeep754
	{"ieeep754", "1"},
#endif
#ifdef is68k
	{"is68k", "1"},
#endif
#ifdef m68k
        {"m68k", "1"},
#endif
#ifdef m88k
        {"m88k", "1"},
#endif
#ifdef __m88k__
	{"__m88k__", "1"},
#endif
#ifdef bsd43
	{"bsd43", "1"},
#endif
#ifdef hcx
	{"hcx", "1"},
#endif
#ifdef sony
	{"sony", "1"},
#ifdef SYSTYPE_SYSV
	{"SYSTYPE_SYSV", "1"},
#endif
#ifdef _SYSTYPE_SYSV
	{"_SYSTYPE_SYSV", "1"},
#endif
#endif
#ifdef __osf__
	{"__osf__", "1"},
#endif
#ifdef __alpha
	{"__alpha", "1"},
#endif
	/* add any additional symbols before this line */
	{NULL, NULL}
};

#endif /* MAKEDEPEND */
#endif /* CCIMAKE */
