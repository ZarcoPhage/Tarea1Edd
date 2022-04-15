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
bool verificarHora (int horaTicket, int minTicket, int horaInicioServ, int minInicioServ, int horaFinServ, int minFinServ){
    if (horaTicket == horaInicioServ){ //si la hora es igual, comparamos minutos
        if(minTicket >= minInicioServ){
            if (horaTicket == horaFinServ){ //misma logica pero para el final del servicio
                if (minTicket <= minFinServ){
                    return true;
                    }
            }
            if (horaTicket < horaFinServ){
                return true;
                }
        }
    }
    else if (horaTicket > horaInicioServ){
        if (horaTicket == horaFinServ){
            if (minTicket <= minFinServ){
                return true;
            }
        }
        if (horaTicket < horaFinServ){
            return true;
        }
    }
    else{ 
        return false;
    }
    return false;
}


void ticketsValidos (persona* personasUnicas, Ticket* datosBinario, datos* datosServicios, int cantidadPersonas, int numBin, int numTxt){
    int contadorLimDiario, contadorLimMensual, limDiario, limMensual, ticketsValidosTotales, horaInicioServ, minInicioServ, horaFinServ, minFinServ, horaTicket, minTicket;
    string rutAnalizar, servicioAnalizar, strHoraInicio, strHoraFin, strHoraTicket;
    for(int i = 0; i<cantidadPersonas; i++){
        rutAnalizar = personasUnicas[i].rut; //lee cada persona y entra su rut al ciclo que analizará los tickets de aquel rut
        cout<<"analizando rut: "<<rutAnalizar<<endl;
        ticketsValidosTotales = 0;
        for(int j = 0; j<numBin; j++){
            cout<<"analizando ticket nro: "<<j+1<<" correspondiente a rut: "<< datosBinario[j].rut_funcionario<<endl;
            if (rutAnalizar == datosBinario[j].rut_funcionario){ //toma los tickets del rut a analizar
                contadorLimMensual = 0; 
                for (int dias = 1; dias<=31; dias++){ //analiza los tickets del mes
                    cout<<"dia "<<dias<<" del mes"<<endl;
                    if (datosBinario[j].day_of_month == dias){ //analiza un dia especifico del mes
                        contadorLimDiario = 0;
                        for(int l = 0; l<numTxt; l++){
                            servicioAnalizar = datosServicios[l].nombreServicio; 
                            strHoraInicio = datosServicios[l].hora1; //de aqui hacia abajo es solo string handling para sacar las horas y minutos y comparar
                            strHoraFin = datosServicios[l].hora2;
                            strHoraTicket = string(datosBinario[j].time);
                            horaInicioServ = stoi(strHoraInicio.substr(0,2));
                            minInicioServ = stoi(strHoraInicio.substr(3,5));
                            horaFinServ = stoi(strHoraFin.substr(0,2));
                            minFinServ = stoi(strHoraFin.substr(3,5));
                            horaTicket = stoi(strHoraTicket.substr(0,2));
                            minTicket = stoi(strHoraTicket.substr(3,5));
                            limDiario = datosServicios[l].ticketDiario;
                            limMensual = datosServicios[l].ticketMensual;
                            if (ticketsValidosTotales <= 100){
                                cout<<"entra al primero"<<endl;
                                if (horaInicioServ > horaFinServ){
                                    cout<<"entra al segundo"<<endl;
                                    if (verificarHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                        ticketsValidosTotales++;
                                        cout<<"SISISISISISI"<<endl;
                                    }
                                if (horaInicioServ < horaFinServ){
                                    cout<<"entra al segundo"<<endl;
                                    if (verificarHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                        ticketsValidosTotales++;
                                        cout<<"SISISISI"<<endl;
                                    }
                                }
                            }                            
                        }
                    } 
                    
                    }
                }
            }
        }
    personasUnicas[i].tickets_val = ticketsValidosTotales;
    cout<<"PERSONITA: "<<personasUnicas[i].rut<<endl<<"sus tickets: "<<personasUnicas[i].tickets_val<<endl;
    }}


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

    ticketsValidos(personas, datosBinary, datosTxt, cantidadPersonas, numBin, numTxt);
    delete [] datosBinary;
    delete [] datosTxt;

    return 0;
}