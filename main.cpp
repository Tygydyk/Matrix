#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <io.h>
using namespace std;

struct Matrix {
	int height; int width;
	int ** values;
};

Matrix* create(int height, int width) { // Создание матрицы
	int ** lines = new int*[height];
	for (int i = 0; i < height; i++) {
		lines[i] = new int[width]; // Создаем пустую матрицу
	}
	Matrix* res = new Matrix;
	res->height = height; res->width = width;
	res->values = lines;
	return res;
}

void print(Matrix* m) {
	if (m == 0) {
		wcout << L"Матрица пустая\n";
		return;
	}
	for (int i = 0; i < m->height; i++) {
		for (int k = 0; k < m->width; k++) {
			wcout << m->values[i][k] << L" ";
		}
		wcout << L"\n";
	}
}

Matrix* sum(int height, int width, Matrix* a1, Matrix* a2) {
	Matrix* res = create(height, width);
	for (int i = 0; i < height; i++)
		for (int k = 0; k < width; k++)
			res->values[i][k] = a1->values[i][k] + a2->values[i][k];
	return res;
}

Matrix* read(int height, int width) {
	Matrix* res = create(height, width);
	string line;
	std::getline(cin, line); // Пропскам пустую строку
	for (int i = 0; i < height; i++) {
		std::getline(cin, line); // Считываем строку
		std::istringstream iss(line);
		int value; int k = 0;
		while (iss >> value) {
			res->values[i][k] = value; // Записыаем значение в матрицу
			k++;
		}
	}
	return res;
}

Matrix* multiply(Matrix* a1, Matrix* a2) {
	Matrix* res = create(a1->height, a2->width);
	for (int i = 0; i < a1->height; i++) {
		for (int k = 0; k < a2->width; k++) {
			int sum = 0;
			for (int j = 0; j < a1->width; j++) {
				sum += a1->values[i][j] * a2->values[j][k];
			}
			res->values[i][k] = sum;
		}
	}
	return res;
}

Matrix* transpose(Matrix* m) {
	Matrix* res = create(m->width, m->height);
	for (int i = 0; i < m->height; i++)
		for (int k = 0; k < m->width; k++)
			res->values[k][i] = m->values[i][k];
	return res;
}

vector<int>* sorted_values(Matrix * m) { // Собирвает все значения матрицы в отсортированный массив
	vector<int>* res = new vector<int>;
	for (int i = 0; i < m->height; i++)
		for (int k = 0; k < m->width; k++)
			res->push_back(m->values[i][k]);
	std::sort(res->begin(), res->end()); // Библиотечная сортировка
	return res;
}

void ant_sort(Matrix* m) {
	vector<int>* values = sorted_values(m);
	for(int i = 0; i<m->height; i++)
		for (int k = 0; k < m->width; k++) {
			m->values[i][k] = (*values)[(i*m->width)+k];
		}
	// -------->
	// -------->
	// -------->
}

void snake_sort(Matrix* m) {
	vector<int>* values = sorted_values(m);
	int cursor = 0;
	for (int k = 0; k < m->width; k++) {
		if (k % 2 == 0) {
			for (int i = 0; i < m->height; i++) {
				m->values[i][k] = (*values)[cursor];
				cursor++;
			}
		}
		else if (k % 2 == 1) {
			for (int i = m->height-1; i >= 0; i--) {
				m->values[i][k] = (*values)[cursor];
				cursor++;
			}
		}
	}
	/*
	 ^  ^  ^
	|| || ||
	|| || ||
	\/ \/ \/
	
	*/
}

void snail_sort(Matrix* inp)
{
	vector<int>* values = sorted_values(inp);
	int vector_iter = 0;
	int i, k = 0, l = 0;
	int m = inp->height, n = inp->width;

	while (k < m && l < n)
	{
		/* Print the first row from the remaining rows */
		for (i = l; i < n; ++i)
		{
			//printf("%d ", a[k][i]);
			inp->values[k][i] = (*values)[vector_iter];
			vector_iter++;
		}
		k++;

		/* Print the last column from the remaining columns */
		for (i = k; i < m; ++i)
		{
			inp->values[i][n - 1] = (*values)[vector_iter];
			vector_iter++;
			//printf("%d ", a[i][n - 1]);
		}
		n--;

		/* Print the last row from the remaining rows */
		if (k < m)
		{
			for (i = n - 1; i >= l; --i)
			{
				inp->values[m - 1][i] = (*values)[vector_iter];
				vector_iter++;
				//printf("%d ", a[m - 1][i]);
			}
			m--;
		}

		/* Print the first column from the remaining columns */
		if (l < n)
		{
			for (i = m - 1; i >= k; --i)
			{
				inp->values[i][l] = (*values)[vector_iter];
				vector_iter++;
				//printf("%d ", a[i][l]);
			}
			l++;
		}
	}
}

void dump_to_file(Matrix* m, string filename) {
	ofstream myfile;
	myfile.open(filename);
	myfile << m->height << "x" << m->width << "\n";
	for (int i = 0; i < m->height; i++) {
		for (int k = 0; k < m->width; k++) {
			myfile << m->values[i][k] << " ";
		}
		myfile << "\n";
	}
	myfile.close();
}

Matrix* load_from_file(string filename) {

	ifstream myfile(filename);
	string line;
	getline(myfile, line);
	istringstream ss(line);
	int height, width; char x;
	ss >> height >> x >> width;
	Matrix* m = create(height, width);

	for (int i = 0; i < m->height; i++) {
		getline(myfile, line);
		ss = istringstream(line);
		for (int k = 0; k < m->width; k++) {
			int value;
			ss >> value;
			m->values[i][k] = value;
		}
	}

	myfile.close();
	return m;
}

int main(int argc, char *argv[]) {
	_setmode(_fileno(stdout), _O_WTEXT);
	int height=0; char x; int width=0;
	Matrix* m;
	if (argc == 1) { // Если матрица не передана
		m = 0; 
	}
	else { 
		istringstream ss(argv[1]);
		ss >> height >> x >> width; // считываем высоту\ширину
		m = create(height, width); // создаем пустую матрицу
	}

	istringstream ss2;
	if (argc >= 3)
		ss2 = istringstream(argv[2]);
	else
		ss2 = istringstream("");


	for (int i = 0; i < height; i++)
		for (int k = 0; k < width; k++) {
			if (ss2.eof()) { // ввод закончился
				m->values[i][k] = 0; // оставшиеся в 0
			}
			else {
				int value; // устаналвиваем значения по одному
				ss2 >> value >> x;
				m->values[i][k] = value;
			}
		}

	while (true) {
		wcout << L"\n1. Вывести матрицу\n2. Сложить матрицу\n3. Умножить матрицу\n4. Транспонировать матрицу\n5. Сохранить в файл\n6. Загрузить из файла\n7. Сортировать матрицу\n";
		int cmd;
		cin >> cmd;

		if (cmd == 1) { // Распечатать матрицу
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				print(m);
			}
		}
		else if (cmd == 2) { // Сложить матрицы
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				wcout << L"Введите матрицу " << height << L"x" << width << L":\n";
				Matrix* add = read(height, width);
				Matrix* other = sum(height, width, m, add);
				print(other);
			}
		}
		else if (cmd == 3) { // Перемножить матрицы
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				wcout << L"Введите размер матрицы:\n";
				int height; char x; int width;
				cin >> height >> x >> width;
				if (height != m->width) {
					wcout << L"Неверный размер\n";
				}
				else {
					Matrix* other = read(height, width);
					Matrix* res = multiply(m, other);
					print(res);
				}
			}
		}
		else if (cmd == 4) {
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				m = transpose(m);
			}
		}
		else if (cmd == 5) {
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				wcout << L"Укажите название файла:\n";
				string filename;
				cin >> filename;
				ifstream my_file(filename);
				if (my_file)
				{
					wcout << L"Перезаписать файл ? (y, N):";
					char yes;
					cin >> yes;
					if (yes == 'y' || yes == 'Y')
						dump_to_file(m, filename);
				}
				else
					dump_to_file(m, filename);
			}
		}
		else if (cmd == 6) {
			wcout << L"Укажите название файла:\n";
			string filename;
			cin >> filename;
			ifstream my_file(filename);
			if (my_file)
			{
				m = load_from_file(filename);
			}
			else {
				wcout << L"Файла не существует \n";
			}
		}
		else if (cmd == 7) {
			if (m == 0)
				wcout << L"Матрица пустая\n";
			else {
				wcout << L"Выберите порядок сортировки\ns : 🐍\ne : 🐌\na : 🐜\n";
				char s;
				cin >> s;
				if (s == 's')
					snake_sort(m);
				if (s == 'e')
					snail_sort(m);
				if (s == 'a')
					ant_sort(m);
			}
		}

	}
	return 0;
}
