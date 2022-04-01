#include <stdio.h>

#define N 10
#define TEST_NUM 10
void top(int a[N], int b[N], int dout[N]);
void val(int a[N], int b[N], int dout[N]){
        for(int i=0; i<N; i++){
                dout[i] = a[i] + b[i];
        }
}

bool validate(int v_res[N], int v_ref[N]){
        for(int i=0; i<N; i++){
                if(v_res[i] != v_ref[i]) return false;
        }
        return true;
}


int main()
{
        int a[N],b[N],c[N],c_ref[N];
        int error_count = 0;
        bool match;
        for (int k=0; k<TEST_NUM; k++)
        {
                //TEST_NUM个testcase
                for(int i=0; i<N; i++)
                {
                        a[i] =  i;
                        b[i] =  2*i;
                }

                val(a, b, c_ref);
                top(a, b, c);

                match = validate(c, c_ref);
                if(!match)  error_count++;
        }

        if (error_count>0)
        {
                printf("TEST FAILED with %d errors\n", error_count);
                return 1;
        }
        else
        {
                printf("TEST SUCCESS!\n");
                return 0;
        }

}
