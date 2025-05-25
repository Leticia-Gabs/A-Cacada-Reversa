#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "menu.h"

// Exibe o menu inicial e captura o nome do jogador
void exibir_menu(char* nome_jogador, int tamanho_max) {
    screenClear();
    init_screen();

    draw_text(10, 3, "BEM-VINDO AO JOGO: A CACADA REVERSA");
    draw_text(10, 5, "Digite seu nome:");

    // Move o cursor para a posição após o texto
    printf("\033[6;28H"); // linha 6, coluna 28
    scanf("%s", nome_jogador);

    // Tela de boas-vindas
    screenClear();
    init_screen();

    char mensagem[100];
    snprintf(mensagem, sizeof(mensagem), "Bem-vindo, %s!", nome_jogador);
    draw_text(10, 5, mensagem);

    draw_text(10, 7, "Pressione ENTER para comecar...");

    keyboardInit();
    while (readch() != 10);
    keyboardDestroy();

    // Limpa antes de iniciar o jogo
    screenClear();
    init_screen();
}

