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

struct thread_arguments
{
    int8_t *buffer_addr;
    uint32_t buffer_size;
};

struct thread_arguments_wtv
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
struct thread_arguments* p = (struct thread_arguments*) arguments;

TestFnc(p->buffer_addr,p->buffer_size);

return NULL;
}

/********************************************************************
 * F NAME ThreadPrgAsm1();
 *
 ********************************************************************/

void * ThreadPrgAsm1 (void *arguments)
{
struct thread_arguments* p = (struct thread_arguments*) arguments;

TestFnAsm1(p->buffer_addr,p->buffer_size);

return NULL;
}

/********************************************************************
 * F NAME ThreadPrgAsm2();
 *
 ********************************************************************/

void * ThreadPrgAsm2 (void *arguments)
{
struct thread_arguments* p = (struct thread_arguments*) arguments;

TestFnAsm2(p->buffer_addr,p->buffer_size);

return NULL;
}

/********************************************************************
 * F NAME ThreadPrgAsm3();
 *
 ********************************************************************/

void * ThreadPrgAsm3 (void *arguments)
{
struct thread_arguments_wtv* p = (struct thread_arguments_wtv*) arguments;

TestFnAsm3(p->buffer_addr,p->buffer_size,&p->tmp_var);

return NULL;
}

/********************************************************************
 * F NAME ThreadPrgAsm4();
 *
 ********************************************************************/

void * ThreadPrgAsm4 (void *arguments)
{
struct thread_arguments* p = (struct thread_arguments*) arguments;

TestFnAsm4(p->buffer_addr,p->buffer_size);

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
            break;
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

    struct thread_arguments thread_args_1;
    struct thread_arguments thread_args_2;

    struct thread_arguments_wtv thread_args_wtv_1;
    struct thread_arguments_wtv thread_args_wtv_2;

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

    printf("testing asm, without temp variable\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_1.buffer_addr = test_buffer_1;
    thread_args_1.buffer_size = SIZE_OF_BUFFER;

    thread_args_2.buffer_addr = test_buffer_2;
    thread_args_2.buffer_size = SIZE_OF_BUFFER;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgAsm1, &thread_args_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgAsm1, &thread_args_2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    TestBuffer( test_buffer_1, SIZE_OF_BUFFER,  2, "buffer 1");
    TestBuffer( test_buffer_2, SIZE_OF_BUFFER,  3, "buffer 2");

/*********************************************************************/

    printf("testing asm, with TLS (thread-local storage)\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_1.buffer_addr = test_buffer_1;
    thread_args_1.buffer_size = SIZE_OF_BUFFER;

    thread_args_2.buffer_addr = test_buffer_2;
    thread_args_2.buffer_size = SIZE_OF_BUFFER;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgAsm2, &thread_args_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgAsm2, &thread_args_2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    TestBuffer( test_buffer_1, SIZE_OF_BUFFER,  2, "buffer 1");
    TestBuffer( test_buffer_2, SIZE_OF_BUFFER,  3, "buffer 2");

/*********************************************************************/

    printf("testing asm, with temporary variable from C\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_wtv_1.buffer_addr = test_buffer_1;
    thread_args_wtv_1.buffer_size = SIZE_OF_BUFFER;
    //thread_args_wtv_1.tmp_var = 0;

    thread_args_wtv_2.buffer_addr = test_buffer_2;
    thread_args_wtv_2.buffer_size = SIZE_OF_BUFFER;
    //thread_args_wtv_2.tmp_var = 0;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgAsm3, &thread_args_wtv_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgAsm3, &thread_args_wtv_2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    TestBuffer( test_buffer_1, SIZE_OF_BUFFER,  2, "buffer 1");
    TestBuffer( test_buffer_2, SIZE_OF_BUFFER,  3, "buffer 2");

/*********************************************************************/

    printf("testing asm, with stack\n");

    memset(test_buffer_1, 1, SIZE_OF_BUFFER);
    memset(test_buffer_2, 2, SIZE_OF_BUFFER);

    thread_args_1.buffer_addr = test_buffer_1;
    thread_args_1.buffer_size = SIZE_OF_BUFFER;

    thread_args_2.buffer_addr = test_buffer_2;
    thread_args_2.buffer_size = SIZE_OF_BUFFER;

    pthread_create ( &thread_id_1, NULL, &ThreadPrgAsm4, &thread_args_1);
    pthread_create ( &thread_id_2, NULL, &ThreadPrgAsm4, &thread_args_2);

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
