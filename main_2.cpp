#include <iostream>
#include <iomanip>
using namespace std;


// Функция для нахождения НОД двух чисел
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
// Функция для нахождения НОД m чисел с использованием 
int gcd_m(int** numbers, int size) {
    if (size == 0) return 0; // Если нет чисел, возвращаем 0
    int result = numbers[0][1];
    for (int i = 1; i < size; ++i) {
        result = gcd(result, numbers[i][1]);
        if (result == 1) return 1; // Если НОД 1, дальше можно не считать
    }
    return result;
}

int* res_bellman; //для записи результатов работы функции
int* bellman(int**catalog, int m, int Y, int y, int i){
    int* best_x = new int[m+1]();   //0-max ЦФ, дальше количество СрЗИ 
    if (y == 0 or i==-1) return best_x;   //F(0) = 0 
    else{
        int x_max = catalog[i][2], max_f=0, f;
        if (y/catalog[i][1]<catalog[i][2]) x_max = y/catalog[i][1];   //max_x зависит от ограничения!!
        for (int xi = 0; xi<x_max+1; xi++){     //для всех допустимых 0<=xi<=Y/wi
            res_bellman = bellman(catalog, m, Y,y-xi*catalog[i][1],i-1);
            f = catalog[i][0]*xi + res_bellman[0];    //Рекуррентное выражение Беллмана
            if (f>max_f) {                      //ищем max {ci*xi + fi-1(yi - wi*xi)}
                max_f = f;
                best_x[0] = f;                     
                for (int j=1; j<i+1; j++)
                    best_x[j] = res_bellman[j];
                best_x[i+1] = xi;
            }
            delete[] res_bellman;
        }
        return best_x;
    }
    delete[] best_x;
}

int main (){
    int m, Y;
    cout << "Введите количество типов СрЗИ:  ";
    cin >> m;
    cout << "Введите объём финансирования:  ";
    cin >> Y;
    //выделяем динамическую память для каталога 
    int** catalog = new int* [m];
    for (int i=0; i<m; i++)
        catalog[i] = new int[3];
    cout << "Введите эффект применения, стоимость,  ограничение на использование для каждого СрЗИ:\n";
    for (int i=0; i<m; i++){
        cin >> catalog[i][0] >> catalog[i][1] >> catalog[i][2];
    }
    cout<< "\nТаблица исходных данных\n";
    cout << "Тип  Эффект применения  Стоимость Ограничение\n";
    for (int i=0; i<m; i++){
        cout << left << setw(5) << i+1 << setw(19) << catalog[i][0] << setw(10) <<catalog[i][1] << setw(10) <<catalog[i][2] << endl;
    }

    //найдем шаг yi
    int delta_y = gcd_m(catalog, m);

    //проход по этапам
    for (int j = 0; j<m; j++ ){
        cout << "\n" << j+1 << " этап\n";
        cout << "y   F" << j+1 << "(y)  ";
        for (int t=1; t<j+2; t++) cout << "x" << t << " ";
        cout << "\n";
        // находим ЦФ для каждого F(y)
        for (int y = 0; y < Y+1; y++){
            res_bellman = bellman(catalog, m, Y, y, j);
            cout << setw(4) << y << setw(5)<< res_bellman[0] << "  ";
            for (int i=1; i<j+2; i++) cout << setw(3) << res_bellman[i];
            cout << "\n";
            delete[] res_bellman;
        }
    }

    cout << "\nДинамическая шкала = {";
    for (int i = 0; i<m; i++){
        res_bellman = bellman(catalog, m, Y, Y, i);
        cout << res_bellman[0] << " ";    
        delete[] res_bellman;
    }
    cout << "}";

    //удаляем все из динамической памяти
    for (int i=0; i<m; i++)
        delete [] catalog[i];
    delete [] catalog;
}