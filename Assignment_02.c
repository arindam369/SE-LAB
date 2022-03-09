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
    ptr2 = fopen("Secret_info9.txt", "r");   // file pointer to secret_info

    int cover_data[256][256], secret_info[256][512];
    int embedded_data1[256][256]; // 2 LSB without OPAP
    int embedded_data2[256][256]; // 2 LSB with OPAP

    // Scanning cover data text file && storing it in cover_data[256][256]
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            fscanf(ptr1, "%d", &cover_data[i][j]);
        }
    }
    // Scanning secret info text file && storing it in secret_info[256][512]
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 512; j++)
        {
            fscanf(ptr2, "%d", &secret_info[i][j]);
        }
    }
    fclose(ptr1);
    fclose(ptr2);

    /*embedding secret info into cover data by 2 LSB substitution and storing it in embedded_data1[][] array*/
    for (int i = 0; i < 256; i++)
    {

        for (int j = 0; j < 256; j++)
        {
            // taking two consecutive no. from secret info & embeding it into cover data
            embedded_data1[i][j] = cover_data[i][j] - (cover_data[i][j] % 4) + 2 * secret_info[i][2 * j] + secret_info[i][2 * j + 1];
        }
    }

    double square_sum = 0, temp1;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            square_sum = square_sum + (cover_data[i][j] - embedded_data1[i][j]) * (cover_data[i][j] - embedded_data1[i][j]);
        }
    }

    // MSE & PSNR without OPAP
    double MSE1, PSNR1;
    MSE1 = square_sum / (256 * 256);
    printf("Without OPAP:\n");
    printf("MSE = %f\n", MSE1);
    PSNR1 = 10 * log10((255 * 255) / MSE1);
    printf("PSNR = %f ", PSNR1);

    // Optical Pixel Adjustment Process (OPAP)

    double square_sum2 = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            int d = embedded_data1[i][j] - cover_data[i][j];
            if (d > 2 && d < 4 && embedded_data1[i][j] >= 4)
            {
                embedded_data2[i][j] = embedded_data1[i][j] - 4;
            }
            else if (d >= -4 && d <= -2 && embedded_data1[i][j] < (256 - 4))
            {
                embedded_data2[i][j] = embedded_data1[i][j] + 4;
            }
            else
            {
                embedded_data2[i][j] = embedded_data1[i][j];
            }
            square_sum2 = square_sum2 + (cover_data[i][j] - embedded_data2[i][j]) * (cover_data[i][j] - embedded_data2[i][j]);
        }
    }
    // MSE & PSNR with OPAP
    double MSE2, PSNR2, max;
    MSE2 = square_sum2 / (256 * 256);
    max = 255;
    PSNR2 = 10 * log10((max * max) / MSE2);
    printf("\nWith OPAP:\n");
    printf("MSE is = %f\n", MSE2);
    printf("PSNR is = %f\n", PSNR2);

    // Creating PGM file
    int i, j;
    int width = 256, height = 256;
    FILE *pgm_img;
    pgm_img = fopen("pgm_img2.pgm", "wb");

    // Writing Magic Number to the File
    fprintf(pgm_img, "P2\n"); // The magic number P2 means a gray-tone image in ASCII format

    // Writing Width and Height
    fprintf(pgm_img, "%d %d\n", width, height);

    // Writing the maximum gray value
    fprintf(pgm_img, "255\n"); // Maximum Value: An integer indicating the maximum value of a pixel (picture element) in the given image.
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // Writing the gray values in the 2D array to the file
            fprintf(pgm_img, "%d ", embedded_data2[i][j]);
        }
        fprintf(pgm_img, "\n");
    }
    fclose(pgm_img);
    return 0;
}