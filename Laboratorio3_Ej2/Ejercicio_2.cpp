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
        if (i + 8 <= binario.length()) {  // Asegurar que hay un bloque de 8 bits
            string bloque_binario = binario.substr(i, 8);
            texto += binario_a_char(bloque_binario);
        }
    }
    return texto;
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

// Aplica las reglas de decodificación para los bloques siguientes al primero
string bloque_descodificado(const string& bloque_actual, const string& bloque_anterior) {
    size_t count0 = 0, count1 = 0;

    // Contar cantidad de 1s y 0s en el bloque anterior descodificado
    contarBits(bloque_anterior, count0, count1);

    string bloque_nuevo = bloque_actual;

    // Buscar la posición del primer '1' en el bloque actual
    size_t posicionPrimeraUno = bloque_actual.find('1');


    // Regla inversa 1: Si hay igual cantidad de 0s y 1s, volver a invertir todos los bits
    if (count0 == count1) {
        bloque_nuevo = invertirBits(bloque_actual);
    }
    // Regla inversa 2: Si había más 0s, deshacer la inversión de cada 2 bits
    else if (count0 > count1) {
        bloque_nuevo = invertirDesdePosicion(bloque_actual, posicionPrimeraUno, 3);
    }
    // Regla inversa 3: Si había más 1s, deshacer la inversión de cada 3 bits
    else {
        bloque_nuevo = invertirDesdePosicion(bloque_actual, posicionPrimeraUno, 2);
    }

    return bloque_nuevo;
}

// Decodifica el binario codificado en bloques basado en las reglas especificadas
string binario_descodificado(const string& binario, int tamano_bloque) {
    string binario_descodificado_final = "";

    for (size_t i = 0; i < binario.length(); i += tamano_bloque) {
        string bloque_actual = binario.substr(i, tamano_bloque);

        // Si es el primer bloque, invertir todos los bits (porque fue codificado así)
        if (i == 0) {
            binario_descodificado_final = invertirBits(bloque_actual);
        }
        // Utilizamos el bloque descodificado anterior para procesar el siguiente
        else {
            string bloque_descodificado_anterior = binario_descodificado_final.substr(i - tamano_bloque, tamano_bloque);
            string bloque_descodificado_actual = bloque_descodificado(bloque_actual, bloque_descodificado_anterior);
            binario_descodificado_final += bloque_descodificado_actual;
        }
    }
    return binario_descodificado_final;
}

int main() {
    string binarioCodificado;
    int tamano_bloque;

    // Leer la cadena de texto codificado desde un archivo
    ifstream archivoCodificado("D:/Documentos/Estudio/UdeA/2024-2/Informatica II/Laboratorios/Laboratorio_3/Laboratorio3_Ej1/Binario_Codificado.txt");
    if (!archivoCodificado) {
        cerr << "Error al abrir el archivo de entrada codificado." << endl;
        return 1;
    }

    getline(archivoCodificado, binarioCodificado);
        archivoCodificado.close();

        // Solicitar el tamaño del bloque
        cout << "Ingresa el tamano del bloque (n): ";
        cin >> tamano_bloque;

        // Decodificar el binario
        string binario_descodificado_final = binario_descodificado(binarioCodificado, tamano_bloque);

        // Mostrar el binario descodificado
        cout << "Texto descodificado en binario: " << binario_descodificado_final << endl;

        // Convertir el binario a texto
        string texto_descodificado = binario_a_texto(binario_descodificado_final);

        // Mostrar el texto descodificado
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

