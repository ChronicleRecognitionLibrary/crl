/** ***********************************************************************************
 * \file TestDelayThen.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Delay Then
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

void testDelayThen1()
{
  std::cout << "------- Tests with chronicle ((B A) 3.0)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& BA3 = ( $(B) + $(A) ) + 3.0; //(B A) then 3
  ChronicleSequence&   BA = $(B) + $(A);             //(B A)
  engine.addChronicle(BA3);
  engine.addChronicle(BA);

  //  -   -   -   -   -   -   -   -   -   -   -   -    
  //  0       1       2       3          4.5  5       
  //  C       B       A       F           A   A   

  engine << 0.0 << "C" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(),  1, false);
  engine << 3.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(),  1, false);
  engine << 5.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(),  1, false);
  engine << 6.0 << flush;
  CRL::testInteger((long)BA3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)BA.getRecognitionSet().size(),  1, false);

  std::cout << std::endl;

  BA3.deepDelete();
  BA.deepDelete();
  delete &BA3;
  delete &BA;
}


void testDelayThen2()
{
  std::cout << "------- Tests with chronicles (A 5.0) vs ((A 3.0) 2.0)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& A5 = $(A) + 5.0;
  ChronicleDelayThen& A32 = ($(A) + 3.0) + 2.0;
  engine.addChronicle(A5);
  engine.addChronicle(A32);

  //  -   -   -   -   -   -   -   -   -   -   -   -
  //  0       1       2       3            5  6
  //  A       B       A

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(), 0, false);
  engine << 3.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(), 0, false);
  engine << 5.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(),  1, false);
  engine << 6.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(),  1, false);
  std::cout << std::endl;

  A5.deepDelete();
  A32.deepDelete();
  delete &A5;
  delete &A32;
}

void testDelayThen3() //allows testing the lookahead
{
  std::cout << "------- Tests with chronicles (A 5.0) vs ((A 3.0) 2.0)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& A5 = $(A) + 5.0;
  ChronicleDelayThen& A32 = ($(A) + 3.0) + 2.0;
  engine.addChronicle(A5);
  engine.addChronicle(A32);

  //  -   -   -   -   -   -   -   -   -   -   -   -
  //  0       1       2       3            6
  //  A       B       A

  engine << 0.0 << "A" << 1.0 << "B" << 2.0 << "A" << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(), 0, false);
  engine << 6.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A32.getRecognitionSet().size(),  1, false);
  std::cout << std::endl;

  A5.deepDelete();
  A32.deepDelete();
  delete &A5;
  delete &A32;
}


void testDelayThenAbsence() //tests the combination of then with an absence
{
  std::cout << "------- Tests with chronicle (A then 5.0) - [ D ]" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDelayThen& A5 = $(A) + 5.0;
  engine.addChronicle(A5);
  ChronicleAbsence& A5mD = ( $(A) + 5.0 ) - $(D);
  engine.addChronicle(A5mD);

  //  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  
  //  0       1       2       3      4      5      6           9
  //          A               D      A                          

  engine << 0.0 << 1.0 << "A" << 3.0 << "D" << 4.0 << "A" << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A5mD.getRecognitionSet().size(), 0, false);
  engine << 6.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A5mD.getRecognitionSet().size(), 0, false);
  engine << 9.0 << flush;
  CRL::testInteger((long)A5.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A5mD.getRecognitionSet().size(), 1, false);
  std::cout << std::endl;

  A5.deepDelete();
  A5mD.deepDelete();
  delete &A5;
  delete &A5mD;
}

void testChronicleDelayThen()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleDelayThen");
  std::cout << "##### ------- Tests of ChronicleDelayThen class"
              << std::endl << std::endl;
  testDelayThen1();
  testDelayThen2();
  testDelayThen3();
  testDelayThenAbsence();
  std::cout << std::endl;
}
