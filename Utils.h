/*
 * Copyright (c) 2012 by Xavier HINAULT - support@mon-club-elec.fr
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

/* Cette librairie contient plusieurs fonctions générales utiles
notamment pour la réception et le décodage d'instruction en réception sur le port série
pour la conversion de chaines en valeurs numériques, 
etc...
Ces fonctions ont pour but d'être facilement réutilisées par d'autres librairies 
ou au sein d'un programme Arduino. 
*/

#ifndef Utils_h
#define Utils_h

#include "Arduino.h"



class Utils {

  private : // variables internes
  
  public :

  //String paramString; 
  
  public :
    Utils(); // constructeur
  
	//---- fonctions de réception sur le port Série
	String waitingString(boolean debugIn); 
	String waitingString(); 

	void waitForString(String stringForWaitIn);

	//----- fonctions d'analyse de chaîne 
	String testInstructionString (String chaineTest, String  chaineRefIn);
	String testInstructionString (String chaineTest, String  chaineRefIn, boolean debugIn);

	boolean testInstruction2(String chaineReception,String chaineTest, int nbParam, long paramsIn[]); // fonction initiale laissée en l'état
	boolean testInstructionLong(String chaineReception,String chaineTest, int nbParam, long paramsIn[]); // fonction équivalente à utiliser

	//------ fonctions de conversion chaine ---- 
	long stringToLong(String chaineLong); 
	float stringToFloat(String chaineFloat); 

  private : // fonctions internes


};


#endif
