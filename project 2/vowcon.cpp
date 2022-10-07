#include <pthread.h>
#include <sched.h>
#include <stdio.h>
/*
student@tuffix-vm:~$ gcc -o test vowcon.cpp -pthread
student@tuffix-vm:~$ ./test Operating Systems Class CSFU
Vow : Operating
Cons : Systems
Cons : Class
Cons : CSFU
student@tuffix-vm:~$ ./test Operating Systems Class at CSUF
Vow : Operating
Cons : Systems
Cons : Class
Vow : at
Cons : CSUF
student@tuffix-vm:~$ 
*/
static int currentIndex;

static int count;

void *consThread(void *param)

{

	char **str = (char **)param;

	while (currentIndex < count)

	{

		char letter_inp = str[currentIndex][0];
		// checks the sting at index 1
		if ((letter_inp != 'A') && (letter_inp != 'a') &&

			(letter_inp != 'E') && (letter_inp != 'e') &&

			(letter_inp != 'I') && (letter_inp != 'i') &&

			(letter_inp != 'O') && (letter_inp != 'o') &&

			(letter_inp != 'U') && (letter_inp != 'u'))

		{

			printf("Cons : %s\n", str[currentIndex]);
			// increase the index
			currentIndex++;

		}

		else

		{

			// this function yields the processor
			sched_yield();
		}
	}

	pthread_exit(0);
}

void *vowThread(void *param)

{

	char **str = (char **)param;

	while (currentIndex < count)

	{

		char letter_inp = str[currentIndex][0];
		// checks the string at index 1
		if ((letter_inp == 'A') || (letter_inp == 'a') ||

			(letter_inp == 'E') || (letter_inp == 'e') ||

			(letter_inp == 'I') || (letter_inp == 'i') ||

			(letter_inp == 'O') || (letter_inp == 'o') ||

			(letter_inp == 'U') || (letter_inp == 'u'))

		{
			// prints the vowels
			printf("Vow : %s\n", str[currentIndex]);

			currentIndex++;

		}

		else

		{
			// this function yield the processor
			sched_yield();
		}
	}

	pthread_exit(0);
}

int main(int argc, char *argv[])

{

	currentIndex = 1;
	count = argc;
	pthread_t thread_1, thread_2;
	pthread_attr_t attr;

	// checks to see if there are enough argument if not it returns error
	// statement
	if (argc < 2) {

		printf("Error: not enough arguments\n");

		return -1;
	}

	pthread_attr_init(&attr);
	// creates 2 threads
	pthread_create(&thread_1, &attr, vowThread, (void *)argv);

	pthread_create(&thread_2, &attr, consThread, (void *)argv);

	// joins for both threads
	pthread_join(thread_1, NULL);

	pthread_join(thread_2, NULL);

	return 0;
}
