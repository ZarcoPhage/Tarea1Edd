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
    string rut;
    int tickets_val;
    int tickets_Total;
};
datos* lectura_text(int &number){
    ifstream file;
    file.open("casoT1/servicios.txt");
    if (!file.is_open()){
        cout<<"file damaged"<<endl;
        return NULL;
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
            return NULL;
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
    file.open("casoT1/tickets.dat", ios::binary);
    if(!file.is_open()){
        cout<<"file damaged"<<endl;
        return NULL;
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

void ordenarTicketsPersona(Ticket* datosBinario, int numBin){

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
        cout<<datosBinario[i].rut_funcionario<<"\n";
    };

    delete [] toSort;
};

persona* personasUnicas(Ticket* datosBinary, int numBin, int &cantidadPersonas){
    string rutActual;
    for(int i = 1; i<numBin; i++){
        rutActual = datosBinary[i].rut_funcionario;
        if (rutActual != string(datosBinary[i-1].rut_funcionario)){
            cantidadPersonas++;
        }
    }
    persona* personas = new persona[cantidadPersonas];

    personas[0].rut = string(datosBinary[0].rut_funcionario);

    int posPersonas = 1;

    for(int i = 1; i<numBin; i++){
        rutActual = datosBinary[i].rut_funcionario;
        if (rutActual != string(datosBinary[i-1].rut_funcionario)){
            personas[posPersonas].rut = string(datosBinary[i].rut_funcionario);
            posPersonas++;
        }
    }
    cout << endl;
    for (int j = 0; j<cantidadPersonas; j++){
        cout<<personas[j].rut<<endl;
    }

    return personas;
}

void TicketsTotalesPersonas (persona* personasUnicas, Ticket* datosBinario, int cantidadPersonas, int numBin){
    string rutActual, rutAContar;
    int contadorTicketsPersona;
    for (int i = 0; i<cantidadPersonas; i++){
        rutAContar = personasUnicas[i].rut;
        contadorTicketsPersona = 0;
        for (int j = 0; j<numBin; j++){
            rutActual = string(datosBinario[j].rut_funcionario);
            if (rutAContar == rutActual){
                contadorTicketsPersona++;
            }
        }
        personasUnicas[i].tickets_Total = contadorTicketsPersona;
    }

}

int main(){
    int numBin,numTxt= 0; 
    int cantidadPersonas = 1;   
    datos* datosTxt = lectura_text(numTxt);
    Ticket* datosBinary = leer_bin(numBin);

    ordenarTicketsPersona(datosBinary, numBin);

    persona* personas = personasUnicas(datosBinary, numBin, cantidadPersonas);

    TicketsTotalesPersonas(personas, datosBinary, cantidadPersonas, numBin);

    for(int i = 0; i<cantidadPersonas; i++){
        cout<<personas[i].rut<<endl;
        cout<<personas[i].tickets_Total<<endl;
    }
    //int ticketsInválidosHorario = ticketsFueraHorario(rut, p, numBin, datosTxt, numTxt);

    delete [] datosBinary;
    delete [] datosTxt;

    return 0;
}