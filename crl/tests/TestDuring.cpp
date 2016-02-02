/** ***********************************************************************************
 * \file TestDuring.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test During
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

void testDuring1()
{
  std::cout << "------- Tests with chronicle (A B) &= (C D)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleDuring& ABdCD = ($(A) + $(B)) &= ($(C) + $(D));
  engine.addChronicle(ABdCD);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7    
  // A   B   C   A   D   B   D   B 

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "C" << 3.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "A" << 4.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "D" << 5.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "B" << 6.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "D" << 7.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 1, false);
  engine << "B" << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABdCD.deepDelete();
  delete &ABdCD;
}

bool testDuringWithPredicate_pred(const PropertyManager& p)
{
  bool tmp1 = ( (int)p["a"]["x"] < (int)p["b"]["x"] );
  bool tmp2 = ( (int)p["a"]["x"] < (int)p["d"]["x"] );
  return ( tmp1 || tmp2 );
}

void testDuringWithPredicate()
{
  std::cout << "------- Tests with chronicle ((A B) &= (C D))<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleDuring& ABdCD = ($$($(A),a) + $$($(B),b)) &= ($(C) + $$($(D),d));
  engine.addChronicle(ABdCD);
  ABdCD.setPredicateFunction(testDuringWithPredicate_pred);

  Event a1("A");
  Event b1("B");
  Event b2("B");
  Event d1("D");
  a1["x"] = 4;
  b1["x"] = 5;
  b2["x"] = 1;
  d1["x"] = 2;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7   8
  // A   B   C   A   D   B   D   B   D

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "C" << 3.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << a1 << 4.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "D" << 5.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << b1 << 6.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << "D" << 7.0 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << b2 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 0, false);
  engine << d1 << flush;
  CRL::testInteger((long)ABdCD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABdCD.deepDelete();
  delete &ABdCD;
}


void testChronicleDuring()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleDuring");
  std::cout << "##### ------- Tests of ChronicleDuring class" 
              << std::endl << std::endl;
  testDuring1();
  testDuringWithPredicate();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleDuring();
    
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
