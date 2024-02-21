#include "lambda.c"
//char* get_unique_name();
int main(){
	char* name=get_unique_name();
	if(name==NULL){
		return 1;
	}
	printf("%s\n",name);
	free(name);
	return 0;
}