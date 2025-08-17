#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "config.h"

_Thread_local int tls_counter = 0;

static void *worker(void *arg) {
    (void)arg;
    double v = sin(0.5);
    tls_counter += (int)(v * 1000.0);
    return NULL;
}

int main(void) {
    printf("Project: %s\n", PROJECT_NAME);

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

    printf("tls_counter=%d\n", tls_counter);
    return 0;
}

