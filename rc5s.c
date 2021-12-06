#include <stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<omp.h> 
#include<time.h>
typedef unsigned int WORD; 
#define w 32
#define r 16
#define b 16
 
#define c 4
#define t 26 
WORD S[t];
WORD P = 0xb7e15163, Q = 0x9e3779b9;
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1))))) 
void RC5_ENCRYPT(WORD *pt, WORD *ct)
{ 
WORD i, A=pt[0]+S[0], B=pt[1]+S[1];
for(i=1; i<=r; i++)
{ 
A =ROTL(A^B,B)+S[2*i]; 
B = ROTL(B^A,A)+S[2*i+1];
}
ct[0] = A; 
ct[1] = B;
}
void RC5_DECRYPT(WORD *ct, WORD *pt)
{ 
WORD i, B=ct[1], A=ct[0];
for (i=r; i>0; i--)
{ 
B = ROTR(B-S[2*i+1],A)^A;
A = ROTR(A-S[2*i],B)^B;
}
pt[1] = B-S[1]; 
pt[0] = A-S[0];
}
void RC5_SETUP(unsigned char *K)
{ 
WORD i, j, k, u=w/8, A, B, L[c];
 
for (i=b-1,L[c-1]=0; i!=-1; i--) 
L[i/u] = (L[i/u]<<8)+K[i];
for (S[0]=P,i=1; i<t; i++)
S[i] = S[i-1]+Q;
for (A=B=i=j=k=0; k<3*t; k++,i=(i+1)%t,j=(j+1)%c)
{ 
A = S[i] = ROTL(S[i]+(A+B),3);
B = L[j] = ROTL(L[j]+(A+B),(A+B));
}
}
void main()
{

clock_t begin = clock();
double start; double end;
WORD i,j,pt1[2],pt2[2], ct[2]={0,0};
unsigned char key[b]; 
if (sizeof(WORD)!=4)
printf("RC5 error: WORD has %ld bytes.\n",sizeof(WORD)); 
printf("RC5 serial encryption and decryption:\n"); 
for(i=0;i<3;i++)
{
if(i==0)
{ 
pt1[0]="Hello";
pt1[1]="Bipasa";
} 
if(i==1)
{ 
pt1[0]="Hello";
pt1[1]="Biswarup";
} 
if(i==2)
{ 
pt1[0]="Hello";
pt1[1]="Mukherjee";
} 
for(j=0;j<b;j++)
key[j]=ct[0]%(255-j);
//User secret key is zero. 
RC5_SETUP(key);
RC5_ENCRYPT(pt1,ct); 
RC5_DECRYPT(ct,pt2);
 
printf("\n key = "); 
for(j=0;j<b;j++)

printf("%.2X ",key[j]);
printf("\nEncryption\n plaintext %d %d ---> ciphertext %.8X %.8X \ n",pt1[0],pt1[1],ct[0],ct[1]);
printf("\nDecryption\n ciphertext %.8X %.8X---> plaintext %d %d \ n",ct[0],ct[1],pt2[0],pt2[1]);
if(pt1[0]!=pt2[0] || pt1[1]!=pt2[1]) 
printf("Decryption Error!");

clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
printf("\nTime Taken:%f\n",time_spent);
}
}










