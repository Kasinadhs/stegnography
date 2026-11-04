#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include"common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status read_and_validate_decode_args(int argc, char*argv[],EncodeInfo *encInfo)
{   if((argc>5)||(argc<4))
    {
        printf("Input doesn't contain sufficient file \n");
        return e_failure;
    }
    int s_length=strlen(argv[2]);
    if(s_length<4)
    {
        printf("Invalid source file\n");
        return e_failure;
    }
    if((argv[2][s_length-4]!='.')||(argv[2][s_length-3]!='b')||(argv[2][s_length-2]!='m')||(argv[2][s_length-1]!='p'))
    {
        printf("Error: Extension mismatch\n");
        return e_failure;
    }
    else{
        encInfo->src_image_fname=argv[2];
    }
    encInfo ->fptr_src_image=fopen(argv[2],"r"); 
    if(encInfo->fptr_src_image==NULL){
        printf("Error: Source File Does not Exist\n");
        return e_failure;
    }
    


    // Secret file
    encInfo->secret_fname=argv[3];
    encInfo->fptr_secret=fopen(argv[3],"r");
    if(encInfo->fptr_secret==NULL){
        printf("Error: Secret File Does not Exist\n");
        return e_failure;
    }

    // Stego Image file
    if(argc>4){
    encInfo->stego_image_fname=argv[4];
    int steg_len=strlen(argv[4]);
        if(steg_len<4){
        printf("Error: Invalid Destination File Name\n");
        return e_failure;
        }
        if((argv[4][steg_len-4]!='.')||(argv[4][steg_len-3]!='b')||(argv[4][steg_len-2]!='m')||(argv[4][steg_len-1]!='p')){
             printf("Error: Extension Mismatch in the Destination File\n");
            return e_failure;
        }
    }
    else
    encInfo->stego_image_fname="default.bmp";
    
    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"wb");
        if(encInfo->fptr_stego_image==NULL){
            printf("Error: Unable to create destination stego image file\n");
        return e_failure;
        }

        printf("\n---------IMAGE STEGANOGRAPHY---------\n");
        printf("INFO: Encoding operation selected\n");
        printf("INFO: Source image : %s\n",encInfo->src_image_fname);
        printf("INFO: Secret file  : %s\n",encInfo->secret_fname);
        printf("INFO: Destination image: %s\n",encInfo->stego_image_fname);
        printf("INFO: Encoding argument validation successful\n");
        printf("\n");




    return e_success;
}
