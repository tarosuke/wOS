/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _MAP_
#define _MAP_

#include <types.h>
#include <resource.h>


class MAP : public RESOURCE{
public:
	virtual punit GetPage(punit entry) = 0;
private:
};



#endif
