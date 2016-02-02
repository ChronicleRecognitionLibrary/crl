/** ***********************************************************************************
 * \file TestFinishes.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Finishes
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

void testFinishes1()
{
  std::cout << "------- Tests with chronicles (B>=(A B)), (B C)>=(A C)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleFinishes& BAB  = $(B) >= ($(A)+$(B));
  CRL::ChronicleFinishes& BCAC = ( $(B) + $(C) ) >= ( $(A) + $(C) );
  engine.addChronicle(BAB);
  engine.addChronicle(BCAC);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7    
  // A   B   E      C E      A

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << flush;
  CRL::testInteger((long)BAB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 0, false);

  engine << "E" << 4.0 << "C" << "E" << flush;
  CRL::testInteger((long)BAB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 1, false);

  engine << 6.0 << "C" << "B" << flush;
  CRL::testInteger((long)BAB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  BAB.deepDelete();
  BCAC.deepDelete();
  delete &BAB;
  delete &BCAC;
}

bool testFinishesWithPredicate_pred(const PropertyManager& p)
{
  bool tmp1 = ( (int)p["a"]["x"] == (int)p["b"]["x"] );
  bool tmp2 = ( (int)p["a"]["x"] == (int)p["c"]["x"] );
  return ( tmp1 && tmp2 );
}

void testFinishesWithPredicate()
{
  std::cout << "------- Tests with chronicle (B C)>=(A C)<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleFinishes& BCAC = ( $$($(B),b) + $$($(C),c) ) >= ( $$($(A),a) + $(C) );
  engine.addChronicle(BCAC);

  BCAC.setPredicateFunction(testFinishesWithPredicate_pred);

  Event a1("A");
  Event b1("B");
  Event b2("B");
  Event c1("C");
  Event c2("C");
  a1["x"] = 3;
  b1["x"] = 3;
  b2["x"] = 3;
  c1["x"] = 3;
  c2["x"] = 4;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7    
  // A   B   E      C E      A

  engine << 0.0 << a1 << 1.0 << b1 << 2.0 << flush;
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 0, false);

  engine << "E" << 4.0 << c1 << "E" << flush;
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 1, false);

  engine << 6.0 << "C" << b2 << flush;
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 1, false);

  engine << 7.0 << c2 << flush;
  CRL::testInteger((long)BCAC.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  BCAC.deepDelete();
  delete &BCAC;
}


void testChronicleFinishes()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleFinishes");
  std::cout << "##### ------- Tests of ChronicleFinishes class" 
              << std::endl << std::endl;
  testFinishes1();
  testFinishesWithPredicate();
  std::cout << std::endl;
}

#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleFinishes();
    
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
