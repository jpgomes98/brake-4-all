# brake-4-all

## Índice
1. Descrição
2. Material e Configuração
3. Bibliotecas
4. Código Exemplo
5. Autores e Agradecimentos

## Descrição
*Brake-4-All* é um projeto *open-source* baseado na plataforma *Arduino*, cujo objetivo é conferir mais segurança a todos os velocípedes que partilham a via pública com automóveis e motas. Seguindo o lema, *Ser visto para estar protegido*, é essencial que os velocípetes garantam que o seu movimento é identificado facilmente pelos automóveis circulantes na via, tanto de dia como à noite. 

Este projeto permite que qualquer pessoa possa, em regime **faça você mesmo** e com um **baixo custo associado**, construir um sensor de travagem baseado num acelerómetro de 3 eixos que permita acionar a luz traseira de uma bicicleta quando se dá uma travagem, colocando a luz vermelha traseira em modo intermitente e regressando de seguida ao modo de brilho contínuo. Adicionalmente, é altamente personalizável, portátil, compacto e fácil de montar em qualquer bicicleta, reduzindo o número de cabos que são necessários instalar face a um sensor baseado no acionamento do manípulo do travão.

## Material e Montagem

### Material
O sensor baseia-se num Arduino, num acelerómetro de 3 eixos e numa matriz de LEDs. Para o caso aqui testado recorreu-se a:

* Microcontrolador ESP-32 Dev Kit v1 a funcionar na *framework* Arduino
* *SparkFun Triple Axis Accelerometer Breakout - MMA8452Q* - Acelerómetro de 3-eixos - biblioteca [SparkFun MMA8452Q Arduino Library](https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library/tree/V_1.1.0) e [*datasheet*](https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/MMA8452Q.pdf)
* *Velleman VMW107* - Anel de 10 LEDs RGB - biblioteca [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
* Resistência de 220 Ω

### Montagem

![Montagem](https://github.com/jpgomes98/brake-4-all/blob/master/docs/montagem.png)

### Circuito
As ligações efetuadas na breadboard e no anel são:

* Acelerómetro

  Arduino  ---------------    MMA8452Q
    3.3V   ---------------     3.3V
    GND    ---------------     GND
  SDA (A4) ---------------     SDA
  SCL (A5) ---------------     SCL
  
* Matriz LED

  Arduino  ---------------    VMW107
    3.3V   ---------------     VDD
    GND    ---------------     GND
    D25    --\/330 Ohm\/--     D_IN
    
    (Resistência para minimização do ruído)
  
