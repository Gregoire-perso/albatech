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

lib::h_bridge right_bridge;
lib::h_bridge left_bridge;

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

void displayPoints(int points) {
  //Gerer l'ecran et afficher le nombre de points
}

void setup() {
  //Definiton entree joystick moteurs
  const int joy_motor_x = A0; // Entree analogique
  const int joy_motor_y = A1; // Entree analogique

  pinMode(joy_motor_x, INPUT);
  pinMode(joy_motor_y, INPUT);

  //Definition entree joystick bras
  const int joy_arm_x = A2; // Entree analogique
  const int joy_arm_button = 9; // Entree numerique
  bool arm_opened = false; //Correspond a l'etat d'ouverture du bras

  pinMode(joy_arm_x, INPUT);
  pinMode(joy_arm_button, INPUT);
  digitalWrite(joy_arm_button, HIGH);

  //Definition des boutons pour compter les points
  const int more_five = 8; // Entree numerique
  const int more_one = 7; // Entree numerique
  int points = 0; //Nombre de points durant la partie

  pinMode(more_five, INPUT);
  pinMode(more_one, INPUT);

  //Definition des pins de gestion de l'ecran
  // (Aucune idee des pins dont on a besoin XD

  //Definition dont ont besoin les moteurs
  right_bridge = lib::h_bridge(13, 12);
  left_bridge = lib::h_bridge(11, 10);
}

void loop() {
  //----- Lecture des info pour les moteurs de mouvements -----
  //Action de l'axe X
  if (analog2digital(analogRead(joy_motor_x)) == 1) {
    //Fait avancer les 2 moteurs
    right_bridge.forward();
    left_bridge.forward();
  }
  else if (analog2digital(analogRead(joy_motor_x)) == -1) {
    //Fait reculer les 2 moteurs
    right_bridge.backward();
    left_bridge.backward();
  }
  //Action de l'axe Y
  if (analog2digital(analogRead(joy_motor_y)) == 1) {
    left_bridge.forward(); //Fait tourner le robot vers la droite
  }
  else if (analog2digital(analogRead(joy_motor_y)) == -1) {
    right_bridge.forward(); //Fait tourner le robot vers la gauche
  }

  //----- Lecture des infos pour les moteurs du bras -----
  if (analog2digital(analogRead(joy_arm_x)) == 1) {
    //Envoyer info au moteur pour lever le bras
  }
  else if (analog2digital(analogRead(joy_arm_x)) == -1) {
    //Envoyer info au moteur pour baisser le bras
  }
  if (digitalRead(joy_arm_button)) {
    //Envoyer info au moteur pour fermer le bras ou l'ouvrir
    if (arm_opened) {
      //Fermer le bras
    }
    else {
      //Ouvrir le bras
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
