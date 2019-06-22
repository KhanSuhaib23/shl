#define SHL_THREAD_IMPLEMENTATION
#include "shl_thread.h"


int main(int argc, char** argv) {

    int initial_count = 0;
    int maximum_count = 5;

    shl_semaphore semaphore;

    shl_create_semaphore(&semaphore, 0, 5);

}

