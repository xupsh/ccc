
#define N 10

void top(int a[N], int b[N], int dout[N])
{
	int a_reg, b_reg;

	for(int i=0; i<N; i++)
	{
		#pragma HLS UNROLL

		a_reg  = a[i];
		b_reg  = b[i];
		dout[i] = a_reg + b_reg;
	}

}
