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
        number = stoi(stringLine);

        if (number>1){
            //cout << "numero del txt: "<<number<<endl;
            break;
        }
        else{
            return NULL;
        }
    }

    datos* p = new datos[number];
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
        rut = string(datosBinario[i].rut_funcionario);
        toSort[i] = stoi(rut.substr(0,8));
    }

    heapSort(datosBinario, toSort, numBin);

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

bool verificarValHora(int horaTicket, int minTicket, int horaInicioServ, int minInicioServ, int horaFinServ, int minFinServ){
    if (horaTicket == horaInicioServ){ //si horas son iguales, comparamos minutos
        if (minTicket >= minInicioServ){ //si el ticket se emitió posterior a la hora de inicio, comparar si está dentro del rango de horas de fin 
            return true;
        }
    }
    if (horaTicket > horaInicioServ){   //si el ticket se emitió posterior a la hora de inicio, comparar si está dentro del rango de horas de fin
        if (horaTicket == horaFinServ){ //si las horas son iguales, comparamos minutos
            if (minTicket <= minFinServ){ //si el ticket se emitió antes de la hora del fin del servicio
                return true;
            }
        }
        if (horaTicket < horaFinServ){ //si el ticket se emitió antes de la hora de fin de servicio
            return true;
        }
    }
    return false;
}

void ticketsValidos (persona* personasUnicas, Ticket* datosBinario, datos* datosServicios, int cantidadPersonas, int numBin, int numTxt){
    //cout<<"----FUNCION TICKETS VALIDOS----"<<endl;
    int horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ, contadorTicketsVal;
    string rutPersonaUnica, tiempoTicket, tiempoInicioServ, tiempoFinServ;
    for(int posPers = 0; posPers < cantidadPersonas; posPers++){
        //cout<<"persona unica numero "<<posPers+1<<": "<<personasUnicas[posPers].rut<<endl;
        rutPersonaUnica = personasUnicas[posPers].rut;
        //cout<<"TICKETS DE ESTA PERSONA:"<<endl;
        contadorTicketsVal = 0;
        for(int posTicketsBin = 0; posTicketsBin<numBin; posTicketsBin++){
            if (rutPersonaUnica == string(datosBinario[posTicketsBin].rut_funcionario)){
                //cout<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                //cout<<datosBinario[posTicketsBin].day_of_month<<endl;
                tiempoTicket = string(datosBinario[posTicketsBin].time);
                //cout<<tiempoTicket<<endl;
                horaTicket = stoi(tiempoTicket.substr(0,2));
                minTicket = stoi(tiempoTicket.substr(3,5));
                //cout<<horaTicket<<endl;
                //cout<<minTicket<<endl;
                for (int posServ = 0; posServ<numTxt; posServ++){
                    //cout<<datosServicios[posServ].nombreServicio<<endl;

                    tiempoInicioServ = datosServicios[posServ].hora1;
                    tiempoFinServ = datosServicios[posServ].hora2;

                    horaInicioServ = stoi(tiempoInicioServ.substr(0,2));
                    //cout<<"HORA INICIO: "<<horaInicioServ<<endl;
                    minInicioServ = stoi(tiempoInicioServ.substr(3,5));
                    //cout<<"MIN INICIO: "<<minInicioServ<<endl;

                    horaFinServ = stoi(tiempoFinServ.substr(0,2));
                    //cout<<"HORA FIN: "<<horaFinServ<<endl;
                    minFinServ = stoi(tiempoFinServ.substr(3,5));
                    //cout<<"MIN FIN: "<<minFinServ<<endl;

                    if (horaInicioServ > horaFinServ){
                        //cout<<"servicio que pasa la medianoche"<<endl;
                        if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                            //cout<<"**VALIDO**"<<endl;
                            contadorTicketsVal++;
                            break;
                        }
                    }
                    else if (horaInicioServ < horaFinServ){
                        //cout<<"servicio normal"<<endl;
                        if (verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                            //cout<<"**VALIDO**"<<endl;
                            contadorTicketsVal++;
                            break;
                        }
                    }

                }
            }
        }
        personasUnicas[posPers].tickets_val = contadorTicketsVal;
    }

    for (int i = 0; i<cantidadPersonas; i++){
        //cout<<personasUnicas[i].rut<<endl;
        //cout<<personasUnicas[i].tickets_val<<endl;
        //cout<<personasUnicas[i].tickets_Total<<endl;
    }
    //cout<<"----FIN FUNCION TICKETS VALIDOS----"<<endl;
}

int main(){
    int numBin,numTxt= 0;
    int cantidadPersonas = 1;   
    datos* datosTxt = lectura_text(numTxt);
    Ticket* datosBinary = leer_bin(numBin);

    ordenarTicketsPersona(datosBinary, numBin);

    persona* personas = personasUnicas(datosBinary, numBin, cantidadPersonas);

    TicketsTotalesPersonas(personas, datosBinary, cantidadPersonas, numBin);

    ticketsValidos(personas, datosBinary, datosTxt, cantidadPersonas, numBin, numTxt);

    for (int posPersonas = 0; posPersonas<cantidadPersonas;posPersonas++){
        cout<<personas[posPersonas].rut<<" "<<personas[posPersonas].tickets_val<<"/"<<personas[posPersonas].tickets_Total<<"\n";
    }
    delete [] datosBinary;
    delete [] datosTxt;

    return 0;
}