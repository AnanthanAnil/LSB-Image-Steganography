#ifndef DECODE_H

#define DECODE_H

#include<stdio.h>
#include"color.h"
#include"common.h"
#include<string.h>

#include "types.h"  // contains user defined types.

#define MAX_FILE_SUFFIX 4

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
        char *src_image_fname;
        FILE *fptr_src_image;

        char secret_fname[30];
        FILE *fptr_secret;
        int extn_size;
        char extn_secret_file[MAX_FILE_SUFFIX + 1];
        int size_secret_file;

        char deco_magic_str[5];

} DecodeInfo;


//Decoding Function prototypes:

// Function to read and validate decode arguments
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decoInfo);

// Function to perform decoding
Status do_decoding(DecodeInfo *decoInfo);

// Function to open the input file
Status open_input_file(DecodeInfo *decoInfo);

// Function to decode the magic string from the source image
Status decode_magic_string(DecodeInfo *decoInfo);

// Function to decode the size of the extension
Status decode_output_extn_size(DecodeInfo *decoInfo);

// Function to decode byte array to integer size
int decode_byte_to_size(char *image_buffer);

// Function to decode the extension from the source image
Status decode_output_extn(DecodeInfo *decoInfo);

// Function to merge extension with file name
Status merge_extn_to_filename(char *file_name, char *extn);

// Function to open the output file
Status open_output_file(DecodeInfo *decoInfo);

// Function to decode the size of the output file
Status decode_output_file_size(DecodeInfo *decoInfo);

// Function to decode the output file data
Status decode_output_file_data(DecodeInfo *decoInfo);

// Function to decode least significant bits to bytes
Status decode_lsb_to_bytes(char *image_buffer, char *data, int index);

// Function to check if magic strings match
Status check_magic_string_match(char *deco_magic_str, char *user_string);

#endif
