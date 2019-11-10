#ifndef _ARRAY.H_
#define _array.h_
	  	
	//Vetor com os valores do motes que serão indicados, o valor "-1" equivale a null entre os motes
	int id_nodes_ev[3][3]= {{2,-1,-1}, //Motes escolhidos no evento 1 (2 e 3)
							{-1,3,4}, //Motes escolhidos no evento 2 (3 e 4)
							{2,-1,4}};//Motes escolhidos no evento 3 (2 e 4)

	//Calcula o número de motes em cada evento, dividindo o tamanho total do vetor (9) pelo número de elemtos na linha[0]
	int numero_Motes=sizeof(id_nodes_ev)/sizeof(id_nodes_ev[0]); //Resultado = 3

#endif



