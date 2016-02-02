/** ***********************************************************************************
 * \file TestDelayLasts.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Delay Lasts
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "TestUtils.h"
#include "Operators.h"
#include "RecognitionEngine.h"

using namespace CRL;

// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

void testDelayLasts1()
{
  std::cout << "------- Tests with chronicles ((B A) == 3) and (B A)" 
              << std::endl << std::endl;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0       1       2       3       4       5       
  // C       B       A       F       A       A       

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleDelayLasts& BA3 = ( ( $(B) + $(A) ) == 3.0 );  //(B A) lasts 3
  ChronicleSequence&   BA  = ( $(B) + $(A) );             //(B A)
  engine.addChronicle(&BA3);
  engine.addChronicle(&BA);

  engine << 0.0 << "C" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(), 1, false);

  engine << 3.0 << "F" << 4.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(), 2, false);

  engine << 5.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  BA3.deepDelete();
  BA.deepDelete();
  delete &BA3;
  delete &BA;
}



void testChronicleDelayLasts(){
  CRL::CRL_ErrReport::START("CRL","ChronicleDelayLasts");
  std::cout << "##### ------- Tests of ChronicleDelayLasts class" 
              << std::endl << std::endl;
  testDelayLasts1();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleDelayLasts();
    
    CRL::CRL_ErrReport::PRINT_ALL();

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
#endif
