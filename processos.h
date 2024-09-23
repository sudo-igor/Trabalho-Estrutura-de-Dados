#ifndef PROCESSOS_H
#define PROCESSOS_H

typedef struct {
	int id;
	char numero[21];
	char data_ajuizamento[24];
	char classe[100];
	char assunto[100];
	int ano_eleicao;
} Processo;

void extrairCampoComAspas(char *buffer, char *campo);

Processo strToProcesso(const char *linha) ;

Processo* lerArquivo(const char *nomeArquivo, int *quantidade);

void ordenarCrescentePorId(Processo processos[], int quantidade);

void ordenarDecrescentePorData(Processo processos[], int quantidade);

void printProcesso(const Processo *processo);

char* formatarCampoComVirgula(const char *texto);

char* formatarCampoComVirgula(const char *texto);

void escreverArquivo(const Processo processos[], int numProcessos, const char *nomeArquivo);

int contemClasse(const char *classeStr, const char *classeNum);

int contarProcessosPorClasse(Processo *processos, int numProcessos, const char *classeNum);

int jaExiste(int *assuntosUnicos, int tamanho, int assunto);

int contarAssuntosDistintos(Processo *processos, int numProcessos);

int diasDecorridos(const char *dataStr);

Processo encontrarProcessoPorId(Processo *processos, int numProcessos, int idProcurado);

#endif // PROCESSO_H
