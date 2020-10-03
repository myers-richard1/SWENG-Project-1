 #include <stdio.h>
#include <unistd.h>
int main(){
	int i,j,result;
	FILE *fp;
	fp = fopen("samplejob_output.txt", "w");
	fprintf(fp, "\n");
	for (i = 1; i < 100; i++){
		for(j = 1; j < 100; j++){
			result = i * j;
			fprintf(fp, "%d*%d=%-3d",i,j,result);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
    sleep(5000);
}