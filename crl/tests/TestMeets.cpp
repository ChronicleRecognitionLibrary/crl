/** ***********************************************************************************
 * \file TestMeets.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Meets
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

void testMeets1()
{
  std::cout << "------- Tests with chronicles (A^A) and (A^B)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleMeets& AmA    = $(A) ^ $(A);
  ChronicleMeets& AmB    = $(A) ^ $(B);
  engine.addChronicle(AmA);
  engine.addChronicle(AmB);

  //   -   -   -   -   -   -     
  //   0       1       
  //  A A     A B  

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)AmA.getRecognitionSet().size(), 1, false);
  engine << "A" << flush;
  CRL::testInteger((long)AmA.getRecognitionSet().size(), 2, false); //3
  engine << 1.0 << "A" << flush;
  CRL::testInteger((long)AmA.getRecognitionSet().size(), 3, false); //4

  engine << "B" << flush;
  CRL::testInteger((long)AmA.getRecognitionSet().size(), 3, false);//4
  CRL::testInteger((long)AmB.getRecognitionSet().size(), 0, false);//1

  std::cout << std::endl;

  AmA.deepDelete();
  AmB.deepDelete();
  delete &AmA;
  delete &AmB;
}


void testMeets2()
{
  std::cout << "------- Tests with chronicles (A B)^(B C) and (A B)^(D C)"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleMeets& ABmBC  = ($(A)+$(B)) ^ ($(B)+$(C));
  ChronicleMeets& ABmDC  = ($(A)+$(B)) ^ ($(D)+$(C));
  engine.addChronicle(ABmBC);
  engine.addChronicle(ABmDC);

  // -   -   -   -   -   -   -   -   -   -   -   -    
  //   0         1
  // A B C D     C

  engine << 0.0 << "A" << "B" << "C" << flush;
  CRL::testInteger((long)ABmBC.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)ABmDC.getRecognitionSet().size(), 0, false);

  engine << "D" << 1.0 << "C" << flush;
  CRL::testInteger((long)ABmBC.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)ABmDC.getRecognitionSet().size(), 0, false);//1

  std::cout << std::endl;

  ABmBC.deepDelete();
  ABmDC.deepDelete();
  delete &ABmBC;
  delete &ABmDC;
}

bool testMeetsWithPredicate_pred(const PropertyManager& p)
{
  return ( (int)p["a"]["x"] < (int)p["c"]["x"] );
}

void testMeetsWithPredicate()
{
  std::cout << "------- Tests with chronicle (A B)^(B C)<pred>"
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleMeets& ABmBC  = ($$($(A),a)+$(B)) ^ ($(B)+$$($(C),c));
  engine.addChronicle(ABmBC);
  ABmBC.setPredicateFunction(testMeetsWithPredicate_pred);

  Event a("A");
  Event c1("C");
  Event c2("C");
  a["x"] = 4;
  c1["x"] = 5;
  c2["x"] = 4;

  // -   -   -   -   -   -   -   -   -   -   -   -    
  //   0         1
  // A B C D     C

  engine << 0.0 << a << "B" << c1 << flush;
  CRL::testInteger((long)ABmBC.getRecognitionSet().size(), 1, false);

  engine << "D" << 1.0 << c2 << 2.0 << "C" << flush;
  CRL::testInteger((long)ABmBC.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABmBC.deepDelete();
  delete &ABmBC;
}

void testChronicleMeets()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleMeets");
  std::cout << "##### ------- Tests of ChronicleMeets class" 
              << std::endl << std::endl;
  testMeets1();
  testMeets2();
  testMeetsWithPredicate();
  std::cout << std::endl;
}
