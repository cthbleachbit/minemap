#include <cstring>
#include <climits>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#define LINE_MAX 4096
#endif

/* Used to convert text describing colors into GIMP palette
 *
 * This specific program is not designed with security in mind (there is no return code checks, out-of-bound checks or
 * whatsoever) and is only used to generate static gif files at build time.
 *
 * There's probably no point installing this program after build phase,  * since code changes are required to support
 * new minecraft versions.
 */

void usage() {
	using namespace std;
	cout << "Usage:" << endl;
	cout << "    gen_gimp_palette <input.txt> <output-prefix>" << endl;
#if defined(_WIN32) || defined(_WIN64)
	printf("Executable built at %s %s", __DATE__, __TIME__);
#endif
}

void run_colors(ssize_t& num_colors, FILE* input_file, unsigned char *colors) {
	bool dry_run = false;
	if (num_colors == 0 || colors == nullptr) {
		// Dry run mode
		dry_run = true;
		num_colors = 0;
	}
	auto *line_buffer = new char[LINE_MAX];

	int i = 0;
	while(fgets(line_buffer, LINE_MAX, input_file) != nullptr) {
		if (line_buffer[0] == '\n' || line_buffer[0] == '#') {
			continue;
		}
		if (dry_run) {
			num_colors++;
		} else {
			unsigned char r, g, b, a;
			sscanf(line_buffer, "{%hhd, %hhd, %hhd, %hhd},", &r, &g, &b, &a);
			colors[i * 4] = r;
			colors[i * 4 + 1] = g;
			colors[i * 4 + 2] = b;
			colors[i * 4 + 3] = a;
			i++;
		}
		//printf("Index %i:\t%i\t%i\t%i\t%i\n", i , r, g, b, a);
	}
	rewind(input_file);

	delete[] line_buffer;
}

int main(int argc, char **argv) {
#if defined(_WIN32) || defined(_WIN64)
	_wputenv_s(L"MAGICK_CODER_MODULE_PATH", WIN_MAGICK_CODER_MODULE_PATH);
#endif
	if (argc != 3) {
		usage();
		return 1;
	}
	std::string input_filename = argv[1];
	std::string gpl_prefix = argv[2];
	std::string gpl_filename = gpl_prefix + ".gpl";

	FILE *input_file = fopen(input_filename.c_str(), "r");
	if (input_file == NULL) {
		fprintf(stderr, "%s\n", "Fail to open file");
		return 1;
	}
	// Determine number of lines in input
	ssize_t num_colors = 0;
	run_colors(num_colors, input_file, nullptr);
#if defined(_WIN32) || defined(_WIN64)
	printf("Number of colors in %s: %lli\n", input_filename.c_str(), num_colors);
#else
	printf("Number of colors in %s: %li\n", input_filename.c_str(), num_colors);
#endif

	// Parse to array of colors
	auto *colors = new unsigned char[sizeof(short) * 4 * num_colors];
	run_colors(num_colors, input_file, colors);
	fclose(input_file);

	// Output to gimp palette gpl
	{
		FILE *output_file = fopen(gpl_filename.c_str(), "w");
		if (output_file == NULL) {
			fprintf(stderr, "%s\n", "Fail to open file");
			return 1;
		}
		// Write header
		fprintf(output_file, "GIMP Palette\nName: %s\nColumns: 4\n#\n", gpl_prefix.c_str());
		// Write out colors
		for (int i = 0; i < num_colors; i++) {
			fprintf(output_file, "%3i %3i %3i #%i\n", colors[i * 4], colors[i * 4 + 1], colors[i * 4 + 2], i);
		}
		fclose(output_file);
	}
	return 0;
}
