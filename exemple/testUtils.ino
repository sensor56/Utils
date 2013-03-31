#include <Utils.h>

Utils utils; // déclare objet racine d'accès aux fonctions de la librairie Utils

String chaineIn; // déclare un String de réception

void setup() {

  Serial.begin(115200); // Initialisation vitesse port Série
  // Utiliser vitesse idem cté Terminal série

}

void loop() {

    //chaineIn=utils.waitingString(false);
    chaineIn=utils.waitingString();

    Serial.println("Arduino a recu : " + chaineIn);

}
