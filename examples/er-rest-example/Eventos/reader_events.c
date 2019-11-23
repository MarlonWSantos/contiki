


































#include <stdio.h>
#include "reader_events.h"

void reader_events(){

 FILE *arquivo;
   char linha;
   arquivo = fopen("/home/marlon/Github/contiki/examples/er-rest-example/Eventos/Eventos.txt","r");   /* Arquivo ASCII, para leitura */
   if(!arquivo)
    {
     printf( "Erro na abertura do arquivo");
     exit(0);
    }
   while((linha = getc(arquivo) ) != EOF){      /* Enquanto não chegar ao final do arquivo */
    printf("%c\n",linha);                 /* imprime o caracter lido */
   fclose(arquivo);
	}
};
