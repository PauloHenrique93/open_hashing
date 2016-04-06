// OpenPlayering.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define SIZE 100

typedef struct Player{
    int id;
    char name[100];
    char classPlayer[100];
    float points;
	char status; // F = Livre, O = ocupado, R = removido
	int hashValue;
	int vectorPosition;

}Player;

//Function Signatures
int hashFunction(int id);
void showPlayers(Player table[]);
void insertPlayer(Player table[], int position, Player player);
int searchPlayer(Player table[], int id);
void deletePlayer(Player table[], int id);
Player* readFile(Player table[], FILE *fp);
Player* loadFactor(Player table[]);
void applicationMenu(Player table[], FILE *fp);

//Global Variable
int dimension = SIZE;

int _tmain(int argc, _TCHAR* argv[])
{
	Player* table = (Player*) malloc (sizeof(Player)*SIZE);
	Player player;
    int i;
	FILE* fp;
	
	fp = fopen("documentoTeste.txt", "rt");

    //table start
    for(i = 0; i < dimension; i++)
        table[i].status='F';

	applicationMenu(table, fp);

    

	//system("PAUSE");
	return 0;
}

int hashFunction(int id){
    return id % dimension;
}

void showPlayers(Player table[]){
    int i = 0;
	for (i; i < dimension; i++){
		if(table[i].status == 'O')
			printf("%s - Posicao Hash %d - Posicao Vetor - %d\n", table[i].name, table[i].hashValue, table[i].vectorPosition);
	}
}

void insertPlayer(Player table[], int position, Player player){
    int i = 0, vectorPosition = position; 

	while(i < dimension && table[(position+i)%dimension].status != 'F' && table[(position+i)%dimension].status != 'R'){
            i++;
			vectorPosition = (position+i)%dimension; //getting player position in vector
	}

    if(i < dimension){
		table[(position+i)%dimension].id = player.id;
		strcpy(table[(position+i)%dimension].name, player.name); 
		strcpy(table[(position+i)%dimension].classPlayer, player.classPlayer); 
		table[(position+i)%dimension].points = player.points;
		table[(position+i)%dimension].hashValue = position;
		table[(position+i)%dimension].vectorPosition = vectorPosition;
        table[(position+i)%dimension].status = 'O';
    }
    else
        printf("\n\nTABELA CHEIA!");
}

int searchPlayer(Player table[], int id){
    int i = 0;
    int position = hashFunction(id);

	while(i < dimension && table[(position+i)%dimension].status != 'F' && table[(position+i)%dimension].id != id)
            i++;

    if(table[(position+i)%dimension].id == id && table[(position+i)%dimension].status != 'R')
        return (position+i)%dimension;
    else
        return dimension; //not found
}

void deletePlayer(Player table[], int id){
    int position = searchPlayer(table, id);

	if (position < dimension){
        table[position].status = 'R';
		printf("JOGADOR DELETADO!!");
	}
    else
        printf("ELEMENTO NAO ESTA PRESENTE");
}

Player* readFile(Player table[], FILE *fp){
	char nameAndClass[200];
	
	int h, y, quant;//index for name and classes
	int loadMeasure = 0; //increases when the players are inserted

	char character;
	int i = 0; 
	int id = 0, position = 0;
	float points = 0;
	char classPlayer[100];
	char name[100];
	
	//SCROLL TO THE END OF FILE
	while(!feof(fp)){
		
		//GETTING ID         
		fscanf(fp,"%d", &id); 
		
		//JUMPING O CHARACTER ; 
		fgetc(fp); 
		
		//GETTING NAME AND CLASS

		y = 0;
		quant = 0;
		while(quant < 2){
			fscanf(fp, "%c", &character);
			if(character == ';')
				quant++;
			if(quant < 2){
				nameAndClass[y] = character;
				y++;
			}
		}

		nameAndClass[y] = '\0';
		
		//separating the name and class putting in their respective vectors
		int z = 0;
		int x = 0;
		while(1){
			name[z] = nameAndClass[z];

			if(nameAndClass[z] == ';'){
				name[z] = '\0';
				while(nameAndClass[z] != '\0'){
					classPlayer[x] = nameAndClass[z+1];
					x++; z++;
				}
				classPlayer[y] = '\0';
				break;
			}

			z++;
		}

		//GETTING THE POINTS
		fscanf(fp,"%f", &points);

		//ADDING IN PLAYER STRUCT
		Player player;
		player.id = id;
		strcpy(player.name, name);
		strcpy(player.classPlayer, classPlayer);
		player.points = points;

		position = hashFunction(player.id);
		insertPlayer(table, position, player);

		loadMeasure++;
		if( loadMeasure > (dimension * 0.7) )
			table = loadFactor(table);

	}

	fclose(fp);

	return table;
}

Player* loadFactor(Player table[]){
	int position = 0, i, j;
	Player* helper = (Player*) malloc( sizeof(Player) * (dimension*2) );	

	dimension = ( dimension * 2 ); //revalorando a variavel dimension relativo ao tamanho da HashTable;

	//helper start
	for(i = 0; i < dimension; i++)
		helper[i].status='F';
	
	for(i = 0; i < (dimension/2); i++){
		if(table[i].status == 'O'){
			position = hashFunction(table[i].id);
			insertPlayer(helper, position, table[i]);
		}
	}

	return helper;
}

void applicationMenu(Player table[], FILE *fp){
	int selected = 0, position = 0, id = 0;

	do{
        printf("\n1 - Carregar Arquivo\n");
        printf("2 - Exibir Jogadores\n");
		printf("3 - Pesquisar Jogador\n");
        printf("4 - Excluir Jogador\n");
        printf("5 - Sair\n");
        printf("=> "); scanf("%d", &selected);

        if(selected < 1 || selected > 5)
            printf("\nOPCAO INVALIDA!!\n");
        else{
            switch(selected){
                case 1:
					table = readFile(table, fp);
					printf("ARQUIVO CARREGADO");
                    break;

                case 2:
                    showPlayers(table);
                    break;

				case 3:
					printf("\nDigite o id: ");scanf("%d", &id);
					position = searchPlayer(table, id);
					if(position == dimension) 
						printf("JOGADOR NAO ENCONTRADO!!");
					else 
						printf("NOME: %s", table[position].name);
					break;

                case 4:
                    printf("\nDigite o id: "); scanf("%d", &id);
                    deletePlayer(table, id);
					break;
            }
        }

    }while(selected != 5);
}
	






