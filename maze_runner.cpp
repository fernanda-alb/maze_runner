#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <thread>
#include <chrono>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	pos_t pos;
	pos.i = 1;
	pos.j = 3;
	valid_positions.push(pos)
*/
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
				// Abre o arquivo para leitura (fopen)
	FILE * MAZE;
	MAZE= fopen(file_name, "r");
				// Le o numero de linhas e colunas (fscanf) 
	fscanf(MAZE, "%d %d", &num_rows, &num_cols);
	//fgetc(MAZE);
				// e salva em num_rows e num_cols

				// Aloca a matriz maze (malloc)
	maze = (char **) malloc(num_rows* sizeof(char*));

	for (int i = 0; i < num_rows; ++i){
				// Aloca cada linha da matriz
		maze[i] = (char *) malloc(num_cols* sizeof(char));
	}

	for (int i = 0; i < num_rows; ++i) {
		fgetc(MAZE);
		for (int j = 0; j < num_cols; ++j) {
				// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			fscanf(MAZE, "%c", &maze[i][j]);
				// Se o valor for 'e' salvar o valor em initial_pos
			if(maze[i][j] == 'e'){
				pos_t initial_pos;
				initial_pos.i= i;
				initial_pos.j= j; 
				valid_positions.push(initial_pos);
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	pos_t pos_valida;
	while(!valid_positions.empty()){
		pos.i= valid_positions.top().i;
		pos.j= valid_positions.top().j;

		valid_positions.pop();
		maze[pos.i][pos.j]= 'o';
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		system("clear");
		maze[pos.i][pos.j]= '.';
		
		if(maze[pos.i][pos.j+1]== 's' && pos.j+1 < num_cols){
			maze[pos.i][pos.j+1]= 'o';
			print_maze();
			return 1;
		}
		else if(maze[pos.i][pos.j+1]== 'x' && pos.j+1 < num_cols){
			pos_valida.i= pos.i;
			pos_valida.j=pos.j+1;
			valid_positions.push(pos_valida);
		}
		
		if(maze[pos.i][pos.j-1]== 's' && pos.j-1 >= 0){
			maze[pos.i][pos.j-1]= 'o';
			print_maze();
			return 1;
		}
		else if(maze[pos.i][pos.j-1]== 'x' && pos.j-1 >= 0){
			pos_valida.i= pos.i;
			pos_valida.j=pos.j-1;
			valid_positions.push(pos_valida);
		}

		if(pos.i+1 < num_rows){
			if(maze[pos.i+1][pos.j]== 's'){
				maze[pos.i+1][pos.j]= 'o';
				print_maze();
				return 1;
			}
			else if(maze[pos.i+1][pos.j]== 'x'){
				pos_valida.i= pos.i+1;
				pos_valida.j=pos.j;
				valid_positions.push(pos_valida);
			}
		}

		if(pos.i-1 >= 0){
			if(maze[pos.i-1][pos.j]== 's'){
				maze[pos.i-1][pos.j]= 'o';
				print_maze();
				return 1;
			}
			else if(maze[pos.i-1][pos.j]== 'x'){
				pos_valida.i= pos.i-1;
				pos_valida.j=pos.j;
				valid_positions.push(pos_valida);
			}
		}
	}
			// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
				// Marcar a posição atual com o símbolo '.'
				// Limpa a tela
				// Imprime o labirinto
				
				/* Dado a posição atual, verifica quais sao as próximas posições válidas
					Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
					e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
					cada uma delas no vetor valid_positions
						- pos.i, pos.j+1
						- pos.i, pos.j-1
						- pos.i+1, pos.j
						- pos.i-1, pos.j
					Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
				*/

		
	
			// Verifica se a pilha de posições nao esta vazia 
			//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
			// Caso contrario, retornar falso
		// if (!valid_positions.empty()) {
		// 	pos_t next_position = valid_positions.top();
		// 	valid_positions.pop();
		// }
	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	// Tratar o retorno (imprimir mensagem)
	if(exit_found){
		printf("Parabens! Saida encontrada :)\n");
	}
	else{
		printf(":( Saida nao encontrada\n");
	}
	return 0;
}
