//Anthony Ruiz		888795127
//CPSC 351 "operating systems" spring 2021
//Project 3

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>

using namespace std;


int main(int argc, char** argv, char** argo )//added a second file to write out to(anthony)
{
	/* Make sure the command line is correct */
		if (argc < 3)
		{
			cout << "FILE NAME missing\n";
			exit(1);
		}

	/* Open the specified file */
	int inFile = open(argv[1], O_RDONLY);//open file file for input
	/*open the output file to copy to*/
  int outFile = open(argo[2], O_RDWR|O_CREAT|O_TRUNC,0664);//file for output,

	if (inFile < 0)
	{
		cout << "\n" << "input file cannot be opened" << "\n";
		exit(1);
	}

	if (outFile < 0)/*checking if output file cannot be opened*/
	{
		cout << "\n" << "output file cannot be opened" << "\n";
		exit(1);
	}



	struct stat stats;
	if (stat(argv[1], &stats) == 0)
		cout << endl << "file size " << stats.st_size;
	else
		cout << "Unable to get file properties.\n";


	/* Get the page size  */
	int pagesize = getpagesize();
	cout << endl << "page size is " << pagesize << "\n";
int filesize = stats.st_size;

// initializing the outputfile
lseek(outFile, filesize - 1, SEEK_SET);
write(outFile, "", 1);
lseek(outFile, 0, SEEK_SET);
// initializing the offset to 0
off_t offset = 0;
// copying from inFile to outFile
while (filesize > 0)
{
	// adjusting the filesize and pagesize
	 		if (filesize < pagesize)
	 		{ pagesize = filesize; filesize = 0; }
				else { filesize -= pagesize; }

			/*map the file into memory*/
char* src = (char*)mmap(NULL, pagesize, PROT_READ, MAP_SHARED, inFile, offset);


/* Did the mapping fail ? */
if (src == MAP_FAILED)
 	{ cout << "\n" << "source mapping did not succeed" << "\n";
 	exit(1); }

  	/* map the file into memory */
char* dest = (char*)mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, outFile, offset);

		/* Did the mapping fail ? */
 if (dest == MAP_FAILED)
  { cout << "\n" << "destination mapping did not succeed" << "\n";
	exit(1); }
					// copying pagesize bytes from src to dest
					memcpy(dest, src, pagesize);

					/* Unmap the shared memory region */
	 				munmap(src, pagesize);
	  			munmap(dest, pagesize);

 // moving inFd and outFd read/write file offsets
lseek(inFile, pagesize, SEEK_SET);
lseek(outFile, pagesize, SEEK_SET);

offset+= pagesize; // updating file offset 
 }
/*end of while loop*/

  close(inFile);// close the read file
 	close(outFile);// close the write file


	// end of program

	return 0; }
