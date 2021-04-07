/** ***************************************************************************
 * @file
 *
 * @brief main cpp file, contains the main() function
 *****************************************************************************/

#include "netPBM.h"

 /** ***************************************************************************
   * @mainpage Program 2 - Image Manipulation
   *
   * @section course_section Course Information
   *
   * @authors Aidan Coopman
   *
   * @date  November 07, 2020
   *
   * @par Instructor
   *         Professor Roger Schrader
   *
   * @par Course
   *         CSC 215 - Section ? - 9:00 am
   *
   * @par Location
   *          - Room
   *
   * @section program_section Program Information
   *
   * @details This program takes in an image inputed by the user and is
   * then manipulated to the users specifactions. Depending on the inputed
   * arguments it can  manipulate the image in 5 different ways, it can
   * negate the image, brighten the image, sharpen/smooth the image, convert
   * the image to grayscale, or add contrast to the image. It will then output
   * the image based on what the user has inputed. If -oa is specified it
   * will output the image in ASCII format. If -ob is specified it will
   * output the image in BINARY format.
   *
   * This program starts by finding the magic number, then copying and
   * skiping over the comment, then taking in the number of rows and columns
   * in the image, then finally taking in the maximum pixel value which
   * leaves it pointing now to the actual RGB pixel values.
   *
   * Now the rgb data will be inputed in two different ways based on the
   * inputted image's magic number. If the inputted image is in ASCII format
   * it will be read into the struct line by line. If the format is in
   * BINARY it will read the pixel values into a vector then into the
   * struct.
   *
   * After the image is read in it will change the image to the users
   * inputted specifactions. Then it will be outputed to the users
   * specifactions(BINARY or ASCII) and it will change the magic
   * number to the correct type and output the final image.
   *
   *
   * The program will return 0.
   *
   *
   * @section compile_section Compiling and Usage
   *
   * @par Compiling Instructions:
   *      None
   *
   * @par Usage
     @verbatim
     c:\> prog1.exe [option] -o[ab] basename image.ppm
     option:
         -n - negate the image(invert)
         -b # - brighten the image by # amount
         -p - sharpen the image
         -s - smooths the image
         -g - convert the image to grayscale
         -c - add contrast to input image (grayscale only)
         -oa - output to ASCII
         -ob - output to BINARY
     @endverbatim
   *
   * @section todo_bugs_modification_section Todo, Bugs, and Modifications
   *
   * @bug contrast not working propely
   *
   * @todo fix contrast function
   *
   * @par Modifications and Development Timeline:
     @verbatim

   * @par Modifications and Development Timeline:
   *<a href="https://gitlab.mcs.sdsmt.edu/101067892/csc215f20programs/-/tree/master/prog1" target=_blank>
   *     Gitlab Commit Log</a>
   *
   *****************************************************************************/




   /** ***************************************************************************
    * @author Aidan Coopman
    *
    * @par Description: contains the main()
    *
    *
    *
    *
    * @param[in]      argc - a count of the command line arguments used to start
    *                        the program.
    * @param[in]     argv - a 2d character array of each argument.  Each token
    *                        occupies one line in the array.
    *
    * @param[out] outputs image file
    *
    * @returns 0.
    *
    *
    *****************************************************************************/
int main(int argc, char** argv)
{
    //varibles
    ifstream in;
    ofstream out;
    image info;
    string input_type; //magic number INPUT
    string output_type; //magic number OUTPUT
    string comment_line;
    string output_name;
    string input_name;
    string pgm = ".pgm";
    string ppm = ".ppm";
    string option_code = "FAIL";
    int bright_value = 0;
    int max_pixel_value;
    char temp;

    //make sure there is enough arguments and they open successfully
    if (argc < 4 || argc > 6)
    {
        cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" << endl;
        return 0;
    }

    output_name = argv[argc - 2];
    input_name = argv[argc - 1];

    //check if it is going to grayscale or not (ppm vs pgm)
    if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-c") == 0)
        output_name.append(pgm);
    else
        output_name.append(ppm);

    //-oa
    if (strcmp(argv[1], "-oa") == 0)
    {
        output_type = "P3";
        option_code = "TRUE";
        open_files(in, out, input_name, output_name);
    }

    //-ob
    if (strcmp(argv[1], "-ob") == 0)
    {
        output_type = "P6";
        option_code = "TRUE";
        open_files(in, out, input_name, output_name);
    }

    //-n
    if (strcmp(argv[1], "-n") == 0)
    {
        option_code = "n";
        if (strcmp(argv[2], "-oa") == 0)
        {
            output_type = "P3";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[2], "-ob") == 0)
        {
            output_type = "P6";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }

    }


    //-b
    if (strcmp(argv[1], "-b") == 0)
    {
        option_code = "b";
        bright_value = stoi(argv[2]);

        if (strcmp(argv[3], "-oa") == 0)
        {
            output_type = "P3";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[3], "-ob") == 0)
        {
            output_type = "P6";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }

    }

    //-p
    if (strcmp(argv[1], "-p") == 0)
    {
        option_code = "p";
        if (strcmp(argv[2], "-oa") == 0)
        {
            output_type = "P3";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[2], "-ob") == 0)
        {
            output_type = "P6";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }
    }
    //-s
    if (strcmp(argv[1], "-s") == 0)
    {
        option_code = "s";
        if (strcmp(argv[2], "-oa") == 0)
        {
            output_type = "P3";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[2], "-ob") == 0)
        {
            output_type = "P6";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }

    }

    //-g
    if (strcmp(argv[1], "-g") == 0)
    {
        option_code = "g";
        if (strcmp(argv[2], "-oa") == 0)
        {
            output_type = "P2";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[2], "-ob") == 0)
        {
            output_type = "P5";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }

    }

    //-c
    if (strcmp(argv[1], "-c") == 0)
    {
        option_code = "c";
        if (strcmp(argv[2], "-oa") == 0)
        {
            output_type = "P2";
            open_files(in, out, input_name, output_name);
        }
        else if (strcmp(argv[2], "-ob") == 0)
        {
            output_type = "P5";
            open_files(in, out, input_name, output_name);
        }
        else
        {
            cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
                << endl;
            return 0;
        }

    }


    if (option_code == "FAIL")
    {
        cout << "Usage: prog1.exe [option] -o[ab] basename image.ppm" 
            << endl;
        return 0;
    }

    //make sure files successfully opened
    if (in.is_open() == 0)
    {
        cout << "Unable to open input file: " << argv[argc - 2] << endl;
        return 0;
    }
    if (out.is_open() == 0)
    {
        cout << "Unable to open output file: " << argv[2] << endl;
        return 0;
    }


    //get the type(magic number)
    in >> input_type;
    //make sure it is a P3 or P6
    if (input_type == "P3" || input_type == "P6")
    {
        //cout << "equal\n";
    }
    else
    {
        cout << "Invalid magic number " << input_type << endl;
        return 0;
    }

    //check if a comment line is present
    while (temp = in.peek(), temp == '\n')
        in.get();
    if (temp == '#')
    {
        //grab the comment line
        getline(in, comment_line);
    }

    //get the rows and cols for dynamic allocation
    in >> info.cols;
    in >> info.rows;

    //get the max pixel value
    in >> max_pixel_value;

    //Allocate memory --------------------------------------------------
    //allocate memory for redgray, green and red
    info.redgray = alloc2d(info.rows, info.cols); 
    info.green = alloc2d(info.rows, info.cols);
    info.blue = alloc2d(info.rows, info.cols);

    if (info.redgray == nullptr 
        || info.green == nullptr 
        || info.blue == nullptr)
    {
        cout << "Memory allocation failed. Exiting" << endl;
        return 0;
    }

    //input the data into the 2D arrays
    input_data(info, in, input_type);

    //Image Operations -----------
    if (option_code == "n")
    {
        negate(info);
    }
    if (option_code == "b")
    {
        brighten(info, bright_value);
    }
    if (option_code == "p")
    {
        if (sharpen(info) == 0)
        {
            cout << "Memory allocation failed in sharpen(). Exiting" << endl;
            return 0;
        }
    }
    if (option_code == "s")
    {
        if (smooth(info) == 0)
        {
            cout << "Memory allocation failed in smooth(). Exiting" << endl;
            return 0;
        }
    }
    if (option_code == "g")
    {
        if (grayscale(info) == 0)
        {
            cout << "Memory allocation failed in grayscale(). Exiting" << endl;
            return 0;
        }
    }
    if (option_code == "c")
    {
        if (contrast(info) == 0)
        {
            cout << "Memory allocation failed in contrast(). Exiting" << endl;
            return 0;
        }
    }

    //output the data
    output_data(info, out, output_type, comment_line, max_pixel_value);

    //free the memory and close files
    free2d(info.redgray, info.rows);
    free2d(info.green, info.rows);
    free2d(info.blue, info.rows);

    close_files(in, out);

    return 0;
} //main
