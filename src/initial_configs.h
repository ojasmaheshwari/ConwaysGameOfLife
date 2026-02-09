#pragma once

#include <utility>
#include <vector>
namespace offsets {
	const int glider[5][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 1}};
	const int f_pentonimo[5][2] = {{0, 0}, {0, 1}, {1, -1}, {1, 0}, {2, 0}};
}

std::vector<std::pair<int, int>> applyOffsets(int* offsets, int N, int initial_i = 0, int initial_j = 0);
