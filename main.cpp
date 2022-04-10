#include <iostream>
#include <fstream>

using namespace std;

struct Ticket {
    char rut_funcionario[10];
    int day_of_month;
    char time[6];
};

struct datos{
    string dia;
    int ticket1;
    int ticket2;
    string hora1;
    string hora2;
};

void lectura_text(){
    ifstream file;
    file.open("casoT1/servicios.txt");
    if (!file.is_open()){
        cout<<"file damaged"<<endl;
        return;
    }
    string stringLine;
    int num;

    while(getline(file, stringLine)){
        cout<< stringLine<<endl;
        num = stoi(stringLine);

        if (num>1){
            cout << "numero del txt: "<<num<<endl;
            break;
        }
        else{
            return;
        }
    }
    cout<<"nuevo ciclo"<<endl;
    datos* p = new datos[num];
    string* p2 = new string[num];
    while (getline(file, stringLine)){
        cout<< stringLine << endl;

    }
    delete [] p;
    file.close();
};

void leer_bin(){
    ifstream file;
    file.open("casoT1/tickets.dat", ios::binary);
    if(!file.is_open()){
        cout<<"file damaged"<<endl;
        return;
    }
    int num;
    file.read((char*) &num, sizeof(int));
    cout<<num<<endl;
    Ticket* p = new Ticket[num];
    while(file.read((char*) p, sizeof(Ticket)));
    cout<< p[0].rut_funcionario<<endl;

    delete [] p;
    file.close();
    return;
}

int main(){
    leer_bin();
    lectura_text();
    return 0;
}