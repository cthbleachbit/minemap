/* Minemap marker editor
 *
 * minemarker.cpp: Main program: edit map banners and markers
 */

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#endif

#include <cstdio>
#include "libminemap.h"

void usage() {
    printf("minemarkers <options> <operation>\n");
    printf("Where <options> may be:");
    printf("\t-i, --input INPUT\n");
    printf("\t\tRequired, nbt input to modify\n");
    printf("\t--no-gz\n");
    printf("\t\tOptional, indicates that the input file is uncompressed\n");
    printf("Where <operation> may be one of\n");
    printf("\tls-banner\n");
    printf("\t\tPrint a numbered list of banners contained in the map\n");
    printf("\ttouch-banner abs:<color>:[name]:<x>:<y>:<z>\n");
    printf("\t\tAdd/Replace a banner marker at absolute XYZ coordinates. name field is optional\n");
    printf("\ttouch-banner rel:<color>:[name]:<x>:<y>:<z>\n");
    printf("\t\tAdd/Replace a banner marker at absolute Y but XZ relative to map center. name field is optional\n");
    printf("\trm-banner abs:<x>:<y>:<z>\n");
    printf("\t\tDelete all banner marker at absolute XYZ coordinates\n");
    printf("\trm-banner rel:<x>:<y>:<z>\n");
    printf("\t\tDelete all banner marker at absolute Y but XZ relative to map center\n");
    printf("\trm-banner idx:<index>\n");
    printf("\t\tDelete all banner at specified index\n");
    printf("\t");
    printf("\n");
    Minemap::print_version();
}

int main(int argc, char **argv) {
    usage();
    return 0;
}