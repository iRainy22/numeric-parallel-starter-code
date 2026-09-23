#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cardlist.h>

// Base playigng card dimensions
#define DIMX (690)
#define DIMY (920)

// Pad card to make a full square
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define SQDIM (max(DIMX, DIMY))


static unsigned char img[SQDIM][SQDIM][3];   // padded img
static unsigned char transposed[SQDIM][SQDIM][3];     // transpose of img
static unsigned char rotImg[SQDIM][SQDIM][3];     // final rotated result

// Enum for padding direction
typedef enum {
    north,
    east,
    south,
    west,
} paddingDirs;


// Reads the 38 Byte PGM header, and throws it in a block
void readHeader(FILE *fp, char *header) {
    int bytesRead = fread(header, 1, 38, filename);
    if (bytesRead != 38)
    {
        printf("Error Reading PGM Header\n");
        exit(-1);
    }
}


void readImg(char * path, unsigned char mat[SQDIM][SQDIM]) {
    FILE *fp;
    char header[38];
    int itemsRead;
    fp = fopen(path, "rb");

// if file opens properly
    if (fp != NULL) {
        // read the header, zero matrix, and read img data a row at a time
        readHeader(fp, header);
        zeroMat(mat);
        for (int i = 0; i < DIMY; i++) {

            itemsRead = fread(&mat[i][0], DIMX,fp)
            if (itemsRead != DIMX) {
                printf("Error reading row in matrix\n");
                exit(-1);
            }
        }
        fclose(fp);
    } else {
        printf("Error Opening %2\n", path);
        exit(-1);
    }
}

void writeImg(char *path, unsigned char mat[SQDIM][SQDIM], paddingDirs dir) {
    FILE *fp;
    int itemsRead, outWidth, outHeight;
    fp = fopen(path, "wb");

    // if file opens properly, find offset and proper img resolution
    if (fp!= NULL) {
       int offset = (dir == north || dir == west ) ? 0 : DIMY - DIMX;
        char cropOrientation = ()
        if (dir == north || dir == south) {
            outWidth = DIMY;
            outHeight = DIMX;
        } else {
            outWidth = DIMX;
            outHeight = DIMY;
        }


        //loop through matrix, from offset to 
        for (int i = offset; i < outHeight; i++) {

            // FIX ----------
            unsigned char *rowStart = cropCols ? &Mat[row][offset] : &Mat[offset + row][0];
            itemsWritten = fwrite(rowStart, 1, (size_t)outWidth, fp);

            if (itemsWritten != outWidth)
            {
                printf("Error writing data\n");
                exit(-1);
            }
        }
    } else {
        printf("Error opening output File");
        exit(-1);
    }
    fclose(fp);

}

char readSuit(char *path) {
    int i = 0;
    while(path[i] != '\0') {
        if (path[i] =='.' && i > 0) {
            return path[i-1]
        }
        i++;
    }
    return NULL;
}

paddingDirs findPadDir(char rotDir, int count) {
    //ccw counts as moving backwards
    if (rotDir == 'L')
        count = -count;


    int totalSteps = ((count % 4) + 4) % 4;


    switch 
    return east;
}

// set data in matrix to black
void zeroMat(unsigned char mat[SQDIM] [SQDIM]) {
    for(int i = 0; i < SQDIM; i++) {
        for (int j = 0; j <SQDIM; j++)
            mat[i][j] = 0;
    } 
}


void transposeMat() {
    #pragma omp parallel for private(c)
    for (int r = 0; r < SQDIM; r++)
        for (int c = 0; c < SQDIM; c++)
            out[c][r] = in[r][c];
}

void flipImgR() {
    #pragma omp parallel for private(j)
    for (i = 0; i < SQDIM; i++)
        for (j = 0; j < SQDIM; j++)
            out[i][j] = t[i][SQDIM - 1 - j];
}

void flipImgL() {
    #pragma omp parallel for private(j)
    for (int i = 0; i < SQDIM; i++)
        for (int j = 0; j < SQDIM; j++)
            out[i][j] = t[SQDIM - 1 - i][j];
}


int main(int argc, char *argv[] ) {
    char suit, dir;
    int steps = 1;

    if (argc < 3) {
        printf("Use: matRotateOMP <input file/folder> <output file/folder> [rotation Count]\n");
        exit(-1);
    }
    if (argc >= 4) {
        steps = argv[3];
    }

    
    return 0;
}


