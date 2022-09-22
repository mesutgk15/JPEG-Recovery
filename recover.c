#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
BYTE buffer[512];
int file_counter = 0;
char *filename;
FILE *jpeg;


int main(int argc, char *argv[])
{
    //there should be only one input file as a command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //open the memory card inputted
    FILE *img = fopen(argv[1], "r");
    if (img == NULL)
    {
        printf("Could not open the file\n");
        return 1;
    }



    //read very 512 bytes into buffer
    while ((fread(buffer, sizeof(BYTE), 512, img)) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            //check if this is the first image recovered
            if (file_counter == 0)
            {
                filename = malloc(sizeof(BYTE) * 9);
                sprintf(filename, "%03i.jpg", file_counter);
                jpeg = fopen(filename, "w");
                if (jpeg == NULL)
                {
                    fclose(jpeg);
                    printf("Could not create image file");
                    return 1;
                }
                fwrite(buffer, sizeof(BYTE), 512, jpeg);
                file_counter++;

            }
            //if this is not the first image recovered do the previous step but close the existing file first
            else
            {
                fclose(jpeg);

                sprintf(filename, "%03i.jpg", file_counter);
                jpeg = fopen(filename, "w");
                if (jpeg == NULL)
                {
                    fclose(jpeg);
                    printf("Could not create image file");
                    return 1;
                }
                fwrite(buffer, sizeof(BYTE), 512, jpeg);
                file_counter++;


            }
        }
        //if there is a already opened image file keep and not start of a new image, keep writing to existing one
        else if (file_counter != 0)
        {
            fwrite(buffer, sizeof(BYTE), 512, jpeg);
        }


    }
    free(filename);
    fclose(jpeg);
    fclose(img);


}