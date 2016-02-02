/** ***********************************************************************************
 * \file TestEquals.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Equals
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

void testEquals1()
{
  std::cout << "------- Tests with chronicles (A==A), ((A||B)(C||D))==(A E)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleEquals& AA = $(A) == $(A);
  CRL::ChronicleEquals& C1 = (($(A)||$(B))+($(C)||$(D)))==($(A)+$(C));
  engine.addChronicle(AA);
  engine.addChronicle(C1);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7    
  // A   B   E       C    

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << flush;
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)C1.getRecognitionSet().size(), 0, false);

  engine << "E" << 4.0 << "C" << "E" << flush;
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)C1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  AA.deepDelete();
  C1.deepDelete();
  delete &AA;
  delete &C1;
}

bool testEqualsWithPredicate_pred(const PropertyManager& p)
{
  bool tmp1 = ( (int)p["a"]["x"] < (int)p["b"]["x"] ) && ( (int)p["aa"]["x"] < (int)p["bb"]["x"] );
  bool tmp2 = ( (int)p["a"]["x"] > (int)p["d"]["x"] ) && ( (int)p["aa"]["x"] > (int)p["d"]["x"] );
  return ( tmp1 || tmp2 );
}

void testEqualsWithPredicate()
{
  std::cout << "------- Tests with chronicles ((A C B) == (A D B))<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleEquals& ACBeqADB = ($$($(A),aa) + $(C) + $$($(B),bb)) 
                                  == ($$($(A),a) + $$($(D),d) + $$($(B),b));
  CRL::ChronicleEquals& ACBeqADBbis = ($$($(A),aa) + $(C) + $$($(B),bb)) 
                                  == ($$($(A),a) + $$($(D),d) + $$($(B),b));
  engine.addChronicle(ACBeqADB);
  engine.addChronicle(ACBeqADBbis);
  ACBeqADB.setPredicateFunction(testEqualsWithPredicate_pred);

  Event a1("A");
  Event a2("A");
  Event d1("D");
  Event b1("B");
  a1["x"] = 4;
  a2["x"] = 2;
  d1["x"] = 2;
  b1["x"] = 5;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7   
  // A   A   C   A   C   D   B   B

  engine << 0.0 << "A" << 1.0 << a1 << 2.0 << flush;
  //CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 0, false);
  engine << "C" << 3.0 << flush;
  //CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 0, false);
  engine << a2 << 4.0 << flush;
  //CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 0, false);
  engine << "C" << 5.0 << flush;
  //CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 0, false);
  engine << d1 << 6.0 << flush;
  CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ACBeqADBbis.getRecognitionSet().size(), 0, false);
  engine << b1 << 7.0 << flush;
  CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)ACBeqADBbis.getRecognitionSet().size(), 5, false);
  engine << "B" << flush;
  CRL::testInteger((long)ACBeqADB.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)ACBeqADBbis.getRecognitionSet().size(), 10, false);

  std::cout << std::endl;

  ACBeqADB.deepDelete();
  ACBeqADBbis.deepDelete();
  delete &ACBeqADB;
  delete &ACBeqADBbis;
}

void testChronicleEquals()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleEquals");
  std::cout << "##### ------- Tests of ChronicleEquals class" 
              << std::endl << std::endl;
  testEquals1();
  testEqualsWithPredicate();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleEquals();
    
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
