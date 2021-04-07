/** ***************************************************************************
 * @file
 *
 * @brief contains the source code for functions that does image processing
 *****************************************************************************/

/*
Written by Aidan Coopman
This .cpp file contains all the functions that modify and change the image
*/

#include "netPBM.h"


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function negates the image by subtracting 255 from each pixel value.
 *
 *
 * @param[in]      info - the main image struct
 *****************************************************************************/
void negate(image& info)
{
    for (int i = 0; i < info.rows; i++)
    {
        for (int j = 0; j < info.cols; j++)
        {
            info.redgray[i][j] = 255 - (int)info.redgray[i][j];
            info.green[i][j] = 255 - (int)info.green[i][j];
            info.blue[i][j] = 255 - (int)info.blue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function brightens the image by adding the user specified int to the 
 * pixel value for RGB.
 *
 * @param[in]      info - the main image struct
 * @param[in]      bright_value - the brightness value to be added
 *****************************************************************************/
void brighten(image& info, int bright_value)
{
    int count = 0;
    int value;

    for (int i = 0; i < info.rows; i++)
    {
        count = 0;

        for (int j = 0; j < info.cols; j++)
        {
            //brighten RED------------------------------------
            value = (int)info.redgray[i][j];
            //make sure it stays in the boundries
            if (value + bright_value < 0)
                info.redgray[i][j] = 0;
            else if (value + bright_value > 255)
                info.redgray[i][j] = 255;
            else
                info.redgray[i][j] = value + bright_value;

            //brighten GREEN----------------------------------
            value = (int)info.green[i][j];
            //make sure it stays in the boundries
            if (value + bright_value < 0)
                info.green[i][j] = 0;
            else if (value + bright_value > 255)
                info.green[i][j] = 255;
            else
                info.green[i][j] = value + bright_value;

            //brighten BLUE-----------------------------------
            value = (int)info.blue[i][j];
            //make sure it stays in the boundries
            if (value + bright_value < 0)
                info.blue[i][j] = 0;
            else if (value + bright_value > 255)
                info.blue[i][j] = 255;
            else
                info.blue[i][j] = value + bright_value;
        }
    }
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function sharpens the image in a 3x3 kernal. First it creates and
 * allocates memory for 3 new arrays. Then it sets the pixel values and uses
 * the formula: Newred[i][j] = 5 * bb - b - aa - cc - bbb. Then it copies the
 * data from the newly created array into the old array to create a sharper 
 * image. Then it deletes the memory created by the arrays.
 *
 * @param[in]      info - the main image struct
 * 
 * @return true if the image was sucessfully sharpened
 * @returns false if something failed in the process of sharpening the image.
 *****************************************************************************/
bool sharpen(image& info)
{
    int b;
    int aa, bb, cc;
    int bbb;
    int sum;
    pixel** new_red;
    pixel** new_green;
    pixel** new_blue;

    //allocate memory for this array RED
    new_red = alloc2d(info.rows, info.cols); 
    //allocate memory for this array GREEN
    new_green = alloc2d(info.rows, info.cols);
    //allocate memory for this array BLUE
    new_blue = alloc2d(info.rows, info.cols); 
    if (new_red == nullptr || new_green == nullptr || new_blue == nullptr)
    {
        return 0;
    }

    for (int i = 1; i < (info.rows - 1); i++)
    {
        for (int j = 1; j < (info.cols - 1); j++)
        {
            //RED ----------------------------------------
            b   = (int)info.redgray[i - 1][j + 1];
            aa  = (int)info.redgray[i][j - 1];
            bb  = (int)info.redgray[i][j];
            cc  = (int)info.redgray[i + 1][j + 1];
            bbb = (int)info.redgray[i + 1][j];
            sum = 5 * bb - b - aa - cc - bbb;

            if (sum > 255) //make sure it doesnt go out of boundries
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_red[i][j] = sum;

            //GREEN --------------------------------------
            b   = (int)info.green[i - 1][j + 1];
            aa  = (int)info.green[i][j - 1];
            bb  = (int)info.green[i][j];
            cc  = (int)info.green[i + 1][j + 1];
            bbb = (int)info.green[i + 1][j];
            sum = 5 * bb - b - aa - cc - bbb;

            if (sum > 255) //make sure it doesnt go out of boundries
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_green[i][j] = sum;

            //BLUE ---------------------------------------
            b   = (int)info.blue[i - 1][j + 1];
            aa  = (int)info.blue[i][j - 1];
            bb  = (int)info.blue[i][j];
            cc  = (int)info.blue[i + 1][j + 1];
            bbb = (int)info.blue[i + 1][j];
            sum = 5 * bb - b - aa - cc - bbb;

            if (sum > 255) //make sure it doesnt go out of boundries
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_blue[i][j] = sum;
        }
    }

    //move the data back to the proper 2D arrays
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            //set border to 0
            if (i == 0 || j == 0 || i == info.rows-1 || j == info.cols-1)
            {
                info.redgray[i][j] = 0;
                info.blue[i][j] = 0;
                info.green[i][j] = 0;
            }
            else
            {
                info.redgray[i][j] = new_red[i][j];
                info.blue[i][j] = new_blue[i][j];
                info.green[i][j] = new_green[i][j];
            }
        }
    }

    //free the memory
    free2d(new_red, info.rows);
    free2d(new_green, info.rows);
    free2d(new_blue, info.rows);
    return 1;
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function smooths the image in a 3x3 kernal. First it creates and
 * allocates memory for 3 new arrays. Then it sets the pixel values and uses
 * the formula: 
 * Newred[i][j] = (a + b + c + aa + bb + cc + aaa + bbb + ccc) / 9
 * Then it copies the data from the newly created array into the old array to 
 * create a smoother image.
 * 
 *
 * @param[in]      info - the main image struct
 *
 * @return true if the image was sucessfully smoothed
 * @returns false if something failed in the process of sharpening the image.
 *****************************************************************************/
bool smooth(image& info)
{
    int a, b, c;
    int aa, bb, cc;
    int aaa, bbb, ccc;
    int sum;
    pixel** new_red;
    pixel** new_green;
    pixel** new_blue;

    //allocate memory for this array RED
    new_red = alloc2d(info.rows, info.cols); 
    //allocate memory for this array GREEN
    new_green = alloc2d(info.rows, info.cols);
    //allocate memory for this array BLUE
    new_blue = alloc2d(info.rows, info.cols); 
    if (new_red == nullptr || new_green == nullptr || new_blue == nullptr)
    {
        return 0;
    }

    for (int i = 1; i < (info.rows - 1); i++)
    {
        for (int j = 1; j < (info.cols - 1); j++)
        {
            //RED ----------------------------------------
            a   = (int)info.redgray[i - 1][j - 1];
            b   = (int)info.redgray[i - 1][j];
            c   = (int)info.redgray[i - 1][j + 1];
            aa  = (int)info.redgray[i][j - 1];
            bb  = (int)info.redgray[i][j]; //mid point
            cc  = (int)info.redgray[i][j + 1];
            aaa = (int)info.redgray[i + 1][j - 1];
            bbb = (int)info.redgray[i + 1][j];
            ccc = (int)info.redgray[i + 1][j + 1];

            sum = (a + b + c + aa + bb + cc + aaa + bbb + ccc) / 9;

            if (sum > 255) //make sure it doesnt go out of boundries
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_red[i][j] = sum;

            //GREEN --------------------------------------
            a   = (int)info.green[i - 1][j - 1];
            b   = (int)info.green[i - 1][j];
            c   = (int)info.green[i - 1][j + 1];
            aa  = (int)info.green[i][j - 1];
            bb  = (int)info.green[i][j]; //mid point
            cc  = (int)info.green[i][j + 1];
            aaa = (int)info.green[i + 1][j - 1];
            bbb = (int)info.green[i + 1][j];
            ccc = (int)info.green[i + 1][j + 1];

            sum = (a + b + c + aa + bb + cc + aaa + bbb + ccc) / 9;
            if (sum > 255) //make sure it doesnt go out of boundries
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_green[i][j] = sum;

            //BLUE ---------------------------------------
            a   = (int)info.blue[i - 1][j - 1];
            b   = (int)info.blue[i - 1][j];
            c   = (int)info.blue[i - 1][j + 1];
            aa  = (int)info.blue[i][j - 1];
            bb  = (int)info.blue[i][j]; //mid point
            cc  = (int)info.blue[i][j + 1];
            aaa = (int)info.blue[i + 1][j - 1];
            bbb = (int)info.blue[i + 1][j];
            ccc = (int)info.blue[i + 1][j + 1];

            sum = (a + b + c + aa + bb + cc + aaa + bbb + ccc) / 9;

            //make sure it doesnt go out of boundries
            if (sum > 255) 
                sum = 255;
            if (sum < 0)
                sum = 0;
            new_blue[i][j] = sum;
        }
    }

    //move the data back to the proper 2D arrays
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            //set border to 0
            if (i == 0 || j == 0 || i == info.rows - 1 || j == info.cols - 1)
            {
                info.redgray[i][j] = 0;
                info.blue[i][j] = 0;
                info.green[i][j] = 0;
            }
            else
            {
                info.redgray[i][j] = new_red[i][j];
                info.blue[i][j] = new_blue[i][j];
                info.green[i][j] = new_green[i][j];
            }
        }
    }

    //free the memory
    free2d(new_red, info.rows);
    free2d(new_green, info.rows);
    free2d(new_blue, info.rows);
    return 1;
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function converts the inputed struct into grayscale format. It does
 * this by creating and allocating a new array to store new pixel values in.
 * Next it uses the formula:
 * gray[i][j] = .3 * redgray[i][j] + .6 * green[i][j] + .1 * blue[i][j]
 * Once it is converted into gray[][], it copies the data across into the 
 * array redgray[][]
 *
 * @param[in]      info - the main image struct
 *
 * @return true if the image was sucessfully smoothed
 * @returns false if something failed in the process of converting the image
 * to grayscale.
 *****************************************************************************/
bool grayscale(image& info)
{
    pixel** gray;
    double temp;
    //allocate memory for this array GRAY
    gray = alloc2d(info.rows, info.cols); 
    if (gray == nullptr)
    {
        return 0;
    }

    //convert to grayscale
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            temp = .3 * (int)info.redgray[i][j] + .6 
                * (int)info.green[i][j] + .1 
                * (int)info.blue[i][j];
            gray[i][j] = (int)temp;
        }
    }

    //move data from gray to redgray
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            info.redgray[i][j] = (int)gray[i][j];
        }
    }

    free2d(gray, info.rows);
    return 1;
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function converts the image into grayscale then adds contrast to the
 * grayscale image. First it converts it to grayscale and records the max
 * pixel value and the min pixel value, then it calculates the scale:
 * scale = 255 / (max - min);
 * Next it multiplies the scale to the pixel value
 * 
 * 
 *
 * @param[in]      info - the main image struct
 *
 * @return true if the image was sucessfully smoothed
 * @returns false if something failed in the process of converting the image
 * to grayscale and adding contrast.
 *****************************************************************************/
bool contrast(image& info)
{
    pixel** gray;
    double temp;
    double max = 0.0;
    double min = 256.0;
    double scale;
    pixel temp_pixel;

    //allocate memory for this array GRAY
    gray = alloc2d(info.rows, info.cols); 
    if (gray == nullptr)
    {
        return 0;
    }

    //convert to grayscale
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            temp = .3 * (int)info.redgray[i][j] + .6 
                * (int)info.green[i][j] + .1 
                * (int)info.blue[i][j];

            gray[i][j] = (int)temp;
            //cout << "temp = " << temp << endl;
            if ((int)gray[i][j] > max)
                max = (int)gray[i][j];
            
            if ((int)gray[i][j] < min)
                min = (int)gray[i][j];
        }
    }

    scale = 255.0 / (max - min);

    //move data from gray to redgray
    for (int i = 0; i < (info.rows); i++)
    {
        for (int j = 0; j < (info.cols); j++)
        {
            temp_pixel = (pixel)(scale * ((double)gray[i][j] - min));
            if ((int)temp_pixel > 255)
                info.redgray[i][j] = 255;
            if ((int)temp_pixel < 0)
                info.redgray[i][j] = 0;
            info.redgray[i][j] = temp_pixel;

        }
    }

    free2d(gray, info.rows);
    return 1;
}