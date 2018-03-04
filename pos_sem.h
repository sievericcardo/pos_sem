/**
 *  pos_sem.h
 *  pos_sem
 *
 *  Created by Riccardo Sieve on 25/01/2018.
 *
 *  https://github.com/sievericcardo/pos_sem
 *
 *  Copyright © 2018 Riccardo Sieve. All rights reserved.
 *  complete license can be found in the file LICENSE.md
 */

#ifndef POS_SEM_INCLUDED
#define POS_SEM_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h> /* To use INT_MAX */

#include <sys/sem.h>

/* We define SEM_VALUE_MAX since we cannot use semaphore.h and we will use */
/* this value as a checker when creating a new semaphore */
#define SEM_VALUE_MAX INT_MAX

/* Definition of the sem_t type contained in the semaphore.h library */
// typedef volatile unsigned char atomic_t;
// typedef atomic_t sem_t;
typedef int sem_t; // TODO: implement it with correct POSIX type

int sem_init(sem_t *, int, unsigned int);
int sem_wait (sem_t *);
int sem_trywait (sem_t *);
int sem_post(sem_t *);
int sem_destroy(sem_t *);

#endif /* POS_SEM_INCLUDED */
