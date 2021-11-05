/* 

  SimpleRShellServer.c

  Created by Xinyuan Wang for CS 468

  All rights reserved.

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <openssl/evp.h>
#define DEBUG

int
serversock(int UDPorTCP, int portN, int qlen)
{
	struct sockaddr_in svr_addr;	/* my server endpoint address		*/
	int    sock;			/* socket descriptor to be allocated	*/

	if (portN<0 || portN>65535 || qlen<0)	/* sanity test of parameters */
		return -2;

	bzero((char *)&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;

    /* Set destination port number */
	svr_addr.sin_port = htons(portN);

    /* Allocate a socket */
	sock = socket(PF_INET, UDPorTCP, 0);
	if (sock < 0)
		return -3;

    /* Bind the socket */
	if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0)
		return -4;

	if (UDPorTCP == SOCK_STREAM && listen(sock, qlen) < 0)
		return -5;

	return sock;
}

int 
serverTCPsock(int portN, int qlen) 
{
  return serversock(SOCK_STREAM, portN, qlen);
}

int 
serverUDPsock(int portN) 
{
  return serversock(SOCK_DGRAM, portN, 0);
}

void 
usage(char *self)
{
	fprintf(stderr, "Usage: %s port\n", self);
	exit(1);
}

void 
errmesg(char *msg)
{
	fprintf(stderr, "**** %s\n", msg);
	exit(1);

}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
void
reaper(int signum)
{
/*
	union wait	status;
*/

	int status;

	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}


/*------------------------------------------------------------------------
 *  This is a very simplified remote shell, there are some shell command it 
	can not handle properly:

	cd
 *------------------------------------------------------------------------
 */
 
char *concatenate(char *a, char *b, char *c)
{
  int size = strlen(a) + strlen(b) + strlen(c) + 1;
  char *str = malloc(size);
  strcpy (str, a);
  strcat (str, b);
  strcat (str, c); 

  return str;
}
 char * decrypt(key,iv,cipher){

 //initilize ctx
	EVP_CIPHER_CTX ctx; 
	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

	int x;
	int cipherLength= strlen(cipher);
	unsigned char *plaintext = malloc(cipherLength);
	EVP_DecryptUpdate(&ctx, plaintext, &cipherLength, cipher, cipherLength);
	EVP_DecryptFinal_ex(&ctx, plaintext + cipherLength, &x);
	//plaintext is decrypted
	cipherLength = cipherLength + x;
	return plaintext;
 }
int
RemoteShellD(int sock, char * passFile)
{
#define	BUFSZ		128
#define resultSz	4096
	char cmd[BUFSZ+20];
	char nonce1[32];
	char result[resultSz];
	int	cc, len;
	int rc=0;
	FILE *fp;
	char line[BUFSZ], line2[BUFSZ];
	char* user, * pass;
	
	fp = fopen(passFile, "rt");
	if(fp == NULL)
	{
		printf("cannot open file\n");
		return 0;
	}
	//get a line from file then get sha1 hash of that 
	while(fgets(line, sizeof(line), fp) != NULL)
	{
		//printf(line);
		//get user
		
		stpcpy(line2,line);
		user = strtok (line2,"; ");
		//printf("user is %s\n", user);
		//get the password
		pass = strtok (NULL, "; ");
		//printf ("pass is %s\n",pass);
	}
	fclose(fp);
#ifdef DEBUG
	printf("***** RemoteShellD(sock=%d) called\n", sock);
#endif

	//catch pass from client and compare
	//char receivedpass[]="0c8f72ea98dc74c71f9ff5bb72941036ae5120d9";
	char receivedpass[BUFSZ];
	char receiveduser[BUFSZ];
	// read the message from client and copy it in buffer 
    read(sock, receiveduser, sizeof(receiveduser));
	printf("Recieved user is %s user is %s\n",receiveduser,user);
	int y=strcmp(receiveduser,user);
	if(y!=0)
	{
		printf("invalid ID\n");
		return(0);
	}
	//ask for auth
	char req[]= "AUTH_REQ";
	write(sock,req , sizeof(req)); 
	printf("sent auth req\n");
	//read nonce1
	read(sock, nonce1, sizeof(nonce1));
	//int x=strcmp(nonce1,pass);
	//printf("nonce is %s pass is %s",receivedpass,pass);

	//char req2[]= "AUTH_RESP";
	int nonce2 = rand() & 0xff;
	nonce2 |= (rand() & 0xff) << 8;
	nonce2 |= (rand() & 0xff) << 16;
	nonce2 |= (rand() & 0xff) << 24;
	
	write(sock,nonce2 , sizeof(nonce2));
	//get back encrypted cmd 
	char iv=SHA256(strcat(nonce1,nonce2));
	
	while ((cc = read(sock, cmd, BUFSZ)) > 0)	/* received something */
	{	
		char *comd= decrypt(concatenate(SHA1(pass),nonce1,nonce2),iv,cmd);

#ifdef DEBUG
		printf("***** RemoteShellD(%d): received %d bytes: `%s`\n", sock, cc, comd);
#endif

		strcat(comd, " 2>&1");
#ifdef DEBUG
	printf("***** cmd: `%s`\n", cmd); 
#endif 
		if ((fp=popen(comd, "r"))==NULL)	/* stream open failed */
			return -1;

		/* stream open successful */

		while ((fgets(result, resultSz, fp)) != NULL)	/* got execution result */
		{
			len = strlen(result);
			printf("***** sending %d bytes result to client: \n`%s` \n", len, result);

			if (write(sock, result, len) < 0)
		{ rc=-1;
			  break;
			}
		}
		fclose(fp);

	}

	if (cc < 0)
		return -1;

	return rc;
}

/*------------------------------------------------------------------------
 * main - Concurrent TCP server 
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	int	 msock;			/* master server socket		*/
	int	 ssock;			/* slave server socket		*/
	int  portN;			/* port number to listen */
	struct sockaddr_in fromAddr;	/* the from address of a client	*/
	unsigned int  fromAddrLen;		/* from-address length          */
	int  prefixL, r;
	char* passwordFile;

	if (argc==3){
		portN = atoi(argv[1]);
		passwordFile= argv[2];
	}
	else usage(argv[0]);
	msock = serverTCPsock(portN, 5);


	(void) signal(SIGCHLD, reaper);

	while (1) 
	{
		fromAddrLen = sizeof(fromAddr);
		ssock = accept(msock, (struct sockaddr *)&fromAddr, &fromAddrLen);
		if (ssock < 0) {
			if (errno == EINTR)
				continue;
			errmesg("accept error\n");
		}

		switch (fork()) 
		{
			case 0:		/* child */
				close(msock);
				r=RemoteShellD(ssock,passwordFile);
				close(ssock);
				exit(r);

			default:	/* parent */
				(void) close(ssock);
				break;
			case -1:
				errmesg("fork error\n");
		}
	}
	close(msock);
}
