#include <stdio.h>              // This header file is used for input and output functions
#include "encode.h"             // This header contains encoding related declarations
#include "types.h"              // This header contains user defined types
#include <string.h>             // This header is used for string comparison functions
#include "decode.h"             // This header contains decoding related declarations
#define RED     "\033[1;91m"    // Bright Red
#define GREEN   "\033[1;92m"    // Bright Green
#define YELLOW  "\033[1;93m"    // Bright Yellow
#define RESET   "\033[0m"       // Reset colour

int main(int argc, char *argv[])   // This is the main function where execution starts
{
    if(argc<3)   // Check if minimum required arguments are provided
    {
        printf(RED "invalid arguments "RESET"\nfor encode -> ./a.out -e beautiful.bmp secret.txt\n");   // Print encode usage
        printf("for decoding -> ./a.out -d stego.bmp\n");   // Print decode usage
        return e_failure;   // Return failure if arguments are insufficient
    }

    if(check_operation_type(argv)==e_encode)   // Check if user selected encoding option
    {
        printf(YELLOW "you chosed encoding\n"RESET);   // Print encoding message

        EncodeInfo encInfo;   // Create structure variable for encoding

        if(read_and_validate_encode_args(argv, &encInfo)==e_success)   // Validate encode arguments
        {
            printf(GREEN "Read and validate successfull\n" RESET);   // Print success message

            if(do_encoding(&encInfo)==e_success)   // Perform encoding process
            {
                printf(GREEN "Encoding successfull\n" RESET );   // Print encoding success
                return e_success;   // Return success
            }
        }
        else
        {
            printf(GREEN "Read and validate unsuccessfull\n" RESET);   // Print validation failure
        }
        return e_success;   // Return success even if encoding fails internally
    }
    else if(check_operation_type(argv)==e_decode)   // Check if user selected decoding option
    {
        printf(YELLOW "you chosing decoding\n"RESET);   // Print decoding message

        DecodeInfo decInfo;   // Create structure variable for decoding

        if(read_and_validate_decode_args(argv, &decInfo) == e_success)   // Validate decode arguments
        {
            if(do_decoding(&decInfo) == e_success)   // Perform decoding process
            {
                printf(GREEN "Decoding successful\n" RESET);   // Print decoding success
            }
            else
            {
                printf(RED "Decoding failed\n" RESET);   // Print decoding failure
            }
        }
        else
        {
            printf(RED "Invalid decode arguments\n" RESET);   // Print invalid argument message
        }

        return e_success;   // Return success after decoding flow
    }
    else   // If neither encoding nor decoding option is selected
    {
        printf(RED "invalid arguments\n" RESET);
        printf("for encode -> ./a.out -e beautiful.bmp secret.txt\n");   // Print encode usage
        printf("for decoding -> ./a.out -d stego.bmp\n");   // Print decode usage
        return e_failure;   // Return failure
    }
}

OperationType check_operation_type(char *argv[])   // This function checks whether user selected encode or decode
{
    if(strcmp(argv[1], "-e")==0)   // Compare argument with "-e"
    {
        return e_encode;   // Return encode operation type
    }
    else if(strcmp(argv[1], "-d")==0)   // Compare argument with "-d"
    {
        return e_decode;   // Return decode operation type
    }
    else
    {
        return e_unsupported;   // Return unsupported operation if input is invalid
    }
}