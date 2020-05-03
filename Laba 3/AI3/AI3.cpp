#include <stdafx.h>
#include <iostream>
#include "HILLCL.h"

int main()
{
	HILLCL hillcl;
	Node start(true, false, true);
	hillcl.solve(start);
}
