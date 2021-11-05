/* 

  SimpleRShellClient.c

  Created by Xinyuan Wang for CS 468
 
  All rights reserved.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <openssl/evp.h>
//#define DEBUG

int
clientsock(int UDPorTCP, const char *destination, int portN)
{
	struct hostent	*phe;		/* pointer to host information entry	*/
	struct sockaddr_in dest_addr;	/* destination endpoint address		*/
	int    sock;			/* socket descriptor to be allocated	*/


	bzero((char *)&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;

    /* Set destination port number */
	dest_addr.sin_port = htons(portN);

    /* Map host name to IPv4 address, does not work well for IPv6 */
	if ( (phe = gethostbyname(destination)) != 0 )
		bcopy(phe->h_addr, (char *)&dest_addr.sin_addr, phe->h_length);
	else if (inet_aton(destination, &(dest_addr.sin_addr))==0) /* invalid destination address */
		return -2;

/* version that support IPv6 
	else if (inet_pton(AF_INET, destination, &(dest_addr.sin_addr)) != 1) 
*/

    /* Allocate a socket */
	sock = socket(PF_INET, UDPorTCP, 0);
	if (sock < 0)
		return -3;

    /* Connect the socket */
	if (connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
		return -4;

	return sock;
}
int 
clientTCPsock(const char *destination, int portN) 
{
  return clientsock(SOCK_STREAM, destination, portN);
}


int 
clientUDPsock(const char *destination, int portN) 
{
  return clientsock(SOCK_DGRAM, destination, portN);
}

#define	LINELEN		128
#define resultSz	4096

void usage(char *self)
{
	fprintf(stderr, "Usage: %s destination port\n", self);
	exit(1);
}

void errmesg(char *msg)
{
	fprintf(stderr, "**** %s\n", msg);
	exit(1);

}

/*------------------------------------------------------------------------------
 * TCPrecv - read TCP socket sock w/ flag for up to buflen bytes into buf

 * return:
	>=0: number of bytes read
	<0: error
 *------------------------------------------------------------------------------
 */
int
TCPrecv(int sock, char *buf, int buflen, int flag)
{
	int inbytes, n;

	if (buflen <= 0) return 0;

  /* first recv could be blocking */
	inbytes = 0; 
	n=recv(sock, &buf[inbytes], buflen - inbytes, flag);
	if (n<=0 && n != EINTR)
		return n;

	buf[n] = 0;

#ifdef DEBUG
	printf("\tTCPrecv(sock=%d, buflen=%d, flag=%d): first read %d bytes : `%s`\n", 
			   sock, buflen, flag, n, buf);
#endif /* DEBUG */

  /* subsequent tries for for anything left available */

	for (inbytes += n; inbytes < buflen; inbytes += n)
	{ 
	 	if (recv(sock, &buf[inbytes], buflen - inbytes, MSG_PEEK|MSG_DONTWAIT)<=0) /* no more to recv */
			break;
	 	n=recv(sock, &buf[inbytes], buflen - inbytes, MSG_DONTWAIT);
		buf[n] = 0;
		
#ifdef DEBUG
		printf("\tTCPrecv(sock=%d, buflen=%d, flag=%d): subsequent read %d bytes : `%s`\n", 
			   sock, buflen, flag, n, &buf[inbytes]);
#endif /* DEBUG */

	  if (n<=0) /* no more bytes to receive */
		break;
	};

#ifdef DEBUG
		printf("\tTCPrecv(sock=%d, buflen=%d): read totally %d bytes : `%s`\n", 
			   sock, buflen, inbytes, buf);
#endif /* DEBUG */

	return inbytes;
}
char *concatenate(char *a, char *b, char *c)
{
  int size = strlen(a) + strlen(b) + strlen(c) + 1;
  char *str = malloc(size);
  strcpy (str, a);
  strcat (str, b);
  strcat (str, c); 

  return str;
}
char * encrypt(key, iv,plaintext){
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);
		
	//encrypt using DES
	int x;
	int length=128 + 8;
	unsigned char *ciphertext = malloc(128);
	
	// use evp to encypt 
	EVP_EncryptUpdate(&ctx, ciphertext, &length, plaintext, 128);
	EVP_EncryptFinal_ex(&ctx, ciphertext+length, &x);
	
	return ciphertext;
}
int RemoteShell(char *destination, int portN, char* ID, char *password)
{
	char	buf[LINELEN+1];		/* buffer for one line of text	*/
	char	nonce2[32];		/* buffer for one line of text	*/
	char	result[resultSz+1];
	int	sock;				/* socket descriptor, read count*/
#define	BUFSZ		128
	int	outchars, inchars;	/* characters sent and received	*/
	int n;

	char pass[128], user[128];
	strcpy(pass,password);
	strcpy(user,ID);
	printf("user is %s",user);
	size_t length = strlen(pass);
    unsigned char hash[20];
	char output[20]="";
    SHA1(pass, length, hash);

    unsigned char str[100],strH[200];
    int i,j;
	// printf("SHA1 of %s is ", pass);
    // for (i = 0; i < 20; i++) {
        // //printf("%02x", hash[i]);
    // }
    // //printf("\n");
	memset(strH,0,sizeof(strH));
	
	/*converting str character into Hex and adding into strH*/
	for(i=0,j=0;i<20;i++,j+=2)
	{ 
		sprintf((char*)strH+j,"%02x",hash[i]);
	}
	strH[j]='\0';
	//printf("strH is %s\n",strH);
	
	int nonce1 = rand() & 0xff;
	nonce1 |= (rand() & 0xff) << 8;
	nonce1 |= (rand() & 0xff) << 16;
	nonce1 |= (rand() & 0xff) << 24;
	
	
/*	
    bzero(user, sizeof(user)); 
	//get message back 
	//check if message asks for pass
	read(sock, buf, sizeof(buf));
	if((strcmp(buf,"AUTH_REQ"))!=0){
		return 0;
	}
	//else send hashed password
	write(sock, hash, sizeof(hash)); 
    bzero(hash, sizeof(hash));
	*/
	if ((sock = clientTCPsock(destination, portN)) < 0)
		errmesg("fail to obtain TCP socket");
	//send user
	printf("\nsending user %s\n",user);
	write(sock, user, sizeof(user));
	//read message back 
	read(sock, buf, sizeof(buf));
	printf("buf is %s\n",buf);
	if((strcmp(buf,"AUTH_REQ"))!=0){
		printf("INVALID ID\n");
		return(0);
	}
	//else send nonce1
	printf("\nsending nonce %s\n",nonce1);
	write(sock, nonce1, sizeof(nonce1));
	//read the nonce2 we got back 
	read(sock, nonce2, sizeof(nonce2));
	printf("\nrecieved %s\n",nonce2);
	
	//printf("buf is %s\n",buf);
	//add buf is nonce 2 

	//char key[32]=SHA256(strH|nonce1|nonce2);

	//write encrypted message
	//write(sock, nonce1, sizeof(nonce1));
	char iv=SHA256(strcat(nonce1,nonce2));
	while (fgets(buf, sizeof(buf), stdin)) 
	{
		buf[LINELEN] = '\0';	/* insure line null-terminated	*/
		outchars = strlen(buf);
		char * encrypted= encrypt(concatenate(strH,nonce1,nonce2),iv,buf );
		if ((n=write(sock, encrypted, outchars))!=outchars)	/* send error */
		{
#ifdef DEBUG
			printf("RemoteShell(%s, %d): has %d byte send when trying to send %d bytes to RemoteShell: `%s`\n", 
			   destination, portN, n, outchars, buf);
#endif /* DEBUG */
			close(sock);
			return -1;
		}
#ifdef DEBUG
		printf("RemoteShell(%s, %d): sent %d bytes to RemoteShell: `%s`\n", 
			   destination, portN, n, buf);
#endif /* DEBUG */

		/* Get the result */

		if ((inchars=recv(sock, result, resultSz, 0))>0) /* got some result */
		{
			result[inchars]=0;	
			fputs(result, stdout);			
		}
		if (inchars < 0)
				errmesg("socket read failed\n");
	}

	close(sock);
	return 0;
}

/*------------------------------------------------------------------------
 * main  *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char *destination;
	int  portN;
	char * ID;
	char* password;
	
	if (argc==5)
	{ 
	  destination = argv[1];
	  portN = atoi(argv[2]);
	  ID = argv[3];
	  password= argv[4];
	}
	else usage(argv[0]);
		
	RemoteShell(destination, portN, ID, password);

	exit(0);
}

