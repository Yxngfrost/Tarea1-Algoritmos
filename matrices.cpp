#include <iostream>
#include <vector>
#include <fstream>
#include <chrono> // Para medir el tiempo

using namespace std;
using namespace std::chrono;

// Función para cargar una matriz desde un archivo binario
vector<vector<int>> cargarMatriz(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    
    if (!archivo) {
        throw runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
    }

    int filas, columnas;
    archivo.read(reinterpret_cast<char*>(&filas), sizeof(int));
    archivo.read(reinterpret_cast<char*>(&columnas), sizeof(int));

    vector<vector<int>> matriz(filas, vector<int>(columnas));
    
    for (int i = 0; i < filas; ++i) {
        archivo.read(reinterpret_cast<char*>(matriz[i].data()), columnas * sizeof(int));
    }

    archivo.close();
    return matriz;
}

// Funciones para la multiplicación de matrices 2x2
vector<vector<int>> multiplicacionMatriz(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    vector<vector<int>> matriz(2, vector<int>(2));
    matriz[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    matriz[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
    matriz[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    matriz[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];
    return matriz;
}

vector<vector<int>> sumarMatrices(const vector<vector<int>>& matrizA, const vector<vector<int>>& matrizB) {
    int tamano = matrizA.size();
    vector<vector<int>> resultado(tamano, vector<int>(tamano));
    for (int fila = 0; fila < tamano; ++fila) {
        for (int col = 0; col < tamano; ++col) {
            resultado[fila][col] = matrizA[fila][col] + matrizB[fila][col];
        }
    }
    return resultado;
}

vector<vector<int>> restarMatrices(const vector<vector<int>>& matrizA, const vector<vector<int>>& matrizB) {
    int tamano = matrizA.size();
    vector<vector<int>> resultado(tamano, vector<int>(tamano));
    for (int fila = 0; fila < tamano; ++fila) {
        for (int col = 0; col < tamano; ++col) {
            resultado[fila][col] = matrizA[fila][col] - matrizB[fila][col];
        }
    }
    return resultado;
}

void dividirMatriz(const vector<vector<int>>& matriz, vector<vector<int>>& esquinaSuperiorIzquierda, vector<vector<int>>& esquinaSuperiorDerecha, vector<vector<int>>& esquinaInferiorIzquierda, vector<vector<int>>& esquinaInferiorDerecha) {
    int tamano = matriz.size();
    int mitad = tamano / 2;
    
    esquinaSuperiorIzquierda.resize(mitad, vector<int>(mitad));
    esquinaSuperiorDerecha.resize(mitad, vector<int>(mitad));
    esquinaInferiorIzquierda.resize(mitad, vector<int>(mitad));
    esquinaInferiorDerecha.resize(mitad, vector<int>(mitad));
    
    for (int i = 0; i < mitad; ++i) {
        for (int j = 0; j < mitad; ++j) {
            esquinaSuperiorIzquierda[i][j] = matriz[i][j];
            esquinaSuperiorDerecha[i][j] = matriz[i][j + mitad];
            esquinaInferiorIzquierda[i][j] = matriz[i + mitad][j];
            esquinaInferiorDerecha[i][j] = matriz[i + mitad][j + mitad];
        }
    }
}

vector<vector<int>> strassen(const vector<vector<int>>& matrizA, const vector<vector<int>>& matrizB) {
    int tamano = matrizA.size();
    if (tamano == 2) {
        return multiplicacionMatriz(matrizA, matrizB);
    }
    
    vector<vector<int>> A, B, C, D, E, F, G, H;
    dividirMatriz(matrizA, A, B, C, D);
    dividirMatriz(matrizB, E, F, G, H);
    
    vector<vector<int>> p1 = strassen(A, restarMatrices(F, H));
    vector<vector<int>> p2 = strassen(sumarMatrices(A, B), H);
    vector<vector<int>> p3 = strassen(sumarMatrices(C, D), E);
    vector<vector<int>> p4 = strassen(D, restarMatrices(G, E));
    vector<vector<int>> p5 = strassen(sumarMatrices(A, D), sumarMatrices(E, H));
    vector<vector<int>> p6 = strassen(restarMatrices(B, D), sumarMatrices(G, H));
    vector<vector<int>> p7 = strassen(restarMatrices(A, C), sumarMatrices(E, F));
    
    vector<vector<int>> esquinaSuperiorIzquierda = sumarMatrices(restarMatrices(sumarMatrices(p5, p4), p2), p6);
    vector<vector<int>> esquinaSuperiorDerecha = sumarMatrices(p1, p2);
    vector<vector<int>> esquinaInferiorIzquierda = sumarMatrices(p3, p4);
    vector<vector<int>> esquinaInferiorDerecha = restarMatrices(restarMatrices(sumarMatrices(p1, p5), p3), p7);
    
    vector<vector<int>> resultado(tamano, vector<int>(tamano));
    for (int i = 0; i < tamano / 2; ++i) {
        for (int j = 0; j < tamano / 2; ++j) {
            resultado[i][j] = esquinaSuperiorIzquierda[i][j];
            resultado[i][j + tamano / 2] = esquinaSuperiorDerecha[i][j];
            resultado[i + tamano / 2][j] = esquinaInferiorIzquierda[i][j];
            resultado[i + tamano / 2][j + tamano / 2] = esquinaInferiorDerecha[i][j];
        }
    }
    return resultado;
}

vector<vector<int>> generarMatriz(int filas, int columnas) {
    vector<vector<int>> matriz(filas, vector<int>(columnas));
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            matriz[i][j] = rand() % 11; // Valores aleatorios entre 0 y 10
        }
    }
    return matriz;
}

void printearMatriz(const vector<vector<int>>& matriz) {
    for (const auto& fila : matriz) {
        for (int valor : fila) {
            cout << valor << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<int>> multiplicacionMatrizTradicional(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int filasA = a.size();
    int columnasA = a[0].size();
    int filasB = b.size();
    int columnasB = b[0].size();
    
    if (columnasA != filasB) {
        throw invalid_argument("Las dimensiones de las matrices no coinciden para la multiplicación.");
    }
    
    vector<vector<int>> producto(filasA, vector<int>(columnasB, 0));
    for (int i = 0; i < filasA; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            for (int k = 0; k < columnasA; ++k) {
                producto[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return producto;
}

vector<vector<int>> transponerMatriz(const vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    vector<vector<int>> transpuesta(columnas, vector<int>(filas));
    
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            transpuesta[j][i] = matriz[i][j];
        }
    }
    return transpuesta;
}

vector<vector<int>> multiplicacionMatrizOptimizada(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int filasA = a.size();
    int columnasA = a[0].size();
    int filasB = b.size();
    int columnasB = b[0].size();
    
    if (columnasA != filasB) {
        throw invalid_argument("Las dimensiones de las matrices no coinciden para la multiplicación.");
    }

    vector<vector<int>> bTranspuesta = transponerMatriz(b);

    vector<vector<int>> producto(filasA, vector<int>(columnasB, 0));
    
    for (int i = 0; i < filasA; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            for (int k = 0; k < columnasA; ++k) {
                producto[i][j] += a[i][k] * bTranspuesta[j][k];
            }
        }
    }
    return producto;
}

int main() {
    try {
        // Nombres de los archivos
        vector<string> nombresArchivos = {
            "matriz_128x128.bin",
            "matriz_256x256.bin",
            "matriz_512x512.bin",
            "matriz_1024x1024.bin",
            "matriz_2048x2048.bin"
        };

        for (const auto& nombreArchivo : nombresArchivos) {
            cout << "Cargando matriz desde: " << nombreArchivo << endl;
            
            // Cargar matrices desde archivos binarios
            vector<vector<int>> A = cargarMatriz(nombreArchivo);
            vector<vector<int>> B = cargarMatriz(nombreArchivo); // Usamos el mismo archivo para A y B

            // Medir tiempo para Strassen
            auto inicio = high_resolution_clock::now();
            vector<vector<int>> C = strassen(A, B);
            auto fin = high_resolution_clock::now();
            double tiempoStrassen = duration_cast<duration<double>>(fin - inicio).count();

            cout << "Tiempo de multiplicación Strassen: " << tiempoStrassen << " segundos" << endl;

            // Medir tiempo para Tradicional
            inicio = high_resolution_clock::now();
            vector<vector<int>> D = multiplicacionMatrizTradicional(A, B);
            fin = high_resolution_clock::now();
            double tiempoTradicional = duration_cast<duration<double>>(fin - inicio).count();

            cout << "Tiempo de multiplicación Tradicional: " << tiempoTradicional << " segundos" << endl;

            // Medición del tiempo para Optimizada
            inicio = high_resolution_clock::now();
            vector<vector<int>> E = multiplicacionMatrizOptimizada(A, B);
            fin = high_resolution_clock::now();
            double tiempoOptimizado = duration_cast<duration<double>>(fin - inicio).count();

            cout << "Tiempo de multiplicación Optimizada: " << tiempoOptimizado << " segundos" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}