#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "config.h"

int tls_counter = 0;
pthread_mutex_t counter_mutex;

static void *worker(void *arg) {
	(void)arg;
	double v = sin(0.5);
	pthread_mutex_lock(&counter_mutex);
	tls_counter += (int)(v * 1000.0);
	pthread_mutex_unlock(&counter_mutex);
	return NULL;
}

int main(void) {
	printf("Project: %s\n", PROJECT_NAME);

	if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
		perror("pthread_mutex_init");
		return 1;
	}

	pthread_t th[2];

	for (int i = 0; i < 2; i++) {
		if (pthread_create(&th[i], NULL, worker, NULL) != 0) {
			perror("pthread_create");
			return 1;
		}
	}
	for (int i = 0; i < 2; i++) {
		pthread_join(th[i], NULL);
	}

	printf("tls_counter = %d\n", tls_counter);

	pthread_mutex_destroy(&counter_mutex);
	return 0;
}
