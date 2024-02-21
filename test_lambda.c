#include "lambda.h"
#include <stdio.h>
#include <dlfcn.h> // For dlclose

int main() {
    Lambda_Example_Function first = make_example_lambda(1);
    if (!first.handle || !first.func) {
        fprintf(stderr, "Failed to load or find the function in the first instance\n");
        // If handle is valid but func is not, we still need to close the handle
        if (first.handle) dlclose(first.handle);
        return 1; // Exit with an error
    }

    Lambda_Example_Function second = make_example_lambda(2);
    if (!second.handle || !second.func) {
        fprintf(stderr, "Failed to load or find the function in the second instance\n");
        if (first.handle) dlclose(first.handle); // Clean up the first before exiting
        if (second.handle) dlclose(second.handle);
        return 1; // Exit with an error
    }

    // Call the functions via their pointers
    int result1 = first.func();
    int result2 = second.func();

    printf("id=1 gives %d, id=2 gives %d\n", result1, result2);

    // Clean up: Close the handles
    if (first.handle) dlclose(first.handle);
    if (second.handle) dlclose(second.handle);

    return 0;
}
