#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "menu.h"

// Exibe o menu inicial e captura o nome do jogador
void exibir_menu(char* nome_jogador, int tamanho_max) {
    screenClear();
    init_screen(); // Inicializa a tela e, SE implementado em screen.c, desenha as bordas

    // Título "CACADA" em estilo pixelado (aproximado)
    draw_text(10, 5, " CCC   AAA   CCC   AAA   DDD   AAA ");
    draw_text(10, 6, "C   C A   A C   C A   A D   D A   A ");
    draw_text(10, 7, "C     AAAAA C     AAAAA D   D AAAAA ");
    draw_text(10, 8, "C   C A   A C   C A   A D   D A   A ");
    draw_text(10, 9, " CCC  A   A  CCC  A   A DDD   A   A ");

    draw_text(10, 12, "👻 Digite seu nome, espírito corajoso:");
    printf("\033[13;45H"); // posiciona o cursor para entrada do nome

    scanf("%s", nome_jogador);

    // Tela de boas-vindas
    screenClear();
    init_screen(); // Inicializa novamente para limpar o prompt do nome

    char mensagem[100];
    snprintf(mensagem, sizeof(mensagem), "🌀 Bem-vindo à Caçada, %s!", nome_jogador);
    draw_text(10, 6, mensagem);

    draw_text(10, 8, "Pressione ENTER para comecar a captura...");

    keyboardInit();
    while (readch() != 10);
    keyboardDestroy();

    // Limpa antes de iniciar o jogo
    screenClear();
    init_screen(); // Inicializa novamente para limpar a tela de boas-vindas
}