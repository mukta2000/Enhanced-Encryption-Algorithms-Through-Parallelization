#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h> 
#include "aes.h"


unsigned long ToUInt(char* str)
{
    unsigned long mult = 1;
    unsigned long re = 0;
    int len = strlen(str);
    for(int i = len -1 ; i >= 0 ; i--)
    {
        re = re + ((int)str[i] -48)*mult;
        mult = mult*10;
    }
    return re;
}


int main(int argc, char** argv) {

    int size,rank=0;
clock_t begin = clock();

    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 532123;
    uint8_t data_file[10];


     //start Crypto Section



    uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 
    };

    uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
    };
    ///    

    struct AES_ctx ctx;


    //printf("this is in main\n");

    if (rank == 0) {
    printf("data rank: 0: %d\n", data);
    //convert in to string
    sprintf( data_file, "%d", data);
    //string
    printf("data rank: 0: %s\n", data_file);

    //~ key and iv
    ///

    uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 
    };

    uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
    };
    ///    
    ///Enc

    //set
    AES_init_ctx_iv(&ctx, key, iv); 

    //enc
    AES_CTR_xcrypt_buffer(&ctx, data_file, strlen(data_file));


    printf("Sending in 0 after crypt: %s\n", data_file);
    printf("strlen send: %ld\n", strlen(data_file));

    MPI_Send(data_file,10,MPI_UINT8_T,1,0,MPI_COMM_WORLD);

}else{

    MPI_Recv(data_file,10,MPI_UINT8_T,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf("Recieved in 1 before: %s\n",data_file);
    printf("strlen recv: %ld\n", strlen(data_file));



    //~ key and iv
    ///

    uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 
    };

    uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
    };
    ///    
    ///Dec
    //set
    AES_init_ctx_iv(&ctx, key, iv); 

    //dec
    AES_CTR_xcrypt_buffer(&ctx, data_file, strlen(data_file));  

    printf("DEC at Rank 1 string: %s\n", data_file);
    printf("DEC at Rank 1 int: %lu\n", ToUInt(data_file));
}


    MPI_Finalize();
clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
printf("\nTime Taken:%f\n",time_spent);
}