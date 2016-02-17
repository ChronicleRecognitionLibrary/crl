/** ***********************************************************************************
 * \file TestAbsence.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Absence
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

void testAbsence1()
{
  std::cout << "------- Tests with chronicles (A B)-[A], (A B)-[B] et (A B)-[C]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleAbsence& ABmA = ( $(A)+$(B) ) - $(A);
  ChronicleAbsence& ABmB = ( $(A)+$(B) ) - $(B);
  ChronicleAbsence& ABmC = (($(A)+$(B))  - $(C));
  engine.addChronicle(ABmC);
  engine.addChronicle(ABmA);
  engine.addChronicle(ABmB);

  // -------------- First part

  engine << 0.0 << "C" << 1 << "A" << flush;
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmA.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmB.getRecognitionSet().size(), 0, false);

  engine << "C" << flush;
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmA.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmB.getRecognitionSet().size(), 0, false);

  engine << "F" << 2 << "B" << flush;
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmA.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmB.getRecognitionSet().size(), 1, false);

  engine << 3 << "B" << flush;
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmA.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABmB.getRecognitionSet().size(), 1, false);
  std::cout << std::endl;

  // -------------- Second part

  engine << clear << "A" << "C" << "B" << flush;  // Note the "clear"
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 0, false);

  engine << "A" << "B" << flush; 
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 1, false);

  engine << "C" << flush; 
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 1, false);

  engine << "D" << "D" << flush; 
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 1, false);

  engine << "C" << flush; 
  CRL::testInteger((long)ABmC.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABmA.deepDestroy();
  ABmB.deepDestroy();
  ABmC.deepDestroy();
}


void testAbsence2()
{
  std::cout << "------- Tests with chronicle ((A B)-[C]) D) - [E]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& ABCDE = ( ( ($(A)+$(B) ) - $(C) ) + $(D) ) - $(E);
  engine.addChronicle(ABCDE);

  // -------------- First part
  engine << 0.0 << "A" << "B" << "E" << "A" << "B" << "C" << "D" << flush;
  CRL::testInteger((long)ABCDE.getRecognitionSet().size(), 1, false);

  // -------------- Second part
  engine << clear;
  engine << "A" << "B" << "E" << "D" << flush;
  CRL::testInteger((long)ABCDE.getRecognitionSet().size(), 1, false);
  engine << "A" << "B" << "C" << "D" << flush;
  CRL::testInteger((long)ABCDE.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  ABCDE.deepDestroy();
}


void testAbsenceDisj()
{
  std::cout << "------- Tests with chronicle (A B)-[D||E]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& c1 = ( $(A) + $(B) ) - ( $(D) || $(E) );//(A B)-[D||E]
  engine.addChronicle(c1);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);

  engine << "D" << "B" << "A" << "E" << "E" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  c1.deepDestroy();
}


void testAbsenceConj()
{
  std::cout << "------- Tests with chronicle (A B)-[D&&E]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleAbsence& c1 = ( $(A) + $(B) ) - ( $(D) && $(E) );//(A B)-[D&&E]
  engine.addChronicle(c1);

  engine << 0.0 << "D" << "A" << "E" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  engine << "D" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  c1.deepDestroy();
}


void testAbsenceSeq()
{
  std::cout << "------- Tests with chronicle (A B)-[D E]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& c1 = ( $(A) + $(B) ) - ( $(D) + $(E) );//(A B)-[D E]
  engine.addChronicle(c1);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  engine << "E" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  c1.deepDestroy();
}


void testDoubleAbsence()
{
  std::cout << "------- Tests with chronicle (A B)-[(D E)-[F G]]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& c1 = ($(A)+$(B)) - ( ($(D)+$(E)) - ($(F)+$(G)) );
  engine.addChronicle(c1);

  engine << 0.0 << "A" << "D" << "F" << "G" << "E" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  c1.deepDestroy();
}


bool testAbsenceWithPredicate_pred1(const PropertyManager& p)
{
  return ( (int)p["a"]["id"] == (int)p["b"]["id"] );
}

bool testAbsenceWithPredicate_pred2(const PropertyManager& p)
{
  return ( (int)p["a"]["id"] == (int)p["d"]["id"] );
}

void testAbsenceWithPredicate()
{
  std::cout << "------- Tests with chronicle (A && B)<pred1>-[D]<pred2>" 
              << std::endl << std::endl;

  // ((A && B)<pred1> - [D])<pred2>
  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& c1 = $$($(A),a) && $$($(B),b);
  Chronicle& c2 = c1 - $$($(D),d);
  engine.addChronicle(c2);
  c1.setPredicateFunction(testAbsenceWithPredicate_pred1);
  c2.setPredicateFunction(testAbsenceWithPredicate_pred2);

  Event a1("A"), a2("A"), a3("A"), a4("A");
  Event b1("B"), b2("B"), b3("B"), b4("B");
  Event d1("D"), d2("D"), d3("D");

  a2["id"] = a3["id"] = a4["id"] = 1;
  b2["id"] = b3["id"] = b4["id"] = 1;
  d2["id"] = 1;
  d3["id"] = 0;

  engine << 0.0 << a1 << b1 << flush;          // a1 and b1 have no ID
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);
  engine << 1.0 << a2 << b2 << flush;          // a2.id ==  b2.id
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);
  engine << 2.0 << d1 << b3 << flush;          // a2.id ==  b3.id, and d1.id <> a1.id
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);
  engine << 3.0 << d2 << a3 << flush;          // d2 blocks the new recognitions
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);
  engine << 4.0 << a4 << d3 << b4 << flush;    // but not d3
  CRL::testInteger((long)c2.getRecognitionSet().size(), 4, false);

  std::cout << std::endl;

  c2.deepDestroy();
}

void testBornesAbsence()
{
  std::cout << "------- Tests with chronicle (A !! ((A then 5) - ]A])" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& c0 = ( $(A) + 5.0 ) - ( $(A) );
  c0.setExclInf(true);
  c0.setExclSup(false);
  ChronicleStateChange& c1 = ( $(A) != c0 );
  engine.addChronicle(c1);

  engine << 0.0 << "A" << 2.0 << "A" << 3.0 << "A" << 4.0 
         << "A" << 5.0 << "A" << 6.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << 12.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  c1.deepDestroy();
}


void testChronicleAbsence()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleAbsence");
  std::cout << "##### ------- Tests of ChronicleAbsence class" 
              << std::endl << std::endl ;
  testAbsence1();
  testAbsence2();
  testAbsenceDisj();
  testAbsenceConj();
  testAbsenceSeq();
  testDoubleAbsence();
  testAbsenceWithPredicate();
  testBornesAbsence();
  Event::freeAllInstances();
  std::cout << std::endl;
}

#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleAbsence();
    
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
