/** ***********************************************************************************
 * \file TestSequence.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Sequence
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

void testSequence1()
{
  std::cout << "------- Tests with chronicles (A A) and (A B)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& AA = ($(A) + $(A));
  ChronicleSequence& AB = ($(A) + $(B));
  engine.addChronicle(AA);
  engine.addChronicle(AB);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 0, false);

  engine << "F" << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  AA.deepDelete();
  AB.deepDelete();
  delete &AA;
  delete &AB;
}



void testNoDistributivityDisjOverSeq()
{
  std::cout << "------- NoDistributivityDisjOverSeq test A||(B D) != (A||B) (A||D)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleDisjunction& c1 = $(A) || ($(B)+$(D) );             //A||(B D)
  ChronicleSequence&    c2 = ($(A) || $(B)) + ($(A) || $(D));  //(A||B) (A||D)
  engine.addChronicle(c1);
  engine.addChronicle(c2);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);
  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);
  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);
  engine << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 5, false);

  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testNoDistributivityConjOverSeq()
{
  std::cout << "------- NoDistributivityConjOverSeq test A&(B D) != (A&B) (A&D)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& c1 = $(A) && ($(B)+$(D) );            //A&(B D)
  ChronicleSequence&    c2 = ($(A) && $(B)) + ($(A) && $(D));   //(A&B) (A&D)
  engine.addChronicle(c1);
  engine.addChronicle(c2);

  engine << 0.0 << "A" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);
  engine << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);
  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);
  
  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}

void testSurplusDeRecoSeq()
{
  std::cout << "------- SurplusDeRecoSeq test with chronicle (A&&A) (A&&A)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& c1 = ($(A) && $(A)) + ($(A) && $(A));
  engine.addChronicle(&c1);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 7, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}


bool testSequenceWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["a"]["id"] == (int)p["b"]["id"] );
}

void testSequenceWithPredicate()
{
  std::cout << "------- SequenceWithPredicate test (A B)<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& c1 = $$($(A),a) + $$($(B),b);
  engine.addChronicle(&c1);
  c1.setPredicateFunction(testSequenceWithPredicate_pred);

  Event a1("A"), a2("A");
  Event b1("B"), b2("B");
  
  a1["id"] = 1;
  b1["id"] = 0;
  b2["id"] = 1;

  engine << 0.0 << a1 << a2 << b1 << flush;    // b1 fits neither with a1, nor with a2
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << b2 << flush;                // b2 fits with a1
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}


void testChronicleSequence()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleSequence");
  std::cout << "##### ------- Tests of ChronicleSequence class" 
              << std::endl << std::endl;
  testSequence1();
  testNoDistributivityDisjOverSeq();
  testNoDistributivityConjOverSeq();
  testSurplusDeRecoSeq();
  testSequenceWithPredicate();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleSequence();
    
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
