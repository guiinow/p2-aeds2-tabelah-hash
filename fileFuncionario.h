#ifndef FILEFUNCIONARIO_H_INCLUDED
#define FILEFUNCIONARIO_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Operario{
    int id;
    char name[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} Funcionario;

struct KeyID {
    int id;
    long RRN;
};

typedef struct File{
    Funcionario *func;
    long init_p;
    long end_p;
    FILE *filePartition;
}VetorFile;

void saveDataBase(Funcionario *func, FILE *out);
Funcionario *readArq(FILE *in);
void dataBase(FILE *arq, int *vet);
Funcionario *buscaBinaria(int id, FILE *file, int sizeFile, int *totalComparacoes);
void printFuncionario (Funcionario *func);
Funcionario *buscaSequencial(int id, FILE *file, int *totalComparacoes);
void functionRandomNumbers(int *vet, int vetSize);
Funcionario *insertionSort(FILE *file, int sizeFile);
void keySort(FILE *file, FILE *sortedFile, int sizeFile);
int sizeFile(FILE *file, int contSizeFile);
void printParticaoCodFuncionario(FILE *file, char nomeParticao[]);
int registroMemoria (int vet[6]);
void mergeSort(int numeroDeParticoes, char nameFilePartition[]);
int selecaoSubstituicao (FILE *file, char nameFilePartition[]);
void printAllFuncionarioFile(FILE *file);
void criaParticaoTabelaHash (int numeroDeParticoes);
void tabelaHash(FILE *file, int numeroDeParticoes, int sizeFile);
Funcionario *searchTabelaHash(int codFuncionario, int idParticao);
void insereTabelaHash(Funcionario *funcHashTableInsert,int codFuncionario, int idParticao);
void deletaTabelaHash(int codFuncionario, int idParticao);


#endif // FILEFUNCIONARIO_H_INCLUDED
