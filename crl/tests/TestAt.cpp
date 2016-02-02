/** ***********************************************************************************
 * \file TestAt.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test At chronicle
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

#include "Chronicle.h"
#include "Event.h"
#include "ChronicleSingleEvent.h"
#include "ChronicleConjunction.h"
#include "ChronicleAt.h"
#include "RecognitionEngine.h"
#include "TestUtils.h"
#include "Operators.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

void testAt1()
{
  std::cout << "------- Tests with chronicles @A and @(A&B)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleAt& atA     = AT($(A));
  ChronicleAt& atAandB = AT($(A)&&$(B));
  engine.addChronicle(atA);
  engine.addChronicle(atAandB);

  engine << 0.0 << "C" << "A" << flush;
  CRL::testInteger((long)atA.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)atAandB.getRecognitionSet().size(), 0, false);
  
  engine << "B" << flush;
  CRL::testInteger((long)atA.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)atAandB.getRecognitionSet().size(), 1, false);

  std::cout << atA.prettyPrint();
  std::cout << atAandB.prettyPrint();

  std::cout << std::endl;

  atA.deepDelete();
  atAandB.deepDelete();
  delete &atA;
  delete &atAandB;
}

bool testAt2_pred(const PropertyManager& p)
{
  return ( (int)p["x"]["num"] > 5 );
}


bool testAt2bis_pred(const PropertyManager& p)
{
  return ( (int)p[Context::ANONYMOUS()]["z"] <= 10 );
}


void testAt2bis_func(const PropertyManager& inp, PropertyManager& outp)
{
  try
  {
    outp["z"] = ((int)inp["aa"]["num"]) + ((int)inp["bb"]["num"]);
  }
  catch(...) {}
}

void testAtPredFunct()
{
    std::cout << "------- Tests with chronicles (C @(A->x && B)P[x] D) and @((A->aa B->bb)f[aa,bb])P[z]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleAt& A1 =  AT( $$($(A),x) && $(B) );
  A1.setPredicateFunction(testAt2_pred);
  ChronicleSequence& A2 = $(C) + A1 + $(D);
  engine.addChronicle(A2);

  ChronicleSequence& A3 =  $$($(A),aa) + $$($(B),bb);
  A3.setOutputPropertiesFunction(testAt2bis_func);
  ChronicleAt& A4 =  AT(A3);
  A4.setPredicateFunction(testAt2bis_pred);
  engine.addChronicle(A4);

  Event a6("A");
  Event a4("A");
  Event a7("A");
  Event b5("B");
  Event b4("B");
  a6["num"] = 6;
  a4["num"] = 4;
  a7["num"] = 7;
  b5["num"] = 5;
  b4["num"] = 4;

  engine << 0.0 << a6 << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 0, false);
 // CRL::testInteger((int)((**A3.getRecognitionSet().begin())[Context::ANONYMOUS()]["y"]), 11, true);

  engine << "C" << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 0, false);

  engine << a4 << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 0, false);

  engine << b5 << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  engine << a7 << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  engine << "D" << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  engine << b4 << flush;
  CRL::testInteger((long)A2.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  A2.deepDelete();
  A4.deepDelete();
  delete &A2;
  delete &A4;
}

void testAtThen()
{
  // Recognitions of A then 4 thanks to LookAhead
  std::cout << "------- Tests with chronicle  C @(A then 4) D" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSequence& C1 = $(C) + AT($(A) + 4.0) + $(D);
  engine.addChronicle(C1);

  //   A     A        C     A     A           D
  //   1  2  3  4  5  6  7  8  9  10  11  12  13

  engine << 0.0 << 1.0  << "A" << flush;
  engine << 3.0  << "A" << flush;
  engine << 6.0  << "C" << flush;
  engine << 8.0  << "A" << flush;
  engine << 10.0 << "A" << flush;
  CRL::testInteger((long)C1.getRecognitionSet().size(), 0, false);

  engine << 13.0 << "D" << flush;
  CRL::testInteger((long)C1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  C1.deepDelete();
  delete &C1;
}

void testAtThenBis()
{
  // Events leading to recognitions of A then 4
  std::cout << "------- Tests with chronicle C @(A then 4) D" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSequence& C1 = $(C) + AT($(A) + 4.0) + $(D);
  engine.addChronicle(C1);

  //   A     A     E  C     A     A       E   D
  //   1  2  3  4  5  6  7  8  9  10  11  12  13

  engine << 0.0 << 1.0  << "A" << flush;
  engine << 3.0  << "A" << flush;
  engine << 5.0  << "E" << flush;
  engine << 6.0  << "C" << flush;
  engine << 8.0  << "A" << flush;
  engine << 10.0 << "A" << flush;
  engine << 12.0 << "E" << flush;
  CRL::testInteger((long)C1.getRecognitionSet().size(), 0, false);

  engine << 13.0 << "D" << flush;
  CRL::testInteger((long)C1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  C1.deepDelete();
  delete &C1;
}

void testChronicleAt()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleAt");
  std::cout << "##### ------- Tests of ChronicleAt class" 
              << std::endl << std::endl ;
  testAt1();
  testAtPredFunct();
  testAtThen();
  testAtThenBis();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleAt();
    
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