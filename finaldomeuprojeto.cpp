#include <iostream>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

// Funções Matemáticas
void somar(int a, int b) {
    cout << "Soma: " << a + b << endl;
}

void subtrair(int a, int b) {
    cout << "Subtração: " << a - b << endl;
}

void multiplicar(int a, int b) {
    cout << "Multiplicação: " << a * b << endl;
}

void dividir(int a, int b) {
    if (b != 0) {
        cout << "Divisão: " << a / b << endl;
    } else {
        cout << "Erro: Divisão por zero!" << endl;
    }
}

void calcularPotencia(int base, int expoente) {
    int resultado = 1;
    for (int i = 0; i < expoente; ++i) {
        resultado *= base;
    }
    cout << "Potência: " << resultado << endl;
}

void calcularFatorial(int n) {
    int resultado = 1;
    for (int i = 1; i <= n; ++i) {
        resultado *= i;
    }
    cout << "Fatorial: " << resultado << endl;
}

void calcularSeno(double angulo) {
    cout << "Seno: " << sin(angulo) << endl;
}

void calcularCosseno(double angulo) {
    cout << "Cosseno: " << cos(angulo) << endl;
}

void calcularTangente(double angulo) {
    cout << "Tangente: " << tan(angulo) << endl;
}

// Funções de Conversão
void decimalParaBinario(int decimal) {
    cout << "Binário: ";
    if (decimal == 0) {
        cout << "0";
        return;
    }
    int binario[32];
    int i = 0;
    while (decimal > 0) {
        binario[i] = decimal % 2;
        decimal /= 2;
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {
        cout << binario[j];
    }
    cout << endl;
}

void binarioParaDecimal(string binario) {
    int decimal = 0;
    int tamanho = binario.size();
    int expoente = 0;
    for (int i = tamanho - 1; i >= 0; --i) {
        if (binario[i] == '1') {
            decimal += pow(2, expoente);
        }
        expoente++;
    }
    cout << "Decimal: " << decimal << endl;
}

void decimalParaHexadecimal(int decimal) {
    cout << "Hexadecimal: " << hex << decimal << endl;
}

void hexadecimalParaDecimal(string hexadecimal) {
    int decimal;
    stringstream ss;
    ss << hex << hexadecimal;
    ss >> decimal;
    cout << "Decimal: " << decimal << endl;
}

void decimalParaOctal(int decimal) {
    cout << "Octal: " << oct << decimal << endl;
}

void octalParaDecimal(string octal) {
    int decimal;
    stringstream ss;
    ss << oct << octal;
    ss >> decimal;
    cout << "Decimal: " << decimal << endl;
}

// Funções de String
void inverterString(string str) {
    cout << "Invertido: ";
    for (int i = str.size() - 1; i >= 0; --i) {
        cout << str[i];
    }
    cout << endl;
}

void contarCaracteres(string str) {
    cout << "Total de caracteres: " << str.size() << endl;
}

void contarPalavras(string str) {
    int contagem = 0;
    stringstream ss(str);
    string palavra;
    while (ss >> palavra) {
        contagem++;
    }
    cout << "Total de palavras: " << contagem << endl;
}

void converterMinusculas(string str) {
    cout << "Minúsculas: ";
    for (char& c : str) {
        cout << (char)tolower(c);
    }
    cout << endl;
}

void converterMaiusculas(string str) {
    cout << "Maiúsculas: ";
    for (char& c : str) {
        cout << (char)toupper(c);
    }
    cout << endl;
}

void substituirString(string& str, string alvo, string substituto) {
    size_t pos = str.find(alvo);
    if (pos != string::npos) {
        str.replace(pos, alvo.length(), substituto);
        cout << "Substituído: " << str << endl;
    } else {
        cout << "Alvo não encontrado!" << endl;
    }
}

// Funções de Controle de Fluxo
void verificarPrimo(int numero) {
    bool ehPrimo = true;
    if (numero <= 1) {
        ehPrimo = false;
    } else {
        for (int i = 2; i <= sqrt(numero); ++i) {
            if (numero % i == 0) {
                ehPrimo = false;
                break;
            }
        }
    }
    if (ehPrimo) {
        cout << numero << " é primo!" << endl;
    } else {
        cout << numero << " não é primo!" << endl;
    }
}

void contarDigitos(int numero) {
    int contagem = 0;
    while (numero != 0) {
        numero /= 10;
        contagem++;
    }
    cout << "Total de dígitos: " << contagem << endl;
}

void imprimirTabuada(int numero) {
    cout << "Tabuada de " << numero << ":" << endl;
    for (int i = 1; i <= 10; ++i) {
        cout << numero << " x " << i << " = " << numero * i << endl;
    }
}

void calcularMedia(double numeros[], int tamanho) {
    double soma = 0;
    for (int i = 0; i < tamanho; ++i) {
        soma += numeros[i];
    }
    cout << "Média: " << soma / tamanho << endl;
}

int main() {
    // Exemplo de chamadas de função
    somar(5, 3);
    subtrair(10, 7);
    multiplicar(4, 6);
    dividir(20, 4);
    calcularPotencia(2, 5);
    calcularFatorial(5);
    calcularSeno(30);
    calcularCosseno(45);
    calcularTangente(60);

    decimalParaBinario(15);
    binarioParaDecimal("1101");
    decimalParaHexadecimal(255);
    hexadecimalParaDecimal("FF");
    decimalParaOctal(64);
    octalParaDecimal("100");

    inverterString("hello");
    contarCaracteres("hello");
    contarPalavras("hello world");
    converterMinusculas("HELLO");
    converterMaiusculas("hello");
    string str = "hello world";
    substituirString(str, "world", "everyone");

    verificarPrimo(17);
    contarDigitos(12345);
    imprimirTabuada(7);
    double numeros[] = {1.5, 2.5, 3.5, 4.5, 5.5};
    calcularMedia(numeros, 5);

    return 0;
}
