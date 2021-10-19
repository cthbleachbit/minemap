from gen_color_map import run_colors
import sys

def usage():
    print('Usage:\n\tgen_gimp_palette <input.txt> <output-prefix>')


def main():
    if len(sys.argv) != 3:
        usage()
        sys.exit(1)
    input_filename = sys.argv[1]
    gpl_prefix = sys.argv[2]
    output_filename = gpl_prefix + '.gpl'
    with open(input_filename, 'rt') as f:
        contents = f.read()
    num_colors, colors = run_colors(contents)
    print(f'Number of colors in {input_filename}: {num_colors}')
    with open(output_filename, 'wt') as f:
        f.write(f'GIMP Palette\nName: {gpl_prefix}\nColumns: 4\n#\n')
        for i in range(num_colors):
            f.write(f'{colors[i * 4]} {colors[i * 4 + 1]} {colors[i * 4 + 2]} #{i}\n')


if __name__ == '__main__':
    main()
