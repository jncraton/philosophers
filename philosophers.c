#include <pthread.h>
#include <stdio.h>
#include <assert.h>

#define HUNGER 0x10
#define PHILOSOPHERS 0x10
#define DELAY 0x1000000

typedef struct {
  pthread_mutex_t mutex;
  unsigned int held;
  unsigned int uses;
  unsigned int times_held;
} Chopstick;

typedef struct {
  Chopstick *left_chopstick;
  Chopstick *right_chopstick;
  unsigned int food_consumed;
  unsigned int id;
} Philosopher;

void eat(Philosopher * philosopher) {
  /*
   * Simulates a philosopher eating. Requires both chopsticks to be held. 
   */

  philosopher->food_consumed++;
  printf("Philosopher %d has eaten %d times.\n", philosopher->id, philosopher->food_consumed);

  assert(philosopher->left_chopstick->uses < philosopher->left_chopstick->times_held);
  assert(philosopher->left_chopstick->held == 1);
  assert(philosopher->right_chopstick->held == 1);
  philosopher->left_chopstick->uses++;
  philosopher->right_chopstick->uses++;
}

void get_chopstick(Chopstick * chopstick) {
  /*
   * Blocks until `chopstick` is available then picks it up 
   */
  for (unsigned i = 0; i < DELAY; i++);
  pthread_mutex_lock(&chopstick->mutex);
  assert(chopstick->held == 0);
  chopstick->held = 1;
  chopstick->times_held++;
}

void return_chopstick(Chopstick * chopstick) {
  /*
   * Puts `chopstick` back and never blocks 
   */
  assert(chopstick->held == 1);
  chopstick->held = 0;
  pthread_mutex_unlock(&chopstick->mutex);
}

long int run_philosopher(Philosopher * philosopher) {
  /*
   * Implements one dining philosopher
   *
   * The purpose of this thread is to `eat` until no longer hungry. In order 
   * to eat, a philosopher must be holding their right and left chopstick. The 
   * naive solution to this problem creates a deadlock. You must find a 
   * better algorithm. The ideal algorithm will still allow concurrent 
   * eating without creating deadlocks.
   *
   * Note that philosophers must return chopsticks between calls to `eat`.
   */
  while (philosopher->food_consumed < HUNGER) {
    get_chopstick(philosopher->left_chopstick);
    get_chopstick(philosopher->right_chopstick);

    eat(philosopher);

    return_chopstick(philosopher->right_chopstick);
    return_chopstick(philosopher->left_chopstick);
  }

  return philosopher->food_consumed;
}

/*
 * Setup and test code
 */

int main() {
  pthread_t child_thread[PHILOSOPHERS];

  Philosopher philosophers[PHILOSOPHERS];
  Chopstick chopsticks[PHILOSOPHERS];

  for (unsigned long i = 0; i < PHILOSOPHERS; i++) {
    pthread_mutex_init(&chopsticks[i].mutex, 0);

    chopsticks[i].held = 0;
    chopsticks[i].uses = 0;
    chopsticks[i].times_held = 0;

    philosophers[i].id = i;
    philosophers[i].food_consumed = 0;
    philosophers[i].left_chopstick = &chopsticks[i];
    philosophers[i].right_chopstick = &chopsticks[(i + 1) % PHILOSOPHERS];

    int code;
    code = pthread_create(&child_thread[i], NULL, (void *) run_philosopher, (void *) &philosophers[i]);
    if (code) {
      fprintf(stderr, "pthread_create failed with code %d\n", code);
    }
  }

  unsigned long thread_consumed;
  unsigned long reported_total_consumed = 0;

  for (unsigned int i = 0; i < PHILOSOPHERS; i++) {
    pthread_join(child_thread[i], (void *) &thread_consumed);
    assert(thread_consumed == HUNGER);
    assert(philosophers[i].food_consumed == HUNGER);
    reported_total_consumed += thread_consumed;
  }

  printf("%ld total food consumed.\n", reported_total_consumed);
  assert(reported_total_consumed == HUNGER * PHILOSOPHERS);

  unsigned long total_chopstick_uses = 0;
  unsigned long total_chopstick_holds = 0;

  for (unsigned int i = 0; i < PHILOSOPHERS; i++) {
    total_chopstick_uses += chopsticks[i].uses;
    total_chopstick_holds += chopsticks[i].times_held;
  }

  printf("%ld total chopstick uses.\n", total_chopstick_uses);
  assert(total_chopstick_uses == HUNGER * PHILOSOPHERS * 2);
  assert(total_chopstick_holds == HUNGER * PHILOSOPHERS * 2);

  printf("All tests passed.\n");

  return 0;
}
