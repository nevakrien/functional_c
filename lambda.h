#ifndef LAMBDA_H
#define LAMBDA_H

#include <stdlib.h>
#include <stdio.h>

char* get_unique_name();
typedef struct {
    void* handle;              // Handle to the dynamically-loaded library
    int (*func)();             // Corrected function pointer type
} Lambda_Example_Function;
Lambda_Example_Function make_example_lambda(int id);

#endif //LAMBDA_H