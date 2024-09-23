#include "processos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void extrairCampoComAspas(char *buffer, char *campo) {
	
    if (buffer[0] == '"') {
        sscanf(buffer, "\"%[^\"]\",%s", campo, buffer);
    } else {
        sscanf(buffer, "%[^,],%s", campo, buffer);
    }
}

Processo strToProcesso(const char *linha) {
	
    Processo processo;
    char buffer[350];
   
    sscanf(linha, "%d,\"%20[^\"]\",%23[^,],%s",
           &processo.id,
  			processo.numero,
           	processo.data_ajuizamento,
           	buffer);
           
	extrairCampoComAspas(buffer, processo.classe);
	extrairCampoComAspas(buffer, processo.assunto);
	
	sscanf(buffer, "%d", &processo.ano_eleicao);
           
    return processo;
}

Processo* lerArquivo(const char *nomeArquivo, int *quantidade) {
	
	FILE *arquivo = fopen(nomeArquivo, "r");
	
	if (arquivo == NULL) {
	printf ("Erro ao abrir o arquivo.\n");
	return NULL;
	}
	
	Processo *processos = NULL;
	*quantidade = 0;
	char linha[300];
	
	fgets(linha, sizeof(linha), arquivo); 
	
	while (fgets(linha, sizeof(linha), arquivo)) {
		processos = (Processo*) realloc(processos, (*quantidade + 1) * sizeof(Processo));
	        if (processos == NULL) {
	            printf("Erro ao alocar memória.\n");
	            fclose(arquivo);
	            return NULL;
			}

		processos[*quantidade] = strToProcesso(linha);	
		(*quantidade)++;
	}
	
	fclose (arquivo);
	return processos;
}

void ordenarCrescentePorId(Processo processos[], int quantidade) {
	
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (processos[j].id > processos[j + 1].id) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void ordenarDecrescentePorData(Processo processos[], int quantidade) {
	
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (strcmp(processos[j].data_ajuizamento, processos[j + 1].data_ajuizamento) < 0) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void printProcesso(const Processo *processo) {
	
    printf("ID: %d, Número: %s, Data: %s, Classe: %s, Assunto: %s, Ano: %d\n",
        processo->id,
        processo->numero,
        processo->data_ajuizamento,
        processo->classe,
        processo->assunto,
        processo->ano_eleicao);
}

char* formatarCampoComVirgula(const char *texto) {
	
    if (strchr(texto, ',')) {
        char *saida = (char*)malloc(strlen(texto) + 3);
        if (saida == NULL) {
            printf("Erro de alocação de memória");
            exit(1);
        }
        sprintf(saida, "\"%s\"", texto);
        return saida;
    } else {
        return strdup(texto);
    }
}

void escreverArquivo(const Processo processos[], int numProcessos, const char *nomeArquivo) {
	
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo");
        return;
    }
    
	fprintf(arquivo, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
	
    for (int i = 0; i < numProcessos; i++) {
        fprintf(arquivo, "%d,\"%s\",%s,%s,%s,%d\n",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento,
                formatarCampoComVirgula(processos[i].classe),
                formatarCampoComVirgula(processos[i].assunto),
                processos[i].ano_eleicao);
    }

    fclose(arquivo);
}

// --------------------------------------------------------------------------------------------

int contemClasse(const char *classeStr, const char *classeNum) {
	
    char buffer[100];
    strcpy(buffer, classeStr);
    char *token = strtok(buffer, "{},");
    
    while (token != NULL) {
        if (strcmp(token, classeNum) == 0) {
            return 1; 
        }
        token = strtok(NULL, "{},");
    }
    return 0; 
}

int contarProcessosPorClasse(Processo *processos, int numProcessos, const char *classeNum) {
	
    int contador = 0;
    
    for (int i = 0; i < numProcessos; i++) {
        if (contemClasse(processos[i].classe, classeNum)) {
            contador++;
        }
    }
   
    return contador;
}

// --------------------------------------------------------------------------------------------

int jaExiste(int *assuntosUnicos, int tamanho, int assunto) {
	
    for (int i = 0; i < tamanho; i++) {
        if (assuntosUnicos[i] == assunto) {
            return 1; 
        }
    }
    return 0; 
}


int contarAssuntosDistintos(Processo *processos, int numProcessos) {
	
    int assuntosUnicos[1000];  
    int quantidadeDistintos = 0; 

    for (int i = 0; i < numProcessos; i++) {
        char buffer[100];
        strcpy(buffer, processos[i].assunto);
       
        char *token = strtok(buffer, "{},");
       
        while (token != NULL) {
            int assuntoAtual = atoi(token); 
            
            if (!jaExiste(assuntosUnicos, quantidadeDistintos, assuntoAtual)) {
                assuntosUnicos[quantidadeDistintos] = assuntoAtual;
                quantidadeDistintos++;
            }
            token = strtok(NULL, "{},");
        }
    }

    return quantidadeDistintos; 
}

// --------------------------------------------------------------------------------------------
int diasDecorridos(const char *dataStr) {
    struct tm data_inicial = {0};
    time_t tempo_atual, tempo_data;
   
    sscanf(dataStr, "%d-%d-%d", &data_inicial.tm_year, &data_inicial.tm_mon, &data_inicial.tm_mday);
   
    data_inicial.tm_year -= 1900;  
    data_inicial.tm_mon -= 1;    
   
    tempo_data = mktime(&data_inicial);
    
    time(&tempo_atual);
   
    double diferenca_segundos = difftime(tempo_atual, tempo_data);
    int dias_decorridos = diferenca_segundos / (60 * 60 * 24);  
   
    return dias_decorridos;
}

Processo encontrarProcessoPorId(Processo *processos, int numProcessos, int idProcurado) {
    
    for (int i = 0; i < numProcessos; i++) {
        if (processos[i].id == idProcurado) {
            return processos[i]; 
        }
    }
    
    Processo processoNaoEncontrado;
    processoNaoEncontrado.id = 0;
   
    return processoNaoEncontrado;
}

