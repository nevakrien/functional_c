#include "lambda.h"

#include <stdlib.h>
#include <stdio.h>	
#include <uuid/uuid.h>
#include <string.h>
#include <dlfcn.h>

char* get_unique_name() {
    // The rest of the function remains the same, just change the extension to .so
    // Calculate the total length needed: "lambda_" + 36 chars of UUID + ".so" + 1 for null terminator
    const int totalLength = strlen("lambda_") + 36 + strlen(".so") + 1;
    char* filename = malloc(totalLength);
    if (!filename) {
        fprintf(stderr, "Failed to allocate memory for filename\n");
        return NULL;
    }

    uuid_t binuuid;
    uuid_generate_random(binuuid);
    strcpy(filename, "lambda_");
    uuid_unparse_lower(binuuid, filename + strlen("lambda_"));
    strcat(filename, ".so");  // Change extension to .so

    return filename;
}

LambdaFunction load_lambda_function(const char* source_file, const char* define_string) {
    LambdaFunction lambda = {0};

    char* uniqueName = get_unique_name(); // Assume this generates "lambda_uuid.so"
    if (!uniqueName) {
        fprintf(stderr, "Failed to generate unique name\n");
        return lambda;
    }

    // Compile the shared library
    char command[1024];
    snprintf(command, sizeof(command), "gcc -shared -o %s -fPIC %s %s", uniqueName, source_file, define_string);

    if (system(command) != 0) {
        fprintf(stderr, "Failed to compile shared library\n");
        free(uniqueName);
        return lambda;
    }

    // Dynamically load the shared library
    lambda.handle = dlopen(uniqueName, RTLD_LAZY);
    if (!lambda.handle) {
        fprintf(stderr, "Failed to load the shared library: %s\n", dlerror());
    } else {
        // Since the function name is standardized, directly load 'lambda'
        lambda.func = dlsym(lambda.handle, "lambda");
        if (!lambda.func) {
            fprintf(stderr, "Failed to locate the function 'lambda': %s\n", dlerror());
            dlclose(lambda.handle);
            lambda.handle = NULL; // Mark as invalid
        }
    }

    // Clean up
    if (remove(uniqueName) != 0) {
        fprintf(stderr, "Error removing file: %s\n", uniqueName);
    }
    free(uniqueName);

    return lambda;
}

const char* exmple_function="exmple_function.c";
Lambda_Example_Function make_example_lambda(int id) {
    Lambda_Example_Function result = {0}; // Initialize to zeros/nulls

    char* uniqueName = get_unique_name(); // Generates a unique filename like "lambda_uuid.so"
    if (!uniqueName) {
        fprintf(stderr, "Failed to generate unique name\n");
        return result; // Return an empty struct with null handle
    }

    char command[1024];
    snprintf(command, sizeof(command), "gcc -shared -o %s -fPIC %s -DID=%d", uniqueName,exmple_function, id);

    // Compile the shared library
    if (system(command) != 0) {
        fprintf(stderr, "Failed to compile shared library\n");
        free(uniqueName);
        return result;
    }

    // Dynamically load the shared library
    result.handle = dlopen(uniqueName, RTLD_LAZY);
    if (!result.handle) {
        fprintf(stderr, "Failed to load the shared library: %s\n", dlerror());
    } else {
        // Assign the function pointer
        result.func = (int (*)())dlsym(result.handle, "lambda");
        if (!result.func) {
            fprintf(stderr, "Failed to locate the function: %s\n", dlerror());
            dlclose(result.handle);
            result.handle = NULL; // Mark as invalid
        }
    }

    // Delete the .so file after loading it
    if (remove(uniqueName) != 0) {
        fprintf(stderr, "Error removing file: %s\n", uniqueName);
    }

    free(uniqueName);
    return result;
}
