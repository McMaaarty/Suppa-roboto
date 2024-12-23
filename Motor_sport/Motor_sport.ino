
// Inclut la lib Servo pour manipuler le servomoteur
#include <Servo.h>

// Définition des broches de commande des moteurs
// Moteur 1
const int enA = 6;        // Proportion de vitesse GAUCHE
const int moteurPin1 = 2; // Contrôle du moteur
const int moteurPin2 = 4;

// Moteur 2
const int enB = 11;       // Proportion de vitesse DROITE
const int moteurPin3 = 7; // Contrôle du moteur
const int moteurPin4 = 8;


// Variables pour gérer la tâche continue
int position = 45;                // Position initiale
int step = 1;                     // Direction et incrément de déplacement
unsigned long previousMillis = 0; // Stockage du temps précédent
const int interval = 15;          // Intervalle entre les mouvements (en ms)

// Servo
Servo servo;

// Detecteur ultrason
const int trig_pin = 9;
const int echo_pin = 10;
float timing = 0.0;
float distance = 0.0;

// Calcule de la distance
float getDistance()
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  timing = pulseIn(echo_pin, HIGH, 30000);
  if (timing == 0) return -1; // Retourner une valeur spéciale en cas d'erreur

  return (timing * 0.034) / 2;
}

// Commandes moteur
void goForward() 
{
  analogWrite(enA, 255); // Moteur GAUCHE puissance max
  analogWrite(enB, 190);    // Moteur DROITE puissance réduite
  digitalWrite(moteurPin1, LOW);
  digitalWrite(moteurPin2, HIGH);
  
  digitalWrite(moteurPin3, HIGH);
  digitalWrite(moteurPin4, LOW);
}

void goBackward()
{
  digitalWrite(moteurPin1, HIGH);
  digitalWrite(moteurPin2, LOW);
  
  digitalWrite(moteurPin3, LOW);
  digitalWrite(moteurPin4, HIGH);
}

void turnLeft()
{
  digitalWrite(moteurPin1, LOW);
  digitalWrite(moteurPin2, HIGH);
  
  digitalWrite(moteurPin3, LOW);
  digitalWrite(moteurPin4, HIGH);
}

void turnRight()
{
  digitalWrite(moteurPin1, HIGH);
  digitalWrite(moteurPin2, LOW);
  
  digitalWrite(moteurPin3, HIGH);
  digitalWrite(moteurPin4, LOW);
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void setup() {
  // Configuration des broches en sortie
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(moteurPin1, OUTPUT);
  pinMode(moteurPin2, OUTPUT);
  pinMode(moteurPin3, OUTPUT);
  pinMode(moteurPin4, OUTPUT);

  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  
  digitalWrite(trig_pin, LOW);

  goForward();

  servo.attach(3);

  Serial.begin (9300);
}

void loop() {
  
  // unsigned long currentMillis = millis();

  //   // Vérifier si le temps spécifié est écoulé
  // if (currentMillis - previousMillis >= interval) {
  //   previousMillis = currentMillis; // Mettre à jour le temps précédent

  //   // Mettre à jour la position du servo
  //   position += step;

  //   // Vérifier les limites
  //   if (position >= 120) {
  //     position = 120;
  //     step = -1;
  //   } else if (position <= 50) {
  //     position = 50;
  //     step = 1;
  //   }

  //   // Écrire la nouvelle position au servo
  //   servo.write(position);
  // }
  
  // // Logique de gestion des obstacles
  // if (distance > 0 && distance <= 25) { // Obstacle proche détecté
  //   stopMotors(); // Arrêter le robot
  //   delay(500); // Pause pour éviter les mouvements brusques

  //   if (position < 85) { // Servo vers la gauche
  //     turnRight(); // Tourner à droite
  //   } else { // Servo vers la droite
  //     turnLeft(); // Tourner à gauche
  //   }
    
  //   delay(500); // Pause pour permettre au robot de tourner
  //   goForward(); // Reprendre en avant
  // }
}
