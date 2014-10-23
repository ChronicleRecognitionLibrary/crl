/** ***********************************************************************************
 * \file TestOverlaps.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Overlaps
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

void testOverlaps1()
{
  std::cout << "------- Tests with chronicle (A B)/(C D)" << std::endl;
  
  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleOverlaps& ABovCD = ($(A) + $(B)) / ($(C) + $(D));
  engine.addChronicle(ABovCD);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5  
  // A   B   C   D   B   D   

  engine << 0.0 << "A" << 1.0 << "B" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "C" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "D" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "B" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 5.0 << "D" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABovCD.deepDelete();
  delete &ABovCD;
}

bool testOverlapsWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["a"]["x"] != (int)p["d"]["x"] );
}

void testOverlapsWithPredicate()
{
  std::cout << "------- Tests with chronicle (A B)/(C D)< A.x!=D.x >" << std::endl;
  
  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleOverlaps& ABovCD = ($$($(A),a) + $(B)) / ($(C) + $$($(D),d));
  engine.addChronicle(ABovCD);
  ABovCD.setPredicateFunction(testOverlapsWithPredicate_pred);

  Event a("A");
  Event d1("D");
  Event d2("D");
  a["x"] = 7;
  d1["x"] = 7;
  d2["x"] = 8;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  // 0   1   2   3   4   5   6   7
  // A   B   C   D   B   D   D   D
  // 7                   7       8


  engine << 0.0 << a << 1.0 << "B" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 2.0 << "C" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 3.0 << "D" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "B" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 5.0 << d1 << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 6.0 << "D" << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 0, false);

  engine << 7.0 << d2 << flush;
  CRL::testInteger((long)ABovCD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABovCD.deepDelete();
  delete &ABovCD;
}


void testChronicleOverlaps()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleOverlaps");
  std::cout << "##### ------- Tests of ChronicleOverlaps class" 
              << std::endl << std::endl;
  testOverlaps1();
  testOverlapsWithPredicate();
  std::cout << std::endl;
}
