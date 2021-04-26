#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gsl_rng.h"
#define NUM 1000

gsl_rng *tau;

void captarImagen(int spines[],int imagen[], int n);
void calculoPesos(int imagen[], int n, double a, double w[][NUM]);
void montecarlo(int spines[], int n, double w[][NUM]);

int main(){
        
    //Inicializar num aleatorios
    extern gsl_rng *tau;
    int semilla = time(NULL);
    
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semilla);
    
    //Programa
    int spines[NUM], imagen[NUM];
    int n;
    double a;
    double w[NUM][NUM];//Pesos
    
    n = NUM;
    
    captarImagen(spines, imagen, n);
    calculoPesos(imagen,n,a,w);
    
    for(;;) montecarlo(spines,n,w);
    return 0;
    
}

void captarImagen(int spines[],int imagen[], int n){
    for(int i = 0;i<n/2;i++) imagen[i]=1;
    for(int i=n/2;i<n;i++) imagen[i]=0;
    
    //Función mostrar
    /*for(int i=0;i<n/100;i++){
        for(int j=0;j<n/100;j++){
            printf("%i\t",imagen[i*100+j]);
        }
        printf("\n");
    }*/
}

void calculoPesos(int imagen[], int n, double a, double w[][NUM]){
    //Calculo de a
    a = 0;
    for(int i=0;i<n;i++) a+=imagen[i];
    a = a/(1.*n);
    
    for(int i=0;i<n;i++){
        for(int j=0; j<n;j++){
            if(i==j){
                   w[i][i] = 0;
            }else w[i][j] = (imagen[i]-a)*(imagen[j]-a)/(1.*a*(1-a)*n); 
        }
    }
    
}

void montecarlo(int spines[], int n, double w[][NUM]){
    extern gsl_rng *tau;
    int i, j;
    double en, aux,expo;
    
    i = gsl_rng_uniform_int(tau,NUM);
    
    aux = 0;
    for(j=0;j<n;j++)
        aux += w[i][j]*(1-spines[j]); 
    
    en = aux*(spines[i]-1/2.);
    expo = exp(-en/0.1);
    printf("%e.10\n",en);
}
