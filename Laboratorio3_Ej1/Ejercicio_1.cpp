#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Convierte un carácter a su representación binaria en una cadena de 8 bits
string char_a_binario(char c) {
    string binario = "";
    for (int i = 7; i >= 0; --i) {
        binario += ((c >> i) & 1) ? '1' : '0';
    }
    return binario;
}

// Convierte una cadena de texto a binario
string texto_a_binario(const string& texto) {
    string binarioString = "";
    for (size_t i = 0; i < texto.length(); ++i) {
        binarioString += char_a_binario(texto[i]);
    }
    return binarioString;
}

// Función para invertir en pares de 2 bits desde una posición inicial
string invertirDesdePosicion(const string& bloque, size_t start, size_t step) {
    string bloque_nuevo = bloque; // Crea una copia del bloque
    size_t longitud = bloque.length();
    size_t i = start;

    // Invertir bits mientras avanzamos por el bloque
    do {
        bloque_nuevo[i] = (bloque_nuevo[i] == '0') ? '1' : '0'; // Invertir el bit actual
        i = (i + step) % longitud; // Incrementar el índice y hacer un wrap around
    } while (i != start); // Terminar cuando volvamos a la posición inicial

    return bloque_nuevo;
}

// Función para invertir todos los bits de un bloque
string invertirBits(const string& bloque) {
    string bloque_nuevo = bloque;
    for (size_t j = 0; j < bloque_nuevo.length(); ++j) {
        bloque_nuevo[j] = (bloque_nuevo[j] == '0') ? '1' : '0';
    }
    return bloque_nuevo;
}

// Función para contar los 0s y 1s en un bloque dado
void contarBits(const string& bloque, size_t& count0, size_t& count1) {
    count0 = 0;
    count1 = 0;
    for (char bit : bloque) {
        if (bit == '0') count0++;
        else if (bit == '1') count1++;
    }
}

// Aplica las reglas de codificación para los bloques siguientes al primero
string bloque_codificado(const string& bloque_actual, const string& bloque_anterior_codificado) {
    size_t count0 = 0, count1 = 0;

    // Contar cantidad de 1s y 0s en el bloque anterior codificado
    contarBits(bloque_anterior_codificado, count0, count1);

    string bloque_nuevo = bloque_actual;

    // Buscar la posición del primer '1' en el bloque actual
    size_t posicionPrimeraUno = bloque_actual.find('1');


    // Regla 1: Si hay igual cantidad de 0s y 1s, invertir todos los bits
       if (count0 == count1) {
           bloque_nuevo = invertirBits(bloque_actual);
    }
    // Regla 2: Si hay más 0s, invertir cada 2 bits desde la posición del primer '1'
    else if (count0 > count1) {
        bloque_nuevo = invertirDesdePosicion(bloque_actual, posicionPrimeraUno, 2);
    }
    // Regla 3: Si hay más 1s, invertir cada 3 bits desde la posición del primer '1'
    else {
        bloque_nuevo = invertirDesdePosicion(bloque_actual, posicionPrimeraUno, 3);
    }

    return bloque_nuevo;
}

// Codifica el texto dado en binario basado en las reglas especificadas
string binario_codificado(const string& binario, int tamano_bloque) {
    string binario_codificado_final = "";

    for (size_t i = 0; i < binario.length(); i += tamano_bloque) {
        string bloque_actual = binario.substr(i, tamano_bloque);

        // Si es el primer bloque, invertir todos los bits
        if (i == 0) {
            string bloque_codificado = "";
            for (char bit : bloque_actual) {
                bloque_codificado += (bit == '0') ? '1' : '0'; // Invertir el primer bloque
            }
            binario_codificado_final += bloque_codificado;
        }
        // Utilizamos el bloque codificado anterior
        else {
            string bloque_codificado_anterior = binario_codificado_final.substr(i - tamano_bloque, tamano_bloque);
            string bloque_codificado_actual = bloque_codificado(bloque_actual, bloque_codificado_anterior);
            binario_codificado_final += bloque_codificado_actual;
        }
    }
    return binario_codificado_final;
}

int main() {
    string text;
    int tamano_bloque;

    // Leer la cadena de texto desde un archivo
    ifstream inputFile("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej1/Texto.txt");
    if (!inputFile) {
        cerr << "Error al abrir el archivo de entrada." << endl;
        return 1;
    }

    getline(inputFile, text);
    inputFile.close();

    // Solicitar el tamaño del bloque
    cout << "Ingresa el tamano del bloque (n) para codificar: ";
    cin >> tamano_bloque;

    // Convertir el texto a binario
    string binaryText = texto_a_binario(text);

    // Mostrar la representación binaria del texto
    cout << "Texto en binario: " << binaryText << endl;

    // Codificar el binario
    string binario_codificado_final = binario_codificado(binaryText, tamano_bloque);

    // Mostrar el resultado codificado
    cout << "Texto codificado en binario: " << binario_codificado_final << endl;
    // Guardar el binario y el binario codificado en archivos .txt
        ofstream archivoBinario("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej1/Binario.txt");
        if (archivoBinario.is_open()) {
            archivoBinario << binaryText; // Escribir el texto binario
            archivoBinario.close(); // Cerrar el archivo
            cout << "Texto binario guardado en 'binario.txt'" << endl;
        } else {
            cout << "Error al abrir el archivo binario." << endl;
        }

        ofstream archivoCodificado("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej1/Binario_Codificado.txt");
        if (archivoCodificado.is_open()) {
            archivoCodificado << binario_codificado_final; // Escribir el texto codificado
            archivoCodificado.close(); // Cerrar el archivo
            cout << "Texto codificado guardado en 'binario_codificado.txt'" << endl;
        } else {
            cout << "Error al abrir el archivo codificado." << endl;
        }

        return 0;
    }
