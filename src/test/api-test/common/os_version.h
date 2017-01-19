#ifndef __OS_VERSION_H__
#define __OS_VERSION_H__

/* OS Level constants */
#define OSLEVEL_UNKNOWN (-1)	/* default and initializer - shouldn't ever really need this */
#define OSLEVEL_BASE_WIN9X	(1)		/* used to define start of win9x versions */
#define OSLEVEL_WIN95	(1)		/* Windows 95, 95 osr1, 95 osr 2 */
#define OSLEVEL_WIN98	(2)		/* Windows 98, 98SE */
#define OSLEVEL_WINME	(3)		/* Windows ME */
#define OSLEVEL_BASE_NT (64)	/* used to define start of NT versions */
#define OSLEVEL_WINNT	(64)	/* Windows NT 4 */
#define OSLEVEL_WIN2K	(65)	/* Windows 2000 */	
#define OSLEVEL_WINXP	(66)	/* Windows XP */	
#define OSLEVEL_WIN2K3	(67)	/* Windows 2003 Server */	
#define OSLEVEL_WINVISTA (68)	/* Windows Vista */	
#define OSLEVEL_WIN7	(69)	/* Windows 7 */	
#define OSLEVEL_WIN10	(72)	/* Windows 10 */

int getOSLevel();
char *getNameForOSLevel(int oslevel);


#endif // __OS_VERSION_H__