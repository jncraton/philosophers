#include <pthread.h>
#include <stdio.h>
#include <assert.h>

#define HUNGER 0x10
#define PHILOSOPHERS 0x10
#define PICKUP_TIME 0x1000000

typedef struct {
  pthread_mutex_t* left_fork;
  pthread_mutex_t* right_fork;
  unsigned int food_consumed;
  unsigned int id;
} Philosopher;

unsigned total_food_consumed = 0;

void eat(Philosopher * philosopher) {
  total_food_consumed++;
  philosopher->food_consumed++;

  printf("Philosopher %d has eaten %d times.\n", philosopher->id, philosopher->food_consumed);
}

void get_fork(pthread_mutex_t * fork) {
    for (int i = 0; i < PICKUP_TIME; i++);
    pthread_mutex_lock(fork);
}

void return_fork(pthread_mutex_t * fork) {
    pthread_mutex_unlock(fork);
}

long int child(Philosopher * philosopher) {
  while (philosopher->food_consumed < HUNGER) {
    get_fork(philosopher->left_fork);
    get_fork(philosopher->right_fork);

    eat(philosopher);

    return_fork(philosopher->right_fork);
    return_fork(philosopher->left_fork);
  }

  return philosopher->food_consumed;
}

int main() {
  pthread_t child_thread[PHILOSOPHERS];

  Philosopher philosophers[PHILOSOPHERS];
  pthread_mutex_t forks[PHILOSOPHERS];

  for (unsigned long i = 0; i < PHILOSOPHERS; i++) {
    pthread_mutex_init(&forks[i], 0);

    philosophers[i].id = i;
    philosophers[i].food_consumed = 0;
    philosophers[i].right_fork = &forks[i];
    philosophers[i].left_fork = &forks[(i-1) % PHILOSOPHERS];
    
    int code;
    code = pthread_create(&child_thread[i], NULL, (void*)child, (void*)&philosophers[i]);
    if (code) {
      fprintf(stderr, "pthread_create failed with code %d\n", code);
    }
  } 

  unsigned long thread_consumed;
  unsigned long reported_total_consumed = 0;

  for (unsigned int i = 0; i < PHILOSOPHERS; i++) {
    pthread_join(child_thread[i], (void*)&thread_consumed);
    assert(thread_consumed == HUNGER);
    assert(philosophers[i].food_consumed == HUNGER);
    reported_total_consumed += thread_consumed;
  }

  assert(reported_total_consumed == HUNGER * PHILOSOPHERS);
  assert(total_food_consumed == HUNGER * PHILOSOPHERS);

  printf("Success. All tests passed.\n");
  
  return 0;
}