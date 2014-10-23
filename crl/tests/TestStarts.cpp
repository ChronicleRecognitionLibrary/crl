/** ***********************************************************************************
 * \file TestStarts.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Starts
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

void testStarts1()
{
  std::cout << "------- Tests with chronicles (A|=(A B)) and (A B)|=(A C)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleStarts& AAB  = $(A) |= ($(A)+$(B));
  CRL::ChronicleStarts& ABAC = ( $(A) + $(B) ) |= ( $(A) + $(C) );
  engine.addChronicle(AAB);
  engine.addChronicle(ABAC);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0       1       2       3       4       5   
  // A       D       B       C      B C     C B

  engine << 0.0 << "A" << 1.0 << "D" << 2.0 << flush;
  CRL::testInteger((long)AAB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 0, false);

  engine << "B" << 3.0 << "C" << flush;
  CRL::testInteger((long)AAB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 1, false);

  engine << 4.0 << "B" << "C" << flush;
  CRL::testInteger((long)AAB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 3, false);//2

  engine << 5.0 << "C" << "B" << flush;
    CRL::testInteger((long)AAB.getRecognitionSet().size(), 3, false);
    CRL::testInteger((long)ABAC.getRecognitionSet().size(), 5, false);//3

  std::cout << std::endl;

  AAB.deepDelete();
  ABAC.deepDelete();
  delete &AAB;
  delete &ABAC;
}

bool testStartsWithPredicate_pred(const PropertyManager& p)
{
 return ( (int)p["c"]["x"] < (int)p["b"]["x"] );
}

void testStartsWithPredicate()
{
  std::cout << "------- Tests with chronicle (A B->b)|=(A C->c)< b.x>c.x >"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  CRL::ChronicleStarts& ABAC = ( $(A) + $$($(B),b) ) |= ( $(A) + $$($(C),c) );
  engine.addChronicle(ABAC);
  ABAC.setPredicateFunction(testStartsWithPredicate_pred);

  Event b1("B");
  Event c1("C");
  Event c2("C");
  b1["x"] = 4;
  c1["x"] = 3;
  c2["x"] = 4;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0       1       2       3       4          
  // A       D       B       C       C     

  engine << 0.0 << "A" << 1.0 << "D" << 2.0 << flush;
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 0, false);

  engine << b1 << 3.0 << c1 << flush;
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 1, false);

  engine << 4.0 << c2 << flush;
  CRL::testInteger((long)ABAC.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABAC.deepDelete();
  delete &ABAC;
}


void testChronicleStarts()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleStarts");
  std::cout << "##### ------- Tests of ChronicleStarts class" 
              << std::endl << std::endl;
  testStarts1();
  testStartsWithPredicate();
  std::cout << std::endl;
}
