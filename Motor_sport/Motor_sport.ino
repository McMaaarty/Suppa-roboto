#include <TScheduler.hpp>
#include <TSchedulerDeclarations.hpp>
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>

#include <Servo.h>

// Moteurs
const int moteur1 = 5;
const int moteur2 = 3;

// Capteur ultrasons
const int trig_pin = 7;
const int echo_pin = 8;
float duration_us, distance_cm;

// Servo-moteur
Servo myServo;
const int servoPin = 9;

// Dictionnaire pour les angles et distances
struct KeyValuePair {
  int key;    // Angle
  int value;  // Distance
};

const int MAX_ITEMS = 5;
KeyValuePair dictionary[MAX_ITEMS] = {
  {0, 0},    // Angle 0° (gauche)
  {45, 0},   // Angle 45° (avant-gauche)
  {90, 0},   // Angle 90° (avant)
  {135, 0},  // Angle 135° (avant-droit)
  {180, 0}   // Angle 180° (droit)
};

// Distance critique pour détecter un obstacle
const int DISTANCE_CRITIQUE = 30; // En cm
const int DISTANCE_MAX = 1000;     // Plafond des distances

// Création du Scheduler
TsScheduler runner;

// Fonction de détection des obstacles
void detectObstacles() 
{
  // Balayage des distances
  for (int i = 0; i < MAX_ITEMS; i++) {
    myServo.write(dictionary[i].key); // Déplacer le servo à l'angle
    delay(250); // Temps pour que le servo atteigne la position
    int dist = getDistance(); // Mesurer la distance
    dictionary[i].value = (dist > DISTANCE_MAX) ? DISTANCE_MAX : dist; // Limiter à DISTANCE_MAX
    Serial.print("Angle: ");
    Serial.print(dictionary[i].key);
    Serial.print(" Distance: ");
    Serial.println(dictionary[i].value);
  }
}

// Déclaration de la tâche de détection
TsTask taskDetect(1000, TASK_FOREVER, detectObstacles);  // Détection toutes les 1000ms

// Variables globales pour la distance actuelle et direction
int forwardDistance = 0;  // Distance à l'avant
int maxDistanceIndex = 0; // Index de la direction avec la plus grande distance


void setup() {
  pinMode(moteur1, OUTPUT);
  pinMode(moteur2, OUTPUT);
  
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  myServo.attach(servoPin);
  myServo.write(90);
  delay(500);

  Serial.begin(9600);
}

void loop() {
  
  // Exécution des tâches planifiées
  runner.execute();

  // Logique de déplacement continu
  forwardDistance = dictionary[2].value; // Angle 90° (avant)
  
  if (forwardDistance > DISTANCE_CRITIQUE) 
  {
    // Si l'avant est dégagé, avancer
    Serial.println("Avancer...");
    goForward(forwardDistance * 10); // Ajuster la durée en fonction de la distance
  } 
  else 
  {
    // Si un obstacle est détecté à l'avant, chercher la meilleure direction
    Serial.println("Obstacle détecté. Recherche d'un chemin libre...");
    maxDistanceIndex = findMaxDistanceIndex();
    
    // Utilisation de switch pour choisir la direction à prendre
    switch (dictionary[maxDistanceIndex].key) {
      case 0:
        // Si la meilleure distance est à gauche (0°), tourner à gauche
        Serial.println("Tourner à gauche...");
        turnLeft(250);
        break;

      case 45:
        // Si la meilleure distance est en avant-gauche (45°), tourner légèrement à gauche
        Serial.println("Tourner avant-gauche...");
        turnLeft(150);
        break;

      case 90:
        // Si la meilleure distance est devant (90°), avancer
        Serial.println("Avancer...");
        goForward(dictionary[maxDistanceIndex].value * 10);
        break;

      case 135:
        // Si la meilleure distance est en avant-droit (135°), tourner légèrement à droite
        Serial.println("Tourner avant-droit...");
        turnRight(150);
        break;

      case 180:
        // Si la meilleure distance est à droite (180°), tourner à droite
        Serial.println("Tourner à droite...");
        turnRight(250);
        break;

      default:
        // Dans le cas où l'index n'est pas reconnu
        Serial.println("Aucune direction définie.");
        break;
    }
  }
}

// Trouver l'index de la distance maximale
int findMaxDistanceIndex() {
  int maxIndex = 0;
  for (int i = 1; i < MAX_ITEMS; i++) {
    if (dictionary[i].value > dictionary[maxIndex].value) {
      maxIndex = i;
    }
  }
  return maxIndex;
}

// Mesurer la distance
int getDistance()
{
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig_pin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echo_pin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  return distance_cm;
}

// Fonctions pour contrôler les moteurs
void goForward(int duration) 
{
  analogWrite(moteur1, 255);
  analogWrite(moteur2, 255);
  delay(duration);
  //stopMotors();
}

void turnLeft(int duration) 
{
  analogWrite(moteur1, 0);
  analogWrite(moteur2, 255);
  delay(duration);
  stopMotors();
}

void turnRight(int duration) 
{
  analogWrite(moteur1, 255);
  analogWrite(moteur2, 0);
  delay(duration);
  stopMotors();
}

void stopMotors() 
{
  analogWrite(moteur1, 0);
  analogWrite(moteur2, 0);
}

int getValue(int key) 
{
  
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (dictionary[i].key == key) {
      return dictionary[i].value; // Retourne la valeur associée
    }
  }

  // Retourne -1 si la clé n'est pas trouvée
  return -1; 
}