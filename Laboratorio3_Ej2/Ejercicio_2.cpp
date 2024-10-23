#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Convierte una cadena binaria de 8 bits a un carácter
char binario_a_char(const string& binario) {
    char c = 0;
    for (int i = 0; i < 8; ++i) {
        c <<= 1;
        c += (binario[i] == '1') ? 1 : 0;
    }
    return c;
}

// Convierte una cadena binaria en texto
string binario_a_texto(const string& binario) {
    string texto = "";
    for (size_t i = 0; i < binario.length(); i += 8) {
        if (i + 8 <= binario.length()) {
            string bloque_binario = binario.substr(i, 8);
            texto += binario_a_char(bloque_binario);
        }
    }
    return texto;
}

// Función para invertir todos los bits de un bloque
string invertirTodosBits(const string& bloque) {
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

// Función para invertir cada 2 bits
string invertirCada2Bits(const string& bloque) {
    string bloque_nuevo = bloque;
    for (size_t i = 1; i < bloque_nuevo.length(); i += 2) {
        bloque_nuevo[i] = (bloque_nuevo[i] == '0') ? '1' : '0';
    }
    return bloque_nuevo;
}

// Función para invertir cada 3 bits
string invertirCada3Bits(const string& bloque) {
    string bloque_nuevo = bloque;
    for (size_t i = 2; i < bloque_nuevo.length(); i += 3) {
        bloque_nuevo[i] = (bloque_nuevo[i] == '0') ? '1' : '0';
    }
    return bloque_nuevo;
}

// Aplica las reglas de decodificación para los bloques siguientes al primero
string bloque_descodificado(const string& bloque_actual, const string& bloque_anterior) {
    size_t count0 = 0, count1 = 0;
    contarBits(bloque_anterior, count0, count1);
    string bloque_nuevo = bloque_actual;
    if (count0 == count1) {
        bloque_nuevo = invertirTodosBits(bloque_actual);
    } else if (count0 > count1) {
        bloque_nuevo = invertirCada2Bits(bloque_actual);
    } else if (count1 > count0) {
        bloque_nuevo = invertirCada3Bits(bloque_actual);
    }
    return bloque_nuevo;
}

// Decodifica el binario codificado en bloques basado en las reglas especificadas
string binario_descodificado(const string& binario, int tamano_bloque) {
    string binario_descodificado_final = "";
    for (size_t i = 0; i < binario.length(); i += tamano_bloque) {
        string bloque_actual = binario.substr(i, tamano_bloque);
        if (i == 0) {
            binario_descodificado_final = invertirTodosBits(bloque_actual);
        } else {
            string bloque_anterior = binario_descodificado_final.substr(i - tamano_bloque, tamano_bloque);
            string bloque_descodificado_actual = bloque_descodificado(bloque_actual, bloque_anterior);
            binario_descodificado_final += bloque_descodificado_actual;
        }
    }
    return binario_descodificado_final;
}

int main() {
    string binarioCodificado;
    ifstream archivoCodificado("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej1/Binario_codificado.txt"); // Ruta al archivo que contiene el binario codificado

    if (!archivoCodificado) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }
    getline(archivoCodificado, binarioCodificado);
    archivoCodificado.close();

    int tamano_bloque;

    // Solicitar el tamaño del bloque
    cout << "Ingresa el tamano del bloque (n): ";
    cin >> tamano_bloque;

    string binario_descodificado_final = binario_descodificado(binarioCodificado, tamano_bloque);

    cout << "Texto descodificado en binario: " << binario_descodificado_final << endl;

    string texto_descodificado = binario_a_texto(binario_descodificado_final);
    cout << "Texto descodificado: " << texto_descodificado << endl;

    // Guardar el texto decodificado en un archivo
    ofstream archivoTextoDecodificado("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej2/Texto_Decodificado.txt");
    if (archivoTextoDecodificado.is_open()) {
        archivoTextoDecodificado << texto_descodificado; // Escribir el texto decodificado
        archivoTextoDecodificado.close(); // Cerrar el archivo
        cout << "Texto decodificado guardado en 'Texto_Decodificado.txt'" << endl;
    } else {
        cout << "Error al abrir el archivo para guardar el texto decodificado." << endl;
    }
    return 0;
}
