#ifndef LAMBDA_H
#define LAMBDA_H

#include <stdlib.h>
#include <stdio.h>

char* get_unique_name();


typedef struct {
    void* handle; // Handle to the dynamically-loaded library
    void* func;   // Generic pointer to any function
} LambdaFunction;
LambdaFunction load_lambda_function(const char* source_file, const char* define_string);

//specific implementation	
typedef struct {
    void* handle;              // Handle to the dynamically-loaded library
    int (*func)();             // Corrected function pointer type
} Lambda_Example_Function;
Lambda_Example_Function make_example_lambda(int id);

#endif //LAMBDA_H