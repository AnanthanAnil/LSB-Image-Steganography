#include"decode.h"

//Fucntion definitions

/* Get File names and validate
 * Input: Stego Image file name and Output file name
 * Output: Stores the above files names
 * Return Value: e-success or e_failure, on file errors
 */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decoInfo)
{
        if (argc > 2)
        {
                if (strstr(argv[2], ".bmp") != NULL)
                {
                        decoInfo ->src_image_fname = argv[2];
                }
                else
                {
                        fprintf(stderr,BOLDRED"Error"RESET" : Stego image "BOLDMAGENTA"%s"RESET" format should be "BOLDMAGENTA".bmp"RESET"\n", argv[2]);
                        return e_failure;
                }
        }
        else
        {
                fprintf(stderr,BOLDRED"Error"RESET" : Arguments are missing\n");
                printf(BOLDGREEN"./Test_encode"RESET" : Encoding : "BOLDGREEN"./Test_encode"RESET" -e <"BOLDMAGENTA".bmp"RESET" file> <"BOLDGREEN".txt"RESET" file> [Output file]\n");
                printf(BOLDGREEN"./Test_encode"RESET" : "BOLDGREEN"./Test_encode"RESET" -d <"BOLDMAGENTA".bmp"RESET" file> [output file]\n");
                return e_failure;
        }
        if (argv[3] != NULL)
        {
                strcpy(decoInfo -> secret_fname, argv[3]);
        }
        else
        {
                strcpy(decoInfo -> secret_fname, "secret_output");
        }
        //printf("The output file name:%s\n", decoInfo ->secret_fname);
        return e_success;
}


/* Decoding stego image to another file
 * Input: File info of stego image and output file
 * Output: Decoded message copied in output file
 * Return: e_success or e_failure, on file error
 */
Status do_decoding(DecodeInfo *decoInfo)
{
        printf("------------------  "BOLDMAGENTA"Decoding Procedure Started"RESET"  -----------------\n");
        printf(BOLDYELLOW"INFO"RESET" : Opening required files\n");
        if(open_input_file(decoInfo) == e_failure)
        {
                printf(BOLDRED"Error"RESET" : "BOLDGREEN"%s"RESET" Fuction Failed\n", "open_input_files");
                return e_failure;
        }
        else
        {
                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"%s"RESET" Function completed\n", "open_input_files");
                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"%s"RESET" file is open\n", decoInfo -> src_image_fname );

                char magic_string[5];
                printf("-----------------  "BOLDMAGENTA"Decoding Magic String Signature"RESET"  -------------\n");
                printf(BOLDYELLOW"INFO"RESET" : Passcode :");
                scanf("%s", magic_string);
                printf("-----------------------------------------------------------------\n");
                printf(BOLDYELLOW"INFO"RESET" : Entered Passcode "BOLDBLUE"%s"RESET"\n", magic_string);
                printf(BOLDYELLOW"INFO"RESET" : Decoding Magic String Signature\n");
                decode_magic_string(decoInfo);
                if(check_magic_string_match(decoInfo -> deco_magic_str, magic_string) == e_failure)
                {
                        printf("The passwords(Magic strings) are not matching!!\nEnter correct password.\n");
                        return e_failure;
                }
                else
                {
                        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done\n");
                        printf(BOLDYELLOW"INFO"RESET" : Passcode matched\n");
                        decode_output_extn_size(decoInfo);
                        printf(BOLDYELLOW"INFO"RESET" : extn size: "BOLDBLUE"%d\n", decoInfo -> extn_size );
                        printf(BOLDYELLOW"INFO"RESET" : Decoding Output File Extension size\n");
                        decode_output_extn(decoInfo);
                        merge_extn_to_filename(decoInfo -> secret_fname, decoInfo -> extn_secret_file);
                        printf(BOLDYELLOW"INFO"RESET" : Extension : "BOLDBLUE"%s\n",decoInfo ->secret_fname);
                        if (open_output_file(decoInfo) == e_failure)
                        {
                                return e_failure;
                        }
                        else
                        {
                                decode_output_file_size(decoInfo);
                                printf(BOLDYELLOW"INFO"RESET" : Output file size : "BOLDBLUE"%d\n", decoInfo -> size_secret_file);
                                decode_output_file_data(decoInfo);

                        }
                }
                return e_success;
        }
}

/* Get File pointers for i/p and o/p files
 * Inputs: Stego Image file, Output file
 * Ouput: File pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_input_file(DecodeInfo *decoInfo)
{
        decoInfo -> fptr_src_image = fopen(decoInfo -> src_image_fname, "r");
        if (decoInfo -> fptr_src_image == NULL)
        {
                perror(BOLDYELLOW"INFO"RESET" : open_decode_files");
                fprintf(stderr, BOLDRED"Error"RESET": Unable to open file "BOLDGREEN"%s\n", decoInfo ->src_image_fname);
                return e_failure;
        }
        return e_success;
}

/* Decodes the magic string from stego image
 * Input:       File info of input and output
 * Output:      Proceed decoing if magic string if found or else stop decoding
 * Return:      e_success or e_failure
 */
Status decode_magic_string(DecodeInfo *decoInfo)
{
        char str[8];
        fseek(decoInfo -> fptr_src_image, 54, SEEK_SET);
        for(int i = 0; i < 5; i++)
        {
                fread(str, 8, 1, decoInfo -> fptr_src_image);
                decode_lsb_to_bytes(str, decoInfo -> deco_magic_str, i);
        }
        printf(BOLDYELLOW"INFO"RESET" : Decoding Passcode \n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        printf(BOLDYELLOW"INFO"RESET" : The decoded magic string is : "BOLDBLUE"%s\n", decoInfo -> deco_magic_str);
        return e_success;
}

/* Get lsb bit from stego image byte
 * Input: Image_data array and decode_data character
 * Output: Decode the image_data and stores the 1 byte data in decode_data
 * Return: e_success or e_failure
 */
Status decode_lsb_to_bytes(char *image_buffer, char *data, int index)
{
        char ch = 0;
        int j = 0;
        for(int i = 7; i >= 0; i--)
        {
                ch =(ch & (~(1 << i))) | ((image_buffer[j] & 1) << i);
                j++;
        }
        data[index] = ch;
        return e_success;
}

/* Comparing the magic string
 * Input: Two magic_string datas
 * Output: Comparing
 * Return: e_success or e_failure
 */
Status check_magic_string_match(char *deco_magic_str, char *user_string)
{
        if (strcmp(deco_magic_str, user_string) == 0)
        {
                return e_success;
        }
        return e_failure;
}

/* Decode file extenstion size from stego image
 * Input: File info of stego image and output file
 * Output: Decode the extenstion size from stego image and store in image_data_size
 * Return: e_success or e_failure
 */
Status decode_output_extn_size(DecodeInfo *decoInfo)
{
        char str[32];
        fread(str, 32, 1, decoInfo -> fptr_src_image);
        decoInfo -> extn_size = decode_byte_to_size(str);
        printf(BOLDYELLOW"INFO"RESET" : Decoding extension size\n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        return e_success;
}

int  decode_byte_to_size( char *image_buffer)
{
        int j = 0;
        int num;
        for(int i = 31; i >= 0; i--)
        {
                num = (num & (~(1 << i))) | ((image_buffer[j] & 1) << i);
                j++;

        }
        return num;

}

/* Decode file extenstion from stego image
 * Input: File info of stego image and output file
 * Output: Decode the extenstion from stego image and store
 * Return: e_success or e_failure
 */
Status decode_output_extn(DecodeInfo *decoInfo)
{
        char str[8];
        int i;
        for( i = 0; i < (decoInfo -> extn_size); i++)
        {
                fread(str, 8, 1, decoInfo -> fptr_src_image);
                decode_lsb_to_bytes(str, decoInfo -> extn_secret_file, i);
        }
        decoInfo->extn_secret_file[i] = '\0';

        printf(BOLDYELLOW"INFO"RESET" : Decoding output extension\n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        printf(BOLDYELLOW"INFO"RESET" : Extension -> "BOLDBLUE"%s\n", decoInfo -> extn_secret_file);
        return e_success;
}

/* Merging extension
 * Input: Two character array
 * Output: Giving the output file name
 * Return: e_success or e_failure
 */
Status merge_extn_to_filename(char *file_name, char *extn)
{
        strcat(file_name, extn);
        printf(BOLDYELLOW"INFO"RESET" : Merging \n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        return e_success;
}

/* Opening output file
 * Input: File info of stego image and output file
 * Output: Storing the file in fptr_secret
 * Return: e_success or e_failure
 */
Status open_output_file(DecodeInfo *decoInfo)
{
        decoInfo -> fptr_secret = fopen(decoInfo -> secret_fname, "w");
        if (decoInfo -> fptr_secret == NULL)
        {
                perror(BOLDYELLOW"INFO"RESET" : open_output_file");
                fprintf(stderr, BOLDRED"Error"RESET": Unable to open file "BOLDGREEN"%s\n", decoInfo -> secret_fname);
                return e_failure;
        }
        printf(BOLDYELLOW"INFO"RESET" : File "BOLDGREEN"\"%s\""RESET" is open for writing\n", decoInfo -> secret_fname);
        return e_success;

}

/* Decode file size from stego image
 * Input: File info of stego image and output file
 * Output: Decode the extenstion size from stego image
 * Return: e_success or e_failure
 */
Status decode_output_file_size(DecodeInfo *decoInfo)
{
        char str[32];
        fread(str, 32, 1, decoInfo -> fptr_src_image);
        decoInfo -> size_secret_file  = decode_byte_to_size(str);
        printf(BOLDYELLOW"INFO"RESET" : Decoding output file size\n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        return e_success;
}

/* Decode file data from stego image
 * Input: FILE info of stego image and output decode file
 * Output: Write decode data in the output file
 * Return: e_success or e_failure
 */
Status decode_output_file_data(DecodeInfo *decoInfo)
{
        char str[8];
        char str2[decoInfo -> size_secret_file];
        int i;
        for( i = 0; i < decoInfo -> size_secret_file; i++)
        {
                fread(str, 8, 1, decoInfo -> fptr_src_image);
                decode_lsb_to_bytes(str, str2, i);
        }
        fwrite(str2, decoInfo -> size_secret_file, 1,decoInfo ->fptr_secret);
        str2[i] = '\0';
        printf(BOLDYELLOW"INFO"RESET" : Decoding secret data\n");
        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        printf(BOLDYELLOW"INFO"RESET" : Secret data -> %s\n", str2);
        return e_success;
}
