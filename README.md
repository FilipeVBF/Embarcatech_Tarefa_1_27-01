# Embarcatech - Tarefa 1 - Aula Síncrona 27.01

Este projeto foi criado como parte da Tarefa 1 da Aula Síncrona de 27 de Janeiro do EmbarcaTech.

## Descrição do Projeto

Este projeto explora o uso de interrupções no microcontrolador RP2040 por meio da placa de desenvolvimento BitDogLab. O objetivo desse projeto foi a construção de um contador simples, controlado por dois botões (A e B), cujas funções são incrementar e decrementar o valor exibido em uma matriz de LEDs 5x5 WS2812. Para evitar acionamentos indesejados nos botões, foi implementada uma técnica de debouncing via software. Além disso, foi adicionado um LED RGB vermelho que pisca 5 vezes por segundo.

### Funcionalidades:
- A matriz de LEDs 5x5 exibe o número atual do contador.
- O botão A, conectado ao pino 5, incrementa o valor do contador.
- O botão B, conectado ao pino 6, decrementa o valor do contador.
- O LED RGB no pino 13 na cor vermelho piscando 5 vezes por segundo.
- A técnica de debouncing foi aplicada via software para evitar múltiplos acionamentos causados por "bouncing" nos botões.

## Componentes Utilizados

- **Matriz de LEDs 5x5 WS2812**: Exibe os números do contador.
- **Botão A (Pino 5)**: Ação de incremento do contador.
- **Botão B (Pino 6)**: Ação de decremento do contador.
- **LED RGB (Pino 13)**: Pisca 5x por segundo na cor vermelha.

## Requisitos Técnicos

- **Interrupções:** Todas as funcionalidades dos botões são implementadas com rotinas de interrupção (IRQ).
- **Debouncing:** Tratamento do bouncing dos botões via software.
- **Controle de LEDs:** Uso de LEDs comuns e LEDs WS2812 para demonstrar domínio sobre diferentes tipos de controle.
- **Código organizado:** O código-fonte está bem estruturado e comentado para facilitar o entendimento.

## Vídeo da Solução
Segue abaixo o link do vídeo da demonstração do projeto:
