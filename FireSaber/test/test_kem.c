#include "../api.h"
#include "../poly.h"
#include "../rng.h"
#include "../SABER_indcpa.h"
#include "../verify.h"
#include "../pack_unpack.h"
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>


static int test_pke_cpa(int r)
{
    //key mismatch attack on Firesaber
    // parameters: res: b'
    //          vprime: cm
    //            ss_a: message
    //	         query: count the total queries
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];	
    uint8_t ss_a[CRYPTO_BYTES];
    uint16_t res[SABER_L][SABER_N];
    int sk_hat[SABER_L][SABER_N];
    uint16_t vprime[SABER_N];
    uint16_t sksv[SABER_L][SABER_N]; //secret key of the server
    int query=0;
    unsigned char entropy_input[48];
    int i,j,k;
    srand(r);
    for (i=0; i<48; i++){
    	entropy_input[i] = rand()%256;
        //entropy_input[i] = i;
    }
    randombytes_init(entropy_input, NULL, 256);
    //Generation of secret key sk and public key pk pair
	crypto_kem_keypair(pk, sk);
    BS2POLVECq(sk, sksv); //sksv is the secret-key

    for(i=0;i<SABER_L;i++){
    	memset(ss_a,0,sizeof(ss_a));
    	ss_a[0] = 1;
    	memset(vprime,0,sizeof(vprime));
    	for(k=0;k<SABER_N;k++){
    		memset(res,0,sizeof(res));
    		if(k == 0){
    			res[i][k] = 180;
    		}
    		else{
    			res[i][SABER_N-k] = -180;
    		}
    		POLVECp2BS(ct, res);
    		vprime[0]=5;
    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
    		if(oracle(ct,sk) == 0){
    			query ++;
    			if(k == 0){
	    			res[i][k] = 300;
	    		}
	    		else{
	    			res[i][SABER_N-k] = -300;
	    		}
	    		POLVECp2BS(ct, res);
	    		vprime[0]=0;
	    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
	    		if(oracle(ct,sk) == 0){
	    			query ++; 
	    			sk_hat[i][k] = 0;
	    		}
	    		else{
	    			query ++;
	    			sk_hat[i][k] = 1;
	    		}
    		}
    		else{
    			query ++;
    			if(k == 0){
	    			res[i][k] = 200;
	    		}
	    		else{
	    			res[i][SABER_N-k] = -200;
	    		}
	    		POLVECp2BS(ct, res);
	    		vprime[0]=0;
	    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
	    		if(oracle(ct,sk) == 0){
	    			query ++;
	    			sk_hat[i][k] = -1;
	    		}
	    		else{
	    			query ++;
	    			if(k == 0){
		    			res[i][k] = 128;
		    		}
		    		else{
		    			res[i][SABER_N-k] = -128;
		    		}
		    		POLVECp2BS(ct, res);
		    		vprime[0]=0;
		    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
		    		if(oracle(ct,sk) == 0){
		    			query ++;
		    			sk_hat[i][k] = 2;
		    		}
		    		else{
		    			query ++;
		    			if(k == 0){
			    			res[i][k] = 100;
			    		}
			    		else{
			    			res[i][SABER_N-k] = -100;
			    		}
			    		POLVECp2BS(ct, res);
			    		vprime[0]=0;
			    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
			    		if(oracle(ct,sk) == 0){
			    			query ++;
			    			sk_hat[i][k] = -2;
			    		}
			    		else{
			    			query ++;
			    			if(k == 0){
				    			res[i][k] = 85;
				    		}
				    		else{
				    			res[i][SABER_N-k] = -85;
				    		}
				    		POLVECp2BS(ct, res);
				    		vprime[0]=0;
				    		POLT2BS(ct + SABER_POLYVECCOMPRESSEDBYTES, vprime);
				    		if(oracle(ct,sk) == 0){
				    			query ++;
				    			sk_hat[i][k] = 3;
				    		}
				    		else{
				    			query ++;
				    			sk_hat[i][k] = -3;
				    		}
			    		}
		    		}
	    		}
    		}
    	}
    }
   int sum = 0;
   for(i=0;i<SABER_L;i++)
   {
	for(j=0;j<SABER_N;j++)
	{
		//printf("sk[%d][%d]:%d sk_hat[%d][%d]:%d\n",i,j,sksv[i][j],i,j,sk_hat[i][j]);
        	if(sksv[i][j]!=sk_hat[i][j] && sksv[i][j]-sk_hat[i][j] != 8192)
		{
			//printf("sk[%d][%d]:%d sk_hat[%d][%d]:%d\n",i,j,sksv[i][j],i,j,sk_hat[i][j]);
			sum ++;
		}
	}
   }
   if(sum != 0)
   printf("Errors = %d \n",sum);
   return query;
}

int main(int argc, char * argv[])
{
    if(argc == 1) {
        printf("need a number for random\n");
        return 0;
    }
    //get the seed
    int rand = atoi(argv[1]);

    int count,query=0;
    for(count=0;count<rand;count++)
	{ 
		int temp = test_pke_cpa(count);
		printf("Fact query  = %d\n", temp);
		query += temp;
	}
    printf("Query = %f\n",query*1.0/rand);
    return 0;
} 


