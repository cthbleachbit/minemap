//
// Created by cth451 on 2020/05/18.
//

#ifndef MINEMAP_COMMON_H
#define MINEMAP_COMMON_H
#include <cstdio>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

static void print_version() {
#if defined(_MSC_VER)
	printf("Executable built at %s %s\n", __DATE__, __TIME__);
#endif
	printf("Application version %s\n", MINEMAP_APP_VER);
}

#endif //COMMON_H
