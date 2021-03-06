/**
 * Implements a Zoo namespace with methods for constructing Grid objects containing various creatures in the Game of Life.
 *      - Creatures like gliders, light weight spaceships, and r-pentominos can be spawned.
 *          - These creatures are drawn on a Grid the size of their bounding box.
 *
 *      - Grids can be loaded from and saved to an ascii file format.
 *          - Ascii files are composed of:
 *              - A header line containing an integer width and height separated by a space.
 *              - followed by (height) number of lines, each containing (width) number of characters,
 *                terminated by a newline character.
 *              - (space) ' ' is Cell::DEAD, (hash) '#' is Cell::ALIVE.
 *
 *      - Grids can be loaded from and saved to an binary file format.
 *          - Binary files are composed of:
 *              - a 4 byte int representing the grid width
 *              - a 4 byte int representing the grid height
 *              - followed by (width * height) number of individual bits in C-style row/column format,
 *                padded with zero or more 0 bits.
 *              - a 0 bit should be considered Cell::DEAD, a 1 bit should be considered Cell::ALIVE.
 *
 * @author 950530
 * @date March, 2020
 */
#include "zoo.h"
#include <fstream>
#include <stdexcept>

// Include the minimal number of headers needed to support your implementation.
// #include ...

/**
 * Zoo::glider()
 *
 * Construct a 3x3 grid containing a glider.
 * https://www.conwaylife.com/wiki/Glider
 *
 * @example
 *
 *      // Print a glider in a Grid the size of its bounding box.
 *      std::cout << Zoo::glider() << std::endl;
 *
 *      +---+
 *      | # |
 *      |  #|
 *      |###|
 *      +---+
 *
 * @return
 *      Returns a Grid containing a glider.
 */
Grid Zoo::glider() {
    Grid glider_grid = Grid(3);
    glider_grid.set(1, 0, Cell::ALIVE);
    glider_grid.set(2, 1, Cell::ALIVE);
    glider_grid.set(0, 2, Cell::ALIVE);
    glider_grid.set(1, 2, Cell::ALIVE);
    glider_grid.set(2, 2, Cell::ALIVE);
    return glider_grid;
}

/**
 * Zoo::r_pentomino()
 *
 * Construct a 3x3 grid containing an r-pentomino.
 * https://www.conwaylife.com/wiki/R-pentomino
 *
 * @example
 *
 *      // Print an r-pentomino in a Grid the size of its bounding box.
 *      std::cout << Zoo::r_pentomino() << std::endl;
 *
 *      +---+
 *      | ##|
 *      |## |
 *      | # |
 *      +---+
 *
 * @return
 *      Returns a Grid containing a r-pentomino.
 */
Grid Zoo::r_pentomino() {
    Grid r_pentomino_grid = Grid(3);
    r_pentomino_grid.set(1, 0, Cell::ALIVE);
    r_pentomino_grid.set(2, 0, Cell::ALIVE);
    r_pentomino_grid.set(1, 1, Cell::ALIVE);
    r_pentomino_grid.set(0, 1, Cell::ALIVE);
    r_pentomino_grid.set(1, 2, Cell::ALIVE);
    return r_pentomino_grid;
}

/**
 * Zoo::light_weight_spaceship()
 *
 * Construct a 5x4 grid containing a light weight spaceship.
 * https://www.conwaylife.com/wiki/Lightweight_spaceship
 *
 * @example
 *
 *      // Print a light weight spaceship in a Grid the size of its bounding box.
 *      std::cout << Zoo::light_weight_spaceship() << std::endl;
 *
 *      +-----+
 *      | #  #|
 *      |#    |
 *      |#   #|
 *      |#### |
 *      +-----+
 *
 * @return
 *      Returns a grid containing a light weight spaceship.
 */
Grid Zoo::light_weight_spaceship() {
    Grid spaceship_grid = Grid(5, 4);
    spaceship_grid.set(1, 0, Cell::ALIVE);
    spaceship_grid.set(4, 0, Cell::ALIVE);
    spaceship_grid.set(0, 1, Cell::ALIVE);
    spaceship_grid.set(0, 2, Cell::ALIVE);
    spaceship_grid.set(4, 2, Cell::ALIVE);
    spaceship_grid.set(0, 3, Cell::ALIVE);
    spaceship_grid.set(1, 3, Cell::ALIVE);
    spaceship_grid.set(2, 3, Cell::ALIVE);
    spaceship_grid.set(3, 3, Cell::ALIVE);
    return spaceship_grid;
}

/**
 * Zoo::load_ascii(path)
 *
 * Load an ascii file and parse it as a grid of cells.
 * Should be implemented using std::ifstream.
 *
 * @example
 *
 *      // Load an ascii file from a directory
 *      Grid grid = Zoo::load_ascii("path/to/file.gol");
 *
 * @param path
 *      The std::string path to the file to read in.
 *
 * @return
 *      Returns the parsed grid.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if:
 *          - The file cannot be opened.
 *          - The parsed width or height is not a positive integer.
 *          - Newline characters are not found when expected during parsing.
 *          - The character for a cell is not the ALIVE or DEAD character.
 */
Grid Zoo::load_ascii(std::string path) {
    std::ifstream input(path);
    if(input.fail()){
        throw(std::ios_base::failure("File cannot be opened"));
    }
    std::string line;
    int counter = 0;
    int width;
    int height;
    Grid grid;
    unsigned int y = 0;
    while(getline(input, line)){
        if(counter == 0) {
            const size_t pos = line.find(' ');
            width = atoi(line.substr(0, pos).c_str());
            height = atoi(line.substr(pos +1, pos).c_str());
            if(width < 0 || height < 0) {
                throw(std::runtime_error("Width or height is negative"));
            }
            grid = Grid(width, height);
            counter++;
        }else {
            for(unsigned int x = 0; x < line.length()-1; x++) {
                if(line[x] != ' ' && line [x] != '#') {
                    throw(std::runtime_error("Invalid cell characters"));
                }
                if(line[x] == '#') {
                    grid.set(x, y, Cell::ALIVE);

                }
            }
            y++;
        }
    }
    input.close();
    return grid;
}

/**
 * Zoo::save_ascii(path, grid)
 *
 * Save a grid as an ascii .gol file according to the specified file format.
 * Should be implemented using std::ofstream.
 *
 * @example
 *
 *      // Make an 8x8 grid
 *      Grid grid(8);
 *
 *      // Save a grid to an ascii file in a directory
 *      try {
 *          Zoo::save_ascii("path/to/file.gol", grid);
 *      }
 *      catch (const std::exception &ex) {
 *          std::cerr << ex.what() << std::endl;
 *      }
 *
 * @param path
 *      The std::string path to the file to write to.
 *
 * @param grid
 *      The grid to be written out to file.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if the file cannot be opened.
 */
void Zoo::save_ascii(std::string path, Grid grid) {

    std::ofstream output(path);
    if(output.fail()){
        throw(std::ios_base::failure("File cannot be opened"));
    }
    output << grid.get_width() << " " << grid.get_height() << "\n";
    for(int y = 0; y < grid.get_height(); y++) {
        for(int x = 0; x< grid.get_width(); x++) {
            output << (char)grid.get(x, y);
        }
        output << "\n";
    }
}

/**
 * Zoo::load_binary(path)
 *
 * Load a binary file and parse it as a grid of cells.
 * Should be implemented using std::ifstream.
 *
 * @example
 *
 *      // Load an binary file from a directory
 *      Grid grid = Zoo::load_binary("path/to/file.bgol");
 *
 * @param path
 *      The std::string path to the file to read in.
 *
 * @return
 *      Returns the parsed grid.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if:
 *          - The file cannot be opened.
 *          - The file ends unexpectedly.
 */


/**
 * Zoo::save_binary(path, grid)
 *
 * Save a grid as an binary .bgol file according to the specified file format.
 * Should be implemented using std::ofstream.
 *
 * @example
 *
 *      // Make an 8x8 grid
 *      Grid grid(8);
 *
 *      // Save a grid to an binary file in a directory
 *      try {
 *          Zoo::save_binary("path/to/file.bgol", grid);
 *      }
 *      catch (const std::exception &ex) {
 *          std::cerr << ex.what() << std::endl;
 *      }
 *
 * @param path
 *      The std::string path to the file to write to.
 *
 * @param grid
 *      The grid to be written out to file.
 *
 * @throws
 *      Throws std::runtime_error or sub-class if the file cannot be opened.
 */

