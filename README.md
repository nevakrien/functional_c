# functional_c
this is made for linux only because its a proof of concept. nothing is stoping you from implementing a similar idea in windows and mac

inspired by https://github.com/tsoding/nobuild/
why not? its not practical but its fun

# build
## main test
gcc -c lambda.c -luuid
gcc test_lambda.c lambda.o -luuid

## side tests
gcc test_name.c -luuid
gcc test_specific_lambda_exmple.c lambda.o -luuid