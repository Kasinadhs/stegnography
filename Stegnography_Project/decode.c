#include <stdio.h>      
#include <string.h>     
#include "decode.h"     
#define RED     "\033[1;91m"    
#define GREEN   "\033[1;92m"    
#define YELLOW  "\033[1;93m"    
#define BLUE "\033[1;94m"
#define RESET   "\033[0m"       

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)   // This function validates decode arguments
{
    if (strstr(argv[2], ".bmp") != NULL)   
    {
        decInfo->stego_image_fname = argv[2];   
    }
    else
    {
        printf(RED "Invalid stego image file\n" RESET);   
        return e_failure;   
    }

    if (argv[3] != NULL)   
    {
        strcpy(decInfo->output_fname, argv[3]);   
    }
    else
    {
        strcpy(decInfo->output_fname, "decoded");   
    }

    return e_success;   
}

Status open_decode_files(DecodeInfo *decInfo)   // This function opens the stego image file in binary mode for decoding
{
    
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");   
    if (decInfo->fptr_stego_image == NULL)   
    {
        printf(RED "Unable to open stego image\n" RESET);   
        return e_failure;   
    }
    return e_success;   
}

void remove_extn(char *fname)   
{
    char *dot = strrchr(fname, '.');   
    if (dot != NULL) *dot = '\0';   
    if (fname[0] == '\0') strcpy(fname, "decoded");   
}

Status do_decoding(DecodeInfo *decInfo)   // This function performs the complete decoding process
{
printf(YELLOW "\n**********************************************************\n" RESET);
printf(YELLOW "                 DECODING PROCESS STARTED\n" RESET);
printf(YELLOW "\n************************************************************\n" RESET);
    printf(YELLOW "Opening stego image... " RESET);
    if (open_decode_files(decInfo) == e_failure)   
    {
     printf(RED "FAILED\n" RESET);
        return e_failure;   
    }
    printf(GREEN "SUCCESS\n" RESET);
    printf(YELLOW "Preparing output file name... " RESET);
    remove_extn(decInfo->output_fname);   
    printf(GREEN "DONE\n" RESET);
printf(YELLOW "Skipping BMP header... "RESET );
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);   
    printf(GREEN "DONE\n" RESET);

    printf(BLUE "Enter magic string: " RESET);   
    scanf(" %[^\n]", decInfo->magic);   
   printf(YELLOW "Decoding magic string... "RESET);
    char decoded_magic[MAX_MAGIC_LEN];   
    for (int i = 0; i < (int)strlen(decInfo->magic); i++)   
    {
        decode_byte_from_lsb(&decoded_magic[i], decInfo->fptr_stego_image);   
    }
    decoded_magic[strlen(decInfo->magic)] = '\0';   

    if (strcmp(decoded_magic, decInfo->magic) != 0)   
    {
        printf(RED "Magic string mismatch\n" RESET);   
        fclose(decInfo->fptr_stego_image);   
        return e_failure;   
    }
    printf(GREEN "Magic string verified\n" RESET);   

    decode_int_from_lsb_int(&decInfo->extn_size, decInfo->fptr_stego_image);  
    if (decInfo->extn_size <= 0 || decInfo->extn_size > MAX_FILE_SUFFIX)  
    {
        printf(RED "Invalid extension size: %d\n" RESET, decInfo->extn_size);   
        fclose(decInfo->fptr_stego_image);  
        return e_failure;   
    }

    for (int i = 0; i < decInfo->extn_size; i++)   
        decode_byte_from_lsb(&decInfo->extn[i], decInfo->fptr_stego_image);   
    decInfo->extn[decInfo->extn_size] = '\0';  

    strcat(decInfo->output_fname, decInfo->extn);   // Append extension to output file name
    printf(BLUE "Output file: %s\n" RESET, decInfo->output_fname);   

    /* BUG FIX: open output in binary mode */
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");   
    if (decInfo->fptr_output == NULL)   
    {
        printf(RED "Output file creation failed\n" RESET);  
        fclose(decInfo->fptr_stego_image);   
        return e_failure;   
    }

    decode_int_from_lsb_long(&decInfo->file_size, decInfo->fptr_stego_image);   // Decode file size
    if (decInfo->file_size <= 0 || decInfo->file_size > 1000000)   
    {
        printf(RED "Invalid file size: %ld\n" RESET, decInfo->file_size);  
        fclose(decInfo->fptr_stego_image);  
        fclose(decInfo->fptr_output);
        return e_failure;   
    }

    char ch;   
    for (long i = 0; i < decInfo->file_size; i++)  
    {
        decode_byte_from_lsb(&ch, decInfo->fptr_stego_image);   
        fputc(ch, decInfo->fptr_output);  
    }

    printf(GREEN "Decoding completed successfully\n" RESET);   

    fclose(decInfo->fptr_stego_image);  
    fclose(decInfo->fptr_output);   

    return e_success;   
}
void decode_byte_from_lsb(char *data, FILE *fptr)   // This function decodes one character from image using LSB technique
{
    char buffer[8];   
    fread(buffer, 8, 1, fptr);   
    char ch = 0;   
    for (int i = 0; i < 8; i++)   
        ch |= (buffer[i] & 1) << (7 - i);   
    *data = ch;   
}

void decode_int_from_lsb_int(int *data, FILE *fptr)   
{
    char buffer[32];   
    fread(buffer, 32, 1, fptr);   
    int value = 0;   
    for (int i = 0; i < 32; i++)   
        value |= (buffer[i] & 1) << (31 - i);   
    *data = value;   
}

void decode_int_from_lsb_long(long *data, FILE *fptr)   // This function decodes a 32-bit long value from image using LSB technique
{
    char buffer[32];   
    fread(buffer, 32, 1, fptr);   
    long value = 0;   
    for (int i = 0; i < 32; i++)   
        value |= (long)(buffer[i] & 1) << (31 - i);
    *data = value;   
}