#include <iostream>
#include <fstream>

using namespace std;

struct Ticket {
    char rut_funcionario[10];
    int day_of_month;
    char time[6];
};

void lectura_text(){
    ifstream file;
    file.open("casoT1/servicios.txt");
    if (!file.is_open()){
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
        };
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
    delete[] p;
    file.close();
};

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
    translate_binary();

    return 0;
}