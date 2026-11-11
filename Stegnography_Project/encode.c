#include<stdio.h>              
#include<string.h>             
#include "encode.h"            
#include "types.h"             
#define RED     "\033[1;91m"    
#define GREEN   "\033[1;92m"    
#define YELLOW  "\033[1;93m"    
#define BLUE "\033[1;94m"
#define RESET   "\033[0m"       

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) // This function reads and validates command line arguments
{
    // Check required arguments
    if (argv[2] == NULL || argv[3] == NULL)
    {
        printf(RED "Error: Missing arguments\n" RESET); 
        printf("Usage: ./a.out -e <image.bmp> <secret.txt> [output.bmp]\n"); 
        return e_failure; 
    }

    // Validate BMP
    char *ext1 = strrchr(argv[2], '.'); // Extract file extension from input image name
    if (ext1 == NULL || strcmp(ext1, ".bmp") != 0) 
    {
        printf(RED "Error: Input image must be .bmp\n" RESET); 
        return e_failure; 
    }
    encInfo->src_image_fname = argv[2]; 

    // Validate TXT
    char *ext2 = strrchr(argv[3], '.'); 
    if (ext2 == NULL || strcmp(ext2, ".txt") != 0) 
    {
        printf(RED "Error: Secret file must be .txt\n" RESET); 
        return e_failure; 
    }
    encInfo->secret_fname = argv[3]; 

    // Optional output file
    if (argv[4] != NULL) 
    {
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp"; 
    }

    return e_success; 
}
Status do_encoding(EncodeInfo *encInfo){   // This function performs the complete encoding process step by step
printf(YELLOW "\n**********************************************************\n" RESET);
printf(YELLOW "                ENCODING PROCESS STARTED\n" RESET);
printf(YELLOW "\n**********************************************************\n" RESET);
    if( open_files(encInfo) == e_success){   
        printf(GREEN "All files opened successfully\n" RESET);
    }
    else{
        printf(RED "All files did not opened successfully\n" RESET);
        return e_failure;   
    }

    printf(BLUE "Enter magic string: " RESET);   
    scanf(" %[^\n]",encInfo -> magic);   

    if(check_capacity(encInfo)== e_success){   
        printf(GREEN "Capacity checked Successfully\n" RESET);
    }
    else{
        printf(RED "Capacity of the source file is less\n" RESET);
        fclose(encInfo->fptr_src_image);   
        fclose(encInfo->fptr_secret);      
        fclose(encInfo->fptr_stego_image); 
        return e_failure;   
    }

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success){   // Copy first 54 bytes (header) from source to output image
        printf(GREEN "Header copied successfully\n" RESET);
    }
    else{
        printf(RED "Header not copied successfully\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }
     
    if(encode_magic_string(encInfo->magic,encInfo) == e_success){   
        printf(GREEN "Magic string encoded successfully\n" RESET);
    }
    else{
        printf(RED "Magic string did not encode successfully\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    if(encode_secret_file_extn_size(strlen(strchr(encInfo->secret_fname,'.')),encInfo) == e_success){   
        printf(GREEN "Size of the secret extn file encoded successfully\n" RESET);
    }
    else{
        printf(RED "Size of the secret extn file not encoded\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    if(encode_secret_file_extn( strchr(encInfo->secret_fname,'.'), encInfo) == e_success){   
        printf(GREEN "Secret extn encoded successfully\n" RESET);
    }
    else{
        printf(RED "Secret extn encoded unsuccessfully\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success){   
        printf(GREEN "Secret file size encoded success\n" RESET);
    }
    else{
        printf(RED "Secret file size not encoded\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    if(encode_secret_file_data(encInfo) == e_success){   
        printf(GREEN "Secret file data encoded successfully\n" RESET);
    }
    else{
        printf(RED "Secret file data not encoded\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success){ 
        printf(GREEN "Remaining data copied successfully\n" RESET);
    }
    else{
        printf(RED "Remaining data does not copied successfully\n" RESET);
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;   
    }

    
    fclose(encInfo->fptr_src_image);   
    fclose(encInfo->fptr_secret);      
    fclose(encInfo->fptr_stego_image); 

    return e_success;   
}

Status open_files(EncodeInfo *encInfo)   // This function opens source, secret and output files in binary mode
{
    
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");   
    if(encInfo->fptr_src_image == NULL)  
    {
        printf(RED"Source file is not opened\n"RESET);
        return e_failure;   
    }
    else printf(GREEN "Source file is opened\n" RESET);   

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");   
    if(encInfo->fptr_secret == NULL)   
    {
        printf(RED"Secret file is not opened\n"RESET);
        fclose(encInfo->fptr_src_image);   
        return e_failure;  
    }
    else printf(GREEN "Secret file is opened\n" RESET);   

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");   
    if(encInfo->fptr_stego_image == NULL)   
    {
        printf(RED"Stego file is not opened\n" RESET);
        fclose(encInfo->fptr_src_image);   
        fclose(encInfo->fptr_secret);      
        return e_failure;   
    }
    else printf(GREEN "Stego file is opened\n" RESET); 

    return e_success;   
}

Status check_capacity(EncodeInfo *encInfo){   // This function checks whether the image has enough capacity to store secret data
   encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);   
   encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);   

   if(encInfo->image_capacity > (strlen(encInfo->magic)*8 + 32 + 32 + 32 + encInfo->size_secret_file*8)){   
        return e_success;   
   }
   return e_failure;   
}

uint get_image_size_for_bmp(FILE *fptr_image){   
    int wid, len;   
    fseek(fptr_image, 18, SEEK_SET);   
    fread(&wid, 4, 1, fptr_image);   
    fread(&len, 4, 1, fptr_image);   
    return wid * len * 3;   
}

uint get_file_size(FILE *fptr){   
    fseek(fptr, 0, SEEK_END);   
    return ftell(fptr);   
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){   // This function copies the first 54 bytes (header) from source BMP to destination BMP
    rewind(fptr_src_image);   
    char buffer[54];   
    fread(buffer, 54, 1, fptr_src_image);   
    fwrite(buffer, 54, 1, fptr_dest_image); 
    return e_success;   
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo){   // This function encodes the magic string into the image
    if(encode_data_to_image(magic_string, strlen(magic_string), encInfo) == e_success){   
        printf(GREEN "Magic string encoded\n" RESET);
        return e_success;   
    }
    printf(RED "Magic string not encoded\n" RESET);  
    return e_failure;   
}

Status encode_data_to_image(char *data, int size, EncodeInfo *encoInfo){   // This function encodes each byte of data into image buffer
    char buffer[8];   
    for(int i = 0; i < size; i++){   
        fread(buffer, 8, 1, encoInfo->fptr_src_image);   
        encode_byte_to_lsb(data[i], buffer);   
        fwrite(buffer, 8, 1, encoInfo->fptr_stego_image);   
    }
    return e_success;   
}

Status encode_byte_to_lsb(char data, char *image_buffer)   // This function encodes one byte of data into 8 bytes using least significant bits
{
    for(int i = 0; i < 8; i++)   
    {
        int bit = (data >> (7 - i)) & 1;   
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit;   
    }
    return e_success;   
}

Status encode_secret_file_extn_size(int data, EncodeInfo *encInfo){   // This function encodes the size of secret file extension into image
    return encode_int_lsb(data, encInfo);   
}
Status encode_int_lsb(int data, EncodeInfo *encInfo)   // This function encodes an integer (32 bits) into image using LSB technique
{
    char buffer[32];   
    fread(buffer, 32, 1, encInfo->fptr_src_image);   
    for(int i = 0; i < 32; i++)   
    {
        int bit = (data >> (31 - i)) & 1;   
        buffer[i] = (buffer[i] & 0xFE) | bit;   
    }
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);   
    return e_success;   
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo){   // This function encodes the file extension (like .txt) into image
    return encode_data_to_image(file_extn, strlen(file_extn), encInfo);   
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){   // This function encodes the size of secret file
    return encode_int_lsb((int)file_size, encInfo);   
}

Status encode_secret_file_data(EncodeInfo *encInfo){   // This function encodes the actual secret file content into image
    
    rewind(encInfo->fptr_secret);   
    char buffer[encInfo->size_secret_file];   
    fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);   
    return encode_data_to_image(buffer, encInfo->size_secret_file, encInfo);   
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){   // This function copies remaining image data after encoding
    char ch;   
    while(fread(&ch, 1, 1, fptr_src)){   
        fwrite(&ch, 1, 1, fptr_dest);  
    }
    return e_success;   
}