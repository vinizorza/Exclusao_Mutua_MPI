#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define LIDER 0

//Tipos de mensagens
#define SOLICITA 100
#define OK       200
#define AGUARDE  300
#define LIBERA   400


struct Node{
	int num;
	struct Node *prox;
}; 
typedef struct Node node;

int tam;

void inicia(node *LISTA);
node *criaNo();
void insereFim(node *LISTA);
void insereInicio(node *LISTA);
void exibe(node *LISTA);
void libera(node *LISTA);
void insere (node *LISTA);
node *retiraInicio(node *LISTA);
node *retiraFim(node *LISTA);
node *retira(node *LISTA);





int main(int argc, char *argv[])
{
    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    
    if(rank == LIDER){
        
        int dado;
        MPI_Status* status;
        
        while(1){
			
			node *LISTA = (node *) malloc(sizeof(node));
			if(!LISTA){
				printf("Sem memoria disponivel!\n");
			exit(1);
			}else{
				inicia(LISTA);
			
			
            
            MPI_Recv(&dado, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            if(status.MPI_TAG == LIBERA){//Receber a mensagem de liberação do processo.
            
                //Enviar OK para o primeiro processo da fila
                MPI_Send(&dado, 1, MPI_INT, fila.get(0), OK, MPI_COMM_WORLD);
                
                //Remover o processo da fila
                retiraInicio(LISTA);
                
            }else if(status.MPI_TAG == SOLICITA){//Parte para receber a solicatação de uso do recurso.
            
                if(fila.size() == 0){//Se fila vazia
                
                    MPI_Send(&dado, 1, MPI_INT, status.MPI_SOURCE, OK, MPI_COMM_WORLD);
                    
                }else{
                    
                    //Adicionar no final da fila
                    insereInicio(LISTA);
                
                    //Mandar AGUARDE para o processo
                    MPI_Send(&dado, 1, MPI_INT, status.MPI_SOURCE, AGUARDE, MPI_COMM_WORLD);
                    
                }
            
                
                
            }

        }
        
	}else{//Se for os usuários do recursos
	    
	    int dado;
	    MPI_Status* status;
	    
	    while(1){
	        
	        MPI_Send(&dado, 1, MPI_INT, LIDER, SOLICITA, MPI_COMM_WORLD);
	        MPI_Recv(&dado, 1, MPI_INT, LIDER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	        
	        if(status.MPI_TAG == OK){
	            
	            //Usar recurso
	            printf("Processo [%d] usando o recurso",rank);
	            sleep(10000);
	            
	            
	            //Liberar recurso
	            MPI_Send(&dado, 1, MPI_INT, LIDER, LIBERA, MPI_COMM_WORLD);
	            
	        }else if(status.MPI_TAG == AGUARDE){
	            
	            //Aguardar receber mensagem de OK
	            MPI_Recv(&dado, 1, MPI_INT, LIDER, OK, MPI_COMM_WORLD, &status);
	            
	            //Usar recurso
	            printf("Processo [%d] usando o recurso",rank);
	            sleep(10000);
	            
	        }
	    }
	}
	

    MPI_Finalize();
    return(0);
    }
    
    
    
    int vazia(node *LISTA)
{
	if(LISTA->prox == NULL)
		return 1;
	else
		return 0;
}

node *aloca()
{
	node *novo=(node *) malloc(sizeof(node));
	if(!novo){
		printf("Sem memoria disponivel!\n");
		exit(1);
	}else{
		printf("Novo elemento: "); scanf("%d", &novo->num);
		return novo;
	}
}


void insereFim(node *LISTA)
{
	node *novo=aloca();
	novo->prox = NULL;
	
	if(vazia(LISTA))
		LISTA->prox=novo;
	else{
		node *tmp = LISTA->prox;
		
		while(tmp->prox != NULL)
			tmp = tmp->prox;
		
		tmp->prox = novo;
	}
	tam++;
}

void insereInicio(node *LISTA)
{
	node *novo=aloca();	
	node *oldHead = LISTA->prox;
	
	LISTA->prox = novo;
	novo->prox = oldHead;
	
	tam++;
}

void exibe(node *LISTA)
{
	system("clear");
	if(vazia(LISTA)){
		printf("Lista vazia!\n\n");
		return ;
	}
	
	node *tmp;
	tmp = LISTA->prox;
	printf("Lista:");
	while( tmp != NULL){
		printf("%5d", tmp->num);
		tmp = tmp->prox;
	}
	printf("\n        ");
	int count;
	for(count=0 ; count < tam ; count++)
		printf("  ^  ");
	printf("\nOrdem:");
	for(count=0 ; count < tam ; count++)
		printf("%5d", count+1);
	
		
	printf("\n\n");
}

void libera(node *LISTA)
{
	if(!vazia(LISTA)){
		node *proxNode,
			  *atual;
		
		atual = LISTA->prox;
		while(atual != NULL){
			proxNode = atual->prox;
			free(atual);
			atual = proxNode;
		}
	}
}

void insere(node *LISTA)
{
	int pos,
		count;
	printf("Em que posicao, [de 1 ate %d] voce deseja inserir: ", tam);
	scanf("%d", &pos);
	
	if(pos>0 && pos <= tam){
		if(pos==1)
			insereInicio(LISTA);
		else{
			node *atual = LISTA->prox,
				 *anterior=LISTA; 
			node *novo=aloca();
				 
			for(count=1 ; count < pos ; count++){
					anterior=atual;
					atual=atual->prox;
			}
			anterior->prox=novo;
			novo->prox = atual;
			tam++;
		}
			
	}else
		printf("Elemento invalido\n\n");		
}

void *retiraInicio(node *LISTA)
{
	if(LISTA->prox == NULL){
		printf("Lista ja esta vazia\n");
		//return NULL;
	}else{
		node *tmp = LISTA->prox;
		LISTA->prox = tmp->prox;
		tam--;
		//return tmp;
	}
				
}

node *retiraFim(node *LISTA)
{
	if(LISTA->prox == NULL){
		printf("Lista ja vazia\n\n");
		return NULL;
	}else{
		node *ultimo = LISTA->prox,
			 *penultimo = LISTA;
			 
		while(ultimo->prox != NULL){
			penultimo = ultimo;
			ultimo = ultimo->prox;
		}
			 
		penultimo->prox = NULL;
		tam--;
		return ultimo;		
	}
}

node *retira(node *LISTA)
{
	int opt,
		count;
	printf("Que posicao, [de 1 ate %d] voce deseja retirar: ", tam);
	scanf("%d", &opt);
	
	if(opt>0 && opt <= tam){
		if(opt==1)
			return retiraInicio(LISTA);
		else{
			node *atual = LISTA->prox,
				 *anterior=LISTA; 
				 
			for(count=1 ; count < opt ; count++){
				anterior=atual;
				atual=atual->prox;
			}
			
		anterior->prox=atual->prox;
		tam--;
		return atual;
		}
			
	}else{
		printf("Elemento invalido\n\n");
		return NULL;
	}
}



