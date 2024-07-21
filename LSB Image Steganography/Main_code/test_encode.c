/***************************************************************************************************************************************************
 *Author           : Ananthan.A
 *File             : test_encode.h
 *Title            : LSB - steganography
 *Description      : main file 
 ****************************************************************************************************************************************************/

#include "types.h"  // Contains all user defined types
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include <string.h>

int main(int argc, char *argv[])
{
	system("clear");
	printf("\n**********************  "BOLDMAGENTA" STEGANOGRAPHY "RESET" *************************\n\n");
	// Input argument validation
	if (argc > 1 && argc < 8)
	{
		uint img_size;
		// Option validation
		OperationType op_type = check_operation_type(argv);
		// Encoding
		if (op_type == e_encode)
		{
			EncodeInfo encInfo;

			// Fill with sample filenames
			//encInfo.src_image_fname = "beautiful.bmp";
			//encInfo.secret_fname = "secret.txt";
			if(read_and_validate_encode_args(argc, argv,&encInfo) == e_success)
			{
				printf("\n*******************  "BOLDMAGENTA"Validation successful "RESET" *********************\n\n");
				// Encoding secret data
				if (do_encoding(&encInfo) == e_success)
				{
					fclose(encInfo.fptr_secret);
					fclose(encInfo.fptr_src_image);
					fclose(encInfo.fptr_stego_image);

					printf("################### "BOLDMAGENTA"Encoding Done Successfully "RESET"##################\n\n");
					//Test get_image_size_for_bmp
					//int img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
					//printf("INFO: Image size = %u\n", img_size);
				}
				else
				{
					fprintf(stderr,BOLDRED"Error"RESET" :  "BOLDRED"%s"RESET" function failed\n","do_encoding()");
					return 1;
				}
			}
		}
		// Decoding
		else if (op_type == e_decode)
		{
			DecodeInfo decInfo;

			// To read and store file names
			if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
			{
				printf("\n*******************  "BOLDMAGENTA"Validation successful "RESET" *********************\n\n");
				// Decode the stego image
				if (do_decoding(&decInfo) == e_success)
				{
					printf("################### "BOLDMAGENTA"Decoding Done Successfully"RESET" ##################\n\n");
					fclose(decInfo.fptr_src_image);
					fclose(decInfo.fptr_secret);
				}
				else
				{
					fprintf(stderr,BOLDRED"Error"RESET" :  "BOLDRED"%s "RESET"function failed\n","do_decoding()");
					return e_failure;
				}
			}
			else
			{
				fprintf(stderr,BOLDRED"Error"RESET" :  "BOLDRED"%s "RESET"function failed\n", "read_and_validate_decode_args()");
				return e_failure;
			}
		}
		else
		{
			fprintf(stderr,BOLDRED"Error"RESET" :  Invalid option\n");
			printf(BOLDGREEN"./lsb_steg"RESET" : Encoding : "BOLDGREEN"./lsb_steg"RESET" -e <"BOLDMAGENTA".bmp"RESET" file> <"BOLDGREEN".txt"RESET" file> [Output file]\n./lsb_steg: Deconding: "BOLDGREEN"./lsb_steg"RESET" -d <"BOLDMAGENTA".bmp"RESET" file> [output file]\n");
			return 1;
		}
	}
	else
	{
		fprintf(stderr, BOLDRED"Error"RESET" :  "BOLDRED"%s"RESET" function failed\n", "read_and_validation_encode_args()");
		return 1;
	}

	printf("\n*******************  "BOLDMAGENTA" COMPLETED STEGANOGRAPHY "RESET" ******************\n\n");

	return 0;
}
