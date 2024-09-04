#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Estrutura para guardar as palavras e quantas vezes elas aparecem
struct Palavra
{
    string palavra;
    int frequencia;

    Palavra() : palavra(""), frequencia(0) {} // Construtor padrão

    Palavra(string p) : palavra(p), frequencia(1) {} // Construtor com parâmetro
};

unsigned long djb2_hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

class TabelaHash
{
private:
    vector<Palavra> tabela; //Vetor de estruturas "Palavra"
    int capacidade;

public:
    TabelaHash(int tamanho) : capacidade(tamanho)
    {
        tabela.resize(capacidade); // Ajusta o tamanho da tabela
    }

    //Função para adicionar ou atualizar uma palavra na tabela
    void inserir(const string &palavra)
    {
        unsigned long indice = djb2_hash((unsigned char *)palavra.c_str()) % capacidade;

        while (tabela[indice].frequencia != 0 && tabela[indice].palavra != palavra)
        {
            indice = (indice + 1) % capacidade; //tenta a próxima posição
        }

        if (tabela[indice].frequencia == 0)
        {
            tabela[indice] = Palavra(palavra); //Se estiver vazia adiciona nova palavra
        }
        else
        {
            tabela[indice].frequencia++; //Se a palavra existir, aumenta a contagem
        }
    }

    string palavraMaisFrequente()
    {
        string maisFrequente;
        int maiorFrequencia = 0;

        for (int i = 0; i < capacidade; ++i)
        {
            if (tabela[i].frequencia > 0 && tabela[i].frequencia > maiorFrequencia)
            {
                maiorFrequencia = tabela[i].frequencia;
                maisFrequente = tabela[i].palavra;
            }
        }
        return maisFrequente;
    }

    //imprime
    void imprimirFrequencias()
    {
        for (int i = 0; i < capacidade; ++i)
        {
            if (tabela[i].frequencia > 0)
            {
                cout << "(" << tabela[i].palavra << ", " << tabela[i].frequencia << ")" << endl;
            }
        }
    }
};

void processarTexto(TabelaHash &tabelaHash, const string &texto)
{
    string palavra = "";
    for (char c : texto)
    {
        if (c == ' ')
        {
            if (!palavra.empty())
            {
                tabelaHash.inserir(palavra);
                palavra = ""; 
            }
        }
        else
        {
            palavra += c; //Continua construindo a palavra
        }
    }
    if (!palavra.empty())
    {
        tabelaHash.inserir(palavra);
    }
}

int main()
{
    string texto;
    cout << "Digite o texto: ";
    getline(cin, texto);

    TabelaHash tabelaHash(100);
    processarTexto(tabelaHash, texto);

    cout << "Palavra mais usada: " << tabelaHash.palavraMaisFrequente() << endl;
    cout << "Lista de palavras: " << endl;
    tabelaHash.imprimirFrequencias();

    return 0;
}