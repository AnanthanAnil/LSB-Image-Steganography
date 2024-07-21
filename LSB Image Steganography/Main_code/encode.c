#include "encode.h"

/* Check for operation to be performed
 * Input: Command line arguments
 * Output: Operation to be performed
 * Return: integer constant coresponding to operation
 */
OperationType check_operation_type(char *argv[])
{
        if ((strcmp(argv[1],"-e")) == 0)
        {
                return e_encode; // Return encoding operation
        }
        else if ((strcmp(argv[1], "-d")) == 0)
        {
                return e_decode; // Return decoding operation
        }
        else
        {
                return e_unsupported; // Return unsupported operation
        }
}

/* Read and validate command line argument
 * Input: Command line Argument count and Arguments wtih File name info
 * Output: File names stored in encoded Info
 * Return: e_success or e_failure
 */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
        if (argc > 2)
        {
                if (strcmp(strstr(argv[2],"."), ".bmp") == 0)
                {
                        encInfo->src_image_fname = argv[2]; // Store source image file name
                }
                else
                {
                        /* Print error message for invalid format */
                        fprintf(stderr,BOLDRED"Error"RESET" : Source image "BOLDMAGENTA"%s"RESET" format should be "BOLDMAGENTA".bmp"RESET"\n", argv[2]);
                        return e_failure;
                }
        }
        else
        {
                /* Print error message for missing arguments */
                fprintf(stderr,BOLDRED"Error"RESET" : Arguments are missing\n");
                /* Print usage instructions */
                printf(BOLDGREEN"./lsb_steg"RESET" : Deconding : "BOLDGREEN"./lsb_steg"RESET" -d <"BOLDMAGENTA".bmp"RESET" file> [output file]\n");
                printf(BOLDGREEN"./lsb_steg"RESET" : Encoding  : "BOLDGREEN"./lsb_steg"RESET" -e <"BOLDMAGENTA".bmp"RESET" file> <"BOLDGREEN".txt"RESET" file> [Output file]\n");
                printf(BOLDGREEN"./lsb_steg"RESET" : Deconding : "BOLDGREEN"./lsb_steg"RESET" -d <"BOLDMAGENTA".bmp"RESET" file> [output file]\n");
                return e_failure;
        }

        if (argc > 3)
        {
                /* Copy text file extenstion in extn_secret_file */
                strncpy(encInfo->extn_secret_file, strstr(argv[3], "."), 4);
                if ((strncmp(encInfo->extn_secret_file, ".txt", 4) == 0) || (strncmp(encInfo->extn_secret_file, ".c", 2) == 0) || (strncmp(encInfo->extn_secret_file, ".sh", 3) == 0))
                {
                        encInfo->secret_fname = argv[3];
                        strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname,"."));
                        encInfo->secret_extn_size = strlen(encInfo->extn_secret_file);

                }
                else
                {
                        /* Print error message for invalid format */
                        fprintf(stderr,BOLDRED"Error"RESET" : Secrete file "BOLDGREEN"%s"RESET" format should be "BOLDGREEN".txt"RESET" or "BOLDGREEN".c"RESET" or "BOLDGREEN".sh"RESET"\n", argv[3]);
                        return e_failure;
                }
        }
        else
        {
                fprintf(stderr,BOLDRED"Error"RESET" : Arguments are missing\n");
                printf(BOLDGREEN"./lsb_steg"RESET" : Encoding  : "BOLDGREEN"./lsb_steg"RESET" -e <"BOLDMAGENTA".bmp"RESET" file> <"BOLDGREEN".txt"RESET" file> [Output file]\n");
                printf(BOLDGREEN"./lsb_steg"RESET" : Deconding : "BOLDGREEN"./lsb_steg"RESET" -d <"BOLDMAGENTA".bmp"RESET" file> [output file]\n");
                return e_failure;
        }
        if (argc > 4)
        {
                if (strcmp(strstr(argv[4],"."), ".bmp") == 0)
                {
                        encInfo->stego_image_fname = argv[4];
                }
        }
        else
        {
                /* Use default output file name if not provided */
                encInfo ->stego_image_fname = "stego_img.bmp";
        }
        return e_success;
}

/* Encoding the secret file data to stego image
 * Input: FILE info of image, secret file and stego image
 * Output: Encodes the data in secret to stego image
 * Return: e_success or e_failure
 */
Status do_encoding(EncodeInfo *encInfo)
{
        printf("*******************  "BOLDMAGENTA"Opening required files"RESET"  ********************\n");
        /* Open required files */
        if ( open_files(encInfo) == e_success)
        {
                printf("----------------------  "BOLDMAGENTA"Checking the files"RESET"  ---------------------\n");
                printf(BOLDYELLOW"INFO"RESET" : Checking for %s size\n", encInfo->secret_fname);
                encInfo->size_secret_file = (long) get_file_size(encInfo->fptr_secret);

                if (encInfo->size_secret_file)
                {
                        printf(BOLDYELLOW"INFO"RESET" : Done. Not Empty\n");
                printf("----------------------  "BOLDMAGENTA"Image information"RESET"  ----------------------\n");
                        encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
                printf("-----------------------------------------------------------------\n");
                        printf(BOLDYELLOW"INFO"RESET" : Checking for %s"RESET" capacity to handle %s"RESET"\n", encInfo->src_image_fname, encInfo->secret_fname);
                        if (check_capacity(encInfo) == e_success)
                        {
                                printf(BOLDYELLOW"INFO"RESET" : Done. Found OK\n");
                                /* check for output .bmp */
                                if (encInfo->stego_image_fname  == NULL)
                                {
                                        printf(BOLDYELLOW"INFO"RESET" : Output File not mentioned. ");
                                        encInfo->stego_image_fname = "stego_img.bmp";
                                        printf(BOLDYELLOW"INFO"RESET" :Creating "BOLDMAGENTA"stego_img.bmp as default\n");

                                        if (open_files(encInfo) == e_failure)
                                        {
                                                fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n", "open_encode_files");
                                                return e_failure;
                                        }
                                }
                                else
                                {
                                        /* printf("INFO: Output File mentioned.\n"); */
                                        if (open_files(encInfo) == e_failure)
                                        {
                                                fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n", "open_encode_files");
                                                return e_failure;
                                        }
                                }
                                printf("------------------ "BOLDMAGENTA" Encoding Procedure Started "RESET" -----------------\n");
                                /* Copying Image Header */
                                printf(BOLDYELLOW"INFO"RESET" : Copying Image Header\n");
                                if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                {
                                        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                        /* Encode magic string in destination image */
                                        printf(BOLDYELLOW"INFO"RESET" : Encoding Magic String Signature\n");
                                        if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                                        {
                                                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                /* Encoding secret.txt file extenstion size */
                                                printf(BOLDYELLOW"INFO"RESET" : Encoding %s File Extenstion Size\n", encInfo->secret_fname);
                                                if (encode_secret_file_extn_size(encInfo->secret_extn_size, encInfo) == e_success)
                                                {
                                                        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                        /* Encode secret file extenstion */
                                                        printf(BOLDYELLOW"INFO"RESET" : Encoding %s File Extenstion\n", encInfo->secret_fname);
                                                        if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                                                        {
                                                                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                                /* Encode secret file size */
                                                                printf(BOLDYELLOW"INFO"RESET" : Encoding %s File Size\n", encInfo->secret_fname);
                                                                if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                                                                {
                                                                        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                                        /* Encode secret file data */
                                                                        printf(BOLDYELLOW"INFO"RESET" : Encoding %s File Data\n", encInfo->secret_fname);
                                                                        if (encode_secret_file_data(encInfo) == e_success)
                                                                        {
                                                                                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                                                /* Copy remaining image bytes from src to dest */
                                                                                printf(BOLDYELLOW"INFO"RESET" : Copying Left Over Data\n");
                                                                                if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                                                                {
                                                                                        printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
                                                                                }
                                                                                else
                                                                                {
                                                                                        fprintf(stderr, BOLDRED"Error"RESET" : %s function failed\n", "copy_remaining_img_data()");
                                                                                        return e_failure;
                                                                                }
                                                                        }
                                                                        else
                                                                        {
                                                                                fprintf(stderr, BOLDRED"Error"RESET" : %s function failed\n", "encode_secret_file_data()");
                                                                                return e_failure;
                                                                        }
                                                                }
                                                        }
                                                        else
                                                        {
                                                                fprintf(stderr, BOLDRED"Error"RESET" : %s function failed\n", "encode_secret_file_extn()");
                                                                return e_failure;
                                                        }
                                                }
                                                else
                                                {
                                                        fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n","encode_secret_file_extn()");
                                                        return e_failure;
                                                }
                                        }
                                        else
                                        {
                                                fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n","encode_magic_string()");
                                                return e_failure;
                                        }

                                }
                                else
                                {
                                        fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n","copy_bmp_header()");
                                        return e_failure;
                                }

                        }
                        else
                        {
                                fprintf(stderr,BOLDRED"Error"RESET" : %s function failed\n","check_capacity()");
                                return e_failure;
                        }
                }
                else
                {
                        printf(BOLDYELLOW"INFO"RESET" : Secret file is empty\n");
                        return e_failure;
                }
        }
        else
        {
                fprintf(stderr, BOLDRED"Error"RESET" : "BOLDGREEN"%s"RESET" function failed\n","open_encode_files()");
                return 1; // Return failure
        }
        return e_success;
}

/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{

        /* Src Image file */
        encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
        /* Do Error handling */
        if (encInfo->fptr_src_image == NULL)
        {
                perror("fopen");
                fprintf(stderr, BOLDRED"Error"RESET" : Unable to open file "BOLDGREEN"%s"RESET"\n", encInfo->src_image_fname);

                return e_failure;
        }
        else
        {
                printf(BOLDYELLOW"INFO"RESET" : Opened "BOLDGREEN"%s"RESET"\n", encInfo->src_image_fname);
                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        }

        /* Secret file */
        encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
        /* Do Error handling */
        if (encInfo->fptr_secret == NULL)
        {
                perror("fopen");
                fprintf(stderr, BOLDRED"Error"RESET" : Unable to open file "BOLDGREEN"%s"RESET"\n", encInfo->secret_fname);
                return e_failure;
        }
        else
        {
                printf(BOLDYELLOW"INFO"RESET" : Opened "BOLDGREEN"%s"RESET"\n", encInfo->secret_fname);
               printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        }
        /* Stego Image file */
        encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
        /* Do Error handling */
        if (encInfo->fptr_stego_image == NULL)
        {
                perror("fopen");
                fprintf(stderr, BOLDRED"Error"RESET" : Unable to open file "BOLDGREEN"%s"RESET"\n", encInfo->stego_image_fname);
                return e_failure;
        }
        else
        {
                printf(BOLDYELLOW"INFO"RESET" : Opened %s\n", encInfo->stego_image_fname);
                printf(BOLDYELLOW"INFO"RESET" : "BOLDGREEN"Done"RESET"\n");
        }
        /* No failure return e_success */
        return e_success;
}

/* To check the capacity of source image to handle secret data
 * Input: File info source image, stego image and secret file
 * Output: Get Source image capacity and store in image_capacity
 * Return: e_success or e_failure
 */

Status check_capacity(EncodeInfo *encInfo)
{

        encInfo -> image_capacity = get_image_size_for_bmp (encInfo -> fptr_src_image);
        /* Copying the content from secret_file and storeing it into extn_secret_file */
        strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname,"."));
        /* The secret file extn size */
        encInfo->secret_extn_size = strlen(strstr(encInfo->secret_fname,"."));
        /* Find secret file size */
        encInfo->size_secret_file = get_file_size(encInfo -> fptr_secret);
        int extn_size = strlen(encInfo->extn_secret_file);
        if (encInfo->image_capacity > 54 + (strlen(MAGIC_STRING) * 8) + 32 +  (extn_size * 8) + 32 + (encInfo->size_secret_file * 8))
        {
                return e_success;
        }
        else
        {
                return e_failure;
        }
}


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
        uint width, height;
    static int flag = 1;
        /* Seek to 18th byte */
        fseek(fptr_image, 18, SEEK_SET);
        /* Read the width (an int) */
        fread(&width, sizeof(int), 1, fptr_image);
        /* Read the height (an int) */
        fread(&height, sizeof(int), 1, fptr_image);
        if(flag)
        {
                flag = 0;
                printf(BOLDYELLOW"INFO"RESET" : width          = "BOLDBLUE"%u"RESET"\n", width);
                printf(BOLDYELLOW"INFO"RESET" : height         = "BOLDBLUE"%u"RESET"\n", height);
                printf(BOLDYELLOW"INFO"RESET" : Size of image  = "BOLDBLUE"%u"RESET"\n", height * width * 3);
        }
        /* Return image capacity */
        return width * height * 3;
}

/* Get the file size
 * Input:  File pointer
 * Return: Size of file in bytes
 */
uint get_file_size(FILE *fptr)
{
        fseek(fptr, 0L, SEEK_END);
        uint size = (uint) ftell(fptr);
        rewind(fptr);
        return size;
}

/* Function Definitions to copy the 54 bytes (header) to the new created image file
 * Copy the header bytes data to stego image
 * Input: FILE pointers source and stego image
 * Output: Copies header data of source image to stego image
 * Return: e_success or e_failure
 */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
        /* Create char arr 54 */
        char arr[54];

        /* Rewind -> fptr_src_image */
        rewind(fptr_src_image);

        /* Read 54 bytes */
        fread(arr, 54, 1, fptr_src_image);

        /* Write 54 byte */
        fwrite(arr, 54, 1, fptr_stego_image);
        return e_success;
}

/* Function Definitions to write the magic_string to created imagre
 * Encode Magic string in stego image
 * Input: magic string and FILEs info
 * Output: Encode magic character '*' in stego image first 8 bytes from image data
 * Return: e_success or e_failure
 */
Status encode_magic_string(const char * magic_string, EncodeInfo * encInfo)
{
        /* Create char arr 8 */
        char str[8];
        int i = 0;
        for (int i = 0 ; magic_string[i] != '\0'; i++)
        {
                /* Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at 7th index */
                fread(str, 8, 1, encInfo->fptr_src_image);
                /* function to set the bytes to the lsb of the image */
                encode_byte_to_lsb(magic_string[i],str);
                /* Write 8 byte to the created image */
                fwrite(str, 8, 1, encInfo->fptr_stego_image);

        }
        return e_success;
}

/* Encode the secret file extenstion size to stego image
 * Input: FILEs info
 * Output: Encode file extenstion size to stego image
 * Return: e_success or e_failure
 */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
        char str[8];
        int i=0;
        /*  Write extension size */
        while(file_extn[i] != 0)
        {
                fread(str,8,1,encInfo->fptr_src_image);

                for(int j=0;j<8;j++)
                {

                        str[j] =(((file_extn[i] &(1 << (7-j))) >> (7-j)) | (str[j] & (~1)));
                }
                fwrite(str,8,1,encInfo -> fptr_stego_image);
                i++;
        }
        return e_success;
}

/* Function Definitions to write the secret file extension size
 * Encode secret file extension size in stego image
 * Input: extension size and FILEs info
 * Output: Encode size of secret file extension size in stego image next 32 bytes from image data
 * Return: e_success or e_failure
 */
Status encode_secret_file_extn_size(int file_extn, EncodeInfo *encInfo)
{
        char str[32];
        /* Read 32 bytes from */
        fread(str, 32, 1, encInfo -> fptr_src_image);
        /* function to set the bytes to the lsb of the image */
        encode_int_to_lsb(file_extn,str);
        /* Write 32 byte to the created image */
        fwrite(str, 32, 1, encInfo -> fptr_stego_image);
        return e_success;
}

/* Function Definitions to write the secret file size
 * Encode secret file size in stego image
 * Input: extension and FILEs info
 * Output: Encode secret file size in stego image next 32 bytes from image data
 * Return: e_success or e_failure
 */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
        char str[32];
        int index=0;
        fread(str,1,32,encInfo->fptr_src_image);
        /* Integer to lsb */
        for(int i=31; i>=0 ; i--)
        {
                str[index] = (str[index] & ~1) | ((file_size & (1<<i)) >> i); // modify
                index++;
        }

        fwrite(str,1,32,encInfo->fptr_stego_image);
        if (ferror(encInfo->fptr_stego_image))
        {
                fprintf(stderr,BOLDRED"Error"RESET" : While writing the data to destination image file\n");
                clearerr(encInfo->fptr_stego_image);
                return e_failure;
        }
        return e_success;
}

/* Function Definitions to write the secret file data
 * Encode secret file data in stego image
 * Input: FILEs info
 * Output: Encode secret file data in stego image
 * Return: e_success or e_failure
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{

        rewind(encInfo->fptr_secret);
        char arr[encInfo->size_secret_file];
        char str[8];
        /* Encoding the text in secret file to dest image */
        fread(arr,encInfo ->size_secret_file,1,encInfo->fptr_secret);

        for(int j=0;j<encInfo->size_secret_file;j++)
        {
                fread(str,8,1,encInfo->fptr_src_image);
                encode_byte_to_lsb(arr[j],str);
                fwrite(str,8,1,encInfo->fptr_stego_image);
        }
        return e_success;

}

/* Function Definitions to copy remaining image data
 * Copy remaining image data from source to stego image
 * Input: Source and stego image FILEs info
 * Output: Copy remaining image data from source to stego image
 * Return: e_success or e_failure
 */
Status copy_remaining_img_data(FILE *fptr_src_img, FILE *fptr_stego_img)
{
        char ch;
        while (fread(&ch, 1, 1, fptr_src_img) > 0)
        {
                fwrite(&ch, 1, 1, fptr_stego_img);
        }
        return e_success;

}

Status encode_int_to_lsb(int size, char *image_buffer)
{
        /* Encoding logic goes here */
        for (int i = 0; i < 32; i++)
        {
                image_buffer[i] =(((size & (1 << (31-i))) >> (31-i)) | (image_buffer[i] & (~1)));
        }
        return e_success;
}


Status encode_byte_to_lsb(char data, char *image_buffer)
{
        /* Encoding logic goes here */
        for (int i = 0; i < 8; i++)
        {
                image_buffer[i] =(((data & (1 << (7-i))) >> (7-i)) | (image_buffer[i] & (~1)));
        }
        return e_success;
}
