#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "embarcatech_tarefa.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define time 100

const uint led_red = 13;        // Red
const uint button_pin_A = 5;    // Botão A
const uint button_pin_B = 6;    //Botão B


static volatile uint number_display = 0; // Número a ser exibido
static volatile uint32_t last_time = 0; // Tempo do último clique

// Intensidade das cores RGB
uint8_t color_red = 0;
uint8_t color_green = 0;
uint8_t color_blue = 80;


static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

const uint8_t led_map[NUM_PIXELS] = {
    24, 23, 22, 21, 20,
    15, 16, 17, 18, 19,
    14, 13, 12, 11, 10,
     5,  6,  7,  8,  9,
     0,  1,  2,  3,  4
};

uint32_t led_buffer[10][NUM_PIXELS] = {
    // Número 0
    {
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 1 
    {
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0
    },
    // Número 2
    {
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0
    },
    // Número 3 
    {
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 4 
    {
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0
    },
    // Número 5
    {
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 6 
    {
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 7 
    {
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 0, 0
    },
    // Número 8 
    {
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // 9
    {
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    }
}; // Buffer para armazenar as cores de todos os LEDs

void change_led_buffer() {
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(color_red, color_green, color_blue);

    // Define todos os LEDs com a cor especificada
    for (int row = 0; row < 5; row++) {
        int start = row * 5;
        int end = start + 5 - 1;

        // Usando o mapeamento físico para corrigir a ordem
            for (int i = start; i <= end; i++) {
                int mapped_index = led_map[i];
                
                if (led_buffer[number_display][mapped_index]) {
                    put_pixel(color); // Liga o LED com um no buffer
                } else {
                    put_pixel(0);  // Desliga os LEDs com zero no buffer
                }
            }
        }
    }

// Função para inicializar o pino do LED
void init_gpio_led(int led_pin, bool is_output, bool state) {
    gpio_init(led_pin);                                         // Inicializa o pino do LED
    gpio_set_dir(led_pin, is_output ? GPIO_OUT : GPIO_IN);      // Configura o pino como saída ou entrada
    gpio_put(led_pin, state);                                   // Garante que o LED inicie apagado
}

// Função para inicializar o pino do botão
void init_gpio_button(int button_pin, bool is_output) {
    gpio_init(button_pin);                                          // Inicializa o botão
    gpio_set_dir(button_pin, is_output ? GPIO_OUT : GPIO_IN);       // Configura o pino como entrada pu saída
    gpio_pull_up(button_pin);                                       // Habilita o pull-up interno
}

// Função de interrupção para atualizar o número exibido
void update_number_display (uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 500000)
    {
        last_time = current_time;
        if (gpio == 5 && number_display < 9) {
            number_display++;
        } else if (gpio == 6 && number_display > 0) {
            number_display--;
        }     
    }
}

int main() {
    stdio_init_all();

    // Inicializando pino do LED RGB
    init_gpio_led(led_red, true, false);

    // Inicializando pino do botão
    init_gpio_button(button_pin_A, false);
    init_gpio_button(button_pin_B, false);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(button_pin_A, GPIO_IRQ_EDGE_FALL, true, &update_number_display );
    gpio_set_irq_enabled_with_callback(button_pin_B, GPIO_IRQ_EDGE_FALL, true, &update_number_display );

    uint32_t start_time = to_ms_since_boot(get_absolute_time());   
    // Variavel para contar as piscadas por segundo
    int blink_count = 0;

    while (1) {
        // Atualiza o buffer com o LED
        change_led_buffer();

        // Lógica para o LED piscar a cada x tempo
        gpio_put(led_red, 1);
        sleep_ms(time);
        gpio_put(led_red, 0);
        sleep_ms(time);

        // Conta o número de piscadas
        blink_count++;

        if (to_ms_since_boot(get_absolute_time()) - start_time >= 1000) {
            printf("Piscadas em 1s: %d\n", blink_count);
            blink_count = 0;
            start_time = to_ms_since_boot(get_absolute_time());
        }
    }

    return 0;
}