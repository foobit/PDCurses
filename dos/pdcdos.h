/************************************************************************ 
 * This file is part of PDCurses. PDCurses is public domain software;	*
 * you may use it for any purpose. This software is provided AS IS with	*
 * NO WARRANTY whatsoever.						*
 *									*
 * If you use PDCurses in an application, an acknowledgement would be	*
 * appreciated, but is not mandatory. If you make corrections or	*
 * enhancements to PDCurses, please forward them to the current		*
 * maintainer for the benefit of other users.				*
 *									*
 * No distribution of modified PDCurses code may be made under the name	*
 * "PDCurses", except by the current maintainer. (Although PDCurses is	*
 * public domain, the name is a trademark.)				*
 *									*
 * See the file maintain.er for details of the current maintainer.	*
 ************************************************************************/

/* $Id: pdcdos.h,v 1.10 2006/08/12 02:44:08 wmcbrine Exp $ */

#define CURSES_LIBRARY 1
#include <curses.h>

/*----------------------------------------------------------------------
 *	MEMORY MODEL SUPPORT:
 *
 *	MODELS
 *		TINY	cs,ds,ss all in 1 segment (not enough memory!)
 *		SMALL	cs:1 segment, ds:1 segment
 *		MEDIUM	cs:many segments, ds:1 segment
 *		COMPACT	cs:1 segment, ds:many segments
 *		LARGE	cs:many segments, ds:many segments
 *		HUGE	cs:many segments, ds:segments > 64K
 */

#ifdef __TINY__
# define SMALL 1
#endif
#ifdef __SMALL__
# define SMALL 1
#endif
#ifdef __MEDIUM__
# define MEDIUM 1
#endif
#ifdef __COMPACT__
# define COMPACT 1
#endif
#ifdef __LARGE__
# define LARGE 1
#endif
#ifdef __HUGE__
# define HUGE 1
#endif

#include <dos.h>

extern union REGS regs;

extern int	pdc_adapter;
extern bool	pdc_direct_video;
extern bool	pdc_bogus_adapter;
extern unsigned pdc_video_seg;
extern unsigned pdc_video_ofs;

#ifdef __DJGPP__		/* Note: works only in plain DOS... */
# if DJGPP == 2
#  define _FAR_POINTER(s,o)	((((int)(s)) << 4) + ((int)(o)))
# else
#  define _FAR_POINTER(s,o)	(0xe0000000 + (((int)(s)) << 4) + ((int)(o)))
# endif
# define _FP_SEGMENT(p)		(unsigned short)((((long)p) >> 4) & 0xffff)
#else
# ifdef __TURBOC__
#  define _FAR_POINTER(s,o)	MK_FP(s,o)
# else
#  if defined(__WATCOMC__) && defined(__FLAT__)
#   define _FAR_POINTER(s,o)	((((int)(s)) << 4) + ((int)(o)))
#  else
#   define _FAR_POINTER(s,o)	(((long)s << 16) | (long)o)
#  endif
# endif
# define _FP_SEGMENT(p)		(unsigned short)(((long)p) >> 4)
#endif
#define _FP_OFFSET(p)		((unsigned short)p & 0x000f)

#ifdef __DJGPP__
unsigned char getdosmembyte(int offs);
unsigned short getdosmemword(int offs);
void setdosmembyte(int offs, unsigned char b);
void setdosmemword(int offs, unsigned short w);
#else
# if SMALL || MEDIUM || MSC || defined(__PACIFIC__)
#  define PDC_FAR far
# else
#  define PDC_FAR
# endif
# define getdosmembyte(offs) \
	(*((unsigned char PDC_FAR *) _FAR_POINTER(0,offs)))
# define getdosmemword(offs) \
	(*((unsigned short PDC_FAR *) _FAR_POINTER(0,offs)))
# define setdosmembyte(offs,x) \
	(*((unsigned char PDC_FAR *) _FAR_POINTER(0,offs)) = (x))
# define setdosmemword(offs,x) \
	(*((unsigned short PDC_FAR *) _FAR_POINTER(0,offs)) = (x))
#endif

/* Text-mode font size information */
enum
{
	_FONT8 = 8,
	_FONT14 = 14,
	_FONT15,	/* GENIUS */
	_FONT16
};

#ifdef __PACIFIC__
void	movedata(unsigned, unsigned, unsigned, unsigned, unsigned);
#endif

int	PDC_get_font(void);
int	PDC_get_scrn_mode(void);
void	PDC_putctty(chtype);
int	PDC_query_adapter_type(void);
int	PDC_set_80x25(void);
int	PDC_set_font(int);
int	PDC_set_scrn_mode(int);
