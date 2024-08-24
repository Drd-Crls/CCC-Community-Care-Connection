#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_INSTITUICOES 100
#define MAX_USUARIOS 100
#define MAX_NOME 50
#define MAX_SENHA 20
#define MAX_ITENS 6

typedef struct {
    char nome[MAX_NOME];
    char cidade_bairro[MAX_NOME];
    char responsavel[MAX_NOME];
    char senha[MAX_SENHA];
} Instituicao;

typedef struct {
    char nome[MAX_NOME];
    char cpf[MAX_NOME];
    char senha[MAX_SENHA];
} Usuario;

typedef struct {
    char nome[MAX_NOME];
    int quantidade;
} Item;

void carregarDados();
void salvarDados();
void telaPrincipal();
void cadastro();
void login();
void doar();
void conferirDoacoes();
void listarInstituicoes();
void menuPrincipal();
void menuLoginInstituicao();
void menuLoginUsuario();

Instituicao instituicoes[MAX_INSTITUICOES];
Usuario usuarios[MAX_USUARIOS];
Item roupas[MAX_ITENS] = {{"Camiseta", 0}, {"Calça", 0}, {"Sapato", 0}, {"Blusa", 0}, {"Cobertor", 0}};
Item alimentos[MAX_ITENS] = {{"Arroz", 0}, {"Feijão", 0}, {"Macarrão", 0}, {"Café", 0}, {"Sal", 0}, {"Açúcar", 0}};

int num_instituicoes = 0;
int num_usuarios = 0;
int instituicao_logada = -1; // -1 indica que nenhuma instituição está logada

// Função para validar o CPF
int validarCPF(const char *cpf) {
    int num[11], i, j;
    int soma, digito1, digito2, resto;

    // Remove caracteres não numéricos
    int pos = 0;
    for (i = 0; cpf[i]; i++) {
        if (isdigit(cpf[i])) {
            num[pos++] = cpf[i] - '0';
        }
    }

    if (pos != 11) return 0; // CPF deve ter exatamente 11 dígitos

    // Validação dos dígitos verificadores
    for (i = 0; i < 9; i++) {
        soma += num[i] * (10 - i);
    }
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;
    if (num[9] != digito1) return 0;

    soma = 0;
    for (i = 0; i < 10; i++) {
        soma += num[i] * (11 - i);
    }
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;
    if (num[10] != digito2) return 0;

    return 1;
}

// Função para carregar dados dos arquivos
void carregarDados() {
    FILE *file;

    // Carregar Instituições
    file = fopen("instituicoes.txt", "r");
    if (file) {
        while (fscanf(file, "%49[^\n]\n%49[^\n]\n%49[^\n]\n%19[^\n]\n",
                      instituicoes[num_instituicoes].nome,
                      instituicoes[num_instituicoes].cidade_bairro,
                      instituicoes[num_instituicoes].responsavel,
                      instituicoes[num_instituicoes].senha) != EOF) {
            num_instituicoes++;
        }
        fclose(file);
    }

    // Carregar Usuários
    file = fopen("usuarios.txt", "r");
    if (file) {
        while (fscanf(file, "%49[^\n]\n%49[^\n]\n%19[^\n]\n",
                      usuarios[num_usuarios].nome,
                      usuarios[num_usuarios].cpf,
                      usuarios[num_usuarios].senha) != EOF) {
            num_usuarios++;
        }
        fclose(file);
    }

    // Carregar Itens
    file = fopen("roupas.txt", "r");
    if (file) {
        for (int i = 0; i < MAX_ITENS; i++) {
            fscanf(file, "%49[^\n]\n%d\n", roupas[i].nome, &roupas[i].quantidade);
        }
        fclose(file);
    }

    file = fopen("alimentos.txt", "r");
    if (file) {
        for (int i = 0; i < MAX_ITENS; i++) {
            fscanf(file, "%49[^\n]\n%d\n", alimentos[i].nome, &alimentos[i].quantidade);
        }
        fclose(file);
    }
}

// Função para salvar dados nos arquivos
void salvarDados() {
    FILE *file;

    // Salvar Instituições
    file = fopen("instituicoes.txt", "w");
    if (file) {
        for (int i = 0; i < num_instituicoes; i++) {
            fprintf(file, "%s\n%s\n%s\n%s\n",
                    instituicoes[i].nome,
                    instituicoes[i].cidade_bairro,
                    instituicoes[i].responsavel,
                    instituicoes[i].senha);
        }
        fclose(file);
    }

    // Salvar Usuários
    file = fopen("usuarios.txt", "w");
    if (file) {
        for (int i = 0; i < num_usuarios; i++) {
            fprintf(file, "%s\n%s\n%s\n",
                    usuarios[i].nome,
                    usuarios[i].cpf,
                    usuarios[i].senha);
        }
        fclose(file);
    }

    // Salvar Itens
    file = fopen("roupas.txt", "w");
    if (file) {
        for (int i = 0; i < MAX_ITENS; i++) {
            fprintf(file, "%s\n%d\n", roupas[i].nome, roupas[i].quantidade);
        }
        fclose(file);
    }

    file = fopen("alimentos.txt", "w");
    if (file) {
        for (int i = 0; i < MAX_ITENS; i++) {
            fprintf(file, "%s\n%d\n", alimentos[i].nome, alimentos[i].quantidade);
        }
        fclose(file);
    }
}

// Função principal da tela inicial
void telaPrincipal() {
    int escolha;

    do {
        printf("\n=== Tela Principal ===\n");
        printf("1. Cadastro\n");
        printf("2. Login\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer do stdin

        switch (escolha) {
            case 1:
                system("cls"); // Limpa a tela
                cadastro();
                break;
            case 2:
                system("cls");
                login();
                break;
            case 3:
                printf("Saindo...\n");
                fflush(stdout);
                salvarDados(); // Salva os dados antes de sair
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 3);
}

// Função para realizar o cadastro de usuários e instituições
void cadastro() {
    int tipo;
    printf("\n=== Cadastro ===\n");
    printf("1. Usuário Comum\n");
    printf("2. Instituição/ONG\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar(); // Limpa o buffer do stdin

    if (tipo == 1) { // Cadastro de Usuário
        Usuario u;
        printf("Nome: ");
        fflush(stdout);
        fgets(u.nome, MAX_NOME, stdin);
        u.nome[strcspn(u.nome, "\n")] = 0; // Remove o caractere '\n'
        
        // Entrada e validação do CPF
        do {
            printf("CPF (somente números): ");
            fflush(stdout);
            fgets(u.cpf, MAX_NOME, stdin);
            u.cpf[strcspn(u.cpf, "\n")] = 0; // Remove o caractere '\n'
        } while (!validarCPF(u.cpf));

        printf("Senha: ");
        fflush(stdout);
        fgets(u.senha, MAX_SENHA, stdin);
        u.senha[strcspn(u.senha, "\n")] = 0;

        usuarios[num_usuarios++] = u; // Adiciona o usuário ao array
        printf("Usuário cadastrado com sucesso!\n");
        fflush(stdout);
        system("cls");
    } else if (tipo == 2) { // Cadastro de Instituição
        Instituicao i;
        printf("Nome: ");
        fflush(stdout);
        fgets(i.nome, MAX_NOME, stdin);
        i.nome[strcspn(i.nome, "\n")] = 0;
        printf("Cidade/Bairro: ");
        fflush(stdout);
        fgets(i.cidade_bairro, MAX_NOME, stdin);
        i.cidade_bairro[strcspn(i.cidade_bairro, "\n")] = 0;
        printf("Responsável: ");
        fflush(stdout);
        fgets(i.responsavel, MAX_NOME, stdin);
        i.responsavel[strcspn(i.responsavel, "\n")] = 0;
        printf("Senha: ");
        fflush(stdout);
        fgets(i.senha, MAX_SENHA, stdin);
        i.senha[strcspn(i.senha, "\n")] = 0;

        instituicoes[num_instituicoes++] = i; // Adiciona a instituição ao array
        printf("Instituição/ONG cadastrada com sucesso!\n");
        fflush(stdout);
        system("cls");
    } else {
        printf("Tipo de cadastro inválido!\n");
        fflush(stdout);
        system("cls");
    }
}

// Função para realizar o login de usuários e instituições
void login() {
    int tipo;
    printf("\n=== Login ===\n");
    printf("1. Usuário Comum\n");
    printf("2. Instituição/ONG\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar(); // Limpa o buffer do stdin

    if (tipo == 1) { // Login de Usuário
        char cpf[MAX_NOME], senha[MAX_SENHA];
        printf("CPF: ");
        fflush(stdout);
        fgets(cpf, MAX_NOME, stdin);
        cpf[strcspn(cpf, "\n")] = 0; // Remove o caractere '\n'
        printf("Senha: ");
        fflush(stdout);
        fgets(senha, MAX_SENHA, stdin);
        senha[strcspn(senha, "\n")] = 0;

        for (int i = 0; i < num_usuarios; i++) {
            if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
                printf("Login bem-sucedido como Usuário Comum!\n");
                fflush(stdout);
                system("cls");
                menuLoginUsuario(); // Abre o menu para usuários comuns
                return;
            }
        }
        printf("Credenciais inválidas!\n");
        fflush(stdout);
        system("cls");
    } else if (tipo == 2) { // Login de Instituição
        char nome[MAX_NOME], senha[MAX_SENHA];
        printf("Nome: ");
        fflush(stdout);
        fgets(nome, MAX_NOME, stdin);
        nome[strcspn(nome, "\n")] = 0; // Remove o caractere '\n'
        printf("Senha: ");
        fflush(stdout);
        fgets(senha, MAX_SENHA, stdin);
        senha[strcspn(senha, "\n")] = 0;

        for (int i = 0; i < num_instituicoes; i++) {
            if (strcmp(instituicoes[i].nome, nome) == 0 && strcmp(instituicoes[i].senha, senha) == 0) {
                instituicao_logada = i; // Marca a instituição como logada
                printf("Login bem-sucedido como Instituição/ONG!\n");
                fflush(stdout);
                system("cls");
                menuLoginInstituicao(); // Abre o menu para instituições
                return;
            }
        }
        printf("Credenciais inválidas!\n");
        fflush(stdout);
        system("cls");
    } else {
        printf("Tipo de login inválido!\n");
        fflush(stdout);
        system("cls");
    }
}

// Função para realizar doações de roupas ou alimentos
void doar() {
    int tipo;
    printf("\n=== Doar ===\n");
    printf("1. Roupas\n");
    printf("2. Alimentos\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar(); // Limpa o buffer do stdin

    if (tipo == 1) { // Doar roupas
        for (int i = 0; i < MAX_ITENS; i++) {
            printf("Quantidade de %s: ", roupas[i].nome);
            fflush(stdout);
            scanf("%d", &roupas[i].quantidade);
            system("cls");
        }
    } else if (tipo == 2) { // Doar alimentos
        for (int i = 0; i < MAX_ITENS; i++) {
            printf("Quantidade de %s: ", alimentos[i].nome);
            fflush(stdout);
            scanf("%d", &alimentos[i].quantidade);
            system("cls");
        }
    } else {
        printf("Tipo de doação inválido!\n");
        fflush(stdout);
        system("cls");
    }
}

// Função para conferir doações de roupas ou alimentos
void conferirDoacoes() {
    int tipo;
    printf("\n=== Conferir Doações ===\n");
    printf("1. Roupas\n");
    printf("2. Alimentos\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar(); // Limpa o buffer do stdin

    if (tipo == 1) { // Conferir doações de roupas
        printf("\n=== Roupas ===\n");
        for (int i = 0; i < MAX_ITENS; i++) {
            printf("%s: %d\n", roupas[i].nome, roupas[i].quantidade);
        }
        menuLoginInstituicao();
    } else if (tipo == 2) { // Conferir doações de alimentos
        printf("\n=== Alimentos ===\n");
        for (int i = 0; i < MAX_ITENS; i++) {
            printf("%s: %d\n", alimentos[i].nome, alimentos[i].quantidade);
        }
        menuLoginInstituicao();
    } else {
        printf("Tipo de doação inválido!\n");
        fflush(stdout);
        menuLoginInstituicao();
    }
}

// Função para listar todas as instituições cadastradas
void listarInstituicoes() {
    printf("\n=== Instituições ===\n");
    for (int i = 0; i < num_instituicoes; i++) {
        printf("Nome: %s\nCidade/Bairro: %s\nResponsável: %s\n\n",
               instituicoes[i].nome,
               instituicoes[i].cidade_bairro,
               instituicoes[i].responsavel);
    }
    menuLoginUsuario(); // Retorna ao menu do usuário
}

// Função principal do menu para usuários comuns
void menuPrincipal() {
    int escolha;

    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Doar\n");
        printf("2. Conferir Doações\n");
        printf("3. Listar Instituições\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer do stdin

        switch (escolha) {
            case 1:
                doar();
                break;
            case 2:
                conferirDoacoes();
                break;
            case 3:
                listarInstituicoes();
                break;
            case 4:
                printf("Saindo...\n");
                fflush(stdout);
                salvarDados(); // Salva os dados antes de sair
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 4);
}

// Função principal do menu para instituições
void menuLoginInstituicao() {
    int escolha;

    do {
        printf("\n=== Menu Instituição/ONG ===\n");
        printf("1. Doar\n");
        printf("2. Conferir Doações\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer do stdin

        switch (escolha) {
            case 1:
                doar();
                break;
            case 2:
                conferirDoacoes();
                break;
            case 3:
                printf("Saindo...\n");
                fflush(stdout);
                instituicao_logada = -1; // Logout da instituição
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 3);
}

// Função principal do menu para usuários comuns
void menuLoginUsuario() {
    int escolha;

    do {
        printf("\n=== Menu Usuário Comum ===\n");
        printf("1. Listar Instituições\n");
        printf("2. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer do stdin

        switch (escolha) {
            case 1:
                listarInstituicoes();
                break;
            case 2:
                printf("Saindo...\n");
                fflush(stdout);
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 2);
}

// Função principal do programa
int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para português
    carregarDados(); // Carrega os dados dos arquivos
    telaPrincipal(); // Exibe a tela principal do programa
    return 0;
}
