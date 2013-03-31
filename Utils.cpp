/*
 * Copyright (c) 2012 by Xavier HINAULT - support@mon-club-elec.fr
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

#include "Arduino.h"
#include "Utils.h"


  Utils::Utils(){
 
  }
  

//------- fonction d'attente d'un String sur le port Série ----- 
String Utils::waitingString(boolean debugIn) {


    int octetReception=0; // variable de réception octet
    char caractereReception=0; // variable de réception caractère
    String chaineOut=""; // chaine renvoyée


  while (Serial.available()>0) { // si un caractère en réception
    
    octetReception=Serial.read(); // lit le 1er octet de la file d'attente    

    if (octetReception==10) { // si Octet reçu est le saut de ligne 
          if (debugIn)Serial.print ("Saut de ligne recu : ");          
           if (debugIn)Serial.print ("Chaine = "); // affiche la chaine recue
           if (debugIn)Serial.println (chaineOut); 
          delay(100); // pause
          break; // sort de la boucle while
    } // fin if 
    
    else { // si le caractère reçu n'est pas un saut de ligne        

          caractereReception=char(octetReception);
          chaineOut=chaineOut+caractereReception; 
          delay(1); // laisse le temps au caractères d'arriver
          
    } // fin else 

   } // fin while - fin de réception de la chaine
  
  return(chaineOut); 
}

//-- forme sans drapeau  
String Utils::waitingString() {

  return(waitingString(false)); 
}
	
//------- fonction d'attente d'une chaine donnée sur le port Série 
void Utils::waitForString(String stringForWaitIn) {
 
   while (waitingString(true)!=stringForWaitIn); // attend tant que pas reçu chaine voulue
 
} // fin waitToString

//---------------- testInstructionString() : test si instruction de la forme instruction(*****) ----------

//boolean Utils::testInstructionString (String chaineTest, String  chaineRefIn, String paramStringIn) {

//boolean Utils::testInstructionString (String chaineTest, String  chaineRefIn) {
  
String Utils::testInstructionString (String chaineTest, String  chaineRefIn, boolean debugIn) {

     int posRef=chaineTest.length();// position de référence pour analyse (xxx) 
     
    String paramString; // le String reste "null" tant que pas initialisé par =""

     //if (chaineReception.substring(0,posRef)==chaineTest) { // si reçoit l'instruction chaineTest(

     if (chaineRefIn.substring(0,posRef)==chaineTest) { // si reçoit l'instruction chaineTest(
     
         if (debugIn) Serial.print(F("Arduino va executer : ")); 

         if (debugIn) Serial.print(chaineTest); // affiche 
 
     paramString=chaineRefIn.substring(posRef,chaineRefIn.length()-1); // extrait la chaine de caractere recue en parametre 
     
     //Serial.println(chaineReception.length()); 
     if (debugIn)Serial.print(paramString); //affiche la chaine de caractere
     
     if (chaineRefIn.substring(chaineRefIn.length()-1,chaineRefIn.length())==")") { // si fermeture parenthèse = instruction valide
  
       if (debugIn) Serial.println(F(")")); // affiche
       if (debugIn) Serial.println(F("Instruction valide !")); // affiche
      return(paramString); // renvoie true si instruction valide 
      
    } // fin si fermeture parenthèse

    else { // si chaine test absente 
            
       if (debugIn) Serial.println(F("Instruction invalide !")); // affiche
      return(0); // renvoie null 
 
    } // fin else
    
     } // fin si recoit chaineTest(
     
          else { // si pas bonne chaine Test présente
     
           if (debugIn) Serial.print(F(".")); // affiche
          return(0); // renvoie null si instruction invalide
          
     } // fin else 

 
     
} // fin test instruction String

//--- forme dans debug
String Utils::testInstructionString (String chaineTest, String  chaineRefIn) {

	return(testInstructionString(chaineTest, chaineRefIn, false)); 

} // fin forme dans debug

//--------------- testInstruction2 : test si instruction de la forme instruction(xxx, xxx, xxx, xxx, xxx) ou moins ------------

//long* Utils::testInstruction2(String chaineReception, String chaineTest, int nbParam) { // reçoit chaine  et renvoie un tableau de long
boolean Utils::testInstruction2(String chaineReception, String chaineTest, int nbParam, long paramsIn[]) { // reçoit chaine  et renvoie un booléen et modifie tableau
  
  long posRef=chaineTest.length();// position de référence pour analyse (xxx) 
  int posDecal=0; // nombre de position à décaler... 
  

  //long *arrayParamsOut=new long[nbParam];
  //long *arrayParamsOut=new long(2);  // avec des parenthèses... pas des crochets
  // arrayParamsOut=0; //le pointeur est null 

  //long static arrayParamsOut[12]; // tableau statique jusqu'à 12 paramètres - c'est assez...  

  //paramsIn[0]=0; // test

  if (chaineReception==chaineTest+")") { // si reçoit l'instruction chaineTest() - on renvoie false d'emblée

     Serial.println(F("Instruction invalide !")); // affiche
      //arrayParamsOut=0;
      return(false); // renvoie 0 (équiv null en C++) si instruction invalide
 
  }
  else if (chaineReception.substring(0,posRef)==chaineTest) { // si reçoit l'instruction chaineTest(000)
  // nb substring : dernier caractere exclu

    Serial.print(F("Arduino va executer : ")); 

    Serial.print(chaineTest); // affiche 
    
    for (int i=0; i<nbParam; i++) { // défile les n paramètres
    
          
    
        if (chaineReception.substring(posRef,posRef+1)=="-") { // si signe - présent on décale de 1 position la prise en compte du nombre xxx
        
          posRef=posRef+1; // modif valeur posRef

          //-------- détermine le nombre de caractères du nombre suivant ---- 
          posDecal=0; // RAZ posDecal
          
          while (
                (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)!=",") // défile les caractères suivants jusqu'à virgule
                && (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)!=")") // et jusqu'à parenthèse fermée
                && (posDecal<10) // ou limite pour nombre caractères
                ){ 
            //Serial.print(chaineReception.substring(posRef+posDecal,posRef+posDecal+1)); // debug
            posDecal=posDecal+1; // ajoute un caractère de décalage 
            //Serial.println (" posDecal="+posDecal); // debug
            // delay(1000); // debug
            
          } // fin while 

          //--- extrait la valeur en tenant compte du -           
          paramsIn[i]=(-1)*stringToLong(chaineReception.substring(posRef,posRef+posDecal)); // extraction valeur n chiffres à partir caractères et * par -1
        
        } // fin if 
        
        else { // si pas de signe -         

          //-------- détermine le nombre de caractères du nombre suivant ---- 
          posDecal=0; // RAZ posDecal
          
          while (
                (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)!=",") // défile les caractères suivants jusqu'à virgule
                && (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)!=")") // et jusqu'à parenthèse fermée
                && (posDecal<10) // ou limite pour nombre caractères
                ){ 
            //Serial.print(chaineReception.substring(posRef+posDecal,posRef+posDecal+1)); // debug
            posDecal=posDecal+1; // ajoute un caractère de décalage 
            //Serial.println (" posDecal="+posDecal); // debug
            // delay(1000); // debug
            
          } // fin while 
    
          // extrait la valeur -- 
          // params[i]=stringToLong(chaineReception.substring(posRef,posRef+3)); // extraction valeur 3 chiffres à partir position
          paramsIn[i]=stringToLong(chaineReception.substring(posRef,posRef+posDecal)); // extraction valeur n chiffres à partir position
          
        } // fin else 
        
        Serial.print( paramsIn[i]); // affiche 
        
        //Serial.println(posDecal); // debug
        
        //Serial.println(chaineReception.substring(posRef+posDecal,posRef+posDecal+1)); // debug
        
        //if ((chaineReception.substring(posRef+3,posRef+4)==",")&& ((posRef+4)<(chaineTest.length()+(nbParam*4)-1))) { // si virgule attendue présente et si pas long maxi
        //if ((chaineReception.substring(posRef+posDecal,posRef+posDecal+1)==",") && ((posRef+posDecal+1)<(chaineTest.length()+(nbParam*4)-1))) { // si virgule attendue présente et si pas long maxi
	if ((chaineReception.substring(posRef+posDecal,posRef+posDecal+1)==",") && ((posRef+posDecal+1)<chaineReception.length()) && (i!=nbParam-1)) { // si virgule attendue présente et si pas long maxi et si pas dernier paramètre 
        
          Serial.print(","); // affiche
          posRef=posRef+posDecal+1; //décale position de référence de n+1 caractères pour prise en compte nouvelle valeur
  
        } // fin if "," 
  
	else if( (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)==")") && (i!=nbParam-1) ){ // si parenthèses avant nombre de paramètres attendus = invalide

      Serial.println(F("Instruction invalide !")); // affiche

      return(false); // renvoie false si instruction invalide

	}
      
    } // fin for nbParam

   //----- une fois tous les paramètres lus 
    //if ( (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)==")") && (posDecal<=1)){ // si fermeture parenthèse et si un décalage depuis dernière virgule = instruction valide
  if (chaineReception.substring(posRef+posDecal,posRef+posDecal+1)==")") { // si fermeture parenthèse et si aucun décalage depuis dernière valeur = instruction valide
  
      Serial.println(F(")")); // affiche
      Serial.println(F("Instruction valide !")); // affiche
      //return(true); // renvoie true si instruction valide 
     //return arrayParamsOut; 
     return(true); // renvoie 0 (équiv null en C++) // test debug 
      
    } // fin si fermeture parenthèse
    
    else { 
            
      Serial.println(F("Instruction invalide !")); // affiche
      //arrayParamsOut=0;
      return(false); // renvoie 0 (équiv null en C++) si instruction invalide
 
    } // fin else
     
  } // fin si reçoit l'instruction chaineTest(000)
   
 

} // fin fonction testInstruction2  

//------------------- fin test instruction2 ------------ 

//---- fonction idem avec nom plus explicite --- 
boolean Utils::testInstructionLong(String chaineReception, String chaineTest, int nbParam, long paramsIn[]) { 

	return(testInstruction2(chaineReception,chaineTest, nbParam, paramsIn)); 

} // fin testInstructionLong


// ---------- fonction de conversion d'un String numérique en long

long Utils::stringToLong(String chaineLong) { // fonction conversion valeur numérique String en int

    long nombreLong=0; // variable locale 
    int valeurInt=0; // variable locale

    for (int i=0; i<chaineLong.length(); i++) { // défile caractères de la chaine numérique

      valeurInt=chaineLong.charAt(i); // extrait le caractère ASCII à la position voulue - index 0 est le 1er caractère 
      valeurInt=valeurInt-48; // obtient la valeur décimale à partir de la valeur ASCII  

     if (valeurInt>=0 && valeurInt<=9) { // si caractère est entre 0 et 9
       nombreLong=(nombreLong*10)+valeurInt;
     } // fin si caractère est entre 0 et 9


    } // fin for défile caractères

 return (nombreLong); // renvoie valeur numérique

} // ---------- fin stringToLong ------------ 


//------------- fonction de conversion d'un String numérique avec virgule en float

float Utils::stringToFloat(String chaineFloat) { // la fonction recoit la chaine au format xxx.xxx et renvoie un float

  float floatOut=0.0; // variable float locale renvoyée par la fonction 
  
  int indexPoint=chaineFloat.indexOf('.'); // récupère la position du point dans la chaine
  
  if (indexPoint==-1) { // si le point n'est pas dans la chaine...
  
    floatOut=stringToLong(chaineFloat); // renvoie le long et le met dans le float
  
  }
  else { // si le point est présent dans la chaine on calcule la valeur correspondante float en se basant sur la position du point
 
    floatOut= stringToLong(chaineFloat.substring(indexPoint+1, chaineFloat.length())); // récupère valeur après la virgule
    while (abs(floatOut)>1) floatOut=floatOut/10.0; // fait passer la valeur en position derrière virgule
    floatOut=floatOut+stringToLong(chaineFloat.substring(0,indexPoint)); // récupère et ajoute valeur numérique avant virgule
    
  }
  
  return (floatOut); // renvoie le float
  
} // fin stringToFloat





