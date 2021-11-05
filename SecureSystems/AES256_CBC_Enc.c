#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>


char *plainText( char filename[] ){
	unsigned char *text;
	int c;
	FILE *fp;
	int n = 0;
	// open file
	fp = fopen(filename, "r");
	// check for errors 
	if ( !fp ){
		printf("error opening file\n");
		exit(-1);
	}

	// get the length of the file 
	fseek (fp , 0 , SEEK_END);
	int length = ftell (fp);
	fseek (fp , 0 , SEEK_SET);

	text = malloc(1+length);
	
	// read characters from file
	while ( (c = fgetc(fp)) != EOF ){
		text[n++] = (char)c;
	}
	//close file pointer
	fclose(fp);
	//for debugging
	//printf(text);
	return text;
}

int main(int argc, char *argv[]){
  
	char *inputfile,*outfile;
	int i;
	char *key = "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f00";
	char *iv = "d9000a0800ac3b75111d393adeadbeef";
	char *plaintext;
	//check the arguments
  	for (i = 1; i < argc; i++){	
		if (strcmp( "-k" , argv[i] ) == 0){	
			if (argv[i + 1] ){
				// set the name of the key file 
				char* keyfile = argv[i + 1];
				printf("key is %s",key);
				if ( strlen(key)-1 < 8){
					printf("key must be longer\n");
					return 0;
				}
				key = plainText(keyfile);
			}
		}
		else if (strcmp( "-i" , argv[i] ) == 0){
			if (argv[i + 1]){
				// set input file name 
				inputfile = argv[i + 1];
				// get the plaintext from my inputfile
				plaintext = plainText(inputfile);
			}
		}

		else if (strcmp( "-o" , argv[i] ) == 0){
			if (argv[i + 1]){
				//set the  output file
				outfile = argv[i + 1];
			}
		}
		else if (strcmp( "-v" , argv[i] ) == 0){	
			if (argv[i + 1] != NULL){	
				char * ivfile = argv[i + 1];
				// same steps as key
				iv = plainText(ivfile);
				// check that the length is 16 
				if (strlen(iv) < 16 ){
					printf("iv is not long enough\n");
					return 0;
				}
			}
		}
	}
	//check for issues
	if(!key || !outfile|| !inputfile || !iv){
		if(!key){
			printf("key was not initilized\n");
		}
		if(!outfile){
			printf("invalid output file\n");
		}
		if(!inputfile){
			printf("invalid input file\n");
		}
		if(!iv){
			printf("IV was not initilized\n");
		}
		return 0;
	}
	
	
	//start encrypt
	
	// if iv is set, encrypt with iv and key and write to file
	if ( iv && key ){
		int filelength = strlen(plaintext)+1;
		//initilize evp
		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);
		EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);
		
		//encrypt using DES
		int x;
		int length=filelength + 8;
		unsigned char *ciphertext = malloc(filelength + 8);
	
		// use evp to encypt 
		EVP_EncryptUpdate(&ctx, ciphertext, &length, plaintext, filelength);
		EVP_EncryptFinal_ex(&ctx, ciphertext+length, &x);
	
		filelength = length + x;
	
		//open outfile for writing 
		FILE *output = fopen(outfile,"wb");
		int i = 0;
		while(i < filelength){
			fputc(ciphertext[i],output);
			i++;
		}
		fclose(output);
		
		//write the size of the cipher somewhere
		output = fopen("ciphersize.txt","w");
		fprintf(output, "%d", length);
		fclose(output);
	
		// free our key 
		EVP_CIPHER_CTX_cleanup(&ctx);
		
	}

	return 0;
}