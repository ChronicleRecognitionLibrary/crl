/** ***********************************************************************************
 * \file Main.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Main test program
 **************************************************************************************/

/*  Copyright (C) 2012, 2013, 2014  ONERA – http://www.onera.fr
    This file is part of CRL : Chronicle Recognition Library.

    CRL is free software: you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CRL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with CRL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "TestUtils.h"

void testEvent();
void testChronicle();
void testRecognitionEngine();
void testChronicleConjunction();
void testChronicleDisjunction();
void testChronicleSequence();
void testChronicleAbsence();
void testChronicleDelayLasts();
void testChronicleDelayAtLeast();
void testChronicleDelayAtMost();
void testChronicleDelayThen();
void testChronicleOverlaps();
void testChronicleDuring();
void testChronicleEquals();
void testChronicleFinishes();
void testChronicleMeets();
void testChronicleStarts();
void testChronicleAt();
void testProperty();
void testContext();
void testChronicleCut();
void testChronicleStateChange();
void testSingleEvent();
void testNamed();
void testSingleDate();
void testChronicleCoRef();
void testAction();
void testPeremptionDuration();


int main() 
{
  try
  {
    testContext();
    testSingleEvent();
    testSingleDate();
    testNamed();
    testChronicleConjunction();
    testChronicleDisjunction();
    testChronicleSequence();
    testChronicleAbsence();
    testChronicleDuring();
    testChronicleDelayLasts();
    testChronicleDelayThen();
    testChronicleDelayAtLeast();
    testChronicleDelayAtMost();
    testChronicleEquals();
    testChronicleFinishes();
    testChronicleMeets();
    testChronicleOverlaps();
    testChronicleStarts();
    testChronicleAt();
    testChronicleCut();
    testChronicle();
    testRecognitionEngine();
    testProperty();
    testChronicleStateChange();
    testEvent();
    testChronicleCoRef();
    testAction();
    testPeremptionDuration();

    CRL::CRL_ErrReport::PRINT_ALL();

    //char c;
    //std::cin>>c;
    return 0;
  }

  catch(std::string& msg) {                        
    std::cout << "main : "     
    << msg << std::endl;
    return 1;                                      
  }                                                
  catch(const char* msg) {                         
  std::cout << "main : "       
  << msg << std::endl;
  return 1;                                        
  }                                                                                           
  catch(...) {                                     
  std::cout << "main : Unknown Exception"
  << std::endl;
  return 1;                                        
  }

}