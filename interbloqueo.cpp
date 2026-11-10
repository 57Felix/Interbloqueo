#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

using namespace std;

// 4 recursos compartidos
mutex recursos[4];

// Barrera simple para forzar el punto exacto del interbloqueo
mutex mBarrera;
condition_variable cvBarrera;
int hilosListos = 0;

void esperarAmbos()
{
    unique_lock<mutex> lock(mBarrera);
    hilosListos++;

    if (hilosListos == 2)
        cvBarrera.notify_all();
    else
        cvBarrera.wait(lock, [] { return hilosListos == 2; });
}

void procesarEtapa(const string& nombre, int etapa, int& dato)
{
    dato += (etapa + 1) * 10;
    cout << nombre << " procesa etapa " << (etapa + 1)
        << " -> dato = " << dato << endl;

    this_thread::sleep_for(chrono::milliseconds(300));
}

/* =========================================
   OPCION 1: INTERBLOQUEO INTENCIONAL
   ========================================= */

void hiloA_interbloqueo(int dato)
{
    cout << "Hilo A inicia" << endl;

    recursos[0].lock(); // R1
    cout << "Hilo A tomo R1" << endl;
    procesarEtapa("Hilo A", 0, dato);

    recursos[1].lock(); // R2
    cout << "Hilo A tomo R2" << endl;
    procesarEtapa("Hilo A", 1, dato);

 
    esperarAmbos();

    cout << "Hilo A intenta tomar R3..." << endl;
    recursos[2].lock(); 
    cout << "Hilo A tomo R3" << endl;
    procesarEtapa("Hilo A", 2, dato);

    cout << "Hilo A intenta tomar R4..." << endl;
    recursos[3].lock();
    cout << "Hilo A tomo R4" << endl;
    procesarEtapa("Hilo A", 3, dato);

    recursos[3].unlock();
    recursos[2].unlock();
    recursos[1].unlock();
    recursos[0].unlock();
}

void hiloB_interbloqueo(int dato)
{
    cout << "Hilo B inicia" << endl;

    recursos[3].lock(); // R4
    cout << "Hilo B tomo R4" << endl;
    procesarEtapa("Hilo B", 3, dato);

    recursos[2].lock(); // R3
    cout << "Hilo B tomo R3" << endl;
    procesarEtapa("Hilo B", 2, dato);

    
    esperarAmbos();

    cout << "Hilo B intenta tomar R2..." << endl;
    recursos[1].lock(); 
    cout << "Hilo B tomo R2" << endl;
    procesarEtapa("Hilo B", 1, dato);

    cout << "Hilo B intenta tomar R1..." << endl;
    recursos[0].lock();
    cout << "Hilo B tomo R1" << endl;
    procesarEtapa("Hilo B", 0, dato);

    recursos[0].unlock();
    recursos[1].unlock();
    recursos[2].unlock();
    recursos[3].unlock();
}

// SOLUCION SIN INTERBLOQUEO
  

void hiloSeguro(const string& nombre, int dato)
{
    cout << nombre << " inicia" << endl;

    // Ambos hilos usan el MISMO orden global: R1, R2, R3, R4
    recursos[0].lock();
    cout << nombre << " tomo R1" << endl;
    procesarEtapa(nombre, 0, dato);

    recursos[1].lock();
    cout << nombre << " tomo R2" << endl;
    procesarEtapa(nombre, 1, dato);

    recursos[2].lock();
    cout << nombre << " tomo R3" << endl;
    procesarEtapa(nombre, 2, dato);

    recursos[3].lock();
    cout << nombre << " tomo R4" << endl;
    procesarEtapa(nombre, 3, dato);

    cout << nombre << " completo el pipeline" << endl;

    recursos[3].unlock();
    cout << nombre << " libero R4" << endl;

    recursos[2].unlock();
    cout << nombre << " libero R3" << endl;

    recursos[1].unlock();
    cout << nombre << " libero R2" << endl;

    recursos[0].unlock();
    cout << nombre << " libero R1" << endl;
}

int main()
{
    int opcion;

    cout << "1. Simular interbloqueo" << endl;
    cout << "2. Ejecutar solucion sin interbloqueo" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    if (opcion == 1)
    {
        hilosListos = 0;

        thread t1(hiloA_interbloqueo, 100);
        thread t2(hiloB_interbloqueo, 200);

        this_thread::sleep_for(chrono::seconds(4));

        cout << "\n === OBSERVACION ===" << endl;
        cout << "Si la ejecucion se detiene en:" << endl;
        cout << "Hilo A intenta tomar R3..." << endl;
        cout << "Hilo B intenta tomar R2..." << endl;
        cout << "entonces el interbloqueo ya se produjo." << endl;
       
        t1.detach();
        t2.detach();
    }
    else if (opcion == 2)
    {
        thread t1(hiloSeguro, "Hilo A", 100);
        thread t2(hiloSeguro, "Hilo B", 200);

        t1.join();
        t2.join();

        cout << "\nEjecucion finalizada sin interbloqueo." << endl;
    }
    else
    {
        cout << "Opcion invalida." << endl;
    }

    return 0;
}