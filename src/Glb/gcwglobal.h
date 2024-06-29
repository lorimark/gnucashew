#line 2 "src/Glb/gcwglobal.h"

#ifndef __GLOBAL_H___
#define __GLOBAL_H___

/*
** set up some common global definitions
**
*/

#define ISO_DATE_FORMAT "yyyy-MM-ddThh:mm:ss.zzzZ"

#define GCW_NUMERIC DECIMAL::decimal<2>

#define HEADER __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ":<start>"
#define FOOTER __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << ":<end>"
#define BREAKER std::endl << std::endl << std::endl
#define BREAKHEADER BREAKER << HEADER
#define BREAKFOOTER FOOTER << BREAKER

#endif

