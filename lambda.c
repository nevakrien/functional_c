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
        result.func = (int (*)())dlsym(result.handle, "get_id");
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
