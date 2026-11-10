# Simulación y Solución de Interbloqueo (Deadlock) en C++

Este proyecto demonstra el concepto de **Interbloqueo (*Deadlock*)** en sistemas operativos mediante programación multihilo en C++11/C++14. Incluye tanto la simulación del bloqueo circular de recursos compartidos como su solución mediante la imposición de un **orden estricto de adquisición de recursos (Prevención de Interbloqueos)**.

---

## 📌 Conceptos Demostrados

1. **Interbloqueo (*Deadlock*):**
   * Sucede cuando dos o más hilos quedan bloqueados indefinidamente esperando por recursos que están retenidos por otros hilos en el mismo ciclo.
   * En este programa, el **Hilo A** retiene $R_1$ y $R_2$ intentando acceder a $R_3$ y $R_4$, mientras el **Hilo B** retiene $R_4$ y $R_3$ e intenta acceder a $R_2$ y $R_1$.

2. **Condiciones del Interbloqueo Presentes:**
   * **Exclusión Mutua:** Se utilizan mutex (`std::mutex`) para representar los recursos.
   * **Retener y Esperar (*Hold and Wait*):** Cada hilo mantiene recursos bloqueados mientras intenta solicitar más.
   * **No Apropriación (*No Preemption*):** Ningún hilo puede quitarle un recurso bloqueado a otro.
   * **Espera Circular (*Circular Wait*):** El Hilo A espera por lo que retiene el Hilo B y viceversa.

3. **Prevención de Interbloqueos (Solución):**
   * Eliminación de la **Espera Circular** obligando a que **todos los hilos adquieran los recursos exactamente en el mismo orden global** ($R_1 \to R_2 \to R_3 \to R_4$).

---

## 🛠️ Tecnologías Utilizadas

* **Lenguaje:** C++ (Estándar C++11 o superior).
* **Módulos de Concurrencia:** `<thread>`, `<mutex>`, `<condition_variable>`, `<chrono>`.
* **Entorno:** Microsoft Visual Studio / GCC.

---

## 🚀 Funcionalidades del Menú

Al ejecutar el programa se presentan dos modalidades:

### 1. Simulación de Interbloqueo Intencional
* Utiliza una **barrera de sincronización** (`std::condition_variable`) para asegurar que ambos hilos hayan adquirido la mitad de sus recursos antes de intentar obtener la otra mitad.
* **Resultado:** El programa se bloquea infinitamente cuando el Hilo A queda esperando $R_3$ y el Hilo B queda esperando $R_2$.

### 2. Ejecución Segura (Sin Interbloqueo)
* Ambos hilos procesan las etapas solicitando los mutex en el orden jerárquico $R_1 \to R_2 \to R_3 \to R_4$.
* **Resultado:** Ambos hilos completan sus tareas exitosamente sin quedar bloqueados.

---

## 💻 Compilación y Ejecución

### Desde Visual Studio
1. Abrir la solución `interbloqueo.sln`.
2. Compilar con `Ctrl + Shift + B`.
3. Ejecutar con `Ctrl + F5`.

### Desde la Consola (G++)
```bash
g++ -std=c++11 -pthread interbloqueo.cpp -o interbloqueo
./interbloqueo
```