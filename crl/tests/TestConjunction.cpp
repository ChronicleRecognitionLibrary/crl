/** ***********************************************************************************
 * \file TestConjunction.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Conjunction
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

void testConjunction1()
{
  std::cout << "------- Tests with chronicles (A&&B), (A&&A) and (B&&A)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& AandB =  $(A) && $(B) ;
  ChronicleConjunction& AandA =  $(A) && $(A) ;
  ChronicleConjunction& BandA =  $(B) && $(A) ;
  engine.addChronicle(AandB);
  engine.addChronicle(AandA);
  engine.addChronicle(BandA);

  engine << 0.0 << "C" << "B" << "A" << flush;
  CRL::testInteger((long)AandB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AandA.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BandA.getRecognitionSet().size(), 1, false);

  engine << "F" << "A" << flush;
  CRL::testInteger((long)AandB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AandA.getRecognitionSet().size(), 4, false);
  CRL::testInteger((long)BandA.getRecognitionSet().size(), 2, false);
  std::cout << std::endl;

  AandB.deepDelete();
  AandA.deepDelete();
  BandA.deepDelete();
  delete &AandB;
  delete &AandA;
  delete &BandA;
}


void testConjunction2()
{
  std::cout << "------- Tests with chronicles (AB&&BD) and (ABBD|BDAB)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& ABandBD = ($(A)+$(B)) && ($(B)+$(D));  
  ChronicleDisjunction& ABBDorBDAB =  ( $(A)+$(B)+$(B)+$(D) ) || ( $(B)+$(D)+$(A)+$(B) ); 
  engine.addChronicle(ABandBD);
  engine.addChronicle(ABBDorBDAB);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)ABandBD.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABBDorBDAB.getRecognitionSet().size(), 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)ABandBD.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABBDorBDAB.getRecognitionSet().size(), 0, false);

  engine << "D" << flush;
  CRL::testInteger((long)ABandBD.getRecognitionSet().size(),    1, false);
  CRL::testInteger((long)ABBDorBDAB.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  ABandBD.deepDelete();
  ABBDorBDAB.deepDelete();
  delete &ABandBD;
  delete &ABBDorBDAB;
}


void testAssociativityConjunction()
{
  std::cout << "------- Associativity tests : A&&(B&&C) == (A&&B)&&C" 
              << std::endl << std::endl;
  
  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& c1 = $(A) && ( $(B) && $(C) );   //A&&(B&&C)
  ChronicleConjunction& c2 = ( $(A) && $(B) ) && $(C);  //(A&&B)&&C
  engine.addChronicle(c1);
  engine.addChronicle(c2);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "C"<< flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);
  
  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testNoDistributivityDisjOverConj()
{
  std::cout << "------- NoDistributivityDisjOverConj test : A||(B&&D) != (A||B)&&(A||D)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleDisjunction& c1 = $(A) || ( $(B) && $(D) );           //A||(B&D)
  ChronicleConjunction& c2 = ($(A) || $(B) ) && ($(A) || $(D) );  //(A||B)&(A||D)
  engine.addChronicle(c1);
  engine.addChronicle(c2);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testNoDistributivitySeqOverConj()
{
  std::cout << "------- NoDistributivitySeqOverConj test : A (B&&D) != (A B)&&(A D)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + ( $(B) && $(D) );          //A (B&D)
  ChronicleConjunction& c2 = ($(A) + $(B) ) && ($(A) + $(D) ); //(A B)&(A D)
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "A" << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);

  engine << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);
  
  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testChronicleSequenceConjunction()
{
  std::cout << "------- SequenceConjunction test : (A B) && A"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& c1 = ( $(A) + $(B) ) && $(A) ;   // (AB)&A
  engine.addChronicle(&c1);

  engine << 0.0 << "A" << "A" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 4, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}


bool testConjunctionWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["a"]["x"] == (int)p["b"]["x"] );
}


void testConjunctionWithPredicate()
{
  std::cout << "------- ConjunctionWithPredicate test : (A && B)<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& c1 = ( $$( $(A),a) && $$( $(B),b) );
  engine.addChronicle(&c1);
  c1.setPredicateFunction(testConjunctionWithPredicate_pred);

  Event a1("A");
  Event b1("B");
  Event b2("B");
  Event b3("B");
  a1["x"] = 0;
  b1["x"] = 0;
  b2["x"] = 1;

  engine << 0.0 << a1 << b1 << flush;    // a1 and b1 fit
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << b2 << flush;          // b2 doesn't fit
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << b3 << flush;          // b3 doesn't even have any property 
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}


void testChronicleConjunction()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleConjunction");
  std::cout << "##### ------- Tests of ChronicleConjunction class"
              << std::endl << std::endl;
  testConjunction1();
  testConjunction2();
  testAssociativityConjunction();
  testNoDistributivityDisjOverConj();
  testNoDistributivitySeqOverConj();
  testChronicleSequenceConjunction();
  testConjunctionWithPredicate();
  std::cout << std::endl;
}

#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleConjunction();
    
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