/* 
	This code contain N thread for client and 1 thread for server, client thread 
	place tuple in tuple space and wait for response of server, server thread match 
	requested tuple and signal client, and remove record from tuple space. 

*/
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

//Mutex initialization
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

//Structure for operation ID and PARAMETER
struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int id;
	int parameter;
} tp = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0,0};

//Operations 
char services[25][50] = {
	"AddTwoNumbers","SubtractTwoNumbers","MultiplyTwoNumbers","DivideTwoNumbers","SquarRoot","Square","Area","Volume","Perimeter",
	"Circumference","SurfaceArea","Integrate","Differentiate","Power","Logarithm","StringLength","Encrypt","Decrypt","EdgeDetection",
	"FFT","RayTracing","VolumRendering","ZBuffer","TextureMapping","MotionBlurr"}; 

//Prototype of thread functions.
void *client_handler(void *arg);
void *server_handler(void *arg);

//Prototype of RANDOM functions.
int generate_random(int limit);


//Global variable which used by both thread for signaling and waiting
int c = 0,p;

//Global 2D Dynamic array
int **tuple;

int main(int argc, char const *argv[]) {
	int i;

	//Generate random  number of clients
	p = generate_random(10);
	printf("Thread Number : %d\n",p);

	//Dynamically allocate space for place an operation 
	tuple = (int**)malloc(p*sizeof(int*));

	pthread_t client[p], server;
	
	//Creatig client thread	
	for (int i = 0; i < p; ++i) {
		pthread_create(&client[i], NULL, client_handler, &c);
		sleep(1);
	}

	//Creating server thread
	pthread_create(&server, NULL, server_handler, &c);

	for (int i = 0; i < p; ++i) {
		pthread_join(client[i], NULL);
	}

	pthread_join(server, NULL);
	free(tuple);
	return 0;
}


//Server function
void *server_handler(void *arg){	
	int n = 0;
	for (; ;) {
		pthread_mutex_lock(&tp.mutex);
		if (c!=0) {
			printf("\nServer:- tuple <%d %s %d> found for client[%d]\n",tuple[n][0],services[tuple[n][1]-1],tuple[n][2],n+1);
			c -= 1;	
			pthread_cond_signal(&tp.cond);
		}
		++n;
		pthread_mutex_unlock(&tp.mutex);
		sleep(1);
	}
}

//client function
void *client_handler(void *arg) {
	int n = *((int *)arg);
    tuple[n] = (int*)malloc(3*sizeof(int));
	pthread_mutex_lock(&mutex1);
	tp.id += 1;
	tuple[n][0] = tp.id;
	tuple[n][1] = generate_random(25); 
	tuple[n][2] = generate_random(3);

	n = n+1;
	++c;
	pthread_mutex_unlock(&mutex1);

	pthread_mutex_lock(&tp.mutex);
	while(c!=0) {
			pthread_cond_wait(&tp.cond, &tp.mutex);
		printf("client[%d]:- tuples <%d %s %d> has been provided.\n",n,tuple[n-1][0],services[tuple[n-1][1]-1],tuple[n-1][2]);		
	}
	pthread_mutex_unlock(&tp.mutex);
}

//Random function
int generate_random(int limit) {
    int r,i;
    time_t t;
    srand ( time(0) );
    r = 0;
    r = rand()%limit+1;
    return r;
}