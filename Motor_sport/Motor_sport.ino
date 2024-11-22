// Définition des broches de commande des moteurs
const int moteur1 = 6; // Contrôle du moteur Droit
const int moteur2 = 5; // Contrôle du moteur Gauche

const int trig_pin = 9;
const int echo_pin = 10;
float timing = 0.0;
float distance = 0.0;

float getDistance()
{
  digitalWrite(trig_pin, LOW);
  delay(2);
  
  digitalWrite(trig_pin, HIGH);
  delay(10);
  digitalWrite(trig_pin, LOW);
  
  timing = pulseIn(echo_pin, HIGH);
  distance = (timing * 0.034) / 2;

  return distance;
}

void setup() {
  // Configuration des broches en sortie
  pinMode(moteur1, OUTPUT);
  pinMode(moteur2, OUTPUT);

  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  
  digitalWrite(trig_pin, LOW);
}

void loop() {
  
  float distance = getDistance();

  if(distance <= 25)
  {
    digitalWrite(moteur1, LOW);
    digitalWrite(moteur2, LOW);
  }
  else
  {
    digitalWrite(moteur1, HIGH);
    digitalWrite(moteur2, HIGH);
  }

  delay(1000);
}
