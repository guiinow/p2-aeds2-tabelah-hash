#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fileFuncionario.h"

#define TAM 100

int main() {
    printf("\n Comeco da execucao do Programa\n");

        FILE *file = fopen("funcionario.dat", "wb+");

        if (file != NULL) {

            int codFunc = 0, metodoOrd = 0, compSequenciais = 0, compBuscaBinaria = 0;
            int *vet;

            double timeSpent = 0.0, timeSpentBB = 0.0;

            printf("Digite o codigo do funcionario que deseja buscar: ");
            scanf("%i", &codFunc);

            vet = (int *)malloc(TAM * sizeof(int));

            functionRandomNumbers(vet, TAM);

            dataBase(file, vet);

            clock_t begin = clock();

            Funcionario *func = buscaSequencial(codFunc, file, &compSequenciais);

            clock_t end = clock();
            timeSpent += (double)(end - begin) / CLOCKS_PER_SEC;

            if (func == NULL) {
                printf("\nRegistro de funcionario nao encontrado\n");
            } else {
                printf("\nRegistro do funcionario encontrado\n");
                printFuncionario(func);
            }

            printf("\n\tMenu das ordenacoes\n 1. InsertionSort\n 2. KeySort\n 3. Selecao com Substituicao (MergeSort)\n 4. Tabela Hash.\n ");
            scanf("%i", &metodoOrd);

            if (metodoOrd == 1) {

                printf("\nOrdenando com Insertion Sort...");

                insertionSort(file, TAM);

                printf("\nChamando busca binaria...");

                clock_t beginBinary = clock();

                Funcionario *funcBinaryFetch = buscaBinaria(codFunc, file, TAM, &compBuscaBinaria);

                clock_t endBinary = clock();
                timeSpentBB += (double)(endBinary - beginBinary) / CLOCKS_PER_SEC;

                if (funcBinaryFetch == NULL) {
                    printf("\nRegistro nao encontrado!!!\n");
                } else {
                    printf("\nRegistro encontrado por busca binaria: \n");
                    printFuncionario(funcBinaryFetch);
                }

            } else if (metodoOrd == 2) {

                FILE *sortedFile = fopen("sortedRegister.dat", "wb+");

                printf("\nChamando KeySort...");

                double timeSpentKeySort = 0.0;

                clock_t beginKeySort = clock();

                keySort(file, sortedFile, TAM);

                clock_t endKeySort = clock();
                timeSpentKeySort += (double)(endKeySort - beginKeySort) / CLOCKS_PER_SEC;

                clock_t beginBinary = clock();

                Funcionario *funcBinaryFetch = buscaBinaria(codFunc, sortedFile, TAM, &compBuscaBinaria);

                clock_t endBinary = clock();
                timeSpentBB += (double)(endBinary - beginBinary) / CLOCKS_PER_SEC;

                if (funcBinaryFetch == NULL) {
                    printf("\nRegistro nao encontrado!!!\n");
                } else {
                    printf("\nRegistro encontrado por Busca Binaria no KeySort: \n");
                    printFuncionario(func);
                }

                printf("\nTempo de busca do KeySort: %f\n", timeSpentKeySort);

                fclose(sortedFile);
            } else if (metodoOrd == 3){

                int numeroDeParticoes = 0;
                numeroDeParticoes = selecaoSubstituicao(file, "substitutionSelectionPartition");
                mergeSort(numeroDeParticoes, "substitutionSelectionPartition");

                fclose(file);

                return 0;
            } else {

                int numeroDeParticoes;
                char nomeParticao[100];
                char str1[100];
                char str2[100] = ".dat";

                printf("\nInforme o numero de particoes para a Tabela Hash: ");
                scanf("%i", &numeroDeParticoes);

                criaParticaoTabelaHash(numeroDeParticoes);
                tabelaHash(file, numeroDeParticoes, TAM);

                printf("Tabela Hash criada com sucesso");

                int flagAux = 0;

                while (flagAux != 5) {

                    printf("\n\nInforme operacao com Tabela Hash: \n1 Inserir\n2 Buscar\n3 Excluir\n4 Exibir Tabela\n5 Exit  ");
                    scanf("%i", &flagAux);

                    if (flagAux == 1) {
                        int codFuncionario;
                        int idParticao;

                        printf("\nIforme o codigo do funcionario: ");
                        scanf("%i", &codFuncionario);

                        Funcionario funcHashIsert;

                        if (codFuncionario < 0)
                        {
                            printf("\nErro. Id < 0, portanto invalido");
                            break;
                        }

                        funcHashIsert.id = codFuncionario;

                        sprintf(funcHashIsert.name, "Funcionario qualquer");
                        sprintf(funcHashIsert.cpf, "000.000.000-00");
                        sprintf(funcHashIsert.data_nascimento, "DD/MM/AAAA");
                        funcHashIsert.salario = 1050 + rand();

                        // h(x) = x % a, a valor informado
                        idParticao = codFuncionario % numeroDeParticoes;

                        insereTabelaHash(&funcHashIsert, codFuncionario, idParticao);


                    } else if (flagAux == 2) {

                        int codFuncionario;
                        int idParticao;

                        printf("\nID do Funcionario: ");
                        scanf("%i", &codFuncionario);

                        idParticao = codFuncionario % numeroDeParticoes;

                        Funcionario *funcSearchHashTable = searchTabelaHash(codFuncionario, idParticao);

                        if (funcSearchHashTable != NULL) {
                            printFuncionario(funcSearchHashTable);
                        } else {
                            printf("\nFuncionario nao encontrado");
                        }

                    } else if (flagAux == 3) {

                        int codFuncionario;
                        int idParticao;

                        printf("\nInforme o ID do funcionario: ");
                        scanf("%i", &codFuncionario);

                        idParticao = codFuncionario % numeroDeParticoes;

                        deletaTabelaHash(codFuncionario, idParticao);

                    } else if (flagAux == 4) {

                        for (int i = 0; i < numeroDeParticoes; ++i) {

                            itoa(i, str1, 10);
                            strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
                            strcat(strcpy(nomeParticao, nomeParticao), str2);

                            FILE *filePartition = fopen(nomeParticao,"rb+");

                            printParticaoCodFuncionario(filePartition, nomeParticao);

                            fclose(filePartition);
                        }

                    } else {
                        printf("Fim");
                    }
                }

                fclose(file);
                return 0;
            }

            printf("\nTempo da busca sequencial %f\n", timeSpent);
            printf("Quantidade de comparacoes para a busca sequncial: %i.\n", compSequenciais);

            printf("\nTempo da busca binaria %f\n", timeSpentBB);
            printf("Quantidade de comparacoes para a busca binaria: %i.\n", compBuscaBinaria);

            free(func);
            free(vet);
            fclose(file);

        } else {
            printf("Falha no arquivo\n");
            return 1;
        }
    return 0;
}
