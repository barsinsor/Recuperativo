#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <mpi.h>

using namespace std;

float pendiente (vector<float> x, vector<float> y){
    float sumatoriaxy=0, sumatoriax=0, sumatoriay=0,sumatoriaxquad=0,valorpendiente=0;
    for(int i = 0; i < x.size(); i++){
        sumatoriax+= x[i];
        sumatoriay+= y[i];
        sumatoriaxy += (x[i] * y[i]);
        sumatoriaxquad += + (x[i] * x[i]);
    }
    valorpendiente = (x.size() * sumatoriaxy - sumatoriax * sumatoriay) / ((x.size() * sumatoriaxquad) - (sumatoriax * sumatoriax));
    return valorpendiente;
}
float interseccion (vector<float> x, vector<float> y,float valorpendiente){
    float  sumatoriax=0, sumatoriay=0, valorinterseccion=0;
    for(int i = 0; i < x.size(); i++){
        sumatoriax+= x[i];
        sumatoriay+= y[i];
    }
    valorinterseccion = (sumatoriay - valorpendiente * sumatoriax) / (x.size());
    return valorinterseccion;
}

int main(int argc, char** argv ){
    int mi_rango; 
    int p;
    int fuente; 
    int dest; 
    int tag = 0; 
    char mensaje[100]; 
    MPI_Status estado; 
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (mi_rango != 0) {
        sprintf(mensaje, "Saludos del proceso %d!", mi_rango);
        dest = 0;
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
    } else { 
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }
    MPI_Finalize();


    std::ifstream ubicacionCsv;
    std::string linea = "";
    std::string lectura = "";
    ubicacionCsv.open("/home/arielp/Desktop/trabajo recuperativo/Recuperativo/csv/smi.csv");
    std::vector<int>  anioSMI;
    std::vector<float> valorsmi;

    while(std::getline(ubicacionCsv, linea)){
        if(linea[0]== '"'){


            std::stringstream procesoDeDatos(linea);
            std::getline(procesoDeDatos, lectura, ';');
            std::stringstream datos(lectura);
            std::string auxlectura1(lectura);
            auxlectura1.erase(std::remove(auxlectura1.begin(), auxlectura1.end(), '"'),
            auxlectura1.end());
            anioSMI.push_back(stoi(auxlectura1));

            std::getline(procesoDeDatos, lectura, ';');
            std::stringstream datoss(lectura);
            std::string auxlectura2(lectura);
            auxlectura2.erase(std::remove(auxlectura2.begin(), auxlectura2.end(), '"'),
            auxlectura2.end());
            valorsmi.push_back(stof(auxlectura2));

         }
    }
        
    std::ifstream ubicasionCsv2;
    std::string linea2 = "";
    std::string lectura2 = "";
    ubicasionCsv2.open("/home/arielp/Desktop/trabajo recuperativo/Recuperativo/csv/dollars.csv");
    std::vector<int> anioDolar;  
    std::vector<float> valorDolar; 

    while(std::getline(ubicasionCsv2, linea2)){
        if(linea2[0]== '"'){


            std::stringstream procesoDeDatos(linea2);

            std::getline(procesoDeDatos, lectura2, ';');
            std::stringstream datos(lectura2);
            std::string auxlectura1(lectura2);
            auxlectura1.erase(std::remove(auxlectura1.begin(), auxlectura1.end(), '"'),
            auxlectura1.end());
            auxlectura1.erase(4);
            anioDolar.push_back(stoi(auxlectura1));

            std::getline(procesoDeDatos, lectura2, ';');
            std::stringstream datoss(lectura2);
            std::string auxlectura2(lectura2);
            auxlectura2.erase(std::remove(auxlectura2.begin(), auxlectura2.end(), '"'),
            auxlectura2.end());
            valorDolar.push_back(stof(auxlectura2));

         }  

    }
    int contador=0;
    float sumaDolar=0;
    float promedioAnual=0;
    std::vector<float> promedioAnualDolar, conversionSMIdolar ;
    for(int i = 0;i<anioSMI.size() ;i++){
        // cout << anioDolar[i] << '\n';
        for(int j=0; j< anioDolar.size() ; j++){
            if (anioDolar[j] == anioSMI[i])
            {
                // cout << anioSMI[j] << '\n';
                contador++;
                sumaDolar= sumaDolar + valorDolar[j];
            }

        }
        promedioAnual= sumaDolar/contador;
        promedioAnualDolar.push_back(promedioAnual);
        conversionSMIdolar.push_back(valorsmi[i]/promedioAnual);
        contador=0;
        sumaDolar=0;
            
        
    }
    float m=0,b=0;
    m=pendiente(promedioAnualDolar,conversionSMIdolar);
    b=interseccion(promedioAnualDolar,conversionSMIdolar,m);
    cout<< "la ecuacion es Y = "<< m <<"X + "<<b<< endl;

}
