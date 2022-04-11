#include <iostream>
#include <fstream>

using namespace std;

struct Ticket {
    char rut_funcionario[10];
    int day_of_month;
    char time[6];
};

struct datos{
    string nombreServicio;
    int ticketDiario;
    int ticketMensual;
    string hora1;
    string hora2;
};

datos* lectura_text(){
    ifstream file;
    file.open("casoT1/servicios.txt");
    datos* null = new datos;
    if (!file.is_open()){
        cout<<"file damaged"<<endl;
        return null;
    }
    string stringLine;
    int num;

    while(getline(file, stringLine)){
        //cout<< stringLine<<endl;
        num = stoi(stringLine);

        if (num>1){
            cout << "numero del txt: "<<num<<endl;
            break;
        }
        else{
            return null;
        }
    }
    cout<<"nuevo ciclo"<<endl;
    datos* p = new datos[num];
    //string* p2 = new string[num];
    int i = 0;
    while (!file.eof()){
        file >> p[i].nombreServicio;
        file >> p[i].ticketDiario;
        file >> p[i].ticketMensual;
        file >> p[i].hora1;
        file >> p[i].hora2;
        i++;

    }
    file.close();
    return p;
};


Ticket* leer_bin(int &number){
    ifstream file;
    Ticket* null = new Ticket;
    file.open("casoT1/tickets.dat", ios::binary);
    if(!file.is_open()){
        cout<<"file damaged"<<endl;
        return null;
    }
    file.read((char*) &number, sizeof(int));
    cout<<number<<endl;
    Ticket* p = new Ticket[number];
    file.read((char*) p, sizeof(Ticket)*number);
    file.close();
    return p;
}

void translate_binary(){
    Ticket tickets;
    int totalTickets;
    ifstream binaryFile;
    ofstream textFile;
    binaryFile.open("casoT1/tickets.dat", ios::binary);
    if (!binaryFile.is_open()){
        cout<<"Binary file is damaged or missing"<<endl;
        return;
    }; 
    textFile.open("casoT1/textTickets.txt");
    if (!textFile.is_open()){
        cout<<"New file is damaged"<<endl;
        return;
    }

    binaryFile.read((char*) &totalTickets, sizeof(int)); //pointer to memory address where the value will be stored, and byte size of the value
    textFile << totalTickets << endl;
    binaryFile.seekg(1*sizeof(int)); //repositioning cursor inside the file

    while (binaryFile.read((char*) &tickets, sizeof(Ticket))){
        textFile << tickets.rut_funcionario << " " <<tickets.day_of_month<< " " << tickets.time << endl;

    };

    binaryFile.close();
    textFile.close();
}

int main(){
    int num = 0;
    datos* datosTxt = lectura_text();
    Ticket* p = leer_bin(num);

    for (int i = 0; i<int(sizeof(datosTxt)-2); i++){
        cout<<datosTxt[i].nombreServicio<<endl;
        cout<<datosTxt[i].ticketDiario<<endl;
        cout<<datosTxt[i].ticketMensual<<endl;
        cout<<datosTxt[i].hora1<<endl;
        cout<<datosTxt[i].hora2<<endl;
    }

    for (int i = 0; i < num; i++){
        cout<<i<<endl;
        cout<< p[i].rut_funcionario<<endl;
        cout<< p[i].day_of_month<<endl;
        cout<< p[i].time << endl;
    }
    delete [] p;
    delete [] datosTxt;

    return 0;
}