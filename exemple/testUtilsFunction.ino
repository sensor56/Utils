// Programme de test de la librairie Utils
// par X. HINAULT - www.mon-club-elec.fr
// Tous droits reserves - GPLv3

#include <Utils.h> // inclusion de la librairie 
Utils utils; // déclare objet racine d'accès aux fonctions de la librairie Utils

String chaineReception=""; // déclare un String
long params[6]; // déclare un tableau de long - taille en fonction nombre max paramètres attendus

void setup() {

 Serial.begin(115200); // Initialisation vitesse port Série
  // Utiliser vitesse idem coté Terminal série
  Serial.println("Saisir une chaine de la forme FONCTION(valeur)"); // message initial

} // fin setup

void loop() {

    chaineReception=utils.waitingString();// sans debug

    if (chaineReception!="") { // si une chaine a été reçue 

    	if(utils.testInstructionLong(chaineReception,"FONCTION(",1,params)==true) { // si chaine FONCTION(valeur) bien reçue 

        Serial.println("Arduino a recu le parametre : " + (String)params[0]);

    	} // fin si testInstructionLong==true
    } // fin // si une chaine a été reçue 
} // fin loop
