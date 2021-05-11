#include <stdio.h>
#include <math.h>
#include <time.h>
#include "gsl_rng.h"

#define NUM 100 //Número de neuronas
#define NIM 1 //Número de imágenes recordadas

gsl_rng *tau; //Variable para los num aleatorios

void iniImagen(int imagen[][NIM]);
void iniSpinesAl(int spines[]);

int main(){
    
    //Inicializar num aleatorios
    extern gsl_rng *tau;
    int semilla = 3;//time(NULL);
    
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semilla);
    
    //Declaración variables
    int imagen[NUM][NIM], spines[NUM];
    double w[NUM][NUM];
    FILE *f1;
    
    //f1 = fopen("imagen.txt","w");
    
    iniImagen(imagen);
    iniSpinesAl(spines);
    
    //for(int i=0;i<NUM;i++) fprintf(f1,"%i\t",spines[i]);
    //fclose(f1);
}

void iniImagen(int imagen[][NIM]){
    for(int i=0;i<NUM;i++){
        if(i%2==0){
            imagen[i][0] = 0;
        }else imagen[i][0] = 1;
    }
}

void iniSpinesAl(int spines[]){
    extern gsl_rng *tau;
    
    for(int i=0;i<NUM;i++){
        if(gsl_rng_uniform(tau)<=0.5){
            spines[i] = 0;
        }else spines[i] = 1;
    }
}
