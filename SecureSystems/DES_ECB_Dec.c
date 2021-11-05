
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

char *plainText( char filename[] ){
	unsigned char *text;
	int c;
	FILE *fp;
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


	text = malloc(length );
	int n = 0;
	
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

	char *key = "0f1571c947d9e859";
	char *cipher;

	int cipherLength;
	
	//check the args
	int i;
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
  
 
		cipher = plainText(inputfile);
		
		//get the cipher size from a file
		FILE *outf;
	
		outf = fopen("size.txt","r");
		fscanf(outf,"%d",&cipherLength); 
		fclose(outf);
		
		//initilize ctx
		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);
		EVP_DecryptInit_ex(&ctx, EVP_des_ecb(), NULL, key, NULL);

		int temp;
		unsigned char *plaintext = malloc(cipherLength);
		EVP_DecryptUpdate(&ctx, plaintext, &cipherLength, cipher, cipherLength);
		EVP_DecryptFinal_ex(&ctx, plaintext + cipherLength, &temp);
		//plaintext is decrypted
		cipherLength = cipherLength + temp;
		//write the plaintext to ouput
		FILE *wf = fopen(outfile , "w");
		int x;
		for(x = 0; plaintext[x] ; x++){
			fputc(plaintext[x],wf);
		}
		fclose(wf);
		
		//free the cipher key
		EVP_CIPHER_CTX_cleanup(&ctx);
		
	}
	return 0;
}