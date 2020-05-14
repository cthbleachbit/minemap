//
// Created by cth451 on 2020/05/14.
//

#ifndef MAP_H
#define MAP_H

#include "libnbtp.h"

namespace Minemap {
	namespace Map {
		std::shared_ptr<NBTP::CompoundTag>
		makeMapData(char scale, char dim, int16_t width, int16_t height, int32_t xC, int32_t zC);
	}
}


#endif //MAP_H
