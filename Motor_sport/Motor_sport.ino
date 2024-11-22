#include <Servo.h>

// Moteurs
const int moteur1 = 5;
const int moteur2 = 3;

// Capteur ultrasons
const int triggerPin = 7;
const int echoPin = 8;

// Servo-moteur
Servo myServo;
const int servoPin = 9;

// Paramètres
const int DISTANCE_MAX = 50;
const int angles[] = {0, 45, 90, 135, 180};
int distances[5];

// Grille mémoire
const int GRID_SIZE = 20;
int map[GRID_SIZE][GRID_SIZE] = {0};
int posX = GRID_SIZE / 2;
int posY = GRID_SIZE / 2;

void setup() {
  pinMode(moteur1, OUTPUT);
  pinMode(moteur2, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(90);
  delay(500);

  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    myServo.write(angles[i]);
    delay(500);
    distances[i] = measureDistance();
    if (distances[i] > DISTANCE_MAX) distances[i] = 0;
    updateMap(angles[i], distances[i]);
  }

  int maxDistanceIndex = 0;
  for (int i = 1; i < 5; i++) {
    if (distances[i] > distances[maxDistanceIndex]) {
      maxDistanceIndex = i;
    }
  }

  if (distances[maxDistanceIndex] > 20) {
    int moveDuration = distances[maxDistanceIndex] * 10;
    if (angles[maxDistanceIndex] == 90) {
      avancer(moveDuration);
    } else if (angles[maxDistanceIndex] < 90) {
      tournerGauche(moveDuration);
    } else {
      tournerDroite(moveDuration);
    }
  } else {
    tournerGauche(500); // Action par défaut
  }

  delay(500);
}

int measureDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void updateMap(int angle, int distance) {
  int deltaX = cos(radians(angle)) * distance / 10;
  int deltaY = sin(radians(angle)) * distance / 10;
  int x = posX + deltaX;
  int y = posY + deltaY;

  if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
    map[x][y] = 1;
  }
}

void avancer(int duration) {
  digitalWrite(moteur1, HIGH);
  digitalWrite(moteur2, HIGH);
  delay(duration);
  stopMoteurs();
}

void tournerGauche(int duration) {
  digitalWrite(moteur1, LOW);
  digitalWrite(moteur2, HIGH);
  delay(duration);
  stopMoteurs();
}

void tournerDroite(int duration) {
  digitalWrite(moteur1, HIGH);
  digitalWrite(moteur2, LOW);
  delay(duration);
  stopMoteurs();
}

void stopMoteurs() {
  digitalWrite(moteur1, LOW);
  digitalWrite(moteur2, LOW);
}
