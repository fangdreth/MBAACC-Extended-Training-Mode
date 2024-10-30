
#include "version.h"

bool isBleeding() {
	#ifdef BLEEDING
		return true;
	#else
		return false;
	#endif
}

const char* getBuildInfo() {
	#ifdef BLEEDING
		return "BLEEDING " GIT_VERSION;
	#else
		return GIT_VERSION;
	#endif
}