// Project 1 by Aria Askaryar, Anthony, and Parth


/*		Anthony Ruiz 	anthony.ruiz@csu.fullerton.edu		*/


/*
output:
888046638@vclvm011316-225-86:~/Desktop$ clang++ skel.cpp -o test
888046638@vclvm011316-225-86:~/Desktop$ ./test3 /bin/ls
md5sum HASH VALUE: e7793f15c2ff7e747b4bc7079f5cd4f7  /bin/ls

sha1sum HASH VALUE: 732458574c63c3790cad093a36eadfb990d11ee6  /bin/ls

sha224sum HASH VALUE: 906fa19b9bfa3a08e309feac2e7197ee0c32b27af71749262dc13ddf  /bin/ls

sha256sum HASH VALUE: 1e39354a6e481dac48375bfebb126fd96aed4e23bab3c53ed6ecf1c5e4d5736d  /bin/ls

sha384sum HASH VALUE: f666e2eb6a5d999181e0a6bdb00fa60ac11717b7272fe8d010b50666f84945daeb8c4716b05cf4ed59ecd167af54f78b  /bin/ls

sha512sum HASH VALUE: 233382698c722f0af209865f7e998bc5a0a957ca8389e8a84ba4172f2413bea1889dd79b12607d9577fd2fc17f300c8e7f223c2179f66786e5a11e28f4d68e53  /bin/ls


*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

/* The pipe for parent-to-child communications */
int parentToChildPipe[2];

/* The pipe for the child-to-parent communication */
int childToParentPipe[2];

/* The read end of the pipe */
#define READ_END 0

/* The write end of the pipe */
#define WRITE_END 1

/* The maximum size of the array of hash programs */
#define HASH_PROG_ARRAY_SIZE 6

/* The maximum length of the hash value */
#define HASH_VALUE_LENGTH 1000

/* The maximum length of the file name */
#define MAX_FILE_NAME_LENGTH 1000

/* The process id */
pid_t pid;

/* The array of names of hash programs */
const string hashProgs[] = {"md5sum", "sha1sum", "sha224sum", "sha256sum", "sha384sum", "sha512sum"};

/**
 * The function called by a child
 * @param hashProgName - the name of the hash program
 */
void computeHash(const string& hashProgName)
{

	/* The hash value buffer */
	char hashValue[HASH_VALUE_LENGTH];

	/* The received file name string */
	char fileNameRecv[MAX_FILE_NAME_LENGTH];

	/* Fill the buffer with 0's */
	memset(fileNameRecv, (char)NULL, MAX_FILE_NAME_LENGTH);


		/* Glue together a command line <PROGRAM NAME>.
	 	 * For example, sha512sum fileName.
	 	 */
		string cmdLine(hashProgName);
		cmdLine += " ";
		cmdLine += fileNameRecv;

	       /* TODO: Open the pipe to the program (specified in cmdLine)
		* using popen() and save the ouput into hashValue. See popen.cpp
	        * for examples using popen.*/
		FILE* hashOut = popen(cmdLine.c_str(), "r");
		    if(!hashOut){
		        perror("popen");
		        exit(-1);
		}


		/* Reset the value buffer */
		memset(hashValue, (char)NULL, HASH_VALUE_LENGTH);




	/** TODO: Now, lets read a message from the parent **/
	if(read(parentToChildPipe[READ_END], fileNameRecv, MAX_FILE_NAME_LENGTH) < 0){
	       perror("read");
	       exit(-1);
	   }

	//fprintf(stderr, "recieved: ", fileNameRecv, pid);

	/* TODO: Send a string to the parent */
	if(write(childToParentPipe[WRITE_END], hashValue, HASH_VALUE_LENGTH) < 0)
	{
		perror("write");
		exit(-1);
	}


	/* The child terminates */
	exit(0);

}


int main(int argc, char** argv)
{

	/* Check for errors */
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(-1);
	}

	/* Save the name of the file */
	string fileName(argv[1]);



	/* Run a program for each type of hashing algorithm hash algorithm */
	for(int hashAlgNum = 0; hashAlgNum < HASH_PROG_ARRAY_SIZE; ++hashAlgNum)
	{
		/** TODO: create two pipes **/
		/* Create a parent-to-child pipe */
		if(pipe(parentToChildPipe) < 0)
		{
			perror("pipe");
			exit(-1);
		}


		/* Create a child-to-parent pipe */
		if(pipe(childToParentPipe) < 0)
		{
			perror("pipe");
			exit(-1);
		}




		/* Fork a child process and save the id */
		if((pid = fork()) < 0)
		{
			perror("fork");
			exit(-1);
		}
		/* I am a child */
		else if(pid == 0)

		{
			/** TODO: close the unused ends of two pipes **/
			/* Close the write-end of the parent-to-child pipe */
			if(close(parentToChildPipe[WRITE_END]) < 0)
			//||close(childToParentPipe[READ_END] < 0)
			{
				perror("close");
				exit(-1);
			}
			/* Compute the hash */
			computeHash(hashProgs[hashAlgNum]);
		}

		/* I am the parent */

		/** TODO: close the unused ends of two pipes. **/
		/* Close the read-end of the child-to-parent pipe */
		if(close(parentToChildPipe[READ_END]) < 0 ||
            close(childToParentPipe[WRITE_END]) < 0){
            perror("close");
            exit(-1);
        }

		/* The buffer to hold the string received from the child */
		char hashValue[HASH_VALUE_LENGTH];

		/* Reset the hash buffer */
		memset(hashValue, (char)NULL, HASH_VALUE_LENGTH);


		/* TODO: Send the string to the child*/
		if(write(parentToChildPipe[WRITE_END], fileName.c_str(), sizeof(fileName)) < 0)
		{
			perror("write");
			exit(-1);
		}


		/* TODO: Read the string sent by the child*/
		if(read(childToParentPipe[READ_END], hashValue, HASH_VALUE_LENGTH) < 0)
		{
			perror("read");
			exit(-1);
		}

		/* Print the hash value */
		fprintf(stdout, "%s HASH VALUE: %s\n", hashProgs[hashAlgNum].c_str(), hashValue);
		fflush(stdout);


		/* Wait for the program to terminate */
		if(wait(NULL) < 0)
		{
			perror("wait");
			exit(-1);
		}
	}

	return 0;
}
