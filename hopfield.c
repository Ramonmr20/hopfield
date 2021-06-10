#include <stdio.h>
#include <math.h>
#include <time.h>
#include "gsl_rng.h"

#define NUM 10000 //Número de neuronas
#define NIM 2 //Número de imágenes recordadas
#define T 0.0001

gsl_rng *tau; //Variable para los num aleatorios

void iniImagen(int *imagen[]); 
void iniSpinesAl(int *spines);
void calculoW(int *imagen[], double *w[], double a[]);
double difEnergia(int *spines, double *w[], int i);
void paso(int *spines, double *w[]);
double min(double a, double b);
double solapamiento(int *spines, int *imagen[], double a[], int mu);

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
    double a[NIM];
    FILE *f1, *f2, *f3;
    
    f1 = fopen("imagen.txt","w");
    f2 = fopen("sistema.txt","w");
    
    iniImagen(imagen); //Inicializar imagen en memoria
    iniSpinesAl(spines); //Dar configuración inicial spines
    
    calculoW(imagen, w, a); //Inicializar a y w[][]
    printf("%lf\n",a);
    
    //Output config inicial y spines en 0
    for(i=0;i<NUM;i++) fprintf(f1,"%i\t",imagen[1][i]);
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
    
    f1 = fopen("8.txt","r");
    
    for(int i=0;i<NUM;i++) fscanf(f1,"%i",&imagen[1][i]);
    
    fclose(f1);
}

void iniSpinesAl(int *spines){
    FILE *f1;
    
    f1 = fopen(".imtemp.txt","r");
    
    for(int i=0;i<NUM;i++) fscanf(f1,"%i",&spines[i]);
    
    fclose(f1);
}

void calculoW(int *imagen[], double *w[], double a[]){
    double aux;
    
    //Calculo de a
    for(int i=0;i<NIM;i++){
        a[i] = 0;
        for(int j=0;j<NUM;j++){
            a[i] += imagen[i][j];
        }
        a[i] = a[i]/(1.*NUM);
    }
    
    
    //Calculo pesos
    for(int i=0; i<NUM; i++){
        for(int j=0; j<NUM; j++){
            if(i==j){ w[i][i] = 0;
            }else{
                aux = 0;
                for(int k=0; k<NIM; k++) aux += (imagen[k][i]-a[k])*(imagen[k][j]-a[k]);
                
                w[i][j] = aux/(1.*NUM);
            }
        }
    }
}

double difEnergia(int *spines, double *w[], int i){
    double en, aux;
    int j;
    
    aux = 0;
    for(j=0;j<NUM;j++) aux += w[i][j]*(1/2.-spines[j]);
    
    en = (1-2*spines[i])*aux;
    
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

double solapamiento(int *spines, int *imagen[], double a[], int mu){
    double m, sum;
    
    sum = 0;
    for(int i=0; i<NUM; i++){
        sum += (imagen[mu][i]-a[mu])*(spines[i]-a[mu]);
    }
    
    m = sum/(1.*NUM*a[mu]*(1-a[mu]));
    
    return m;
}
