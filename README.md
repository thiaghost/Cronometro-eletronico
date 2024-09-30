## Visão Geral
Este código implementa um cronômetro usando um microcontrolador AVR, projetado para medir e exibir o tempo decorrido (minutos, segundos e milissegundos) em um display de 7 segmentos. O temporizador é controlado pelo Timer0 no modo CTC (Clear Timer on Compare Match), com interrupções usadas para incrementar os contadores de milissegundos, segundos e minutos.

## Requisitos de Hardware
- Microcontrolador AVR (ex: ATmega328P)
- Display de 7 segmentos (4 dígitos)
- Botões para controle
- Componentes externos, como resistores, capacitores e possivelmente transistores para o controle do display
- Ambiente de desenvolvimento AVR para compilação (ex: Atmel Studio)
- Um clock externo rodando a 16 MHz (F_CPU definida como 16 MHz)

## Funcionalidades
- **Medição de Tempo**: O cronômetro mede o tempo em minutos, segundos e milissegundos, atualizando o display conforme o tempo passa.
- **Display de 7 segmentos**: O tempo atual é exibido em um display de 7 segmentos com 4 dígitos.
- **Função de Iniciar/Pausar**: Botões conectados a pinos específicos (PB4 e PB5) controlam as funções de iniciar e pausar do cronômetro.
- **Interrupção**: O mecanismo de temporização é baseado na interrupção do Timer0, que é disparada a cada milissegundo.

## Estrutura do Código

### Constantes e Variáveis
- `F_CPU`: Define a frequência do clock (16 MHz).
- `display7seg[10]`: Tabela de consulta para converter números (0-9) para sua codificação correspondente no display de 7 segmentos.

### Inicialização do Timer (`timer_init()`)
- Configura o Timer0 no modo CTC, que conta até o valor em `OCR0A` (249 neste caso) antes de reiniciar.
- O prescaler é configurado para 64, o que garante que o Timer0 gere uma interrupção a cada milissegundo.

### Inicialização de I/O (`io_init()`)
- Configura os pinos de PORTD e PORTB para o display de 7 segmentos e os botões.
- Define o estado inicial para PORTB, para monitorar os botões.

### Rotina de Serviço de Interrupção (`ISR(TIMER0_COMPA_vect)`)
- Essa interrupção é disparada a cada milissegundo.
- Incrementa o contador `mili` e, após atingir 1000, incrementa o contador `seg` (segundos).
- Quando `seg` atinge 60, o contador `min` (minutos) é incrementado e `seg` é reiniciado.

### Funções de Controle de Display (`display_on()`, `display_off()`)
- Essas funções controlam qual dígito do display de 7 segmentos (dos 4 disponíveis) está ativo, manipulando os pinos de PORTB.

### Loop Principal (`main()`)
- Atualiza continuamente o display de 7 segmentos para mostrar o tempo atual.
- Utiliza multiplexação, alternando rapidamente entre os dígitos para simular um display contínuo.
- Durante cada iteração do loop, verifica-se os botões:
  - **PB4**: Pausa o cronômetro parando o Timer0.
  - **PB5**: Retoma o cronômetro reiniciando o Timer0 com o prescaler adequado.

## Como Funciona
1. **Configuração**: O microcontrolador inicializa as configurações de I/O e do temporizador. O Timer0 é configurado para gerar uma interrupção a cada milissegundo.
2. **Manipulação de Interrupção**: Cada vez que a interrupção do timer é acionada, o contador de milissegundos é incrementado. Quando 1000 milissegundos passam, o contador de segundos é incrementado e, quando 60 segundos passam, o contador de minutos é atualizado.
3. **Multiplexação de Display**: O display de 7 segmentos é atualizado alternando entre cada dígito e atualizando seu valor rapidamente, criando a ilusão de que todos os dígitos estão sendo exibidos simultaneamente.
4. **Funcionalidade dos Botões**:
   - Se o botão **PB4** for pressionado, o cronômetro para.
   - Se o botão **PB5** for pressionado, o cronômetro retoma.

## Compilação e Uso
- Para compilar e carregar o código, use um conjunto de ferramentas compatível com AVR, como AVR-GCC, e um gravador (ex: USBasp).
- Conecte o microcontrolador ao display de 7 segmentos e aos botões conforme especificado no esquema de hardware.
- Uma vez programado, o microcontrolador iniciará automaticamente o cronômetro, exibindo o tempo decorrido no display de 7 segmentos.
- Utilize os botões conectados para pausar ou retomar o cronômetro.

## Notas
- O cronômetro foi projetado para ser o mais preciso possível, dentro das limitações do clock e das configurações do prescaler do microcontrolador.
- O código assume que os botões são do tipo "active-low", ou seja, são aterrados quando pressionados.
