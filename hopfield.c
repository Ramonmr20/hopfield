#include <stdio.h>
#include <math.h>
#include <time.h>
#include "gsl_rng.h"

#define NUM 10000 //Número de neuronas
#define NIM 1 //Número de imágenes recordadas

gsl_rng *tau; //Variable para los num aleatorios

void iniImagen(int *imagen[]);
void iniSpinesAl(int *spines);
double calculoW(int *imagen[], double *w[]);

int main(){
    
    //Inicializar num aleatorios
    extern gsl_rng *tau;
    int semilla = 3;//time(NULL);
    
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semilla);
    
    //Declaración variables con memoria dinámica
    int *imagen[NIM], *spines;
    double *w[NUM];
    int i,j; //Variables tonta
    
    for(i=0;i<NIM;i++) imagen[i] = (int*)malloc(NUM*sizeof(int)); //En imagen [a][b] a es la imagen y b la neuronas
    for(i=0;i<NUM;i++){
        spines = (int*)malloc(NUM*sizeof(int));
        
        w[i] = (double*)malloc(NUM*sizeof(double));
    }
    
    //Declaración otras variables
    double a;
    FILE *f1;
    
    f1 = fopen("imagen.txt","w");
    
    iniImagen(imagen);
    iniSpinesAl(spines);
    
    a = calculoW(imagen, w);
    
    //for(i=0;i<NUM;i++) fprintf(f1,"%i\t",imagen[0][i]);
    for(i=0;i<NUM;i++) for(j=0;j<NUM;j++) printf("%i,j::%lf\n",i,j,w[i][j]);
    
    fclose(f1);
}

void iniImagen(int *imagen[]){
    for(int i=0;i<NUM;i++){
        if(i%2==0){
            imagen[0][i] = 0;
        }else imagen[0][i] = 1;
    }
}

void iniSpinesAl(int *spines){
    extern gsl_rng *tau;
    
    for(int i=0;i<NUM;i++){
        if(gsl_rng_uniform(tau)<=0.5){
            spines[i] = 0;
        }else spines[i] = 1;
    }
}

double calculoW(int *imagen[], double *w[]){
    double a, aux;
    
    //Calculo de a
    a = 0;
    
    for(int i=0; i<NIM; i++){
        for(int j=0; j<NUM;j++) a += imagen[i][j];
    }
    
    a = a/(1.*NUM*NIM);
    
    //Calculo pesos
    for(int i=0; i<NUM; i++){
        for(int j=0; j<NUM; j++){
            if(i==j){ w[i][i] = 0;
            }else{
                aux = 0;
                for(int k=0; k<NIM; k++) aux += (imagen[k][i]-a)*(imagen[k][j]-a);
                
                w[i][j] = aux/(1.*a*(1-a)*NUM);
            }
        }
    }
    
    return a;
}
