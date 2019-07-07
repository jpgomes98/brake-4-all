# brake-4-all

## Índice
1. Descrição
2. Material e Configuração
3. Código Exemplo
4. Autores e Agradecimentos

## Descrição
*Brake-4-All* é um projeto *open-source* baseado na plataforma *Arduino*, cujo objetivo é conferir mais segurança a todos os velocípedes que partilham a via pública com automóveis e motas. Seguindo o lema, *Ser visto para estar protegido*, é essencial que os velocípetes garantam que o seu movimento é identificado facilmente pelos automóveis circulantes na via, tanto de dia como à noite. 

Este projeto permite que qualquer pessoa possa, em regime **faça você mesmo** e com um **baixo custo associado**, construir um sensor de travagem baseado num acelerómetro de 3 eixos que permita acionar a luz traseira de uma bicicleta quando se dá uma travagem, colocando a luz vermelha traseira em modo intermitente e regressando de seguida ao modo de brilho contínuo. Adicionalmente, é altamente personalizável, portátil, compacto e fácil de montar em qualquer bicicleta, reduzindo o número de cabos que são necessários instalar face a um sensor baseado no acionamento do manípulo do travão.

## Material e Montagem

### Material
O sensor baseia-se num Arduino, num acelerómetro de 3 eixos e numa matriz de LEDs. Para o caso aqui testado recorreu-se a:

* Microcontrolador ESP-32 Dev Kit v1 a funcionar na *framework* Arduino
* *SparkFun Triple Axis Accelerometer Breakout - MMA8452Q* - Acelerómetro de 3-eixos - biblioteca [SparkFun MMA8452Q Arduino Library](https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library/tree/V_1.1.0) e [*datasheet*](https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/MMA8452Q.pdf)
* *Velleman VMW107* - Anel de 10 LEDs RGB - biblioteca [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
* Resistência de 330 Ω

### Montagem

![Montagem](https://github.com/jpgomes98/brake-4-all/blob/master/docs/montagem.png)

### Circuito
As ligações efetuadas na breadboard e no anel são:

* Acelerómetro

Arduino | MMA8452Q
------------ | -------------
 3.3V| 3.3V
 GND | GND
 D21 | SDA
 D22 | SCL
  
* Matriz LED

Arduino | VMW107
------------ | -------------
 3.3V|  VDD
 GND | GND
 D25 | D_IN
   
(para esta última ligação recomenda-se o uso de uma resistência de 300-500 Ω para minimizar o ruído)

## Código Exemplo

## Autores e Agradecimentos

Projeto desenvolvido no âmbito da cadeira de *Laboratório de Inovação e Desenvolvimento* do Mestrado Integrado em Engenharia Física Tecnológica do Instituto Superior Técnico, sob orientação da Profª Ana Mourão pelos alunos **Leandro Sobral**, **João Pedro Ornelas** e **João Pedro Gomes**.

Agradecimento especial ao *Núcleo de Física do Instituto Superior Técnico - NFIST* e à *Hackerschool*, ambos núcleos do IST pelo apoio logístico oferecido ao longo de toda a execução do projeto.


Este produto é open source! Esta livre de utilizar, reutilizar e modificar totalmente estes ficheiros de acordo com o propósito. 
Distribuído como está; sem garantia.

**Cópia da Apache License V2.0**

Copyright 2019 Leandro Sobral, João Pedro Ornelas e João Pedro Gomes.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
