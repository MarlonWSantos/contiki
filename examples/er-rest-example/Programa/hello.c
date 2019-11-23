






























#include <stdio.h>
#include <stdlib.h>

int busca_vetor(){

	le_arquivo();
};

void  le_arquivo(){
  // Abre um arquivo TEXTO para LEITURA
  arq = fopen("Eventos.txt", "rt");
  if (arq == NULL)  // Se houve erro na abertura
  {
     printf("Problemas na abertura do arquivo\n");
  }
  i = 1;
  while (!feof(arq))
  {
	// Lê uma linha (inclusive com o '\n')
      result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
      if (result)  // Se foi possível ler
   		imprime(i,Linha);
	   i++;
		converte(i,Linha);
  }
  fclose(arq);

};

void imprime(){
//	  printf("Linha %d : %s",i,Linha);
};

void converte(){


	fim=sizeof(Linha)/sizeof(char);
	vetor = (int *) malloc(fim*sizeof(int));
	for(mote=0;mote<fim;mote++){
	vetor[mote]=atoi(Linha);
	};
for(mote=0;mote<fim;mote++){
	if(vetor[mote] != 0){
	printf("linha %d",vetor[mote]);
	}	
};
//	printf("Linha %d \n tamanho: %d\n",i,fim);
	

//	for(mote=0;mote<fim;mote++){
//		vetor[mote]=atoi(Linha);
//	}
//	printf("Conteudo %s",vetor);
};



