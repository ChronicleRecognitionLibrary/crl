/** ***********************************************************************************
 * \file TestPeremptionDuration.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Peremption Duration
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

void testPeremptionDurationSequenceSimple()
{
  std::cout << "------- Tests with chronicle (A A) with window of 4.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& AAwindow = ($(A) + $(A));
  engine.addChronicle(AAwindow);
  AAwindow.setPeremptionDuration(4.0, true);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 0, false);

  engine << 1.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 1, false);

  //engine << 5.5 << flush;

  engine << 6.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 0, false);

  engine << 7.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  AAwindow.deepDelete();
  delete &AAwindow;
}

void testPeremptionDurationSequence()
{
  std::cout << "------- Tests with chronicles (A A) with window of 4.0 and (A B) with window of 5.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& AA = ($(A) + $(A));
  ChronicleSequence& AB = ($(A) + $(B));
  engine.addChronicle(AA);
  engine.addChronicle(AB);
  AA.setPeremptionDuration(4.0, true);
  AB.setPeremptionDuration(5.0, true);

  engine << 0.0 << "B" << "B" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 0, false);

  engine << 1.0 << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);

  engine << 3.0 << 4.0 << "B" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 1, false);

  engine << 5.0 << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 3, false);

  engine << 6.0 << "A" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 5, false);

  engine << 8.0 << "B" << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 4, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 4, false);

  engine << 15.0 << flush;
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AA.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  AA.deepDelete();
  AB.deepDelete();
  delete &AA;
  delete &AB;
}




bool testPeremptionDurationSequenceWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["a"]["id"] == (int)p["b"]["id"] );
}

void testPeremptionDurationSequenceWithPredicate()
{
  std::cout << "------- Test Peremption Duration with SequenceWithPredicate : (A B)<pred> with window of 2.0"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  Chronicle& c1 = $$($(A),a) + $$($(B),b);
  engine.addChronicle(&c1);
  c1.setPredicateFunction(testPeremptionDurationSequenceWithPredicate_pred);
  c1.setPeremptionDuration(2.0, true);

  Event a1("A"), a2("A"), a3("A");
  Event b1("B"), b2("B");
  
  a1["id"] = 1;
  a3["id"] = 1;
  b1["id"] = 0;
  b2["id"] = 1;

  engine << 0.0 << a1 << a2 << b1 << flush;    // b1 fits neither with a1, nor with a2
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << 2.0 << a3 << flush;
  engine << 3.0 << b2 << flush;                // b2 fits with a1
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  engine << 6.0 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}


void testPeremptionDurationMultipleSequence()
{
  std::cout << "------- Tests with chronicle (A B C) with recursive window of 2.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& ABC = (($(A) + $(B)) + $(C));
  engine.addChronicle(ABC);
  ABC.setPeremptionDuration(2.0, true);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "B" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 1, false);

  engine << 6.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 1, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  ABC.deepDelete();
  delete &ABC;
}

void testPeremptionDurationMultipleSequenceNoRec()
{
  std::cout << "------- Tests with chronicle (A B C) with non-recursive window of 2.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& ABC = (($(A) + $(B)) + $(C));
  engine.addChronicle(ABC);
  ABC.setPeremptionDuration(2.0, false);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "B" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 2, false);

  engine << 6.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 4, false);

  engine << 9.0 << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  ABC.deepDelete();
  delete &ABC;
}

void testPeremptionDurationMultipleSequenceTwoLevelsNoRec()
{
  std::cout << "------- Tests with chronicle (A B C) with window of 2.0 for the first two levels" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& AB = ($(A) + $(B));
  AB.setPeremptionDuration(2.0, false);
  ChronicleSequence& ABC = ( AB + $(C));
  engine.addChronicle(ABC);
  ABC.setPeremptionDuration(2.0, false);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "B" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 2, false);

  engine << 6.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 2, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  ABC.deepDelete();
  delete &ABC;
}

void testPeremptionDurationMultipleSequenceActivateForget()
{
  std::cout << "------- Tests with chronicle (A B C) with recursive window of 2.0 and activateForget" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& ABC = (($(A) + $(B)) + $(C));
  engine.addChronicle(ABC);
  
  engine.activateForget(2.0);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "A" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "B" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 1, false);

  engine << 6.0 << "C" << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 1, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  ABC.deepDelete();
  delete &ABC;
}

void testPeremptionDurationDelayThenInf()
{
  std::cout << "------- Tests with chronicle ((B A) 3.0) with window of 2.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& BA3 = ($(B) + $(A)) + 3.0; //(B A) then 3
  engine.addChronicle(BA3);

  engine.activateForget(2.0);

  //  -   -   -   -   -   -   
  //  0       1       2          
  //  C       B       A         

  engine << 0.0 << "C" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  engine << 5.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  engine << 6.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  BA3.deepDelete();
  delete &BA3;
}

void testPeremptionDurationDelayThenSup()
{
  std::cout << "------- Tests with chronicle ((B A) 3.0) with window of 4.0" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& BA3 = ( $(B) + $(A) ) + 3.0; //(B A) then 3
  engine.addChronicle(BA3);

  engine.activateForget(4.0);

  //  -   -   -   -   -   -   -   -   -   -   -   -    
  //  0       1       2                   5       
  //  C       B       A                   A   

  engine << 0.0 << "C" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  engine << 3.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  engine << 5.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  engine << 6.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  engine << 8.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 2, false);
  engine << 10.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  engine << 13.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);

  std::cout << std::endl;

  BA3.deepDelete();
  delete &BA3;
}


void testPartlyPeremptionDuration()
{
  std::cout << "------- Tests with chronicles (A A) with window of 4.0 and (A B) without window" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setPurgeOldRecognitions(true);

  ChronicleSequence& AAwindow = ($(A) + $(A));
  ChronicleSequence& AB = ($(A) + $(B));
  engine.addChronicle(AAwindow);
  engine.addChronicle(AB);
  AAwindow.setPeremptionDuration(4.0, true);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AB.getRecognitionSet().size(), 0, false);

  engine << 1.0 << "A" << "B" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);

  engine << 6.0 << "A" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AB.getRecognitionSet().size(), 2, false);

  engine << 7.0 << "A" << "B" << flush;
  CRL::testInteger((long)AAwindow.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AB.getRecognitionSet().size(), 6, false);

  std::cout << std::endl;

  AAwindow.deepDelete();
  AB.deepDelete();
  delete &AAwindow;
  delete &AB;
}

void testPeremptionDuration()
{
  CRL::CRL_ErrReport::START("CRL","PeremptionDuration");
  std::cout << "##### ------- Tests of peremption duration" 
              << std::endl << std::endl;
  testPeremptionDurationSequenceSimple();
  testPeremptionDurationSequence();
  testPeremptionDurationSequenceWithPredicate();
  testPeremptionDurationMultipleSequence();
  testPeremptionDurationMultipleSequenceNoRec();
  testPeremptionDurationMultipleSequenceTwoLevelsNoRec();
  testPeremptionDurationMultipleSequenceActivateForget();
  testPeremptionDurationDelayThenInf();
  testPeremptionDurationDelayThenSup();
  testPartlyPeremptionDuration();
  std::cout << std::endl;
}
