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
	char *plaintext;

	int i;
	char * key = "0f1571c947d9e859";
  	for (i = 1; i < argc; i++){	
		if (strcmp( "-k" , argv[i] ) == 0){	
			if (argv[i + 1] ){
				// set the name of the key file 
				char * keyfile = argv[i + 1];
				printf("key is %s",key);
				if ( strlen(key)-1 < 8){
					printf("Key must be 8 bytes\n");
					exit(1);
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
				//set output file
				outfile = argv[i + 1];
			}
			
		}
	}
	//check for issues
	if(!key || !outfile|| !inputfile){
		if(!key){
			printf("key was not initilized\n");
		}
		if(!outfile){
			printf("invalid output file\n");
		}
		if(!inputfile){
			printf("invalid input file\n");
		}
		return 0;
	}

	else{
		int filelength = strlen(plaintext)+1;
		//initilize evp
		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);
		EVP_EncryptInit_ex(&ctx, EVP_des_ecb(), NULL, key, NULL);
		
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
		
		//Write cipher size to file 
		output = fopen("size.txt","w");
		fprintf(output, "%d", filelength);
		fclose(output);
		
	}
	
	
	return 0;
}