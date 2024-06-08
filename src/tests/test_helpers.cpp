#include "test_helpers.h"
#include <fmt/format.h>

std::ostream& operator<<(std::ostream& os, const TestSquare& square) {
	for (int line = 0; line < C_WIDTH; line++) {
		os << fmt::format("{:02x}", fmt::join(square[line], " ")) << std::endl;
	}
	return os;
}

TestSquare select_corner(bool left, bool up, const std::vector<std::shared_ptr<NBTP::Tag>>& colors) {
	TestSquare corner{};
	for (int line = 0; line < C_WIDTH; line++) {
		ssize_t line_start = (up ? line : F_WIDTH - C_WIDTH + line) * F_WIDTH + (left ? 0 : F_WIDTH - C_WIDTH);
		for (int column = 0; column < C_WIDTH; column++) {
			corner[line][column] = dynamic_cast<NBTP::ByteTag*>(colors[line_start + column].get())->getPayload();
		}
	}
	return corner;
}
