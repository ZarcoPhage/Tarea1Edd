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

struct persona {
    char rut[10];
    int tickets_val;
    int tickets_inval;
};
datos* lectura_text(int &number){
    ifstream file;
    file.open("casoT1/servicios.txt");
    datos* null = new datos;
    if (!file.is_open()){
        cout<<"file damaged"<<endl;
        return null;
    }
    string stringLine;

    while(getline(file, stringLine)){
        //cout<< stringLine<<endl;
        number = stoi(stringLine);

        if (number>1){
            cout << "numero del txt: "<<number<<endl;
            break;
        }
        else{
            return null;
        }
    }
    cout<<"nuevo ciclo"<<endl;
    datos* p = new datos[number];
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

void heapy(Ticket* arregloOriginalHeap, int* arregloAuxHeap, int tamanoArreglo, int i){
    int mayorLongitud = i; 
    int izq = 2*i + 1; 
    int der = 2*i + 2;

    if (izq<tamanoArreglo && arregloAuxHeap[izq] > arregloAuxHeap[mayorLongitud]){
        mayorLongitud = izq;
    };
    if (der<tamanoArreglo && arregloAuxHeap[der]>arregloAuxHeap[mayorLongitud]){
        mayorLongitud = der;
    };
    if (mayorLongitud != i){
        swap(arregloAuxHeap[i],arregloAuxHeap[mayorLongitud]);
        swap(arregloOriginalHeap[i], arregloOriginalHeap[mayorLongitud]);
        heapy(arregloOriginalHeap, arregloAuxHeap, tamanoArreglo, mayorLongitud);
    }
}   

void heapSort(Ticket* arregloOriginal, int* arregloAux, int tamanoArreglo){
    for (int i = tamanoArreglo / 2 - 1; i >= 0; i--){ //crea un heap, reestructurando el arreglo
        heapy(arregloOriginal, arregloAux, tamanoArreglo, i);
    };

    for (int i = tamanoArreglo - 1; i > 0; i--){ //extrae los elementos uno a uno del heap
        swap(arregloAux[0],arregloAux[i]);
        swap(arregloOriginal[0], arregloOriginal[i]);
        heapy(arregloOriginal, arregloAux, i, 0);
    };
};

int contarTicketsPersona(Ticket* datosBinario, int numBin){
    int contadorPersonas = 0;
    bool encontrado;

    int* toSort = new int[numBin];
    string rut;
    
    for (int i = 0; i<numBin; i++){ //crea un arreglo auxiliar que ayudará a ordenar los ruts
        cout<<"rut: "<<datosBinario[i].rut_funcionario<<endl;
        rut = string(datosBinario[i].rut_funcionario);
        cout<<rut<<endl;
        toSort[i] = stoi(rut.substr(0,8));
        cout<<toSort[i]<<endl;
    }

    heapSort(datosBinario, toSort, numBin);

    for (int i = 0; i<numBin; i++){
        cout<<toSort[i]<<endl;
        cout<<datosBinario[i].rut_funcionario<<endl;
    };
    cout<<contadorPersonas<<endl;
    
    return contadorPersonas;
};


int main(){
    int numBin,numTxt = 0;
    int ticketsPersona;
    datos* datosTxt = lectura_text(numTxt);
    Ticket* p = leer_bin(numBin);

    for(int i = 0; i<numBin; i++){
        cout<<p[i].rut_funcionario<<endl;
    }
    ticketsPersona = contarTicketsPersona(p, numBin);

    //int ticketsInválidosHorario = ticketsFueraHorario(rut, p, numBin, datosTxt, numTxt);

    delete [] p;
    delete [] datosTxt;

    return 0;
}