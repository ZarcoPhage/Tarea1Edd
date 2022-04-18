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


/*****
*   Puntero struct datos  lectura_text
******
*   recupera los datos del archivo de servicios y los pasa a un arreglo de una estructura previamente declarada
    de tipo nombre "datos", la cual permite almacenar el nombre de un servicio, su limite diario, su limite mensual y las horas de inicio y final,
    la función lee el archivo y realiza una lectura por formato para obtener los datos y guardarlos en un arreglo dinámico con el numero rescatado
    del archivo de texto, que posteriormente se retorna.

******
*   Input:
*       dirección de memoria de un entero  number : permite guardar el numero que se recupera en el archivo de servicios para futuro uso
******
*   Return:
*       puntero estructura datos, retorna el arreglo dinámico con los datos de los servicios que se encuentran en el archivo servicios.tx

*****/

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

/*****
*   Puntero struct ticket  leer_bin
******
*   lee el archivo binario y almacena los datos relacionados en un arreglo dinámico de tipo Ticket que posteriormente se retorna.
    además, la función modifica el valor contenido en la dirección de memoria del parámetro que recibe y almacena el valor del entero
    contenido en el archivo binario para posterior uso 
******
*   Input:
*       dirección de memoria de un numero entero  number : permite guardar el numero entero almacenado en el archivo binario para su futuro uso
******
*   Returns:
*       puntero de estructura Ticket, retorna un arreglo dinámico de la estructura Ticket, con los datos almacenados en el binario, para su posterior procesamiento

*****/

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

/*****
*   void heapy
******
*   construye un maxHeap, un arbol binario con propiedad de que el nodo padre/raiz sea mayor a sus nodos hijos, intercambiando posiciones en el arreglo y construyendo el heap en el proceso,
*   cuando se produzca un cambio de posiciones en heap, también se producirá un cambio en posiciones de los arreglos de Tickets y el arreglo auxiliar,
*   posteriormente se llama recursivamente a la función para contruir heaps y nodos hijos, al terminar de construirse el maxHeap, se ordenan los arreglos de forma correcta a través de la función heapsort
******
*   Input:
*       Ticket* arregloOriginalHeap : arreglo original con los datos de los tickets emitidos en el mes, es decir, información relacionada a los datos del archivo binario
*       int* arregloAuxHeap : arreglo de ruts acortados que funciona como arreglo de numeros enteros con las mismas posiciones del arreglo original de los Tickets para su ordenamiento y la construcción del maxHeap
*       int tamanoArreglo : tamano del arreglo auxiliar y el arreglo original de los tickets, es decir, la cantidad total de tickets emitidos en el mes
*       int i : iteración del ciclo y posición del arreglo con el que se llamó a la función
*****/
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

/*****
*   void heapSort
******
*   toma el arreglo original de los datos del binario y el arreglo auxiliar de los ruts de los funcionarios y crea un Heap para ordenar los datos presentes en los arreglos,
*   luego extrae uno a uno los elementos del heap creado con los elementos del arreglo y realiza cambios en las posiciones del arreglo auxiliar y a la vez al arreglo de tickets, ordenando
*   lexicográficamente el arreglo original y el auxiliar
******
*   Input:
*       Ticket* arregloOriginal : arreglo relacionado a los datos recuperados del binario, con la información de los tickets
*       int* arregloAux : arreglo auxiliar con los ruts acortados respecto al arreglo relacionado a los tickets
*       int tamanoArreglo : tamano total del arreglo que se pretende ordenar, en este caso, la cantidad de Tickets totales emitidos en el mes
*****/
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

/*****
*   void ordenarTicketsPersona
******
*   crea un arreglo dinámico auxiliar con la parte entera de los ruts de cada funcionario (digitos previos al digito verificador, ya que este es unico para cada rut), y con este arreglo se llama al algoritmo de ordenamiento,
*   asociando las posiciones de este arreglo auxiliar a las posiciones del arreglo de los datos recuperados del archivo binario, permitiendo ordenar ambos arreglos en orden lexicográfico
******
*   Input:
*       Ticket* datosBinario : arreglo que contiene los datos rescatados del archivo binario, es decir, información de todos los tickets emitidos del mes
*       int numBin : numero entero rescatado del archivo binario que indica el total de tickets emitidos en el mes    
*****/

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

/*****
*   persona* personasUnicas
******
*   detecta cada funcionario único dentro de los datos de tickets emitidos, cuenta los funcionarios totales y registra a cada uno en un arreglo dinámico de
*   una estructura de tipo "persona", que contiene el rut de cada funcionario y permitirá almacenar sus tickets validos y totales
******
*   Input:
*       Ticket* datosBinary : arreglo con los datos rescatados del archivo binario, es decir, la información de los tickets emitidos durante el mes
*       int numBin : numero entero rescatado del archivo binario, referente a la cantidad total de tickets emitidos en general
*       int &cantidadPersonas : dirección de memoria de un entero previamente declarado que permitirá almacenar la cantidad total de funcionarios unicos que se detecten en los datos del archivo binario 
*****
*   Return:
*       persona*, arreglo dinámico que contiene cada rut de cada funcionario único detectado en los datos de los tickets, el cual permite guardar los tickets validos y totales de cada funcionario
*****/

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

/*****
*   void TicketsTotalesPersonas
******
*   proceso que cuenta los tickets totales emitidos por cada funcionario único y almacena aquello en la casilla de "tickets totales" de una posición de un arreglo previamente declarado de tipo "personas", modificando
*   aquel arreglo, el cual contendrá los datos de cada persona (rut, tickets válidos y tickets totales) para el retorno final del programa
******
*   Input:
*       persona* personasUnicas : arreglo que contiene datos de cada funcionario unico existente detectado previamente entre los datos de los tickets emitidos
*       Ticket* datosBinario : arreglo que contiene los datos recuperados en el archivo binario tickets.dat, el cual contiene información acerca de todos los tickets emitidos
*       int cantidadPersonas : cantidad de funcionarios unicos detectados previamente en el archivo binario con con la informacion de los tickets emitidos
*       int numBin : cantidad total de tickets emitidos, numero entero recuperado del archivo binario con la información de los tickets emitidos
*****/

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

/*****
*   bool VerificiarValHora
******
*   permite verificar la validez de un ticket respecto a las horas de un servicio, verificando si el ticket fue emitido entre el rango de horas de un servicio
******
*   Input:
*       int horaTicket : hora en la que fue emitido un ticket
*       int minTicket : minuto de cierta hora en que fue emitido un ticket
*       int horaInicioServ : hora de inicio de cierto servicio que se haya conseguido en el archivo Servicios.txt
*       int minInicioServ : minuto de cierta hora en la que inicia el servicio
*       int horaFinServ : hora de fin de cierto servicio que se haya conseguido en el archivo Servicios.txt
*       int minFinServ : minuto de la hora en que finaliza el servicio
******
*   Return:
*       bool, retorna un valor booleano referente a la validez de un ticket, si el ticket es válido con respecto a las horas de un servicio, retorna un valor verdadero, si el ticket fue emitido fuera de hora, retorna un valor falso
*****/

bool verificarValHora(int horaTicket, int minTicket, int horaInicioServ, int minInicioServ, int horaFinServ, int minFinServ){
    if (horaTicket == horaInicioServ){ //si horas son iguales, comparamos minutos
        if (minTicket >= minInicioServ){ //si el ticket se emitió posterior a la hora de inicio, comparar si está dentro del rango de horas de fin 
            //cout<<"VALIDO-----------------------------------------------------------------"<<endl;
            return true;
        }
    }
    if (horaTicket > horaInicioServ){   //si el ticket se emitió posterior a la hora de inicio, comparar si está dentro del rango de horas de fin
        if (horaTicket == horaFinServ){ //si las horas son iguales, comparamos minutos
            if (minTicket <= minFinServ){ //si el ticket se emitió antes de la hora del fin del servicio
                //cout<<"VALIDO-----------------------------------------------------------------"<<endl;
                return true;
            }
        }
        if (horaTicket < horaFinServ){ //si el ticket se emitió antes de la hora de fin de servicio
            //cout<<"VALIDO-----------------------------------------------------------------"<<endl;
            return true;
        }
    }
    return false;
}
/*****
*   void ticketsValidos
******
*   proceso que establece y cuenta los tickets validos de una persona respecto a las horas en que fue emitido, valida si la cantidad de tickets emitidos por una persona supera el limite diario de un servicio
*   o si supera el limite mensual del mismo servicio, y posteriormente guarda esta información en el arreglo que contiene los datos de cada funcionario único, sus tickets totales y sus tickets válidos
******
*   Input:
*       persona* personasUnicas : arreglo que contiene la información (rut, tickets totales y tickets validos) de cada funcionario único detectado en el archivo binario
*       Ticket* datosBinario : arreglo que contiene los datos recuperados del archivo binario, es decir, todos los tickets emitidos durante el mes
*       datos* datosServicios : arreglo que contiene la información recuperada del archivo de texto, es decir, los datos acerca de cada cada servicio (nombre, limite diario de tickets, limite mensual de tickets y rango horario)
*       int cantidadPersonas : cantidad de funcionarios/ruts únicos detectados en los tickets emitidos durante el mes
*       int numBin : numero total de tickets emitidos durante el mes, numero entero rescatado del archivo binario
*       int numTxt : numero total de servicios que se ofrecen, numero entero rescatado del archivo de texto        
*****/
void ticketsValidos (persona* personasUnicas, Ticket* datosBinario, datos* datosServicios, int cantidadPersonas, int numBin, int numTxt){
    cout<<"----FUNCION TICKETS VALIDOS----"<<endl;
    int horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ, contadorTicketsVal, contLimMens, limMensual, contLimDiario, limDiario, contInval;
    string rutPersonaUnica, tiempoTicket, tiempoInicioServ, tiempoFinServ;
    for(int posPers = 0; posPers < cantidadPersonas; posPers++){
        cout<<"persona unica numero "<<posPers+1<<": "<<personasUnicas[posPers].rut<<endl;
        rutPersonaUnica = personasUnicas[posPers].rut;
        cout<<"TICKETS DE ESTA PERSONA:"<<endl;
        contadorTicketsVal = 0;
        for(int posTicketsBin = 0; posTicketsBin<numBin; posTicketsBin++){
            if (rutPersonaUnica == string(datosBinario[posTicketsBin].rut_funcionario)){
                cout<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                cout<<datosBinario[posTicketsBin].day_of_month<<endl;
                tiempoTicket = string(datosBinario[posTicketsBin].time);
                cout<<tiempoTicket<<endl;
                horaTicket = stoi(tiempoTicket.substr(0,2));
                minTicket = stoi(tiempoTicket.substr(3,5));
                cout<<horaTicket<<endl;
                cout<<minTicket<<endl;
                for (int posServ = 0; posServ<numTxt; posServ++){
                    cout<<datosServicios[posServ].nombreServicio<<endl;
                    tiempoInicioServ = datosServicios[posServ].hora1;
                    tiempoFinServ = datosServicios[posServ].hora2;

                    horaInicioServ = stoi(tiempoInicioServ.substr(0,2));
                    cout<<"HORA INICIO: "<<horaInicioServ<<endl;
                    minInicioServ = stoi(tiempoInicioServ.substr(3,5));
                    cout<<"MIN INICIO: "<<minInicioServ<<endl;

                    horaFinServ = stoi(tiempoFinServ.substr(0,2));
                    cout<<"HORA FIN: "<<horaFinServ<<endl;
                    minFinServ = stoi(tiempoFinServ.substr(3,5));
                    cout<<"MIN FIN: "<<minFinServ<<endl;

                    if (horaInicioServ > horaFinServ){
                        cout<<"servicio que pasa la medianoche"<<endl;
                        if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                            cout<<"**VALIDO*-----------------------------------------------------*"<<endl;
                            contadorTicketsVal++;
                            break;
                        }
                    }
                    else if (horaInicioServ < horaFinServ){
                        cout<<"servicio normal"<<endl;
                        if (verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                            cout<<"**VALIDO*----------------------------------------------------*"<<endl;
                            contadorTicketsVal++;
                            break;
                        }
                    }

                }
            }
        }
        personasUnicas[posPers].tickets_val = contadorTicketsVal;
    }

    for (int posServ = 0; posServ<numTxt; posServ++){
        cout<<"------------------"<<datosServicios[posServ].nombreServicio<<"-----------------"<<endl;

        tiempoInicioServ = datosServicios[posServ].hora1;
        tiempoFinServ = datosServicios[posServ].hora2;
            
        horaInicioServ = stoi(tiempoInicioServ.substr(0,2));
        cout<<"HORA INICIO: "<<horaInicioServ<<endl;
        minInicioServ = stoi(tiempoInicioServ.substr(3,5));
        cout<<"MIN INICIO: "<<minInicioServ<<endl;

        horaFinServ = stoi(tiempoFinServ.substr(0,2));
        cout<<"HORA FIN: "<<horaFinServ<<endl;
        minFinServ = stoi(tiempoFinServ.substr(3,5));
        cout<<"MIN FIN: "<<minFinServ<<endl;
            

        limDiario = datosServicios[posServ].ticketDiario;
        for (int posPersonas = 0; posPersonas < cantidadPersonas; posPersonas++){

            cout<<personasUnicas[posPersonas].rut<<endl;
            contInval = 0;
            for (int dia = 1; dia<=31;dia++){
                contLimDiario = 0;
                cout<<"dia numero "<<dia<<" del MES"<<endl;
                for (int posTicketsBin = 0; posTicketsBin < numBin; posTicketsBin++){
                    //cout<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                    //cout<<"dia del TICKET: "<<datosBinario[posTicketsBin].day_of_month<<endl;
                    if (personasUnicas[posPersonas].rut == datosBinario[posTicketsBin].rut_funcionario && dia == datosBinario[posTicketsBin].day_of_month){
                        tiempoTicket = datosBinario[posTicketsBin].time;
                        cout<<"**RUT DE LA PERSONA:**"<<personasUnicas[posPersonas].rut<<endl;
                        cout<<"**RUT DEL TICKET:**"<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                        cout<<"**TIEMPO DEL TICKET:**"<<tiempoTicket<<endl;
                        horaTicket = stoi(tiempoTicket.substr(0,2));
                        minTicket = stoi(tiempoTicket.substr(3,5));
                        if (horaInicioServ > horaFinServ){
                            if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                cout<<contLimDiario<<endl;

                                contLimDiario++;

                                if (contLimDiario > limDiario){
                                    cout<<"INVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDO"<<endl;
                                    contInval++;
                                }
                            }
                        }
                        else if (horaInicioServ < horaFinServ){
                            if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                cout<<contLimDiario<<endl;

                                contLimDiario++;

                                if (contLimDiario > limDiario){
                                    cout<<"INVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDO"<<endl;
                                    contInval++;
                                }
                            }
                        }
                    }
                  }
            }

    for (int posServ = 0; posServ<numTxt; posServ++){
        cout<<"------------------"<<datosServicios[posServ].nombreServicio<<"-----------------"<<endl;

        tiempoInicioServ = datosServicios[posServ].hora1;
        tiempoFinServ = datosServicios[posServ].hora2;
            
        horaInicioServ = stoi(tiempoInicioServ.substr(0,2));
        cout<<"HORA INICIO: "<<horaInicioServ<<endl;
        minInicioServ = stoi(tiempoInicioServ.substr(3,5));
        cout<<"MIN INICIO: "<<minInicioServ<<endl;

        horaFinServ = stoi(tiempoFinServ.substr(0,2));
        cout<<"HORA FIN: "<<horaFinServ<<endl;
        minFinServ = stoi(tiempoFinServ.substr(3,5));
        cout<<"MIN FIN: "<<minFinServ<<endl;
            
        limMensual = datosServicios[posServ].ticketMensual;
        for (int posPersonas = 0; posPersonas < cantidadPersonas; posPersonas++){
            contLimMens = 0;
            cout<<personasUnicas[posPersonas].rut<<endl;
            contInval = 0;
            for (int dia = 1; dia<=31;dia++){
                cout<<"dia numero "<<dia<<" del MES"<<endl;
                for (int posTicketsBin = 0; posTicketsBin < numBin; posTicketsBin++){
                    //cout<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                    //cout<<"dia del TICKET: "<<datosBinario[posTicketsBin].day_of_month<<endl;
                    if (personasUnicas[posPersonas].rut == datosBinario[posTicketsBin].rut_funcionario && dia == datosBinario[posTicketsBin].day_of_month){
                        tiempoTicket = datosBinario[posTicketsBin].time;
                        cout<<"**RUT DE LA PERSONA:**"<<personasUnicas[posPersonas].rut<<endl;
                        cout<<"**RUT DEL TICKET:**"<<datosBinario[posTicketsBin].rut_funcionario<<endl;
                        cout<<"**TIEMPO DEL TICKET:**"<<tiempoTicket<<endl;
                        horaTicket = stoi(tiempoTicket.substr(0,2));
                        minTicket = stoi(tiempoTicket.substr(3,5));
                        if (horaInicioServ > horaFinServ){
                            if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                cout<<contLimDiario<<endl;
                                cout<<contLimMens<<endl;
                                contLimMens++;
                                if (contLimMens > limMensual){
                                    cout<<"INVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDO"<<endl;
                                    contInval++;
                                }
                            }
                        }
                        else if (horaInicioServ < horaFinServ){
                            if(verificarValHora(horaTicket, minTicket, horaInicioServ, minInicioServ, horaFinServ, minFinServ)){
                                cout<<contLimDiario<<endl;
                                cout<<contLimMens<<endl;
                                contLimMens++;
                                if (contLimMens > limMensual){
                                    cout<<"INVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDOINVALIDO"<<endl;
                                    contInval++;
                                }
                            }
                        }
                    }
                  }
            }
            personasUnicas[posPersonas].tickets_val -= contInval;
        }
    }

/*    for (int i = 0; i<cantidadPersonas; i++){
        cout<<personasUnicas[i].rut<<endl;
        cout<<personasUnicas[i].tickets_val<<endl;
        cout<<personasUnicas[i].tickets_Total<<endl;
    }

*/
    cout<<"----FIN FUNCION TICKETS VALIDOS----"<<endl;

} } }

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
