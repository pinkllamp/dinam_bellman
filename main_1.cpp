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


int* bellman(int**catalog, int m, int Y, int y, int i){
    int* best_x = new int[m+1]();   //0-max ЦФ, дальше количество СрЗИ 
    if (y == 0 or i==-1) return best_x;   //F(0) = 0 
    else{
        int x_max = y/catalog[i][1], max_f=0, f;
        for (int xi = 0; xi<x_max+1; xi++){     //для всех допустимых 0<=xi<=Y/wi
            int* tec_x = bellman(catalog, m, Y,y-xi*catalog[i][1],i-1);
            f = catalog[i][0]*xi + tec_x[0];    //Рекуррентное выражение Беллмана
            if (f>max_f) {                      //ищем max {ci*xi + fi-1(yi - wi*xi)}
                max_f = f;
                best_x[0] = f;                     
                for (int j=1; j<i+1; j++)
                    best_x[j] = tec_x[j];
                best_x[i+1] = xi;
            }
            delete[] tec_x;
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
        catalog[i] = new int[2];
    cout << "Введите эффект применения и стоимость для каждого СрЗИ:\n";
    for (int i=0; i<m; i++){
        cin >> catalog[i][0] >> catalog[i][1];
    }
    cout<< "\nТаблица исходных данных\n";
    cout << "Тип  Эффект применения  Стоимость\n";
    for (int i=0; i<m; i++){
        cout << left << setw(5) << i+1 << setw(19) << catalog[i][0] << setw(6) <<catalog[i][1] << endl;
    }
 
    //найдем шаг yi
    int delta_y = gcd_m(catalog, m);

    int* x_bell;    //для записи результатов функции
    //проход по этапам
    for (int j = 0; j<m; j++ ){
        cout << "\n" << j+1 << " этап\n";
        cout << "y   F" << j+1 << "(y)  ";
        for (int t=1; t<j+2; t++) cout << "x" << t << " ";
        cout << "\n";
        // находим ЦФ для каждого F(y)
        for (int y = 0; y < Y+1; y+=delta_y){
            x_bell = bellman(catalog, m, Y, y, j);
            cout << setw(4) << y << setw(5)<< x_bell[0] << "  ";
            for (int i=1; i<j+2; i++) cout << setw(3) << x_bell[i];
            cout << "\n";
            delete[] x_bell;
        }
    }

    cout << "\nДинамическая шкала = {";
    for (int i = 0; i<m; i++){
        x_bell = bellman(catalog, m, Y, Y, i);
        cout << x_bell[0] << " ";    
        delete[] x_bell;
    }
    cout << "}";

    //удаляем все из динамической памяти
    for (int i=0; i<m; i++)
        delete [] catalog[i];
    delete [] catalog;
}