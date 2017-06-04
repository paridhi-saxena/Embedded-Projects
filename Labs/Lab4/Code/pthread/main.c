//
//  main.c
//  pthread-demo3
//
//

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "test_client.h"
#include "gpio-galileo.h"

#define NUM_THREADS 3

#include "facerec_video.hpp"
#include "ProxSensorCamTemp.hpp"

pthread_mutex_t cmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

int initSensor();
int getProxVal();
float getTempVal();
int captureAndProcess();
int picCommand(int select);
extern int call_POST(char* datafile, int adcvalue, char* sensorstatus);

void *ThreadWait(void *t)
{
    int threadid;
    threadid = (int)t;
    int captureImg = 0;
    int prox;
    float tempVal;
    int toSend = 0;
    
    printf("Thread %d is sleeping\n", threadid);
    if(threadid == 0) {
	int sel;
    	printf("Please enter the digit corresponding to the command:\n");
	printf("Ping - 1");
	printf("Get ADC value - 2");
	scanf("%d", &sel);
	printf("You selected %d", sel);
	picCommand(sel);
    }

    if(threadid == 1) {
	   initSensor();
	   int k;
	   for(k=0; k<10;k++) {
		prox = getProxVal();
		tempVal = getTempVal();
		sleep(10);
		FILE *fp = fopen("sensor_data.txt", "w+");
		fprintf(fp, "Proximity value : %d \n", prox);
		fprintf(fp, "Temperature : %f\n", tempVal);
		fclose(fp);
		if(prox > 150 || tempVal > 26) {
			toSend = captureAndProcess();
			if(toSend == 1) {
				pthread_mutex_lock(&cmutex);
				pthread_cond_signal(&cond_var);
				pthread_mutex_unlock(&cmutex);
			}
			break;
		}
	   }
    }

    if(threadid == 2) {
	printf("Start sending\n");
	pthread_mutex_lock(&cmutex);
	pthread_cond_wait(&cond_var, &cmutex);	
	printf("Sending the picture with face to server\n");
	call_POST("face_recognizer.png", 234, "camera-ok");
//	call_POST("sensor_data.txt", 0, "sensor-data");
	pthread_mutex_unlock(&cmutex);
    }

    sleep(3);
    printf("Thread %d is exiting\n", threadid);
    pthread_exit(NULL);
}

int main ()
{
    int ret,i;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    
    // Initialize and set thread attributes to "joinable"
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for( i=0; i < NUM_THREADS; i++ ){
        printf("main() is creating thread %d \n", i);
        ret = pthread_create(&threads[i], NULL, ThreadWait, (void *)i );
        if (ret){
            printf("Unable to create thread %d\n", i);
            return -1;
        }
    }
    
    // deallocate attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for( i=0; i < NUM_THREADS; i++ ){
        ret = pthread_join(threads[i], &status);
        if (ret){
            printf("Error:unable to join thread %d, return = %d\n", i, ret);
            return -1;
        }
        printf("Thread %d has completed with status = %d\n", i, (int)status) ;
    }
    
    printf("program is exiting.\n");
    
    pthread_exit(NULL);
}
