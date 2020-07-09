#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // ���-�� ������
const int INF = 999; // �������������

/*���� ����� �� �����, ��������� ��� ������ ������� */
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}


/* ������ ����� c ���� ����(����� � graphic.png) */
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

/* �������� �������
ifs - ������� �����;
ofs - �������� �����;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "������������� ���� �����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);

		ifs.open("input.txt");
		ifs >> N;

		int **graph_cost_m = new int*[N]; //�������� �������
		for (int i = 0; i < N; i++)
		{
			graph_cost_m[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph_cost_m[i][j] = 0;
		}

		input_matrix(ifs, graph_cost_m);

		do {
			cout << "������� ����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph_cost_m);
		} while (ans != 1 && ans != 0);


		/*������� ������� ������ - ���������� ����� ���� �������
		-1 - ���� ������� �� ����� ����������� ����*/
		vector<int> degree_of_point;
		while (degree_of_point.size() < N) degree_of_point.push_back(-1);

		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (graph_cost_m[i][j] != 0)
					degree_of_point[i]++;


		/*��������� ������ �� ��������*/
		vector<int> vertex(N);
		int max_degree = degree_of_point[0];

		//����� ������������ �������
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

		/*�������� ��������*/

		ofs.open("graphic_color.dot");
		ofs << "graph G{" << "\n";
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (graph_cost_m[i][j] != 0 && i < j)
					ofs << i + 1 << " -- " << j + 1 << "\n";

		int �hromatic_number = 0; // ������������� ����� - ����������� ����� ������������ ������
		int colored_vertex = 0; // ���������� ����������� ������
		vector<int> painted_used; //������ ����������� ������ ������� ������

		vector<int> colored;
		while (colored.size() < N + 1) colored.push_back(-1);

		/*�����������, ���� ��� ������� �� ����� ����������*/
		do {
			int num = 0;
			bool flag = true;

			//������� ������ ������������� ������� �� vertex ������
			for (int i = 0; i < N && flag == true; i++) {
				num = vertex[i];
				if (colored[num] == -1) {				// ���� ��� ������� ��� �� ��������
					colored[num] = i;					// �� � ������ ���������� ���������� �� ������ � ������ VERTEX
					painted_used.push_back(num);		//� ������ ����������� ������ ����� ������, ����� ������� �������
					flag = false;						// ����� �� ������ 
					colored_vertex++;					// �� 1 ���������� ������� ����� ������
				}
			}

			// ������� ��� ������� �������, ��� �����������. 
			for (int i = 0; i < N; i++) {
				flag = true;
				//���� ������� ����� �������, ���� ����� �� �����������, �� �� �� ����� �� ���������
				for (int j = 0; j < painted_used.size() && flag; j++) {
					int x = painted_used[j];								//������ ��� ����������� �������
					int PROWETYEMAY = vertex[i];							//������� � ������� ��������� ���������
					if (colored[PROWETYEMAY] == -1) {						//���� ��� �� ��������
						if (graph_cost_m[x - 1][PROWETYEMAY - 1] == 1) {	// ���� ������ � ������ ��� ����������
							flag = false;									//������� ������
						}
					}
					else {
						flag = false;										//������� ������
					}
				}
				if (flag) {													//���� ����� �������
					painted_used.push_back(vertex[i]);						// ��������� ��������� ������ �������
					colored[vertex[i]] = i;									// � ������ ��� ����������� ������ ������ �������
					colored_vertex++;										//�� 1 ���������� ������� ����� ������
				}
			}

			for (int i = 0; i < painted_used.size(); i++) {
				ofs << painted_used[i] << "[color=";
				switch (�hromatic_number)
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

			�hromatic_number++;			
			painted_used.clear();		//������� ����������� �������, ��� ��������� ����� ������ �� ����� �������
		} while (colored_vertex != N);
		ofs << "}\n";
		ofs.close(); 

		/*����� ���������� � ����*/
		ofs.open("output.txt");
		ofs << "��� ���������� �����, ������������� ����� - " << �hromatic_number << ".";
		ofs.close();
		system("dot -Tpng graphic_color.dot -o graphic_color.png ");

		do {
			cout << "������� �����������? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("graphic_color.png"); //�������� ����������
		} while (ans != 1 && ans != 0);

		do {
			cout << "������� ���������? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt"); //�������� ����������
		} while (ans != 1 && ans != 0);

	}

	catch (...) {
		cout << "������ �����!";
	}

	cout << "��������� ���������." << endl;
	return 0;
}
