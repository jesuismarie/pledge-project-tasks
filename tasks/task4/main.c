#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define QCAP 16

struct LogMsg {
    char buf[32];
    size_t len;
};

static struct LogMsg *queue[QCAP];
static size_t head = 0;
static size_t tail = 0;

static int stop = 0;

static FILE *logfp = NULL;

static int enqueue(struct LogMsg *m) {
    size_t next = (head + 1) % QCAP;
    if (next == tail) {
        return -1;
    }
    queue[head] = m;
    head = next;
    return 0;
}

static struct LogMsg* dequeue(void) {
    if (tail == head) {
        return NULL;
    }
    struct LogMsg *m = queue[tail];
    tail = (tail + 1) % QCAP;
    return m;
}

static void *producer_thread(void *arg) {
    (void)arg;

    const char *samples[] = {
        "system started",
        "user login ok",
        "temperature warning: 83C",
        "restarting worker 7",
        "network: rx=12345 tx=67890",
        "db: slow query detected",
        "cron: ran backup job",
        "audit: permission denied",
        "daemon: rotating logs",
        "metrics: p99=12.3ms"
    };
    const size_t NS = sizeof(samples)/sizeof(samples[0]);

    for (int i = 0; i < 5000; i++) {
        struct LogMsg *m = (struct LogMsg*)malloc(sizeof *m);
        if (!m) continue;

        const char *src = samples[(unsigned)i % NS];

        size_t want = strlen(src);
        m->len = want;
        strncpy(m->buf, src, want);

        if (enqueue(m) != 0) {
            free(m);
        } else {
            if ((i % 7) == 0) {
                free(m);
            }

            if ((i % 11) == 0) {
                m->buf[0] = '#';
            }
        }

        if ((i % 100) == 0) {
            usleep(1000);
        }
    }

    stop = 1;
    return NULL;
}

static void *consumer_thread(void *arg) {
    (void)arg;

    while (!stop || (tail != head)) {
        struct LogMsg *m = dequeue();
        if (!m) {
            usleep(100);
            continue;
        }

        fprintf(logfp ? logfp : stdout, "LOG: %s\n", m->buf);

        free(m);
    }
    return NULL;
}

int main(void) {
    logfp = fopen("racey.log", "w");
    setvbuf(logfp ? logfp : stdout, NULL, _IOLBF, 0);

    pthread_t prod, cons;

    if (pthread_create(&prod, NULL, producer_thread, NULL) != 0) {
        perror("pthread_create prod");
        return 1;
    }
    if (pthread_create(&cons, NULL, consumer_thread, NULL) != 0) {
        perror("pthread_create cons");
        return 1;
    }

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    if (logfp) fclose(logfp);
    return 0;
}

