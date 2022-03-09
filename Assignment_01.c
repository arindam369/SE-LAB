/*
 * Software Engineering Lab : Assignment - 01 : Team - 04
 *
 * 1. Abhishek Agarwal      (Roll No. - 002011001033)
 * 2. Sohan Singhamahapatra (Roll No. - 002011001035)
 * 3. Arindam Halder        (Roll No. - 002011001036)
 * 4. Rajdeep Mallick       (Roll No. - 002011001050)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    FILE *ptr1, *ptr2;
    ptr1 = fopen("256x256_cover2.txt", "r"); // file pointer to cover data
    ptr2 = fopen("Secret_info2.txt", "r");   // file pointer to secret info
    int cover_data[256][256], secret_info[256][256], embedded_data[256][256];

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            fscanf(ptr1, "%d", &cover_data[i][j]);  // scanning file from cover data text file
            fscanf(ptr2, "%d", &secret_info[i][j]); // scanning file from secret info text file
        }
    }
    fclose(ptr1);
    fclose(ptr2);

    /*embedding secret info into cover data by 1 LSB substitution and storing it in embedded_data[][] array*/
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            embedded_data[i][j] = cover_data[i][j] - (cover_data[i][j] % 2) + secret_info[i][j];
        }
    }

    // calculating mean square error(MSE) and peak signal to noise ratio(PSNR)
    float square_sum = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            square_sum = square_sum + (cover_data[i][j] - embedded_data[i][j]) * (cover_data[i][j] - embedded_data[i][j]);
        }
    }

    double MSE, PSNR;
    MSE = square_sum / (256 * 256);
    printf("MSE = %f\n", MSE);
    PSNR = 10 * log10((255 * 255) / MSE);
    printf("PSNR = %f ", PSNR);

    // creating pgm file
    FILE *pgm_img;
    int w = 256, h = 256;
    pgm_img = fopen("pgm_img1.pgm", "wb"); // write the file in binary mode
    fprintf(pgm_img, "P2\n");              // writing magic no. to file
    fprintf(pgm_img, "%d %d\n", w, h);     // writing width and height to the file
    fprintf(pgm_img, "255\n");             // writing the maximum gray value (brightness of a pixel)
    int count = 0;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            fprintf(pgm_img, "%d ", embedded_data[i][j]); // copying gray value from array to pgm file
        }
        fprintf(pgm_img, "\n");
    }
    fclose(pgm_img);

    return 0;
}
