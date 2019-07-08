/*
  brakeSensorBasic.ino
  Codigo para o desenvolvimento básico do sensor de travagem
  Autores: Leandro Sobral, João Pedro Ornelas e João Pedro Gomes
  Projeto no ambito da cadeira de LID
  v1.0 / Julho de 2019
*/

/* Bibliotecas gerais a utilizar */
#include <Wire.h>                 // Tem de incluir para I2C
#include "SparkFun_MMA8452Q.h"    // Acelerometro - Click here to get the library: https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library
#include <Adafruit_NeoPixel.h>    // LEDs RGB - Click here to get the library: https://github.com/adafruit/Adafruit_NeoPixel


MMA8452Q accel; // objeto da classe MMA8452Q, inclui todas as funcoes do acelerometro

/* Codigo especifico do anel */
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    25  // Pino D25 do ESP32 para a ligacao de dados do anel

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 10   // 10 para o anel

/* Instanciar um objeto da classe Adafruit_NeoPixel, permite controlar o anel de LEDs */
// Declare your NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


/* Variaveis */

float aX, aY, aZ; // guardar a aceleracao instantanea medida nos 3 eixos
float aTotal;     // guardar a aceleracao total instantanea medida nos 3 eixos

float aYThreshold = 0.5; 
/* IMPORTANTE: 
 *  alterar este valor de acordo com o uso do sensor de travagem
 *  Determinar qual o valor de aceleracao sentido no eixo dos YYs (direcao do movimento)
 *  Num teste realizado 0.5*g foi o valor que mais se adequou aos valores
 *  de aceleracao obtidos na travagem com a bicicleta de teste
 */

/* Variaveis de calibracao - medidas quando o programa inicia 
 * e requerem que o sensor esteja colocado na orientacao vertical
 * na traseira da bicicleta
*/
float aX0;
float aTotal0;

/* Componentes da forca gravitica 
 * Estas componentes sao calculadas instantaneamente por comparacao
 * do valores medidos a cada instante pelo acelerometro e a configuracao
 * de calibracao inicial de modo a remover o efeito da gravidade das medidas de
 * aceleracao na direcao do movimento
*/
float gY;

/* Variavel que verifica se se deu uma travagem */
int brakeCheck = 0;

/* Variaveis auxiliares */
int count = 0;
int countMax = 2500000;



/*  Modos de iluminacao do anel LED RGB - retirado da biblioteca da Adafruit NeoPixel */
// Adafruit functions to create animated effects

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

/* Programa secundário, corre uma vez apenas */
void setup() {

  /* Setup da comunicação serie para teste do equipamento */
  Serial.begin(9600);

  /* Setup do acelerometro */
  Wire.begin();
  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
  
  accel.setScale(SCALE_2G);
  /************************/

  /* LED Strip setup e teste inicial*/
  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
  colorWipe(strip.Color(255,255,255), 0); // 
  /************************/

  /* SETUPS INICIAIS
   * Testar se, EM REPOUSO, temos uma aceleração total de 1G 
   * pois so a forca da gravidade atua.
  */
  
  aTotal0 = sqrt(aX*aX + aY*aY + aZ*aZ);

  /* Adicionalmente obter o valor de aceleracao no eixo colocado segundo a
   * vertical do sensor (perpendicular ao eixo sensivel - Y) de modo a que 
   * seja possivel detetar alteraçoes da posicao perfeitamente vertical do sensor
   * para que se possa corrigir o sinal do eixo sensivel do efeito da aceleracao
   * da gravidade
  */
  
  aX0 = (-1)*accel.getCalculatedX();
  

}

/* Programa principal, sempre a correr */
void loop() {

  
  if (brakeCheck == 1) {
    
    /* Se houve uma travagem, manter a luz ligada algum tempo
     *  atraves da iteracao por alguns ciclos da variavel count de 0
     *  ate countMax - esta variavel deve ser ajustada para que a luz estar
     *  ligada so o tempo suficiente para se perceber que se acionou uma travagem
    */
    
    ++count;
    if (count == countMax){
      /* Apos se ter iterado o ciclo que mantem a luz ligada,
       * sair do ciclo de contagens desativando flag e colocando  
       * a luz no modo normal de movimento - BRANCO CONTINUO (mudar para VERMELHO INTERMITENTE)*/
      brakeCheck = 0;
      count = 0;
      colorWipe(strip.Color(255,255,255), 0);
    }
  }
  else {
    if (accel.available()) {      // Esperar que o acelerometro tenha dados para enviar
      // Aceleracao nas direcoes x, y, e z em unidades de g = 9.81 m/s^2
      // X
      aX = accel.getCalculatedX();
      // Y
      aY = (-1)*accel.getCalculatedY(); // OPCIONAL - obter os dados do eixo Y no sentido convencional do referencial cartesiano (corrigir o que vem de fabrica)
      // Z
      aZ = accel.getCalculatedZ();

      /* Algoritmo de remocao da gravidade do sinal do sensor - util para tornar o sensor funcional em estradas inclinadas */
      if(aX != aX0){       
             
      /* Se for detetada variacao da inclinacao do sensor 
       * em relacao a vertical (praticamente sempre), entao
       * calcula-se o valor da componente da gravidade que agora se encontra sobre o eixo 
       * sensivel do sensor (na direcao do movimento) a partir do valor total medido na calibracao
       * e os valores das componentes sobre os restantes 2 eixos do sensor. Assume-se que nao se deu
       * aceleracao extra nesses eixos alem da aceleracao gravitica. Este metodo nao e obviamente infalivel
       * mas permite obter uma boa estimativa
       */
        
        gY = sqrt(aTotal0*aTotal0 - aX*aX - aZ*aZ);

        /* Com este novo valor da componente de g sobre o eixo Y (eixo sensivel, orientado no sentido do movimento)
         * subtraimos ao valor medido ao longo de Y esta componente gravitica e assim o nosso sinal a partida, medira
         * so aceleracoes nao provocadas pela gravidade
        */
        
        if(aY > 0){ // Se e maior que zero, subtraimos a componente de Y da gravidade - Ex: ciclista a descer uma rampa
          aY = aY - gY;
        }
        else{       // Se e menor que zero, adicionamos a componente de Y da gravidade - Ex: ciclista a subir uma rampa
          aY = aY + gY;
        }

        /* Em ambos os casos (subir ou descer um desnível) 
         * temos de garantir que o eixo sensível 
         * nao e perturbado pela gravidade 
         */
        
      }
      
      aTotal = sqrt(aX*aX + aY*aY + aZ*aZ); // valor total da aceleracao medida nos 3 eixos

      
    }

    
    /* OPCIONAL - Enviar informação de teste pela porta série se esta estiver ligada */
    Serial.print(-aYThreshold);  
    Serial.print(" ");
    Serial.print(aYThreshold);    
    Serial.print(" ");  
    Serial.println(aY);

    /* Condicao de Travagem - 
     *  Convencionou-se que o sentido da travagem era o contrario ao movimento (eixo positivo dos Y)
     *  Convencionou-se que o movimento se da no sentido negativo do eixo Y
     *  Este aYThreshold e o threshold usado para saber se houve travagem ou nao */
     
    if (aY >= aYThreshold) {
      brakeCheck = 1;
      /* Se houve travagem, entrar no ciclo que mantem a luz ligada e ligar a luz no modo de travagem - VERMELHO CONTINUO */
      colorWipe(strip.Color(255,0,0), 0);
    }
  }

}
