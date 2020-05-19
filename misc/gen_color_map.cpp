#include <cstring>
#include <climits>
#include <iostream>
#include <Magick++.h>

/* Used to convert text describing colors into gif palette
 *
 * This specific program is not designed with security in mind (there is no return code checks, out-of-bound checks or
 * whatsoever) and is only used to generate static gif files at build time.
 */

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

void usage() {
	using namespace std;
	cout << "Usage:" << endl;
	cout << "    gen_color_map <input.txt> <output.gif>" << endl;
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
	char c;
	ssize_t num_colors = 0;
	while ((c = fgetc(input_file)) != EOF) {
		if (c == '\n') {
			num_colors++;
		}
	}
	rewind(input_file);
	printf("Number of colors in this file: %li\n", num_colors);

	// Parse to array of colors
	auto *line_buffer = new char[LINE_MAX];
	auto *colors = new unsigned char[sizeof(short) * 4 * num_colors];

	for (int i = 0; i < num_colors; i++) {
		fgets(line_buffer, LINE_MAX, input_file);
		unsigned char r, g, b, a;
		sscanf(line_buffer, "{%hhd, %hhd, %hhd, %hhd},", &r, &g, &b, &a);
		colors[i * 4] = r;
		colors[i * 4 + 1] = g;
		colors[i * 4 + 2] = b;
		colors[i * 4 + 3] = a;
		//printf("Index %i:\t%i\t%i\t%i\t%i\n", i , r, g, b, a);
	}
	fclose(input_file);

	// Generate palette image
	{
		Magick::InitializeMagick(*argv);
		ssize_t height = num_colors / 4;
		Magick::Image palette_img = Magick::Image(4, height, "RGBA", MagickCore::CharPixel, colors);
		try {
			palette_img.write(output_filename);
		}
		catch (Magick::Exception &e) {
			std::cerr << "Error when writing palette image: " << e.what() << std::endl;
			exit(1);
		}
	}

}
