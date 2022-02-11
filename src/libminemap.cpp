//
// Created by cth451 on 2021/9/15.
//

#include "libminemap.h"

namespace Minemap {
	void print_version() noexcept {
#if defined(_MSC_VER)
		printf("Executable built at %s %s\n", __DATE__, __TIME__);
#endif
		printf("Application version %s\n", MINEMAP_APP_VER);
	}
}