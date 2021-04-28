#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gsl_rng.h"
#define NUM 1000

gsl_rng *tau;

void captarImagen(int imagen[], int n);
void inicializarSpines(int spines[], int n);
void calculoPesos(int imagen[], int n, double *a, double w[][NUM]);
void montecarlo(int spines[], int n, double w[][NUM]);
double min(double a, double b);

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
    
    captarImagen(imagen, n);
    inicializarSpines(spines, n);
    calculoPesos(imagen,n,&a,w);
    
    for(;;) montecarlo(spines,n,w);
    return 0;
    
}

void captarImagen(int imagen[], int n){
    for(int i = 0;i<n/2;i++) imagen[i]=1;
    for(int i=n/2;i<n;i++) imagen[i]=0;
}

void inicializarSpines(int spines[], int n){
    extern gsl_rng *tau;
    double al;
    
    for(int i=0;i<n;i++){
        al = gsl_rng_uniform(tau);
        if(al<0.5){ spines[i] = 0;
        }else spines[i] = 1;
    }
}

void calculoPesos(int imagen[], int n, double *a, double w[][NUM]){
    //Calculo de a
    double aa;
    aa = 0;
    
    for(int i=0;i<n;i++) aa+=imagen[i];
    aa = aa/(1.*n);
    
    //Calculo de w
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){ w[i][j] =0;
            }else w[i][j] = (imagen[i]-aa)*(imagen[j]-aa)/(1.*aa*(1-aa)*n);
        }
    }

    *a = aa;
}

void montecarlo(int spines[], int n, double w[][NUM]){
    extern gsl_rng *tau;
    int i, j;
    double en, aux, expo, prob;
    
    i = gsl_rng_uniform_int(tau,NUM);
    prob = gsl_rng_uniform(tau);
    
    aux = 0;
    for(j=0;j<n;j++)
        aux += w[i][j]*(1-spines[j]); 
    
    en = aux*(1-2*spines[i])/2.;
    expo = exp(-1*en/0.1);
    
    if(prob<min(1,expo)) spines[i] = (1-spines[i]);
}

double min(double a, double b){
    if(a<b){ return a;
    }else return b;
}
