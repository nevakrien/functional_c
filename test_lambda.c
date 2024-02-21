#include "lambda.h"
#include <stdio.h>
#include <dlfcn.h> // For dlclose

int main() {
    LambdaFunction first = load_lambda_function("exmple_function.c","-DID=1");
    if (!first.handle || !first.func) {
        fprintf(stderr, "Failed to load or find the function in the first instance\n");
        // If handle is valid but func is not, we still need to close the handle
        if (first.handle) dlclose(first.handle);
        return 1; // Exit with an error
    }

    LambdaFunction second = load_lambda_function("exmple_function.c","-DID=3");
    if (!second.handle || !second.func) {
        fprintf(stderr, "Failed to load or find the function in the second instance\n");
        if (first.handle) dlclose(first.handle); // Clean up the first before exiting
        if (second.handle) dlclose(second.handle);
        return 1; // Exit with an error
    }

    // Call the functions via their pointers
    int result1 = ((int (*)())first.func)();
    int result2 = ((int (*)())second.func)();

    printf("id=1 gives %d, id=3 gives %d\n", result1, result2);

    // Clean up: Close the handles
    dlclose(first.handle);
    dlclose(second.handle);

    return 0;
}
