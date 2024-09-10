#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm> // Para std::sort

using namespace std;
using namespace std::chrono;

// Función para combinar dos subarrays para Merge Sort
void merge(vector<int>& vec, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftVec(n1), rightVec(n2);

    for (i = 0; i < n1; i++)
        leftVec[i] = vec[left + i];
    for (j = 0; j < n2; j++)
        rightVec[j] = vec[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] <= rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        } else {
            vec[k] = rightVec[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}

// Función de Merge Sort
void mergeSort(vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        merge(vec, left, mid, right);
    }
}

// Función de Quick Sort
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int medianOfThree(vector<int>& elementos, int low, int high) {
    int mid = low + (high - low) / 2;
    if (elementos[low] > elementos[mid])
        swap(elementos[low], elementos[mid]);
    if (elementos[low] > elementos[high])
        swap(elementos[low], elementos[high]);
    if (elementos[mid] > elementos[high])
        swap(elementos[mid], elementos[high]);
    swap(elementos[mid], elementos[high - 1]);  // Coloca el pivote al final
    return elementos[high - 1];  // Pivote
}

int partition(vector<int>& elementos, int low, int high) {
    int pivot = medianOfThree(elementos, low, high);
    int i = low;
    int j = high - 1;

    while (true) {
        while (elementos[++i] < pivot) {}
        while (elementos[--j] > pivot) {}
        if (i >= j)
            break;
        swap(elementos[i], elementos[j]);
    }
    swap(elementos[i], elementos[high - 1]);  // Coloca el pivote en su lugar
    return i;
}

void quickSort(vector<int>& elementos, int low, int high) {
    if (low < high) {
        int pi = partition(elementos, low, high);
        quickSort(elementos, low, pi - 1);
        quickSort(elementos, pi + 1, high);
    }
}


// Función de Selection Sort
void selectionSort(vector<int>& myVector, int sizeOfVect) {
    for (unsigned int i = 0; i < sizeOfVect - 1; i++) {
        int vectMin = i;
        for (int j = i + 1; j < sizeOfVect; j++) {
            if (myVector[j] < myVector[vectMin]) {
                vectMin = j;
            }
        }
        int temp = myVector[i];
        myVector[i] = myVector[vectMin];
        myVector[vectMin] = temp;
    }
}
// Función para cargar el dataset desde un archivo binario y medir el tiempo de ejecución
void testDataset(const string& filename) {
    ifstream infile(filename, ios::binary);
    if (!infile) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }

    infile.seekg(0, infile.end);
    int n = infile.tellg() / sizeof(int);
    infile.seekg(0, infile.beg);

    vector<int> vec(n);
    infile.read(reinterpret_cast<char*>(&vec[0]), n * sizeof(int));
    infile.close();

    // Probar Merge Sort
    auto start = high_resolution_clock::now();
    vector<int> vecMerge = vec;
    mergeSort(vecMerge, 0, n - 1);
    auto end = high_resolution_clock::now();
    cout << "Tiempo de ejecución de Merge Sort en " << filename << ": " << duration_cast<milliseconds>(end - start).count() / 1000.0 << " s" << endl;

    // Probar Quick Sort
    start = high_resolution_clock::now();
    vector<int> vecQuick = vec;
    quickSort(vecQuick, 0, n - 1);
    end = high_resolution_clock::now();
    cout << "Tiempo de ejecución de Quick Sort en " << filename << ": " << duration_cast<milliseconds>(end - start).count() / 1000.0 << " s" << endl;

    // Probar Selection Sort
    start = high_resolution_clock::now();
    vector<int> vecSelection = vec;
    selectionSort(vecSelection, n);
    end = high_resolution_clock::now();
    cout << "Tiempo de ejecución de Selection Sort en " << filename << ": " << duration_cast<milliseconds>(end - start).count() / 1000.0 << " s" << endl;

    // Probar sort de C++
    start = high_resolution_clock::now();
    vector<int> vecStd = vec;
    std::sort(vecStd.begin(), vecStd.end());
    end = high_resolution_clock::now();
    cout << "Tiempo de ejecución de std::sort en " << filename << ": " << duration_cast<milliseconds>(end - start).count() / 1000.0 << " s" << endl;
}

int main() {
    vector<string> datasets = {
        "desorden_dataset.bin",
        "semi_dataset.bin",
        "ordenado_descen_dataset.bin",
        "ordenado_ascen_dataset.bin"
    };

    for (const string& dataset : datasets) {
        testDataset(dataset);
    }

    return 0;
}