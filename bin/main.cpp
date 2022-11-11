#include "lib/sandpile/sandpile.h"
#include "lib/image/image.h"

#include <iostream>
#include <cstring>
#include <set>

int main(int argc, char* argv[]) {
	uint start_time = clock();
	uint16_t n = 0;
	uint16_t m = 0;
	uint64_t max_iter = 0;
	char* path_input;
	std::string path_output;
	uint64_t freq = 0;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--length")) {
			int argument_length = static_cast<int>(strlen(argv[i + 1]));
			for (int j = 0; j < argument_length; j++) {
				n *= 10;
				n += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width")) {
			int argument_length = static_cast<int>(strlen(argv[i + 1]));
			for (int j = 0; j < argument_length; j++) {
				m *= 10;
				m += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) {
			path_input = argv[++i];
		} else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
			path_output = argv[++i];
			if (path_output[path_output.size() - 1] != '/') {
				path_output += '/';
			}
		} else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--max-iter")) {
			int argument_length = static_cast<int>(strlen(argv[i + 1]));
			for (int j = 0; j < argument_length; j++) {
				max_iter *= 10;
				max_iter += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		} else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq")) {
			int argument_length = static_cast<int>(strlen(argv[i + 1]));
			for (int j = 0; j < argument_length; j++) {
				freq *= 10;
				freq += static_cast<int>(argv[i + 1][j] - '0');
			}
			i++;
		}
	}

	std::vector<uint64_t> a(n * m, 0);

	freopen(path_input, "r", stdin);

	uint16_t x;
	uint16_t y;
	uint64_t cnt_sand;

	while (std::cin >> x) {
		std::cin >> y >> cnt_sand;
		a[x * n + y] = cnt_sand;
	}

	Field f(a, n, m);

	f.InitSpilling();

	uint64_t iter_counter = 0;
	while (iter_counter <= max_iter && !f.all_spilling_nodes.empty()) {
		f.Spill();
		iter_counter++;
		if (freq != 0 && iter_counter % freq == 0) {
			std::cout << iter_counter << " iterations\n";
			f.Output(path_output + std::string("image_") + std::to_string(iter_counter / freq) + std::string(".bmp"));
		}
	}

	f.Output(path_output + std::string("image_") + "final" + std::string(".bmp"));
	uint end_time = clock();
	std::cout << (end_time - start_time) / CLOCKS_PER_SEC << " secs" << '\n';
}