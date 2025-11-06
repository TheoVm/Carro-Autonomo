#include <Servo.h>

#define TRIGGER_PIN 3
#define ECHO_PIN 2
#define SERVO_PIN 11
#define IN1 9
#define IN2 10
#define IN3 7
#define IN4 8
#define ENA 6
#define ENB 5

const int DISTANCIA_SEGURA = 15;  // cm
const int ANGULO_CENTRAL = 90;
const int ANG_ESQ = 150;
const int ANG_DIR = 30;

Servo servo;

void robo_frente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void robo_re() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void robo_esquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void robo_direita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void robo_parado() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

float medirDistancia() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracao == 0){ 
    return 400;
  }

  float distancia = (duracao * 0.0343) / 2.0;
  return distancia;
}

float distanciaMedia(int amostras = 5) {
  float soma = 0;
  for (int i = 0; i < amostras; i++) {
    float d = medirDistancia();
    soma += d;
    delay(30);
  }
  return soma / amostras;
}

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);

  servo.attach(SERVO_PIN);
  servo.write(ANGULO_CENTRAL);
  delay(1000);

  Serial.println("Sistema iniciado!");
}

void loop() {
  float distFrente = distanciaMedia();
  Serial.print("Distância frontal: ");
  Serial.println(distFrente);

  if (distFrente > DISTANCIA_SEGURA) {
    robo_frente();
  } else {
    robo_parado();
    Serial.println("Obstáculo detectado!");

    servo.write(ANG_ESQ);
    delay(400);
    float distEsq = distanciaMedia();

    servo.write(ANG_DIR);
    delay(400);
    float distDir = distanciaMedia();

    servo.write(ANGULO_CENTRAL);
    delay(200);

    Serial.print("Dist esquerda: "); Serial.print(distEsq);
    Serial.print(" | Dist direita: "); Serial.println(distDir);

    if (distEsq > distDir && distEsq > DISTANCIA_SEGURA) {
      Serial.println("Virando à esquerda...");
      robo_esquerda();
      delay(700);
    } else if (distDir > distEsq && distDir > DISTANCIA_SEGURA) {
      Serial.println("Virando à direita...");
      robo_direita();
      delay(700);
    } else {
      Serial.println("Sem caminho livre — recuando...");
      robo_re();
      delay(700);
    }

    robo_parado();
    delay(200);
  }

  delay(100);
}

