#include <LiquidCrystal.h>

namespace lib {
class h_bridge {
public:
  h_bridge(int s1, int s2) {
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    m_pin_1 = s1;
    m_pin_2 = s2;
  }
  h_bridge() = default;

  void forward() {
    digitalWrite(m_pin_1, LOW);
    digitalWrite(m_pin_2, HIGH);
  }
  void backward() {
    digitalWrite(m_pin_1, HIGH);
    digitalWrite(m_pin_2, LOW);
  }
  void brake() {
    digitalWrite(m_pin_1, HIGH);
    digitalWrite(m_pin_2, HIGH);
  }
  void coast() {
    digitalWrite(m_pin_1, LOW);
    digitalWrite(m_pin_2, LOW);
  }

private:
  int m_pin_1;
  int m_pin_2;
};
}


//Initialisation de l'ecran
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Definiton entree joystick moteurs
const int joy_motor_x = A0; // Entree analogique
const int joy_motor_y = A1; // Entree analogique

//Definition entree joystick bras
const int joy_arm_x = A2; // Entree analogique
const int joy_arm_button = A3; // Entree numerique

//Definition des boutons pour compter les points
const int more_five = A4; // Entree numerique
const int more_one = A5; // Entree numerique

lib::h_bridge right_bridge;
lib::h_bridge left_bridge;
lib::h_bridge arm;
lib::h_bridge claw;

//Variables diverses
int points = 0; //Nombre de points durant la partie
bool claw_opened = false; //Correspond a l'etat d'ouverture du bras
int precision = 100; //Temps en ms entre la mise en mouvemement et l'arret d'un moteur

void displayPoints(int points) {
  lcd.print("Points : " + String(points));
}

int analog2digital(int input) {
  if (input > 1000) {
    return 1;
  }
  else if (input < 50) {
    return -1;
  }
  else {
    return 0;
  }
}

void setup() {


//Definition dont ont besoin les moteurs
right_bridge = lib::h_bridge(13, 12);
left_bridge = lib::h_bridge(11, 10);

//Moteurs du bras
arm = lib::h_bridge(7, 6);
claw = lib::h_bridge(9, 8);

  //Initialisation de l'ecran

  lcd.begin(16, 2);
  lcd.print("Bienvenue");
  
  pinMode(joy_motor_x, INPUT);
  pinMode(joy_motor_y, INPUT);

  pinMode(joy_arm_x, INPUT);
  pinMode(joy_arm_button, INPUT);
  digitalWrite(joy_arm_button, HIGH);

  pinMode(more_five, INPUT);
  pinMode(more_one, INPUT);
}

void loop() {
  //----- Lecture des info pour les moteurs de mouvements -----
  //Action de l'axe X
  if (analog2digital(analogRead(joy_motor_x)) == 1) {
    //Fait avancer les 2 moteurs
    right_bridge.forward();
    left_bridge.forward();
    delay(precision);
    right_bridge.brake();
    left_bridge.brake();
  }
  else if (analog2digital(analogRead(joy_motor_x)) == -1) {
    //Fait reculer les 2 moteurs
    right_bridge.backward();
    left_bridge.backward();
    delay(precision);
    right_bridge.brake();
    left_bridge.brake();
  }
  //Action de l'axe Y
  if (analog2digital(analogRead(joy_motor_y)) == 1) {
    left_bridge.forward(); //Fait tourner le robot vers la droite
    delay(precision);
    left_bridge.brake();
  }
  else if (analog2digital(analogRead(joy_motor_y)) == -1) {
    right_bridge.forward(); //Fait tourner le robot vers la gauche
    delay(precision);
    right_bridge.brake();
  }

  //----- Lecture des infos pour les moteurs du bras -----
  if (analog2digital(analogRead(joy_arm_x)) == 1) {
    arm.forward();
    delay(precision);
    arm.brake();
  }
  else if (analog2digital(analogRead(joy_arm_x)) == -1) {
    arm.backward();
    delay(precision);
    arm.brake();
  }
  if (digitalRead(joy_arm_button)) {
    if (claw_opened) {
      claw.forward();
      delay(1000);
      claw.brake();
    }
    else {
      claw.backward();
      delay(1000);
      claw.brake();
    }
  }

  //Lecture concernant les boutons
  if (digitalRead(more_five)) {
    points += 5;
    displayPoints(points);
  }
  if (digitalRead(more_one)) {
    points ++;
    displayPoints(points);
  }
}
