/** ***********************************************************************************
 * \file TestDisjunction.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Disjunction
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

void testDisjunction1()
{
  std::cout << "------- Tests with chronicle (A||B), (A||A) and (B||A)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleDisjunction& AorB =  $(A) || $(B) ;
  ChronicleDisjunction& AorA =  $(A) || $(A) ;
  ChronicleDisjunction& BorA =  $(B) || $(A) ;
  engine.addChronicle(AorB);
  engine.addChronicle(AorA);
  engine.addChronicle(BorA);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AorB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AorA.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)BorA.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)AorB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AorA.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)BorA.getRecognitionSet().size(), 2, false);

  engine << "F" << "A" << flush;
  CRL::testInteger((long)AorB.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)AorA.getRecognitionSet().size(), 4, false);
  CRL::testInteger((long)BorA.getRecognitionSet().size(), 3, false);
  std::cout << std::endl;

  AorB.deepDelete();
  AorA.deepDelete();
  BorA.deepDelete();
  delete &AorB;
  delete &AorA;
  delete &BorA;
}

void testDisjunction2()
{
  std::cout << "------- Test comparing re-using/re-declarating" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  Chronicle& a = $(A);

  ChronicleDisjunction& c1 = $(A) || $(B);
  ChronicleDisjunction& c2 =  a   || $(B);

  engine.addChronicle(&a);
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "A" << "A" << "B"<< flush;
  CRL::testInteger((long)a.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 3, false);
  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  //delete &a;
  delete &c1;
  delete &c2;
}


void testAssocativityDisjunction(){
  std::cout << "------- Associativity test A||(B||C) == (A||B)||C" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleDisjunction& c1 = $(A) || ( $(B) || $(C) );   // A||(B||C)
  ChronicleDisjunction& c2 = ( $(A) || $(B) ) || $(C);   // (A||B)||C
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "E" << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);

  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 3, false);

  engine << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 4, false);
 
  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testDistributivitySeqOverDisj()
{
  std::cout << "------- DistributivitySeqOverDisj test A (B||C) == (A B)||(A C)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + ( $(B) || $(C) );         //A (B||C)
  ChronicleDisjunction& c2 = ($(A) + $(B)) || ($(A) + $(C));  //(A B)||(A C)
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


void testDistributivityConjOverDisj()
{
  std::cout << "------- DistributivityConjOverDisj test A&&(B||C) == (A&&B)||(A&&C)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& c1 = $(A) && ( $(B) || $(C) );        //A&(B||C)
  ChronicleDisjunction& c2 = ($(A) && $(B)) || ($(A) && $(C)); //(A&B)||(A&C)
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 2, false);

  engine << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 4, false);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 4, false);

  std::cout << std::endl;

  c1.deepDelete();
  c2.deepDelete();
  delete &c1;
  delete &c2;
}


bool testDisjunctionWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["x"]["num"] == 1 );
}

void testDisjunctionWithPredicate()
{
  std::cout << "------- DisjunctionWithPredicate test ((A->x && B) || (A->x && D))<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& c1 = ( ($$($(A),x) && $(B)) || ($$($(A),x) && $(D)) );
  engine.addChronicle(&c1);
  c1.setPredicateFunction(testDisjunctionWithPredicate_pred);

  Event a1("A");
  Event a2("A");
  Event a3("A");
  Event b1("B");
  Event d1("D");
  
  a1["num"] = 0;
  a2["num"] = 1;

  engine << 0.0 << a1 << b1 << flush;    // a1 doesn't fit
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  engine << a2 << flush;          // a2 corresponds => (a2 b1)
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  engine << d1 << flush;          // => (a2 b1) and (a2 d1) 
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  engine << a3 << flush;          // a3 doesn't fit
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}

int flagL = 0;
int flagR = 0;

void testDisjAction_act(RecoTree& r)
{
  if ( (r.getRightMember())->getLeftMember() == NULL )
  {
    std::cout << "THROUGH RIGHT MEMBER" << std::endl; 
    flagR++;
  }
  else 
  {
    std::cout << "THROUGH LEFT MEMBER" << std::endl;
    flagL++;
  }
}


void testDisjAction()
{
  std::cout << "------- DisjAction test : A (B||C)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + ( $(B) || $(C) );         //A (B||C)
  c1.setActionFunction(testDisjAction_act);
  engine.addChronicle(&c1);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger(flagL, 0, false);
  CRL::testInteger(flagR, 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger(flagL, 1, false);
  CRL::testInteger(flagR, 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger(flagL, 1, false);
  CRL::testInteger(flagR, 0, false);

  engine << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 3, false);
  CRL::testInteger(flagL, 1, false);
  CRL::testInteger(flagR, 2, false);

  std::cout << std::endl;

  c1.deepDelete();
  delete &c1;
}

void testChronicleDisjunction()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleDisjunction");
  std::cout << "##### ------- Tests of ChronicleDisjunction class" 
              << std::endl << std::endl;
  testDisjunction1();
  testDisjunction2();
  testAssocativityDisjunction();
  testDistributivitySeqOverDisj();
  testDisjunctionWithPredicate();
  testDisjAction();
  std::cout << std::endl;
}
