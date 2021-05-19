#include <stdio.h>
#include <math.h>
#include <time.h>
#include "gsl_rng.h"

#define NUM 10000 //Número de neuronas
#define NIM 1 //Número de imágenes recordadas
#define T 0.01

gsl_rng *tau; //Variable para los num aleatorios

void iniImagen(int *imagen[]); 
void iniSpinesAl(int *spines);
double calculoW(int *imagen[], double *w[]);
double difEnergia(int *spines, double *w[], int i);
void paso(int *spines, double *w[]);
double min(double a, double b);

int main(){
    
    //Inicializar num aleatorios
    extern gsl_rng *tau;
    int semilla = 3;//time(NULL);
    
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semilla);
    
    //Declaración variables con memoria dinámica
    int *imagen[NIM], *spines;
    double *w[NUM];
    int i,j; //Variables tontas
    
    for(i=0;i<NIM;i++) imagen[i] = (int*)malloc(NUM*sizeof(int)); //En imagen [a][b] a es la imagen y b la neuronas
    for(i=0;i<NUM;i++){
        spines = (int*)malloc(NUM*sizeof(int));
        
        w[i] = (double*)malloc(NUM*sizeof(double));
    }
    
    //Declaración otras variables
    double a;
    FILE *f1, *f2, *f3;
    
    f1 = fopen("imagen.txt","w");
    f2 = fopen("sistema.txt","w");
    
    printf("kpasa\n");
    
    iniImagen(imagen); //Inicializar imagen en memoria
    iniSpinesAl(spines); //Dar configuración inicial spines
    
    a = calculoW(imagen, w); //Inicializar a y w[][]
    printf("%lf\n",a);
    
    //Output config inicial y spines en 0
    for(i=0;i<NUM;i++) fprintf(f1,"%i\t",imagen[0][i]);
    for(j=0;j<NUM;j++) fprintf(f2,"%i\t",spines[j]);
    fprintf(f2,"\n");
    
    for(i=0;i<100;i++){ 
        
        //Paso montecarlo
        for(j=0;j<NUM;j++) paso(spines, w);
        
        //Output resultado
        for(j=0;j<NUM;j++) fprintf(f2,"%i\t",spines[j]);
        fprintf(f2,"\n");
        printf("%i\n",i);
    }
    
    fclose(f1);
    fclose(f2);
}

void iniImagen(int *imagen[]){
    FILE *f1;
    
    f1 = fopen("1.txt","r");
    
    for(int i=0;i<NUM;i++) fscanf(f1,"%i",&imagen[0][i]);
    
    fclose(f1);
}

void iniSpinesAl(int *spines){
    FILE *f1;
    
    f1 = fopen(".imtemp.txt","r");
    
    for(int i=0;i<NUM;i++) fscanf(f1,"%i",&spines[i]);
    
    fclose(f1);
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

double difEnergia(int *spines, double *w[], int i){
    double en, aux;
    int j;
    
    aux = 0;
    for(j=0;j<NUM;j++) aux += w[i][j]*(1-spines[j]);
    
    en = (1-2*spines[i])*aux/2.;
    
    return en;
}

void paso(int *spines, double *w[]){
    extern gsl_rng *tau;
    double en, p, xi;
    int i;
    
    i = gsl_rng_uniform_int(tau,NUM);
    
    en = difEnergia(spines, w, i);
    p = min(1,exp(-en/T));
    //printf("%lf\n",p);
    
    xi = gsl_rng_uniform(tau);
    
    if(xi<p) spines[i] = 1 - spines[i];
}

double min(double a, double b){
    if(a<b){ return a;
    }else return b;
}
