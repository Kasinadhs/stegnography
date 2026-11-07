#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

    
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if (argc < 2 || argv[1][0] != '-' || argv[1][2] != '\0')
    {
        printf("Incorrect command line format\n");
        return 1;
    }

    OperationType op = check_operation_type(argv[1][1]);

    if (op == e_encode)
    {
        if (read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
           

            if (do_encoding(&encInfo) == e_failure){
            
                printf("Encoding failed\n");
                return 1;
            }

            
        }
        else{
        
            printf("Encoding argument validation failed\n");
            return 1;
        }
    }
    else if(op == e_decode){
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            

            if (do_decoding(&decInfo) == e_failure)
            {
                printf("Decoding failed\n");
                return 1;
            }

            
        }
        else
        {
            printf("Decoding argument validation failed\n");
            return 1;
        }
    }
    
    else{
    
        printf("Unsupported operation\n");
        return 1;
    }

    return 0;

}
OperationType check_operation_type(char opt){
    if(opt=='e')
    return e_encode;
    if(opt=='d')
    return e_decode;
    else
    return e_unsupported;
}