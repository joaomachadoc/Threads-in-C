#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

#define NUM_ARRAYS 100
#define ARRAY_SIZE 10000

int divisor;		

// PROTOTIPOS
void	TimeInit   (void);
double	TimeStart  (void);
double	TimeStop   (double);
void    BubbleSort (int n, int * vetor);

// VALOR DO OVERHEAD DA MEDICAO DE TEMPO
static double TimeOverhead=0.0;

// ESTRUTURA DE DADOS COMPARTILHADA
int work_bag[NUM_ARRAYS][ARRAY_SIZE];

// FUNCAO QUE CALCULA O OVERHEAD DA MEDICAO DE TEMPO
void TimeInit()
{
  double t;

  TimeOverhead=0.0;
  t = TimeStart();
  TimeOverhead=TimeStop(t);
}

// FUNCAO QUE CAPTURA O TEMPO INICIAL DO TRECHO A SER MEDIDO
double TimeStart()
{
  struct timeval tv;
  struct timezone tz;

  if (gettimeofday(&tv,&tz)!=0)
      exit(1);
  return tv.tv_sec + tv.tv_usec/1000000.0;
}

// FUNCAO QUE CALCULA O TEMPO GASTO NO FINAL DO TRECHO A SER MEDIDO
double TimeStop(double TimeInitial)
{
  struct timeval tv;
  struct timezone tz;
  double Time;

  if (gettimeofday(&tv,&tz)!=0)
      return(0.0);
  Time = tv.tv_sec + tv.tv_usec/1000000.0;
  return Time-TimeInitial-TimeOverhead;
}

// BUBBLE SORT
void BubbleSort(int n, int * vetor) {
  int c =0, d, troca, trocou =1;

  while (c < (n-1) & trocou ) {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1]) {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
            }
        c++;
  }
}

/*------------------------------------------------------------------------*/
void * minha_thread(void *arg)
{
	int i=0;
    //a maior posiçao do array onde a thread vai começar a trabalhar
    int inicio = *(int *)(arg);    
    inicio = inicio - 1;
    
    //a menor posiçao do array onde a thread vai trabalhar
    int final = *(int *)(arg);
    final = final - (NUM_ARRAYS / divisor);
	
	//for decrementa até a menor posiçao
    for (i = inicio; i >= final; i--) {	
		BubbleSort(ARRAY_SIZE, &work_bag[i][0]);
   }
   
}
/*------------------------------------------------------------------------*/

int main() {
    int i, j;
    double inicio, total;
  

  TimeInit();
  // INICIALIZA OS ARRAYS A SEREM ORDENADOS
  for (i=0 ; i<NUM_ARRAYS; i++)
      for (j=0 ; j<ARRAY_SIZE; j++) {
          if (i%2 == 0)
             work_bag[i][j] = ARRAY_SIZE-j;
          else
             work_bag[i][j] = j+1;
        }

  // REALIZA A ORDENACAO
  
  inicio = TimeStart(); 

/*------------------------------------------*/    
/*------------------------------------------*/    
/*------------------------------------------*/    
    if(NUM_ARRAYS % 4 == 0){
    	pthread_t t1, t2, t3 , t4;
        
		//setando variavel global para dizer quantas threads o programa vai adotar
		divisor = 4;
    	
    	//Criando as threads, passando a maior posiçao do array onde elas vao começar
		int a = (NUM_ARRAYS / divisor);
        if( pthread_create( &t1, NULL, minha_thread, (void *)(&a))){
    		fprintf(stderr,"Erro em pthread_create()\n");
			abort();
		}
		
		int b = (NUM_ARRAYS / divisor * 2);
		if( pthread_create( &t2, NULL, minha_thread, (void *)(&b))){
    		fprintf(stderr,"Erro em pthread_create()\n");
			abort();
		}
        
		int c = NUM_ARRAYS / divisor * 3;
		if( pthread_create( &t3, NULL, minha_thread, (void *)(&c))){
    		fprintf(stderr,"Erro em pthread_create()\n");
			abort();
		}
		
		int d = NUM_ARRAYS;
		if( pthread_create( &t4, NULL, minha_thread, (void *)(&d))){
    		fprintf(stderr,"Erro em pthread_create()\n");
			abort();
		}	
		
		//Esperar pelas threads
		
		if ( pthread_join ( t1, NULL ) ) {
			fprintf(stderr,"Erro em pthread_join()\n");
			abort();
		}
		if ( pthread_join( t2, NULL ) ) {
			fprintf(stderr,"Erro em pthread_join()\n");
			abort();
		}
		if ( pthread_join ( t3, NULL ) ) {
			fprintf(stderr,"Erro em pthread_join()\n");
			abort();
		}
		if ( pthread_join ( t4, NULL ) ) {
			fprintf(stderr,"Erro em pthread_join()\n");
			abort();
		}
	}else{
		if(NUM_ARRAYS % 2 == 0){
    		pthread_t t1, t2;
            divisor = 2;
    		
            int a = NUM_ARRAYS / divisor;            
            pthread_create( &t1, NULL, minha_thread, (void *)(&a));
	    	
			int b = NUM_ARRAYS;
            pthread_create( &t2, NULL, minha_thread, (void *)(&b));
	    	            
			if ( pthread_join ( t1, NULL ) ) {
				fprintf(stderr,"Erro em pthread_join()\n");
				abort();
			}
			if ( pthread_join ( t2, NULL ) ) {
				fprintf(stderr,"Erro em pthread_join()\n");
				abort();
			}	
		}else{
			printf("Numero de Arrays invalido");
		}
	}   

   
/*------------------------------------------*/    
/*------------------------------------------*/    
/*------------------------------------------*/    
/*------------------------------------------*/

total = TimeStop(inicio);

  // VERIFICA SE OS ARRAYS ESTAO ORDENADOS
  for (i=0 ; i<NUM_ARRAYS; i++)
      for (j=0 ; j<ARRAY_SIZE-1; j++)
          if (work_bag[i][j] > work_bag[i][j+1])
             return 1;

  // MOSTRA O TEMPO DE EXECUCAO
  printf("%d %d %lf\n",NUM_ARRAYS,ARRAY_SIZE,total);
  return 0;
}
