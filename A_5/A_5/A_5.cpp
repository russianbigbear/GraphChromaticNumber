#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // кол-во вершин
const int INF = 999; // бесконечность

/*ввод графа из файла, заданного при помощи матрицы */
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}


/* печать графа c веом рёбер(вывод в graphic.png) */
void print_graph(int **graph) {
	ofstream out;
	out.open("graphic.dot");
	out << "graph G{" << "\n";
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				out << i + 1 << " -- " << j + 1 << "\n";
	out << "}";
	out.close();
	system("dot -Tpng graphic.dot -o graphic.png ");
	system("graphic.png");
}

/*-----------------------------------------------------------------------------------------------------*/

/* Основная функция
ifs - входной поток;
ofs - выходной поток;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "Редактировать файл ввода? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);

		ifs.open("input.txt");
		ifs >> N;

		int **graph_cost_m = new int*[N]; //зануляем матрицу
		for (int i = 0; i < N; i++)
		{
			graph_cost_m[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph_cost_m[i][j] = 0;
		}

		input_matrix(ifs, graph_cost_m);

		do {
			cout << "Вывести граф? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph_cost_m);
		} while (ans != 1 && ans != 0);


		/*подсчет степени вершин - количество ребер этой вершины
		-1 - если вершина не имеет инциндетных рёбер*/
		vector<int> degree_of_point;
		while (degree_of_point.size() < N) degree_of_point.push_back(-1);

		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (graph_cost_m[i][j] != 0)
					degree_of_point[i]++;


		/*сортируем вершиы по убыванию*/
		vector<int> vertex(N);
		int max_degree = degree_of_point[0];

		//поиск максимальной степени
		for (int i = 1; i < degree_of_point.size(); i++) {

			if (i < N) vertex[i] = i;
			else vertex[i] = -1;

			if (degree_of_point[i] > max_degree)
				max_degree = degree_of_point[i];
		}

		int tmp = 0;
		for (int i = 0; i < N - 1; i++)
			for (int j = 0; j < N - i - 1; j++)
				if (degree_of_point[j] < degree_of_point[j + 1]) {
					tmp = degree_of_point[j];
					degree_of_point[j] = degree_of_point[j + 1];
					degree_of_point[j + 1] = tmp;

					tmp = vertex[j];
					vertex[j] = vertex[j + 1];
					vertex[j + 1] = tmp;
				}
		for (int i = 0; i < N; i++) vertex[i]++;

		/*алгоритм покраски*/

		ofs.open("graphic_color.dot");
		ofs << "graph G{" << "\n";
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (graph_cost_m[i][j] != 0 && i < j)
					ofs << i + 1 << " -- " << j + 1 << "\n";

		int сhromatic_number = 0; // хроматическое число - минимальное число используемых красок
		int colored_vertex = 0; // количество покрашенных вершин
		vector<int> painted_used; //вектор покрашенных вершин текущим цветом

		vector<int> colored;
		while (colored.size() < N + 1) colored.push_back(-1);

		/*выполняется, пока все вершины не будут покрашенны*/
		do {
			int num = 0;
			bool flag = true;

			//находим первую непокрашенную вершину из vertex списка
			for (int i = 0; i < N && flag == true; i++) {
				num = vertex[i];
				if (colored[num] == -1) {				// если эта вершина еще не окрашена
					colored[num] = i;					// то в массив окращенных записываем ее индекс в списке VERTEX
					painted_used.push_back(num);		//в вектор покрашенных вершин одним цветом, пишем текущую вершину
					flag = false;						// выход из поиска 
					colored_vertex++;					// на 1 покрашеную вершину стало больше
				}
			}

			// обходим все смежные вершины, уже покрашенным. 
			for (int i = 0; i < N; i++) {
				flag = true;
				//Если вершина будет смежной, хоть одной из покрашенных, то мы не можем ее покрасить
				for (int j = 0; j < painted_used.size() && flag; j++) {
					int x = painted_used[j];								//только что покрашенная вершина
					int PROWETYEMAY = vertex[i];							//вершина с которой проверяем смежность
					if (colored[PROWETYEMAY] == -1) {						//если еще не окрашена
						if (graph_cost_m[x - 1][PROWETYEMAY - 1] == 1) {	// если смежна с только что покрашеной
							flag = false;									//красить нельзя
						}
					}
					else {
						flag = false;										//красить нельзя
					}
				}
				if (flag) {													//если можно красить
					painted_used.push_back(vertex[i]);						// разрешаем покрасить данную вершину
					colored[vertex[i]] = i;									// в массив уже покращенных вершин делаем заметку
					colored_vertex++;										//на 1 покрашеную вершину стало больше
				}
			}

			for (int i = 0; i < painted_used.size(); i++) {
				ofs << painted_used[i] << "[color=";
				switch (сhromatic_number)
				{
				case 0: {
					ofs << "red]" << endl;
					break;
				}
				case 1: {
					ofs << "green]" << endl;
					break;
				}
				case 2: {
					ofs << "blue]" << endl;
					break;
				}
				case 3: {
					ofs << "yellow]" << endl;
					break;
				}
				case 4: {
					ofs << "pink]" << endl;
					break;
				}
				default:	break;
				}
			}

			сhromatic_number++;			
			painted_used.clear();		//очищаем покрашенные вершины, для нанесения новой краски на новые вершины
		} while (colored_vertex != N);
		ofs << "}\n";
		ofs.close(); 

		/*вывод результата в файл*/
		ofs.open("output.txt");
		ofs << "Для указанного графа, хроматическое число - " << сhromatic_number << ".";
		ofs.close();
		system("dot -Tpng graphic_color.dot -o graphic_color.png ");

		do {
			cout << "Вывести покрашенный? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("graphic_color.png"); //открытие результата
		} while (ans != 1 && ans != 0);

		do {
			cout << "Вывести результат? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt"); //открытие результата
		} while (ans != 1 && ans != 0);

	}

	catch (...) {
		cout << "Ошибка файла!";
	}

	cout << "Программа выполнена." << endl;
	return 0;
}
