#if defined(_WIN32) || defined(_WIN64)
#include <BaseTsd.h>
#define LINE_MAX 4096
#define _CRT_SECURE_NO_WARNINGS 1
typedef SSIZE_T ssize_t;
#endif

#include <cstring>
#include <climits>
#include <iostream>
#include <Magick++.h>

/* Used to convert text describing colors into gif palette
 *
 * This specific program is not designed with security in mind (there is no return code checks, out-of-bound checks or
 * whatsoever) and is only used to generate static gif files at build time.
 */

void usage() {
	using namespace std;
	cout << "Usage:" << endl;
	cout << "    gen_color_map <input.txt> <output.gif>" << endl;
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

	delete line_buffer;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		usage();
		return 1;
	}
	std::string input_filename = argv[1];
	std::string output_filename = argv[2];

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

	// Generate palette image
	{
		Magick::InitializeMagick(*argv);
		ssize_t height = num_colors / 4;
		Magick::Image palette_img = Magick::Image(4, height, "RGBA", MagickCore::CharPixel, colors);
		try {
			Magick::Blob output_buf;
			palette_img.write(&output_buf, "PNG");
			FILE* output_fd = fopen(output_filename.c_str(), "wb");
			fwrite(output_buf.data(), output_buf.length(), 1, output_fd);
			fclose(output_fd);
		}
		catch (Magick::Exception &e) {
			std::cerr << "Error when writing palette image: " << e.what() << std::endl;
			exit(1);
		}
	}
	return 0;
}
