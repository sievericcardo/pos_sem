/**
 *  pos_sem.c
 *  pos_sem
 * 
 *  As part of the Operating System project, in which I had to use System V
 *  semaphores, this library has been created to simulate a simple POSIX semaphore
 *  system using System V. This lets the user to use POSIX function while using
 *  System V semaphores.
 *  This is good since the developer will not need many lines of code while
 *  creating or working with semaphores as he normally would.
 *  The function name are the same as POSIX to ensure maximum easiness, and
 *  the type sem_t has been defined in the header file so that the firm of the
 *  function is the same as POSIX.
 *
 *  Created by Riccardo Sieve on 25/01/2018.
 *
 *  https://github.com/sievericcardo/pos_sem
 *
 *  Copyright © 2018 Riccardo Sieve. All rights reserved.
 *  complete license can be found in the file LICENSE.md
 */

#include "pos_sem.h"

/* This function, used to handle exceptions, is not declared in the header */
/* file and appears only here. This is done to emulate private declaration */
/* of methods like in OOP programming like Java, since no one will be able */
/* to access this function. */
void exception_handling (char* message) {
    fprintf(stderr, "%s: %d\n", message, errno);
}

int sem_init (sem_t *sem, int pshared, unsigned int value) {
    
    /* We will return this value if an error is not encountered. As such, we 
     * need it to be negative so that we don't return as if it worked
     * correctly. */
    int result = -1;
    int sem_get; /* semget will be invoke, therefore no need to initialise */
    key_t key = (key_t) time(NULL); /* Long to int -> explicit conversion */
    
    if(value > SEM_VALUE_MAX) {
        char* message = "Error - value out of bound";
        exception_handling(message);
    }
    
    if((sem_get = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1
       && errno == EEXIST) { /* We check if already in use */
        sem_get = semget(key, 1, 0);
        printf("Resourse already in use\n");
        
        if(sem_get < 0){
            /*char* message = "Error - unexpected value for semget function";
            exception_handling(message);*/
            return sem_get;
        }
    }
    else {
        *sem = sem_get;

        if((result = semctl(*sem, 0, SETVAL, value)) == -1) {
            char* message = "Error - semctl did not end correctly";
            exception_handling(message);
        }
    }

    /* Casting of the integer value got from semget to our semaphore */
    // *sem = (sem_t) &sem_get;
    
    return result;
}

/** 
 *  sem_wait stops the execution of the process in which is called waiting for
 *  the lock to be available
 * 
 *  semop is used to let the process to wait (if it works without errors)
 * 
 *  sem_num indicates the position where we operate.
 * 
 *  If sem_op is a positive integer, the operation adds this value to the
 *  semaphore value (semval).
 *  If sem_op is zero, the process must have read permission on the  
 *  semaphore set.  This is a "wait-for-zero" operation.
 *  If sem_op is less than zero, the process must have alter permission
 *  on the semaphore set.  If semval is greater than or equal to the
 *  absolute value of sem_op, the operation can proceed immediately 
 *
 * Flags recognized in sem_flg are IPC_NOWAIT and SEM_UNDO.
 */
int sem_wait (sem_t *sem) {
    
    /* We need an instance of sembuf to pass to semop. We initialize also */
    /* the variable of the struct. */
    struct sembuf semaphore_buffer;

    semaphore_buffer.sem_num = 0; /* semaphore number operate */
    semaphore_buffer.sem_op = -1; /* semaphore operation */
    semaphore_buffer.sem_flg = 0; /* operation flags */

    /* semop returns 0 if successfull */
    if (semop((int) *sem, &semaphore_buffer, 1) == -1) {
        /*char* message = "Error - unexpected behaviour of semop in sem_wait";
        exception_handling(message);*/
        return -1;
    }

    return 0;
}

/**
 *  sem_trywait tries to acquire the lock from the semaphore and, if it is not
 *  available at the time make the process in which is called to keep iterating
 *  trying to find a new element in which it can acquire the lock
 */
int sem_trywait (sem_t *sem) {

    /* We work just like we did in sem_wait, this time though we flag sem_flg */
    /* as IPC_NOWAIT so that if we can't access the resource we don't wait. */
    struct sembuf semaphore_buffer;

    semaphore_buffer.sem_num = 0; /* semaphore number operate */
    semaphore_buffer.sem_op = -1; /* semaphore operation */
    semaphore_buffer.sem_flg = IPC_NOWAIT; /* operation flags */

    if(semop((int) *sem, &semaphore_buffer, 1) == -1) {
        /*char* message = "Couldn't acquire the lock in sem_trywait";
        exception_handling(message);*/
        return -1;
    }
    
    return 0;
}

/**
 *  Once every operation are done by the process, sem_post is called to release
 *  the lock so that another process can acquire it.
 *  As such we will use sem_op with positive flag to write changes and release
 *  our lock
 */
int sem_post(sem_t *sem) {
    
    /* Analog to the previous functions. This time though our sem_op is */
    /* flagged to one to write our changes since the semaphore must be */
    /* released as we completed our operations (we no longer need the lock) */
    struct sembuf semaphore_buffer;

    semaphore_buffer.sem_num = 0; /* semaphore number operate */
    semaphore_buffer.sem_op = 1; /* semaphore operation */
    semaphore_buffer.sem_flg = 0; /* operation flags */

    if(semop((int) *sem, &semaphore_buffer, 1) == -1) {
        /*char* message = "Error - unexpected behaviour of semop in sem_post";
        exception_handling(message);*/
        return -1;
    }

    return 0;
}

/**
 *  sem_destroy destroys the semaphore at the address pointed to by sem
 */
int sem_destroy(sem_t *sem) {

    int result;

    if((result = semctl((int) *sem, 0, IPC_RMID)) == -1) {
        /*char* message = "Error - couldn\'t destroy the semaphore";
        exception_handling(message);*/
        return result;
    }

    return result;
}
