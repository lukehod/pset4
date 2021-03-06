/**
 * recover.c
 *
 * Luke Hodnett
 * Harvard CS50x
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>

#define READ_SIZE 512


int main(int argc, char* argv[])
{
    // error check CLA input from user
    if (argc != 1)
    {
        printf("usage: ./recover.c\n");
        return 1;
    }

    // open RAW file
    FILE* file = fopen("card.raw", "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    // define vars scoped out of the do while loop
    int imgCount = 0;

    // define byte
    typedef unsigned char BYTE;

    // create img file scoped outside of loop
    FILE* jpg = NULL;

    // create array for reading 512 bytes at a time
    BYTE read[READ_SIZE];

    // store jpg header values
    BYTE header0[4] = {0xff, 0xd8, 0xff, 0xe0};
    BYTE header1[4] = {0xff, 0xd8, 0xff, 0xe1};

    // search through file for jpg headers
    do
    {
        // read 512 bytes into memory and store in read array
        if(fread(&read, sizeof(BYTE), READ_SIZE, file) == READ_SIZE)
        {
          // check for header in read array
          if (read[0] == header0[0] && read[1] == header0[1] && read[2] == header0[2] && (read[3] == header0[3] || read[3] == header1[3]))
          {
              // close previous jpg if not the first jpg found
              if(jpg != NULL)
              {
                fclose(jpg);
              }

              // create file name from imgCount in the format asked for in specification
              char imgName[8];
              sprintf(imgName, "%03d.jpg", imgCount);

              // open new file to jpg variable
              jpg = fopen(imgName, "a");

              // iterate counter for next file
              imgCount ++;
          }

          // check to see if jpg has value and if YES write the read array to it
          if (jpg != NULL)
          {
            fwrite(&read, sizeof(BYTE), READ_SIZE, jpg);
          }
        }
    }
    while(!feof(file));

    // close files
    if (jpg != NULL)
    {
      fclose(jpg);
    }

    fclose(file);

    // end program
    return 0;
}
