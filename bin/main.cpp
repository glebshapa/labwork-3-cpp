#include "lib/sandpile/sandpile.h"
#include "lib/image/image.h"

#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
	uint start_time = clock();
	uint16_t n = 0;
	uint16_t m = 0;
	uint64_t max_iter = 0;
	char* path_input;
	char* path_output;
	uint64_t freq = 0;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--lines")) {
			for (int j = 0; j < static_cast<int>(strlen(argv[i + 1])); j++) {
				n *= 10;
				n += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width")) {
			for (int j = 0; j < static_cast<int>(strlen(argv[i + 1])); j++) {
				m *= 10;
				m += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) {
			path_input = argv[++i];
		} else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
			path_output = argv[++i];
		} else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--max-iter")) {
			for (int j = 0; j < static_cast<int>(strlen(argv[i + 1])); j++) {
				max_iter *= 10;
				max_iter += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq")) {
			for (int j = 0; j < static_cast<int>(strlen(argv[i + 1])); j++) {
				freq *= 10;
				freq += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		}
	}

	std::vector<uint64_t> a(n * m);

	freopen(path_input, "r", stdin);

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < m; j++) {
			std::cin >> a[i * n + j];
		}
	}

	Field f(a, n, m);
	f.Spill(UINT64_MAX, freq, path_output);
	f.Output(path_output);
	uint end_time = clock();
	std::cout << (end_time - start_time) / CLOCKS_PER_SEC << " secs" << '\n';
}