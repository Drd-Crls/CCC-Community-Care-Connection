#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_INSTITUICOES 100
#define MAX_USUARIOS 100
#define MAX_NOME 50
#define MAX_SENHA 20
#define MAX_ITENS_ALIMENTOS 6
#define MAX_ITENS_ROUPAS 5

typedef struct {
    char nome[MAX_NOME];
    char cidade_bairro[MAX_NOME];
    char responsavel[MAX_NOME];
    char senha[MAX_SENHA];
    int doacoes_roupas[MAX_ITENS_ROUPAS];
    int doacoes_alimentos[MAX_ITENS_ALIMENTOS];
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

Instituicao instituicoes[MAX_INSTITUICOES];
Usuario usuarios[MAX_USUARIOS];
Item roupas[MAX_ITENS_ROUPAS] = {{"Camiseta", 0}, {"Calça", 0}, {"Sapato", 0}, {"Blusa", 0}, {"Cobertor", 0}};
Item alimentos[MAX_ITENS_ALIMENTOS] = {{"Arroz", 0}, {"Feijão", 0}, {"Macarrão", 0}, {"Café", 0}, {"Sal", 0}, {"Açúcar", 0}};

int num_instituicoes = 0;
int num_usuarios = 0;
int instituicao_logada = -1; // -1 indica que nenhuma instituição está logada

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
            // Inicializa as doações de roupas e alimentos
            for (int i = 0; i < MAX_ITENS_ROUPAS; i++) {
                instituicoes[num_instituicoes].doacoes_roupas[i] = 0;
            }
            for (int i = 0; i < MAX_ITENS_ALIMENTOS; i++) {
                instituicoes[num_instituicoes].doacoes_alimentos[i] = 0;
            }
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
        for (int i = 0; i < MAX_ITENS_ROUPAS; i++) {
            fscanf(file, "%49[^\n]\n%d\n", roupas[i].nome, &roupas[i].quantidade);
        }
        fclose(file);
    }

    file = fopen("alimentos.txt", "r");
    if (file) {
        for (int i = 0; i < MAX_ITENS_ALIMENTOS; i++) {
            fscanf(file, "%49[^\n]\n%d\n", alimentos[i].nome, &alimentos[i].quantidade);
        }
        fclose(file);
    }
}

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
        for (int i = 0; i < MAX_ITENS_ROUPAS; i++) {
            fprintf(file, "%s\n%d\n", roupas[i].nome, roupas[i].quantidade);
        }
        fclose(file);
    }

    file = fopen("alimentos.txt", "w");
    if (file) {
        for (int i = 0; i < MAX_ITENS_ALIMENTOS; i++) {
            fprintf(file, "%s\n%d\n", alimentos[i].nome, alimentos[i].quantidade);
        }
        fclose(file);
    }
}

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

        switch (escolha) {
            case 1:
                system("cls");
                cadastro();
                break;
            case 2:
                system("cls");
                login();
                break;
            case 3:
                printf("Saindo...\n");
                fflush(stdout);
                salvarDados();
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

void doar() {
    if (instituicao_logada == -1) {
        int escolha_instituicao;
        listarInstituicoes();
        printf("Escolha a instituição para a doação (número): ");
        fflush(stdout);
        scanf("%d", &escolha_instituicao);
        getchar();

        if (escolha_instituicao < 1 || escolha_instituicao > num_instituicoes) {
            printf("Instituição inválida!\n");
            return;
        }

        instituicao_logada = escolha_instituicao - 1; // Ajusta para índice base 0
    }

    int tipo;
    printf("\n=== Doar ===\n");
    printf("1. Roupas\n");
    printf("2. Alimentos\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar();
    
    if (tipo == 1) {
        printf("\n=== Doação de Roupas ===\n");
        for (int i = 0; i < MAX_ITENS_ROUPAS; i++) {
            int quantidade;
            printf("Quantidade de %s: ", roupas[i].nome);
            fflush(stdout);
            scanf("%d", &quantidade);
            getchar();
            if (quantidade > 0) {
                instituicoes[instituicao_logada].doacoes_roupas[i] += quantidade;
            }
        }
    } else if (tipo == 2) {
        printf("\n=== Doação de Alimentos ===\n");
        for (int i = 0; i < MAX_ITENS_ALIMENTOS; i++) {
            int quantidade;
            printf("Quantidade de %s: ", alimentos[i].nome);
            fflush(stdout);
            scanf("%d", &quantidade);
            getchar();
            if (quantidade > 0) {
                instituicoes[instituicao_logada].doacoes_alimentos[i] += quantidade;
            }
        }
    } else {
        printf("Tipo de doação inválido!\n");
    }
    instituicao_logada = -1; // Reseta a instituição logada após a doação
    fflush(stdout);
    system("cls");
}

void conferirDoacoes() {
    if (instituicao_logada == -1) {
        printf("Nenhuma instituição está logada!\n");
        return;
    }

    int tipo;
    printf("\n=== Conferir Doações ===\n");
    printf("1. Roupas\n");
    printf("2. Alimentos\n");
    printf("Escolha uma opção: ");
    fflush(stdout);
    scanf("%d", &tipo);
    getchar();

    if (tipo == 1) {
        printf("\n=== Doações de Roupas ===\n");
        for (int i = 0; i < MAX_ITENS_ROUPAS; i++) {
            printf("%s: %d\n", roupas[i].nome, instituicoes[instituicao_logada].doacoes_roupas[i]);
        }
    } else if (tipo == 2) {
        printf("\n=== Doações de Alimentos ===\n");
        for (int i = 0; i < MAX_ITENS_ALIMENTOS; i++) {
            printf("%s: %d\n", alimentos[i].nome, instituicoes[instituicao_logada].doacoes_alimentos[i]);
        }
    } else {
        printf("Tipo de doação inválido!\n");
    }

    fflush(stdout);
    system("cls"); // Limpa a tela
}


void listarInstituicoes() {
    printf("\n=== Instituições ===\n");
    for (int i = 0; i < num_instituicoes; i++) {
        printf("%d. Nome: %s\nCidade/Bairro: %s\nResponsável: %s\n\n",
               i+1,
               instituicoes[i].nome,
               instituicoes[i].cidade_bairro,
               instituicoes[i].responsavel);
    }
}

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
                salvarDados();
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 4);
}

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
                instituicao_logada = -1; // Logout
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 3);
}

void menuLoginUsuario() {
    int escolha;

    do {
        printf("\n=== Menu Usuário Comum ===\n");
        printf("1. Listar Instituições\n");
        printf("2. Doar\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                listarInstituicoes();
                break;
            case 2:
                doar();
                break;
            case 3:
                printf("Saindo...\n");
                fflush(stdout);
                break;
            default:
                printf("Opção inválida!\n");
                fflush(stdout);
        }
    } while (escolha != 3);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    carregarDados();
    telaPrincipal();
    return 0;
}
