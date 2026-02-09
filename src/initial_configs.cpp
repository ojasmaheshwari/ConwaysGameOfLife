#include "initial_configs.h"

std::vector<std::pair<int, int>> applyOffsets(int *offsets, int N, int initial_i, int initial_j) {
	std::vector<std::pair<int, int>> ans;
	for (size_t i = 0; i < N - 1; i += 2) {
		int x = offsets[i];
		int y = offsets[i + 1];
		ans.push_back({initial_i + x, initial_j + y});
	}
	return ans;
}
