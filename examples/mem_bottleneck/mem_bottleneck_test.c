
#include "mem_bottleneck.h"
 
int main () {
  din_t A[N];
	dout_t sum;
	
	int i, retval=0;
	FILE        *fp;

  // Create input data
	for(i=0; i<N;++i) {
	  A[i]=i;
	}
	// Save the results to a file
	fp=fopen("result.dat","w");

	// Call the function
  sum= mem_bottleneck(A); 
  fprintf(fp, "%d \n", sum);
	fclose(fp);

	// Compare the results file with the golden results
	retval = system("diff --brief -w result.dat result.golden.dat");
	if (retval != 0) {
		printf("Test failed  !!!\n"); 
		retval=1;
	} else {
		printf("Test passed !\n");
  }

	// Return 0 if the test passes
  return retval;
}

