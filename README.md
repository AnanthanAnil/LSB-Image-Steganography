# LSB image steganography

 [![Mail](https://img.shields.io/badge/ananthankulathinkara-Gmail-red)](ananthankulathinkara@gmail.com) [![Linkedin](https://img.shields.io/badge/AnanthanAnil-Linkedin-royalblue)](https://www.linkedin.com/in/ananthananil) 
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#About-the-project">About The Project</a>
      <ul>
        <li><a href="#Built-with">Built With</a></li>
        <li><a href="#Prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li>
      <a href="#Getting-started">Getting Started</a>
      <ul>
        <li><a href="#Installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#Roadmap">Roadmap</a></li>
    <li><a href="#Output">Output</a></li>
    <li><a href="#References">References</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<img align="left" height="150" src="https://www.emertxe.com/wp-content/uploads/2023/06/c-project-image-steganography-1.jpg"  />

 Steganography is the art of hiding the fact that communication is taking place, by hiding
 information in other information. Many different carrier file formats can be used, but digital
 images are the most popular because of their frequency on the internet. For hiding secret
 information in images, there exists a large variety of steganography techniques some are
 more complex than others and all of them have respective strong and weak points. Different
 applications may require absolute invisibility of the secret information, while others require a
 large secret message to be hidden. 
 
Steganography is the practice of hiding private or sensitive information within something that
 appears to be nothing out to the usual. Steganography is often confused with cryptography,
 because the two are similar in the way that they both are used to protect important
 information. The difference between two is that steganography involves hiding information so
 it appears that no information is hidden at all. If a person or persons views the object that the
 information is hidden inside of he or she will have no idea that there is any hidden information,
 therefore the person will not attempt to decrypt the information.
 
 What steganography essentially does is exploit human perception, human senses are not
 trained to look for files that have information inside of them, although this software is available
 that can do what is called Steganography. The most common use of stenography is to hide a
 file inside another file

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With
Language used and compiler [GCC] used :

<img src="https://skillicons.dev/icons?i=c" height="30" alt="c logo"  /> <img width="5" /> <img src="https://skillicons.dev/icons?i=linux" height="30" alt="linux logo"  /> 

### Prerequisites :

*  Knowledge of encryption and decryption
*  File I/O
*  Pointers and Structure

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.


### Installation

Compile all the C files using:
 ```gcc
gcc *.c
 ```
You can create a makefile to build this project if you want

For encrypting, you have to use -e followed by an input image filename followed by a secret filename. (Note that these files must be present in the current project directory, otherwise an error message appears. 
Also note that the input image filename must be having ".bmp" format to avoid error while the secret file can be of any format: .c, .sh, .txt. But the format type cannot exceed more than 4 characters because a
macro MAX_FILE_SUFFIX has been set to 4. Also don't miss the dot in any filename. It will result in an error). Example -
 ```sh
./a.out -e beautiful.bmp secret.txt
 ```
The above input will generate an additional image file in the project directory namely "stego_img.bmp" which is default output filename. You can provide your own output filename by giving 4th argument as shown below: (Note that this filename must be having ".bmp" format
, otherwise an error message will be shown)
 ```sh
./a.out -e beautiful.bmp secret.txt stegged_img.bmp
 ```
For decrypting, you have to use -d followed by output image filename. Example -
 ```sh
./a.out -d stego_img.bmp
 ```
It will decrypt the secret message from the image and store it in a newly created file called "decoded.txt". This file has the same format as that of the input secret file format. If you want to provide your own decoded filename, you can do so by using 3rd argument as shown below:
 ```sh
./a.out -d stego_img.bmp decoded_file.txt
 ```
(Note that the file format must match with that of the input secret file's format. Otherwise an error message appears)

** if you want the full code search and find it's secret.


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ROADMAP -->
## Roadmap

<div align="center">
  <img height="700" src="https://raw.githubusercontent.com/AnanthanAnil/LSB-Image-Steganography/main/LSB%20Image%20Steganography/References/ls_fc.jpg?token=GHSAT0AAAAAACT3Z2HGVBJWAJEJN2COVL3CZU5ACJA"  />
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- Outputs -->
## Output

**Encoding**

 ```
**********************   STEGANOGRAPHY  *************************


*******************  Validation successful  *********************

*******************  Opening required files  ********************
INFO : Opened beautiful.bmp
INFO : Done
INFO : Opened secret.txt
INFO : Done
INFO : Opened stego_img.bmp
INFO : Done
----------------------  Checking the files  ---------------------
INFO : Checking for secret.txt size
INFO : Done. Not Empty
----------------------  Image information  ----------------------
INFO : width          = 1024
INFO : height         = 768
INFO : Size of image  = 2359296
-----------------------------------------------------------------
INFO : Checking for beautiful.bmp capacity to handle secret.txt
INFO : Done. Found OK
INFO : Opened beautiful.bmp
INFO : Done
INFO : Opened secret.txt
INFO : Done
INFO : Opened stego_img.bmp
INFO : Done
------------------  Encoding Procedure Started  -----------------
INFO : Copying Image Header
INFO : Done
INFO : Encoding Magic String Signature
INFO : Done
INFO : Encoding secret.txt File Extenstion Size
INFO : Done
INFO : Encoding secret.txt File Extenstion
INFO : Done
INFO : Encoding secret.txt File Size
INFO : Done
INFO : Encoding secret.txt File Data
INFO : Done
INFO : Copying Left Over Data
INFO : Done
################### Encoding Done Successfully ##################


*******************   COMPLETED STEGANOGRAPHY  ******************
```
**Decoding**

```

**********************   STEGANOGRAPHY  *************************


*******************  Validation successful  *********************

------------------  Decoding Procedure Started  -----------------
INFO : Opening required files
INFO : open_input_files Function completed
INFO : stego_img.bmp file is open
-----------------  Decoding Magic String Signature  -------------
INFO : Passcode :23039
-----------------------------------------------------------------
INFO : Entered Passcode 23039
INFO : Decoding Magic String Signature
INFO : Decoding Passcode
INFO : Done
INFO : The decoded magic string is : 23039
INFO : Done
INFO : Passcode matched
INFO : Decoding extension size
INFO : Done
INFO : extn size: 4
INFO : Decoding Output File Extension size
INFO : Decoding output extension
INFO : Done
INFO : Extension -> .txt
INFO : Merging
INFO : Done
INFO : Extension : secret_output.txt
INFO : File "secret_output.txt" is open for writing
INFO : Decoding output file size
INFO : Done
INFO : Output file size : 16
INFO : Decoding secret data
INFO : Done
INFO : Secret data -> extract the pic

################### Decoding Done Successfully ##################


*******************   COMPLETED STEGANOGRAPHY  ******************
```
<p align="right">(<a href="#readme-top">back to top</a>)</p>
<!-- REFERENCES -->

##  References

* [BMP](https://en.wikipedia.org/wiki/BMP)
* [Steganography](https://en.wikipedia.org/wiki/Steganography)

<p align="right">(<a href="#readme-top">back to top</a>)</p>




