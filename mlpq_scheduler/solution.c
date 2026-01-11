#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void scheduler_init(scheduler_t* scheduler) {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    scheduler->queues[i].head = NULL;
    scheduler->queues[i].tail = NULL;
  }
  scheduler->ready_bitset = 0;
}

void scheduler_add_job(scheduler_t* scheduler, job_t* job) {
  assert(job->priority < MAX_PRIORITY);

  // case: queue for this priority empty
  if (scheduler->queues[job->priority].head == NULL) {
    scheduler->queues[job->priority].head = job;
    scheduler->queues[job->priority].tail = job;
    job->prev = NULL;
    job->next = NULL;
  }
  // case: queue contains jobs, add to tail
  else {
    scheduler->queues[job->priority].tail->next = job;
    job->prev = scheduler->queues[job->priority].tail;
    scheduler->queues[job->priority].tail = job;
  }

  // set ready bit
  scheduler->ready_bitset |= (1ULL << job->priority);
}

job_t* scheduler_next(scheduler_t* scheduler) {
  // NOTE: this function has to be O(1), the OS depends on it

  if (scheduler->ready_bitset == 0) return NULL;  // no jobs ready
  size_t priority = HIGHEST_PRIORITY - __builtin_clzll(scheduler->ready_bitset);
  job_queue_t* queue = &scheduler->queues[priority];

  // pop + return first job in queue, clear ready bit
  job_t* job = queue->head;
  if (job == NULL) return NULL;

  scheduler_remove_job(scheduler, job);
  return job;
}

void scheduler_remove_job(scheduler_t* scheduler, job_t* job) {
  if (job == NULL) return;
  assert(job->priority < MAX_PRIORITY);

  job_queue_t* queue = &scheduler->queues[job->priority];
  // fix links
  if (job->prev)
    job->prev->next = job->next;
  else
    queue->head = job->next;  // job was head

  if (job->next)
    job->next->prev = job->prev;
  else
    queue->tail = job->prev;  // job was tail

  job->prev = NULL;
  job->next = NULL;

  // clear ready bit if queue empty
  if (queue->head == NULL) scheduler->ready_bitset &= ~(1ULL << job->priority);
}
