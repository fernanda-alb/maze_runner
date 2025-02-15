#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <thread>


using namespace std; 

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Flag: saída encontrada
bool exit_found;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE * MAZE;
	MAZE= fopen(file_name, "r");
	// Le o numero de linhas e colunas (fscanf) 
	fscanf(MAZE, "%d %d", &num_rows, &num_cols);
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
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c" , maze[i][j]);
		}
		printf("\n");
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos) {
	pos_t pos_valida;
	stack<pos_t> valid_positions;
	valid_positions.push(pos);
	while(!valid_positions.empty()){
		valid_positions.pop();
		maze[pos.i][pos.j]= 'o';

		this_thread::sleep_for(chrono::milliseconds(50));

		maze[pos.i][pos.j]= '.';

		if(maze[pos.i][pos.j-1]== 's' && pos.j-1 >= 0){
			maze[pos.i][pos.j-1]= 'o';
			print_maze();
			exit_found= 1;
		}
		else if(maze[pos.i][pos.j-1]== 'x' && pos.j-1 >= 0){
			pos_valida.i= pos.i;
			pos_valida.j=pos.j-1;
			valid_positions.push(pos_valida);
		}
		
		if(maze[pos.i][pos.j+1]== 's' && pos.j+1 < num_cols){
			maze[pos.i][pos.j+1]= 'o';
			print_maze();
			exit_found= 1;
		}
		else if(maze[pos.i][pos.j+1]== 'x' && pos.j+1 < num_cols){
			pos_valida.i= pos.i;
			pos_valida.j=pos.j+1;
			valid_positions.push(pos_valida);
		}

		if(pos.i+1 < num_rows){
			if(maze[pos.i+1][pos.j]== 's'){
				maze[pos.i+1][pos.j]= 'o';
				print_maze();
				exit_found= 1;
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
				exit_found=1; 
			}
			else if(maze[pos.i-1][pos.j]== 'x'){
				pos_valida.i= pos.i-1;
				pos_valida.j=pos.j;
				valid_positions.push(pos_valida);
			}
		}

		if(valid_positions.size() == 1){
			pos.i= valid_positions.top().i;
			pos.j= valid_positions.top().j;
		} 
		else if(valid_positions.size() == 2){
			pos.i= valid_positions.top().i;
			pos.j= valid_positions.top().j;
			valid_positions.pop();

			pos_valida.i= valid_positions.top().i;
			pos_valida.j= valid_positions.top().j;
			thread f(walk, pos_valida);
			f.detach();
		}
		else if(valid_positions.size() == 3){
			pos.i= valid_positions.top().i;
			pos.j= valid_positions.top().j;
			valid_positions.pop();

			pos_valida.i= valid_positions.top().i;
			pos_valida.j= valid_positions.top().j;
			thread f(walk, pos_valida);
			f.detach();
			valid_positions.pop();

			pos_valida.i= valid_positions.top().i;
			pos_valida.j= valid_positions.top().j;
			thread g(walk, pos_valida);
			g.detach();
		}
	}
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação- thread secundária 
	thread t(walk,initial_pos);
	t.detach();

	// Tratar o retorno (imprimir mensagem)
	while(!exit_found){
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(50));
		system("clear");
		}

	print_maze();
	printf("Parabens! Saida encontrada :)\n");

	return 0;
}

