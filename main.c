/*********************************************************************
 * FILE NAME main.c                                                  *
 *                                                                   *
 * PURPOSE test asm threads                                          *
 *                                                                   *
 *********************************************************************/

/*********************************************************************
 * Includes
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "threadf.h"

/*********************************************************************
 * Macro Definitions
 *********************************************************************/

#define SIZE_OF_BUFFER 100000

/*********************************************************************
 * Global Variables
 *********************************************************************/

struct thread_arguments_c
{
    int8_t *buffer_addr;
    uint32_t buffer_size;
};

struct thread_arguments_a
{
    int8_t *buffer_addr;
    uint32_t buffer_size;
    int8_t tmp_var;
};

/********************************************************************
 * F NAME TestFnc();
 *
 ********************************************************************/

void TestFnc (int8_t *buffer_addr, uint32_t buffer_size)
{

    uint8_t tmp;

    for ( uint32_t i = 0; i < buffer_size; ++i )
    {
        tmp = buffer_addr[i];
        tmp += 1;
        buffer_addr[i] = tmp;
    }

}

/********************************************************************
 * F NAME ThreadPrgC();
 *
 ********************************************************************/

void * ThreadPrgC (void *arguments)
{
struct thread_arguments_c* p = (struct thread_arguments_c*) arguments;

TestFnc(p->buffer_addr,p->buffer_size);

return NULL;
}

/********************************************************************
 * F NAME ThreadPrgAsm();
 *
 ********************************************************************/

void * ThreadPrgAsm (void *arguments)
{
struct thread_arguments_a* p = (struct thread_arguments_a*) arguments;

TestFnAsm(p->buffer_addr,p->buffer_size,&p->tmp_var);

return NULL;
}

/********************************************************************
 * F NAME TestBuffer();
 *
 ********************************************************************/

void TestBuffer(int8_t *buffer_addr, uint32_t buffer_size,
                    int8_t value, char *str)
{
    bool test_flag = false;

    for ( uint32_t i = 0; i < buffer_size; ++i )
    {
        if ( buffer_addr[i] != value )
        {
            test_flag = true;
        }
    }

    if (test_flag)
    {
        printf("%s, do not match.\n", str);
    }
    else
    {
        printf("%s, match.\n", str);
    }
}

/********************************************************************
 * F NAME main
 *
 ********************************************************************/

int main()
{

    struct thread_arguments_c thread_args_1;
    struct thread_arguments_c thread_args_2;

    struct thread_arguments_a thread_args_a_1;
    struct thread_arguments_a thread_args_a_2;

    pthread_t thread_id_1;
    pthread_t thread_id_2;

    int8_t *test_buffer_1;
    int8_t *test_buffer_2;

    printf("Starting program.\n");

    test_buffer_1 = malloc(SIZE_OF_BUFFER);

    if(!test_buffer_1)
    {
        printf("ERROR: cannot allocate memory. (1)\n");
        return 1;
    }

    test_buffer_2 = malloc(SIZE_OF_BUFFER);

    if(!test_buffer_2)
    {
        free(test_buffer_1);
        printf("ERROR: cannot allocate memory. (2)\n");
        return 1;
    }

/*********************************************************************/

    printf("testing c\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_1.buffer_addr = test_buffer_1;
    thread_args_1.buffer_size = SIZE_OF_BUFFER;

    thread_args_2.buffer_addr = test_buffer_2;
    thread_args_2.buffer_size = SIZE_OF_BUFFER;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgC, &thread_args_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgC, &thread_args_2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    TestBuffer( test_buffer_1, SIZE_OF_BUFFER,  2, "buffer 1");
    TestBuffer( test_buffer_2, SIZE_OF_BUFFER,  3, "buffer 2");

/*********************************************************************/

    printf("testing asm\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_a_1.buffer_addr = test_buffer_1;
    thread_args_a_1.buffer_size = SIZE_OF_BUFFER;
    //thread_args_a_1.tmp_var = 0;

    thread_args_a_2.buffer_addr = test_buffer_2;
    thread_args_a_2.buffer_size = SIZE_OF_BUFFER;
    //thread_args_a_2.tmp_var = 0;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgAsm, &thread_args_a_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgAsm, &thread_args_a_2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    TestBuffer( test_buffer_1, SIZE_OF_BUFFER,  2, "buffer 1");
    TestBuffer( test_buffer_2, SIZE_OF_BUFFER,  3, "buffer 2");

/*********************************************************************/

    free(test_buffer_1);
    free(test_buffer_2);

    printf("Bye bye !\n");

    return 0;
}
