#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "processos.h"

int main() {
	setlocale(LC_ALL, "portuguese");
	
	int numProcessos = 0;
	   
//	Processo *processos = lerArquivo("processo_Copia.csv", &numProcessos);
	Processo *processos = lerArquivo("processo_043_202409032338.csv", &numProcessos);
	   
	if (processos == NULL) {
		printf("Erro ao ler os processos.\n");
	    return 1;
	}
	
	if (numProcessos > 0) {
		printf("\nArquivo lido com sucesso!\nTotal de processos: %d \n\n", numProcessos);
	} else {
		printf("Nenhum processo encontrado.\n");
	}

	ordenarCrescentePorId(processos, numProcessos);
	escreverArquivo(processos, numProcessos, "processosOrdenadosPorIdCrescente.csv");
	printf("Processos ordenados por ID salvos em \"processosOrdenadosPorIdCrescente.csv\"\n");
	
	ordenarDecrescentePorData(processos, numProcessos);
	escreverArquivo(processos, numProcessos, "processosOrdenadosPorDataDecrescente.csv");
	printf("Processos ordenados por data salvos em \"processosOrdenadosPorDataDecrescente.csv\"\n\n");
	
	char numClasse[20];
	printf("Informe o id_classe para contar processos: ");
	scanf("%s", numClasse);
	
	int qtdProcessosPorClasse = contarProcessosPorClasse(processos, numProcessos, numClasse);
	printf("Processos por Classe - %s: %d\n", numClasse, qtdProcessosPorClasse);
	printf("Quantidade de assuntos distintos: %d\n\n", contarAssuntosDistintos(processos, numProcessos));
	
	int idProcesso;
	
	printf ("Informe o ID: ");
	scanf("%d", &idProcesso);	
	
	Processo processo = encontrarProcessoPorId(processos, numProcessos, idProcesso);
	
	if (idProcesso != 0) {
		if (processo.id  != 0) {
			const char *data = processo.data_ajuizamento;
			printf("Dias decorridos desde %s: %d dias\n", data, diasDecorridos(data));
		} else {
		   printf("Processo com ID %d não encontrado.\n", idProcesso);
		}
	}
	
	free(processos);
	return 0;
}

