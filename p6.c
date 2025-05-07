/*
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 6
Purpose: Reads, processes, and writes a 24‑bit BMP images so that  you can read the file info, find the edges in the pictures, and add “noise” to the image
Assumptions: None
Known Bugs: None
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

struct HEADER {
    unsigned short int Type; //Magic identifier
    unsigned int Size; //File size in bytes
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset; //Offset to data (in B)
};//14 bytes

struct INFOHEADER{ 
    unsigned int Size; /* Header size in bytes */
    int Width, Height; /* Width / Height of image */
    unsigned short int Planes; /* Number of colour planes */
    unsigned short int Bits; /* Bits per pixel */
    unsigned int Compression; /* Compression type */
    unsigned int ImageSize; /* Image size in bytes */
    int xResolution, yResolution;/* Pixels per meter */
    unsigned int Colors; /* Number of colors */
    unsigned int ImportantColors;/* Important colors */
}; /* -- 40 Bytes -- */

struct PIXEL { unsigned char Red, Green, Blue; };

int read_headers(FILE *ptrFile, struct HEADER *Header, struct INFOHEADER *Info) {// read headers info from file
    int n;// number of bytes read
    
    n = fread(&Header->Type, sizeof(unsigned short int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Type from file.\n");   
    }
    
    n = fread(&Header->Size, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Size from file.\n");
    }
    
    n = fread(&Header->Reserved1, sizeof(unsigned short int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Reserved1 from file.\n");
    }
    
    n = fread(&Header->Reserved2, sizeof(unsigned short int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Reserved2 from file.\n");
    }
    
    n = fread(&Header->Offset, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Offset from file.\n");
    }
    
    n = fread(&Info->Size, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading InfoHeader Size from file.\n");
    }
    
    n = fread(&Info->Width, sizeof(int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Width from file.\n");
    }
    
    n = fread(&Info->Height, sizeof(int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Height from file.\n");
    }
    
    n = fread(&Info->Planes, sizeof(unsigned short int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Planes from file.\n");
    }
    
    n = fread(&Info->Bits, sizeof(unsigned short int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Bits from file.\n");
    }
    
    n = fread(&Info->Compression, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Compression from file.\n");
    }
    
    n = fread(&Info->ImageSize, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading ImageSize from file.\n");
    }
    
    n = fread(&Info->xResolution, sizeof(int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading xResolution from file.\n");
    }
    
    n = fread(&Info->yResolution, sizeof(int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading yResolution from file.\n");
    }
    
    n = fread(&Info->Colors, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading Colors from file.\n");
    }
    
    n = fread(&Info->ImportantColors, sizeof(unsigned int), 1, ptrFile);
    if (n != 1) {
        printf("Error reading ImportantColors from file.\n");
    }
    
    return 1;
}

void outputFileName(char *inputFilename, char *outputFilename,char *suffix){// create output filename
    int periodPos = -1;// position of the last period
    int i = 0;// index for inputFilename
    while (inputFilename[i] != '\0') {// loop until end of string
        if (inputFilename[i] == '.') {// check for period
            periodPos = i;// set to position of period
        }
        i++;// increment index
    }

    if (periodPos != -1) {// if period found
        strncpy(outputFilename, inputFilename, periodPos);// copy up to period
        outputFilename[periodPos] = '\0';
        
        strcat(outputFilename, suffix);// add suffix
        
        strcat(outputFilename, &inputFilename[periodPos]);// add rest of filename
    } else {
        printf("Error: No period found in filename.\n");
    }
}

double UniformRV(double min, double max)// generate uniform random variable
{
    double x;
    x = (double)rand()/((double)(RAND_MAX)+1.0);// generate random number
    
    if (max > min)
    {
        x *= max - min;// scale to range
        x += min;
    }
    return (x);
}

double GaussianRV(void)// generate Gaussian random variable
{
    double u1, u2, g;
    u1 = UniformRV(0,1);// generate uniform random variable
    u2 = UniformRV(0,1);// generate uniform random variable
    g = cos(8.0*atan(1.0)*u2)*sqrt(-2.0*log(u1));// Box-Muller transform
    return g;
}

void read_export(char *inputFilename, char *outputFilename, struct HEADER *Header, struct INFOHEADER *Info){// read and export BMP file
    FILE *ptrFile = fopen(inputFilename, "rb");// open file in binary mode
    if (ptrFile == NULL) {
        printf("Error opening file \"%s\"\n", inputFilename);
        return; // returns nothing since void function(same for the rest of the functions)
    }
    
    read_headers(ptrFile, Header, Info);// read headers from file

    FILE *ptrOutFile = fopen(outputFilename, "w");// open output file in text mode
    if (ptrOutFile == NULL) {
        printf("Error opening output file \"%s\"\n", outputFilename);
        return;
    }

    int padding = (4 - ((Info->Width * 3) % 4)) % 4;// calculate padding

    // write data from input file to output file
    fprintf(ptrOutFile, "\"%s\"\n", inputFilename);                                             
    fprintf(ptrOutFile, "Header.Type = %c\n", Header->Type);
    fprintf(ptrOutFile, "Header.Type = %c\n", Header->Type >> 8);
    fprintf(ptrOutFile, "Header.Size = %u\n", Header->Size);
    fprintf(ptrOutFile, "Header.Offset = %u\n", Header->Offset);
    fprintf(ptrOutFile, "InfoHeader.Size = %u\n", Info->Size);
    fprintf(ptrOutFile, "InfoHeader.Width = %d\n", Info->Width);
    fprintf(ptrOutFile, "InfoHeader.Height = %d\n", Info->Height);                      
    fprintf(ptrOutFile, "InfoHeader.Planes = %u\n", Info->Planes);
    fprintf(ptrOutFile, "InfoHeader.Bits = %u\n", Info->Bits);
    fprintf(ptrOutFile, "InfoHeader.Compression = %u\n", Info->Compression);
    fprintf(ptrOutFile, "InfoHeader.ImageSize = %u\n", Info->ImageSize);
    fprintf(ptrOutFile, "InfoHeader.xResolution = %d\n", Info->xResolution);
    fprintf(ptrOutFile, "InfoHeader.yResolution = %d\n", Info->yResolution);
    fprintf(ptrOutFile, "InfoHeader.Colors = %u\n", Info->Colors);
    fprintf(ptrOutFile, "InfoHeader.ImportantColors = %u\n", Info->ImportantColors);            
    fprintf(ptrOutFile, "Padding = %d\n", padding);
    
    rewind(ptrFile);  // move to beginning of file
    
    unsigned char byte;
    for (int i = 0; i < Header->Offset; i++) {// read header data
        fread(&byte, 1, 1, ptrFile);// read byte
        fprintf(ptrOutFile, "Byte[%d] = %03d\n", i, byte);// write to output file
}
    fseek(ptrFile, Header->Offset, SEEK_SET);// move to pixel data

    for (int row = 0; row < Info->Height; row++) {// loop through rows
        for (int col = 0; col < Info->Width; col++) {// loop through columns
            unsigned char pixel[3];
            fread(pixel, 3, 1, ptrFile);// read pixel data
            fprintf(ptrOutFile, "RGB[%d,%d] = %03d.%03d.%03d\n", row, col, pixel[2], pixel[1], pixel[0]);// write pixel data to output file
        }
        
        unsigned char pad;
        for (int p = 0; p < padding; p++) {// read padding data
            fread(&pad, 1, 1, ptrFile);
            fprintf(ptrOutFile, "Padding[%d] = %03d\n", p, pad);// write padding data to output file
        }
    }
    fclose(ptrFile);
    fclose(ptrOutFile);
}

void edge(char *inputFilename, struct HEADER *Header, struct INFOHEADER *Info){// apply edge detection
    
    FILE *ptrFile = fopen(inputFilename, "rb");// open file in binary mode
    if (ptrFile == NULL) {
        printf("Error opening file \"%s\"\n", inputFilename);
        return;
    }

    read_headers(ptrFile, Header, Info);//read headers from file

    char outputFilename[100];
    outputFileName(inputFilename, outputFilename, "-edge");// create output filename

    struct PIXEL **pixels = (struct PIXEL **)malloc(Info->Height * sizeof(struct PIXEL *));// allocate memory for pixel array
    if (pixels == NULL) {
        printf("Error allocating memory for pixel array.\n");
        return;
    }
    pixels[0] = (struct PIXEL *)malloc(Info->Width * Info->Height * sizeof(struct PIXEL));// allocate memory for pixel array
    if (pixels[0] == NULL) {
        printf("Error allocating memory for pixel array.\n");
        return;
    }
    for (int i = 1; i < Info->Height; i++) {// set up pixel array
        pixels[i] = pixels[0] + i * Info->Width;
    }

    fseek(ptrFile, Header->Offset, SEEK_SET);// move to pixel data
    int padding = (4 - ((Info->Width * 3) % 4)) % 4;// calculate padding

    for (int row = 0; row < Info->Height; row++) {// loop through rows
        for (int col = 0; col < Info->Width; col++) {// loop through pixels
            fread(&pixels[row][col].Blue, 1, 1, ptrFile);
            fread(&pixels[row][col].Green, 1, 1, ptrFile);// read pixel data
            fread(&pixels[row][col].Red, 1, 1, ptrFile);
        }
        fseek(ptrFile, padding, SEEK_CUR);// move to next row
    }

    char Matrix[3][3] = {//edge detection matrix
        { 0, -1, 0 },
        { -1, 4, -1 },
        { 0, -1, 0 }
    };
    
    struct PIXEL **newPixels = (struct PIXEL **)malloc(Info->Height * sizeof(struct PIXEL *));// allocate memory for new pixel array
    if (newPixels == NULL) {
        printf("Error allocating memory for pixel array.\n");
        return;
    }
    
    for (int i = 0; i < Info->Height; i++) {// set up new pixel array
        newPixels[i] = (struct PIXEL *)malloc(Info->Width * sizeof(struct PIXEL));// allocate memory for new pixel array
        if (newPixels[i] == NULL) {
            printf("Error allocating memory for new pixel array.\n");
            return;
        }
    }
    
    for (int row = 0; row < Info->Height; row++) {// loop through rows
        for (int col = 0; col < Info->Width; col++) {// loop through pixels
            int sumRed = 0, sumGreen = 0, sumBlue = 0;// initialize sums
            
            for (int i = -1; i <= 1; i++) {// loop through matrix
                for (int j = -1; j <= 1; j++) {// loop through matrix
                    int r = row + i;
                    int c = col + j;
                    
                    if (r >= 0 && r < Info->Height && c >= 0 && c < Info->Width) {// check bounds
                        sumRed += pixels[r][c].Red * Matrix[i+1][j+1];// multiply by matrix value
                        sumGreen += pixels[r][c].Green * Matrix[i+1][j+1];// multiply by matrix value
                        sumBlue += pixels[r][c].Blue * Matrix[i+1][j+1];// multiply by matrix value
                    }
                }
            }

            if (sumRed < 0) {// check for underflow
                newPixels[row][col].Red = 0;
            } else if (sumRed > 255) {// check for overflow
                newPixels[row][col].Red = 255;
            } else {
                newPixels[row][col].Red = sumRed;
            }
            
            if (sumGreen < 0) {// check for underflow
                newPixels[row][col].Green = 0;
            } else if (sumGreen > 255) {// check for overflow
                newPixels[row][col].Green = 255;
            } else {
                newPixels[row][col].Green = sumGreen;
            }
            
            if (sumBlue < 0) {// check for underflow
                newPixels[row][col].Blue = 0;
            } else if (sumBlue > 255) {// check for overflow
                newPixels[row][col].Blue = 255;
            } else {
                newPixels[row][col].Blue = sumBlue;
            }
        }
    }
    FILE *ptrOutFile = fopen(outputFilename, "wb");// open output file in binary mode
    if (ptrOutFile == NULL) {
        printf("Error opening output file\n");
        return; 
    }
    
    unsigned char headerBlock[54];
    fseek(ptrFile, 0, SEEK_SET); // move to beginning of file
    if (fread(headerBlock, 1, 54, ptrFile) != 54) {// read header block
        printf("Error reading header block.\n");
        fclose(ptrFile);
        fclose(ptrOutFile);
        return;
    }
    fwrite(headerBlock, 1, 54, ptrOutFile);// write header block to output file
    

    for (int row = 0; row < Info->Height; row++) {// loop through rows
        for (int col = 0; col < Info->Width; col++) {// loop through pixels
            unsigned char pixel[3];
            pixel[0] = newPixels[row][col].Blue;
            pixel[1] = newPixels[row][col].Green;// assign pixel values
            pixel[2] = newPixels[row][col].Red;
            fwrite(pixel, 3, 1, ptrOutFile);// write pixel data to output file
        }
        
        unsigned char padByte = 0;
        for (int p = 0; p < padding; p++) {// write padding data
            fwrite(&padByte, 1, 1, ptrOutFile);
        }
    }

    fclose(ptrOutFile);
    
    free(pixels[0]);
    free(pixels);

    for (int i = 0; i < Info->Height; i++) {// free new pixel array
        free(newPixels[i]);
    }

    free(newPixels);
    fclose(ptrFile);    
}

void noise(char *inputFilename){// apply noise to image
    struct HEADER Header;// header structure
    struct INFOHEADER InfoHeader;// info header structure
    FILE *ptrFile = fopen(inputFilename, "rb");// open file in binary mode
    if (ptrFile == NULL) {
        printf("Error opening file \"%s\"\n", inputFilename);
        return;
    }

    read_headers(ptrFile, &Header, &InfoHeader);// read headers from file
    char outputFilename[100];
    outputFileName(inputFilename, outputFilename, "-noise");// create output filename
    
    char input[20];
    printf("Enter standard deviation for noise (5-20): ");// prompt for standard deviation
    fgets(input, sizeof(input), stdin);

    int SD;
    if (sscanf(input, "%d", &SD) != 1) {// check for valid input
        printf("Invalid input. Please enter a number.\n");
        return;
    }

    if (SD < 5 || SD > 20) {// check for valid range
        printf("Standard deviation should be between 5 and 20.\n");
    }

    struct PIXEL **pixels = (struct PIXEL **)malloc(InfoHeader.Height * sizeof(struct PIXEL *));// allocate memory for pixel array
    if (pixels == NULL) {
        printf("Error allocating memory for pixel array.\n");
        fclose(ptrFile);
        return;
    }
    pixels[0] = (struct PIXEL *)malloc(InfoHeader.Width * InfoHeader.Height * sizeof(struct PIXEL));// allocate memory for pixel array
    if (pixels[0] == NULL) {
        printf("Error allocating memory for pixel array.\n");
        free(pixels);
        fclose(ptrFile);
        return;
    }
    for (int i = 1; i < InfoHeader.Height; i++) {// set up pixel array
        pixels[i] = pixels[0] + i * InfoHeader.Width;// allocate memory for pixel array
    }
    int padding = (4 - ((InfoHeader.Width * 3) % 4)) % 4;// calculate padding

    fseek(ptrFile, Header.Offset, SEEK_SET);// move to pixel data
    for (int row = 0; row < InfoHeader.Height; row++) {// loop through rows
        for (int col = 0; col < InfoHeader.Width; col++) {// loop through pixels
            fread(&pixels[row][col].Blue, 1, 1, ptrFile);
            fread(&pixels[row][col].Green, 1, 1, ptrFile);// read pixel data
            fread(&pixels[row][col].Red, 1, 1, ptrFile);
        }
        fseek(ptrFile, padding, SEEK_CUR);// move to next row
    }

    srand(time(NULL));// seed random number generator

    for (int row = 0; row < InfoHeader.Height; row++) {// loop through rows
        for (int col = 0; col < InfoHeader.Width; col++) {// loop through pixels
            
            double noise = GaussianRV() * SD;// generate noise
            int new_red = pixels[row][col].Red + (int)noise;// add noise to red channel
            
            if (new_red < 0){// check for underflow
                new_red = 0;
            }else if (new_red > 255) {// check for overflow
                new_red = 255;
            }else{
                pixels[row][col].Red = new_red;// assign new value
            }

            noise = GaussianRV() * SD;// generate noise
            int new_green = pixels[row][col].Green + (int)noise;// add noise to green channel

            if (new_green < 0) {// check for underflow
                new_green = 0;
            }else if (new_green > 255) {// check for overflow
                new_green = 255;
            }else{
            pixels[row][col].Green = new_green;
            }

            noise = GaussianRV() * SD;
            int new_blue = pixels[row][col].Blue + (int)noise;

            if (new_blue < 0) {
                new_blue = 0;
            }else if (new_blue > 255){
                new_blue = 255;
            }else{ 
                pixels[row][col].Blue = new_blue;
            }
        }
    }

    FILE *ptrOutFile = fopen(outputFilename, "wb");// open output file in binary mode
    if (ptrOutFile == NULL) {
        printf("Error opening output file \"%s\"\n", outputFilename);
        return;
    }
    rewind(ptrFile);  // move to beginning of file
    unsigned char headerData[54];// read header data
    fread(headerData, 1, 54, ptrFile);//read header data
    fwrite(headerData, 1, 54, ptrOutFile);// write header data to output file

    for (int row = 0; row < InfoHeader.Height; row++) {// loop through rows
        for (int col = 0; col < InfoHeader.Width; col++) {// loop through pixels
            fwrite(&pixels[row][col].Blue, 1, 1, ptrOutFile);
            fwrite(&pixels[row][col].Green, 1, 1, ptrOutFile);// write pixel data to output file
            fwrite(&pixels[row][col].Red, 1, 1, ptrOutFile);
        }

        unsigned char padByte = 0;
        for (int p = 0; p < padding; p++) {// write padding data
            fwrite(&padByte, sizeof(unsigned char), 1, ptrOutFile);
        }
    }

    fclose(ptrFile);
    fclose(ptrOutFile);
    free(pixels[0]);
    free(pixels);
}

int main(int argc, char *argv[]) {// main function
    if (argc < 3) {// check for valid number of arguments
        printf("Invalid commands, use one of the following:\n");
        printf("read .BMP-FILE .txt-File\n");
        printf("edge .BMP-FILE\n");
        printf("noise .BMP-FILE\n");
        return 1;
    }
    
    struct HEADER Header;// header structure
    struct INFOHEADER Info;
    
    if (strcmp(argv[1], "read") == 0) {// check for read command
        if (argc < 4) {
            printf("not enough arugment for read\n");
            return 1;
        }
        read_export(argv[2], argv[3], &Header, &Info);
    }
    else if (strcmp(argv[1], "edge") == 0) {// check for edge command
        edge(argv[2], &Header, &Info);
    }
    else if (strcmp(argv[1], "noise") == 0) {// check for noise command
        noise(argv[2]);
    }
    else {
        printf("Unknown command entered\n");// invalid command
        return 1;
    }
    
    return 0;
}