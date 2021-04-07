/** ***************************************************************************
 * @file
 *
 * @brief contains the source code for functions that does file IO 
 * and opening and closing files
 *****************************************************************************/

#include "netPBM.h"



/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function opens the files in binary mode
 *
 *
 *
 * @param[in]      in - ifsteam input file
 * @param[in]      out - ofsteam output file
 * @param[in]      input_name - input file name
 * @param[in]      output_name - output file name
 *****************************************************************************/
void open_files(ifstream& in, ofstream& out, 
    string input_name, string output_name)
{
    in.open(input_name, ios::in | ios::binary);
    out.open(output_name, ios::out | ios::binary);
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function does all the inputing from the inputed file to the struct.
 * 
 * First it determines what the inputed magic type was, if it is P3 it 
 * inputs it right into the struct. If it is P6 it inputs it into a 
 * vector then into the struct.
 *
 *
 *
 * @param[in]      info - the main image struct
 * @param[in]      in - ifsteam input file
 * @param[in]      input_type - input file magic number
 *****************************************************************************/
void input_data(image& info, ifstream& in, string input_type)
{
    int temp;

    if (input_type == "P3") //ASCII
    {
        //initialize the array
        for (int i = 0; i < info.rows; i++) // rows
        {
            for (int j = 0; j < info.cols; j++) // collumns
            {
                in >> temp;
                info.redgray[i][j] = temp;
                in >> temp;
                info.green[i][j] = temp;
                in >> temp;
                info.blue[i][j] = temp;            }
        }
    }


    if (input_type == "P6") //BINARY
    {
        in.get();

        int count = 0;
        for (int i = 0; i < info.rows; i++) // rows
        {
            for (int j = 0; j < info.cols; j++) // collumns
            {
                in.read((char*)&info.redgray[i][j], sizeof(pixel));
                in.read((char*)&info.green[i][j], sizeof(pixel));
                in.read((char*)&info.blue[i][j], sizeof(pixel));

                count++;
            }
        }
    }
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function grabs the comment if there is one and moves over it.
 *
 * @param[in]      in - ifsteam input file
 *****************************************************************************/
void get_comment(ifstream& in)
{
    char linebuffer[1024]; //set line buffer
    char tmp;
    while (tmp = in.peek(), tmp == '\n')
        in.get();
    if (tmp == '#')
        in.getline(linebuffer, 1023);
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function does all the outputting of the final image. 
 * 
 * First it ouputs the magic number, then the comment line, then the columns
 * and rows. Then based on what the output magic number is it will output
 * it in either ASCII format or BINARY format.
 *
 *
 *
 * @param[in]      info - the main image struct
 * @param[out]      out - ifsteam input file
 * @param[in]      output_type - input file magic number
 * @param[in]      comment_line - comment line
 * @param[in]      max_value - max pixel value
 *****************************************************************************/
void output_data(image info, ofstream& out, 
    string output_type, string comment_line, int max_value)
{
    //print out type
    out << output_type << endl;

    //check to see if there is a comment
    //print out comment line (if there is one)
    if (comment_line != "\0") 
        out << comment_line << endl; 

    //print out cols x rows
    out << info.cols << " " << info.rows << endl; 
    //add just a space for the binary files
    out << max_value; 

    //P3 or P2
    if (output_type == "P3" || output_type == "P2") 
    {
        out << endl; // add endline 
    }

    //P6 or P5
    if (output_type == "P6" || output_type == "P5") 
    {
        out << " ";
    }
    if (output_type == "P3") //P3 ASCII
    {
        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out << (int)info.redgray[i][j] << endl;
                out << (int)info.green[i][j] << endl;
                out << (int)info.blue[i][j] << endl;
            }
        }
    }
    if (output_type == "P2") //P2 ASCII
    {
        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out << (int)info.redgray[i][j] << endl;

            }
        }
    }
    if (output_type == "P6") //P6 BINARY
    {
        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out.write(reinterpret_cast<char*> 
                    (&info.redgray[i][j]), sizeof(pixel));
                out.write(reinterpret_cast<char*> 
                    (&info.green[i][j]), sizeof(pixel));
                out.write(reinterpret_cast<char*> 
                    (&info.blue[i][j]), sizeof(pixel));
            }
        }
    }
    if (output_type == "P5") //P5 BINARY
    {
        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out.write(reinterpret_cast<char*> 
                    (&info.redgray[i][j]), sizeof(pixel));
            }
        }
    }
    

}//output_data


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function closes the files that have been opened.
 *
 *
 *
 * @param[in]      in - ifsteam input file
 * @param[out]      out - ifsteam output file
 *****************************************************************************/
void close_files(ifstream& in, ofstream& out)
{
    //close files
    in.close();
    out.close();
}