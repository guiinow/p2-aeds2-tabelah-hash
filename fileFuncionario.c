#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fileFuncionario.h"

#define TAM 100

void saveDataBase(Funcionario *func, FILE *out){
    fwrite(&func->id, sizeof(int), 1, out);
    fwrite(func->name, sizeof(char), sizeof(func->name), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

Funcionario *readArq(FILE *in) {
    Funcionario *func = (Funcionario*) malloc (sizeof(Funcionario));

    if ( 0 >= fread(&func->id, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }

    fread(func->name, sizeof(char), sizeof(func->name), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);

    return func;
}

void dataBase(FILE *arq, int *vet) {
    printf("Fazendo acesso na base de dados");

    for(int i = 1; i <= TAM; i++) {
        Funcionario func;

        func.id = vet[i - 1];

        sprintf(func.name, "Nome do Funcionario %d", i );
        sprintf(func.cpf, "XXX.XXX.XXX-XX");
        sprintf(func.data_nascimento, "DD/MM/AAAA");
        func.salario = 2250 + rand();
        fseek(arq, (i-1)* sizeof(Funcionario), SEEK_SET);
        saveDataBase(&func, arq);
    }
}

Funcionario *buscaBinaria(int id, FILE *file, int sizeFile, int *totalComparacoes) {

    int left = 0, right = sizeFile - 1;

    rewind(file);

    while (left <= right) {
        int middle = (left+right) / 2;
        fseek(file, middle * sizeof(Funcionario), SEEK_SET);
        Funcionario *func = readArq(file);

        if (id == func -> id) {
            *totalComparacoes += 1;
            return func;
        } else if (func->id < id) {
            *totalComparacoes += 1;
            left = middle+1;
        } else {
            *totalComparacoes += 1;
            right = middle-1;
        }
    }

    return NULL;
}

void printFuncionario (Funcionario *func) {

    printf("\nID: %d\n", func->id);
    printf("Nome: %s\n", func->name);
    printf("CPF: %s\n", func->cpf);
    printf("Data de Nascimento: %s\n", func->data_nascimento);
    printf("Salario: %.2f\n", func->salario);
}

Funcionario *buscaSequencial(int id, FILE *file, int *totalComparacoes) {
    int position = 0;

    while (!feof(file)) {

        fseek(file, position * sizeof(Funcionario), SEEK_SET) ;
        Funcionario *func = readArq(file);

        position++;

        if (func == NULL) {
            *totalComparacoes += 1;
            return NULL;
        } else if (id == func->id) {
            *totalComparacoes += 1;
            return func;
        } else {
            *totalComparacoes += 1;
        }

    }

    return NULL;
}

void functionRandomNumbers(int *vet, int vetSize){

    for (int i = 0; i < vetSize; i++) {
        vet[i] = i + 1;
    }

    for (int i = 0; i < vetSize; i++)
    {
        int r = rand() % vetSize;

        int temp = vet[i];
        vet[i] = vet[r];
        vet[r] = temp;
    }

}

Funcionario *insertionSort(FILE *file, int sizeFile) {

    rewind(file);
    int i;

    for (int j = 2; j <= sizeFile; j++) {

        fseek(file, (j - 1) * sizeof(Funcionario), SEEK_SET);
        Funcionario *auxJ = readArq(file);

        i = j - 1;
        fseek(file, (i - 1) * sizeof(Funcionario), SEEK_SET);
        do{
            Funcionario *auxI = readArq(file);
            if( (auxI->id < auxJ->id)){
                break;
            }

            fseek(file, i * sizeof(Funcionario), SEEK_SET);

            saveDataBase(auxI, file);
            i = i - 1;
            fseek(file, (i - 1) * sizeof(Funcionario), SEEK_SET);

            free(auxI);

        }while ((i > 0) );

        fseek(file, (i) * sizeof(Funcionario), SEEK_SET);
        saveDataBase(auxJ, file);
        free(auxJ);
    }

    fflush(file);
}

void keySort(FILE *file, FILE *sortedFile, int sizeFile) {

    rewind(file);

    struct KeyID keys[sizeFile];
    int position = 0;

    while (position < sizeFile){

        fseek(file, position * sizeof(Funcionario), SEEK_SET);

        keys[position].RRN = ftell(file);

        Funcionario *func = readArq(file);

        keys[position].id = func->id;

        position++;
    }

    int i, j;
    struct KeyID aux;

    for (i=0; i< sizeFile; i++) {
        for (j=i+1; j< sizeFile; j++) {
            if (keys[i].id > keys[j].id) {
                aux = keys[i];
                keys[i] = keys[j];
                keys[j] = aux;
            }
        }
    }

    rewind(file);

    for(int k = 0; k < sizeFile; ++k) {

        fseek(file, keys[k].RRN, SEEK_SET);
        Funcionario *aux1 = readArq(file);

        fseek(sortedFile, k * sizeof(Funcionario), SEEK_SET);
        saveDataBase(aux1, sortedFile);
    }

}

int sizeFile(FILE *file, int contSizeFile) {

    int bytesAUX = 0;

    while(!feof(file)) {

        fseek(file, bytesAUX * sizeof(Funcionario), SEEK_SET);

        Funcionario *aux = readArq(file);
        if(aux != NULL) {
            contSizeFile++;
        }

        bytesAUX++;
    }

    return contSizeFile;
}

void printParticaoCodFuncionario(FILE *file, char nomeParticao[]) {

    printf("\nID funcionario da particao %s: \n", nomeParticao);

    for (int i = 0; i < sizeFile(file, 0); ++i) {

        fseek(file, i * sizeof(Funcionario), SEEK_SET) ;
        Funcionario *aux = readArq(file);

        printf(" %i ", aux->id);
    }

    printf("\n");
}

int registroMemoria (int vet[6]) {

    int cont = 0;

    for (int i = 0; i < 6; ++i) {
        if (vet[i] == 1) {
            cont++;
        }
    }

    if (cont == 6) {
        return 1;
    } else {
        return 0;
    }
}

void mergeSort(int numeroDeParticoes, char nameFilePartition[]) {

    int *vetSizePartition = (int *) malloc(numeroDeParticoes * sizeof(int));
    int *vetFinalPartition = (int *) malloc(numeroDeParticoes * sizeof(int));
    int *vetPositionPartition = (int *) malloc(numeroDeParticoes * sizeof(int));
    int *vetValueEmployeePartition = (int *) malloc(numeroDeParticoes * sizeof(int));
    int flagAuxFinal = 0, count, smallElement = INT_MAX, smallElementPosition = 0;

    FILE *auxFileFinal = fopen("mergeSortFileSorted.dat", "wb+");

    for (int i = 0; i <= numeroDeParticoes; ++i) {

        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(nomeParticao, nameFilePartition), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "rb+");

        rewind(filePartition);

        vetSizePartition[i] = sizeFile(filePartition, 0);
        vetFinalPartition[i] = 0;
        vetPositionPartition[i] = 0;

        fseek(filePartition, vetPositionPartition[i] * sizeof(Funcionario), SEEK_SET);
        Funcionario *auxFunc = readArq(filePartition);

        vetValueEmployeePartition[i] = auxFunc->id;

        fclose(filePartition);
    }


    while (flagAuxFinal != 1) {

        count = 0;

        for (int i = 0; i <= numeroDeParticoes; ++i) {

            if (vetFinalPartition[i] == 1) {
                count++;
            }

            if (count == numeroDeParticoes) {
                flagAuxFinal = 1;
            }
        }

        for (int i = 0; i <= numeroDeParticoes; ++i) {

            if (vetValueEmployeePartition[i] < smallElement && vetFinalPartition[i] != 1) {
                smallElement = vetValueEmployeePartition[i];
                smallElementPosition = i;
            }
        }

        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(smallElementPosition, str1, 10);
        strcat(strcpy(nomeParticao, nameFilePartition), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "rb+");

        rewind(filePartition);
        fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(Funcionario), SEEK_SET);
        Funcionario *auxFunc = readArq(filePartition);
        saveDataBase(auxFunc, auxFileFinal);
        vetPositionPartition[smallElementPosition]++;

        rewind(filePartition);

        if (vetPositionPartition[smallElementPosition] >= vetSizePartition[smallElementPosition]) {
            vetFinalPartition[smallElementPosition] = 1;
        } else {
            fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(Funcionario), SEEK_SET);
            Funcionario *auxFunc2 = readArq(filePartition);
            vetValueEmployeePartition[smallElementPosition] = auxFunc2->id;
        }

        fclose(filePartition);

        smallElement = INT_MAX;

    }

    printParticaoCodFuncionario(auxFileFinal, "mergeSortFileSorted.dat");

    fclose(auxFileFinal);
    free(vetFinalPartition);
    free(vetSizePartition);
    free(vetPositionPartition);
    free(vetValueEmployeePartition);
}

int selecaoSubstituicao (FILE *file, char nameFilePartition[]) {

    int numeroDeParticoes = 0, contSizeFile = 0, position = 6, smallElementPosition = 0, smallElement = 999999, sizeFileAux = 0, selectedPosition = 0;
    struct Operario func[6];
    int auxVetFunc [6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    sizeFileAux = sizeFile(file, contSizeFile);


    printf("\nRealizando substituicao por selecao...");

    for (int i = 0; i < 6; ++i) {
        fseek(file, i * sizeof(Funcionario), SEEK_SET);

        Funcionario *aux = readArq(file);
        func[i] = *aux;

    }

    for (int i = 0; i < 6; ++i) {
        auxVetFunc[i] = func[i].id;
    }

    while (position != sizeFileAux) {

        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(numeroDeParticoes, str1, 10);
        strcat(strcpy(nomeParticao, nameFilePartition), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "wb+");

        int auxVetFrozen[6] = {0, 0, 0, 0, 0, 0,};

        while (position != sizeFileAux) {

            smallElement = 9999999;

            for (int i = 0; i < 6; ++i) {

                if (smallElement > auxVetFunc[i] && auxVetFrozen[i] != 1) {
                    smallElement = auxVetFunc[i];
                    smallElementPosition = i;
                }
            }

            saveDataBase(&func[smallElementPosition], filePartition);

            fseek(file, position * sizeof(Funcionario), SEEK_SET);

            Funcionario *aux = readArq(file);

            position++;

            auxVetFunc[smallElementPosition] = aux->id;
            func[smallElementPosition] = *aux;

            if (aux->id < smallElement) {
                auxVetFrozen[smallElementPosition] = 1;
            }

            if(registroMemoria(auxVetFrozen) == 1) {
                numeroDeParticoes++;
                break;
            }

        }

        fclose(filePartition);

        if (position >= sizeFileAux) {
            break;
        }

    }

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";

    itoa(numeroDeParticoes, str1, 10);
    strcat(strcpy(nomeParticao, nameFilePartition), str1);
    strcat(strcpy(nomeParticao, nomeParticao), str2);

    FILE *filePartitionFinal = fopen(nomeParticao, "ab+");

    int k, j;

    struct Operario funcAux;

    for (k = 1; k < 6; k++) {

        for (j = 0; j < 6 - 1; j++) {

            if (func[j].id > func[j + 1].id) {
                funcAux = func[j];
                func[j] = func[j + 1];
                func[j + 1] = funcAux;
            }
        }
    }

    for (int i = 0; i < 6; ++i) {
        saveDataBase(&func[i], filePartitionFinal);
    }

    fclose(filePartitionFinal);

    for (int i = 0; i <= numeroDeParticoes; ++i) {


        itoa(i, str1, 10);
        strcat(strcpy(nomeParticao, nameFilePartition), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "rb+");

        printParticaoCodFuncionario(filePartition, nomeParticao);

        fclose(filePartition);
    }

    return numeroDeParticoes;

}

void printAllFuncionarioFile(FILE *file) {

    rewind(file);

    for (int i = 0; i < sizeFile(file, 0); ++i) {

        fseek(file, i * sizeof(Funcionario), SEEK_SET);
        Funcionario *func = readArq(file);
        printFuncionario(func);
    }

}






// --------------------------------------------------------------------
// P3

void criaParticaoTabelaHash (int numeroDeParticoes) {

    for (int i = 0; i < numeroDeParticoes; ++i) {

        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *file = fopen(nomeParticao,"wb+");

        fclose(file);
    }

}

void tabelaHash(FILE *file, int numeroDeParticoes, int sizeFile) {

    rewind(file);

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";

    for (int i = 0; i < sizeFile; ++i) {

        Funcionario *auxFunc = readArq(file);

        int selectedParticipation = auxFunc->id % numeroDeParticoes;

        itoa(selectedParticipation, str1, 10);
        strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao,"ab+");

        saveDataBase(auxFunc, filePartition);

        fclose(filePartition);

    }

    for (int i = 0; i < numeroDeParticoes; ++i) {

        itoa(i, str1, 10);
        strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao,"rb+");

        printParticaoCodFuncionario(filePartition, nomeParticao);

        fclose(filePartition);
    }
}

Funcionario *searchTabelaHash(int codFuncionario, int idParticao) {

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons;

    itoa(idParticao, str1, 10);
    strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
    strcat(strcpy(nomeParticao, nomeParticao), str2);

    printf("\nEfetuando busca %s...", nomeParticao);

    FILE *filePartition = fopen(nomeParticao,"rb+");

    Funcionario *auxFunc = buscaSequencial(codFuncionario, filePartition, &totalCompararisons);

    fclose(filePartition);

    return auxFunc;
}

void insereTabelaHash(Funcionario *funcHashTableInsert,int codFuncionario, int idParticao) {

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons = 0;
    int totalCompararisonsDeleted = 0;
    int sizefilePartition = 0;

    itoa(idParticao, str1, 10);
    strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
    strcat(strcpy(nomeParticao, nomeParticao), str2);

    FILE *filePartition = fopen(nomeParticao,"r+b");

    sizefilePartition = sizeFile(filePartition, 0);
    rewind(filePartition);

    Funcionario *auxFunc = buscaSequencial(funcHashTableInsert->id, filePartition, &totalCompararisons);
    rewind(filePartition);

    Funcionario *auxFuncDeleted = buscaSequencial(999999999, filePartition, &totalCompararisonsDeleted);
    rewind(filePartition);

    if (auxFunc == NULL && auxFuncDeleted == NULL) {
        fseek(filePartition, (sizefilePartition) * sizeof(Funcionario), SEEK_SET);\
        saveDataBase(funcHashTableInsert, filePartition);
        printf("\nFuncionario inserido no fim da particao");
    } else if (auxFunc == NULL && auxFuncDeleted != NULL){
        fseek(filePartition, (totalCompararisonsDeleted - 1) * sizeof(Funcionario), SEEK_SET);\
        saveDataBase(funcHashTableInsert, filePartition);
        printf("\nFuncionario inserido no campo removido original");
    } else {
        printf("\nFuncionario ja existente na tabela");

    }

    fclose(filePartition);
}

void deletaTabelaHash(int codFuncionario, int idParticao) {

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";
    int totalCompararisons = 0;

    itoa(idParticao, str1, 10);
    strcat(strcpy(nomeParticao, "hashTablePartition"), str1);
    strcat(strcpy(nomeParticao, nomeParticao), str2);

    FILE *filePartition = fopen(nomeParticao,"r+b");

    Funcionario *auxFunc = buscaSequencial(codFuncionario, filePartition, &totalCompararisons);

    if (auxFunc != NULL) {

        fseek(filePartition, (totalCompararisons - 1) * sizeof(Funcionario), SEEK_SET);

        Funcionario funcHashDelet;

        funcHashDelet.id = 999999999;
        sprintf(funcHashDelet.name, "%s", auxFunc->name);
        sprintf(funcHashDelet.cpf, "%s", auxFunc->cpf);
        sprintf(funcHashDelet.data_nascimento, "%s", auxFunc->data_nascimento);
        funcHashDelet.salario = auxFunc->salario;

        saveDataBase(&funcHashDelet, filePartition);

        printf("\nFuncionario exluido com sucesso");
    } else {
        printf("\nFuncionario inexistente");
    }

    fclose(filePartition);
}



