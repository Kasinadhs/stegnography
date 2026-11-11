#include <stdio.h>              
#include "encode.h"             
#include "types.h"              
#include <string.h>             
#include "decode.h"             
#define RED     "\033[1;91m"    
#define GREEN   "\033[1;92m"    
#define YELLOW  "\033[1;93m"    
#define RESET   "\033[0m"       

int main(int argc, char *argv[])   
{
    if(argc<3)   
    {
        printf(RED "invalid arguments "RESET"\nfor encode -> ./a.out -e beautiful.bmp secret.txt\n");   
        printf("for decoding -> ./a.out -d stego.bmp\n");   
        return e_failure;   
    }

    if(check_operation_type(argv)==e_encode)   
    {
        printf(YELLOW "you chosed encoding\n"RESET);   

        EncodeInfo encInfo;   

        if(read_and_validate_encode_args(argv, &encInfo)==e_success)   
        {
            printf(GREEN "Read and validate successfull\n" RESET);   

            if(do_encoding(&encInfo)==e_success)   
            {
                printf(GREEN "Encoding successfull\n" RESET );   
                return e_success;   
            }
        }
        else
        {
            printf(GREEN "Read and validate unsuccessfull\n" RESET);   
        }
        return e_success;   
    }
    else if(check_operation_type(argv)==e_decode)   
    {
        printf(YELLOW "you chosing decoding\n"RESET);   

        DecodeInfo decInfo;   

        if(read_and_validate_decode_args(argv, &decInfo) == e_success)   
        {
            if(do_decoding(&decInfo) == e_success)   
            {
                printf(GREEN "Decoding successful\n" RESET);  
            }
            else
            {
                printf(RED "Decoding failed\n" RESET);  
            }
        }
        else
        {
            printf(RED "Invalid decode arguments\n" RESET);   
        }

        return e_success;   
    }
    else   
    {
        printf(RED "invalid arguments\n" RESET);
        printf("for encode -> ./a.out -e beautiful.bmp secret.txt\n");   
        printf("for decoding -> ./a.out -d stego.bmp\n");   
        return e_failure;   
    }
}

OperationType check_operation_type(char *argv[])   
{
    if(strcmp(argv[1], "-e")==0)   
    {
        return e_encode;   
    }
    else if(strcmp(argv[1], "-d")==0)  
    {
        return e_decode;   
    }
    else
    {
        return e_unsupported;   
    }
}