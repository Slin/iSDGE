/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */


#include <cstdio>

#include <iostream>
#include <sstream>

using namespace std;
//#include "tinyxml.h"

#include "ImportExport.h"

int main(int argc, char * const argv[])
{
	if(argc > 2)
	{
		InitializeSdkManager();
		ImportExport(argv[1], argv[2], -1);
		DestroySdkObjects();
	}
	
    return 0;
}