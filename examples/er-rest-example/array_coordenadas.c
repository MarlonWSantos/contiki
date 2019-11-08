#include <stdio.h>

int main(void){


		//Vetores das coordenadas x e y dos motes
	float vetor_Posicao_X[5]= {100.0,20.0,30.0,40.0,50.0};
	float vetor_Posicao_Y[5]= {50.0,40.0,30.0,20.0,10.0};

		//Exibe as coordenadas armazenadas nos vetores
	for(int i=0;i<=4;i++){
		printf("coordenada X[%d]: %3.2f \n",i,vetor_Posicao_X[i]);
		printf("coordenada Y[%d]: %3.2f \n",i, vetor_Posicao_Y[i]);
		printf("\n");
	}

	return 0;
}
