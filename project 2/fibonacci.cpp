#include<stdio.h>
#include<pthread.h>
#include<tgmath.h>
#include<unistd.h>
/*student@tuffix-vm:~$ gcc -o test fibonacci.cpp -pthread
student@tuffix-vm:~$ ./test -pthread 1 2 3 4 
Parent Thread started
Child Thread Started 
Enter the Nth index of series: 5
Child Thread Closed
Parent Thread now printing the Sequence of fibbonacci series : 
0	1	1	2	3	
Parent Thread Closedstudent@tuffix-vm:~$ ./test -pthread 1 2 3 4 5 6
Parent Thread started
Child Thread Started 
Enter the Nth index of series: 8
Child Thread Closed
Parent Thread now printing the Sequence of fibbonacci series : 
0	1	1	2	3	5	8	13	
*/



void *fibb(void *p);

int data[1000];
int num1;

int main() {

	pthread_t child;
	pthread_attr_t attr; // set of thread attributes

	//get the default attributes
	pthread_attr_init(&attr);
	pthread_create(&child, &attr, fibb, NULL);


	printf("Parent Thread started\n");

	pthread_join(child, NULL);

	printf("Parent Thread now printing the Sequence of fibbonacci series : \n");
	for (int i = 0; i < num1; i++)
	{
		printf("%d\t", data[i]);
	}

	printf("\nParent Thread Closed");


}

void *fibb(void *p) {
	sleep(1);
	int i;
	printf("Child Thread Started \n");
	printf("Enter the Nth index of series: ");
	scanf("%d", &num1);
	data[0] = 0;
	data[1] = 1;
	for (i = 2; i < num1; i++)
		data[i] = data[i - 1] + data[i - 2];

	//printf("The %dth term of series is : %d",num1,data[num1-1]);

	printf("Child Thread Closed\n");
	pthread_exit(0);

}
