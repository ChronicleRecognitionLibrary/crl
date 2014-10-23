/** ***********************************************************************************
 * \file TestConjunction.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Co-Referencing
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


bool testCoRef1_pred(const PropertyManager& p)
{
  return ( (long)p["x"]["CRL ID"] == (long)p["y"]["CRL ID"] );
}

bool testCoRef2_pred(const PropertyManager& p)
{
  return ( (long)p["u"]["CRL ID"] != (long)p["v"]["CRL ID"] );
}

void testCoRef()
{
  std::cout << "------- Tests with chronicle ( (A B->x) && (B->y D) ) coreferencing the Bs" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& ABandBDcoref = ($(A)+$$($(B),x)) && ($$($(B),y)+$(D))  ;
  ChronicleConjunction& ABandBD = ($(A)+$(B)) && ($(B)+$(D))  ;
  ABandBDcoref.setPredicateFunction(testCoRef1_pred);
  engine.addChronicle(ABandBDcoref);
  engine.addChronicle(ABandBD);

  engine << 0.0 << "A" << "B" << "B" << flush;
  CRL::testInteger((long)ABandBD.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABandBDcoref.getRecognitionSet().size(),    0, false);

  engine << "D" << flush;
  CRL::testInteger((long)ABandBD.getRecognitionSet().size(),    4, false);
  CRL::testInteger((long)ABandBDcoref.getRecognitionSet().size(),    2, false);

  std::cout << std::endl;

  ABandBDcoref.deepDelete();
  ABandBD.deepDelete();
  delete &ABandBDcoref;
  delete &ABandBD;
}

void testCoRef1()
{
  std::cout << "------- Tests with chronicle ( (B A->x) && (C A->y) ) coreferencing the As" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleConjunction& BAandCAcoref = ($(B)+$$($(A),x)) && ($(C)+$$($(A),y))  ;
  ChronicleConjunction& BAandCAnocoref = ($(B)+$$($(A),u)) && ($(C)+$$($(A),v))  ;
  ChronicleConjunction& BAandCA = ($(B)+$(A)) && ($(C)+$(A))  ;
  BAandCAcoref.setPredicateFunction(testCoRef1_pred);
  BAandCAnocoref.setPredicateFunction(testCoRef2_pred);
  engine.addChronicle(BAandCAcoref);
  engine.addChronicle(BAandCAnocoref);
  engine.addChronicle(BAandCA);

  engine << 0.0 << "B" << "C" << "A" << flush;
  CRL::testInteger((long)BAandCAcoref.getRecognitionSet().size(),    1, false);
  CRL::testInteger((long)BAandCAnocoref.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)BAandCA.getRecognitionSet().size(),    1, false);

  engine << "A" << flush;
  CRL::testInteger((long)BAandCAcoref.getRecognitionSet().size(),    2, false);
  CRL::testInteger((long)BAandCAnocoref.getRecognitionSet().size(),    2, false);
  CRL::testInteger((long)BAandCA.getRecognitionSet().size(),    4, false);

  std::cout << std::endl;

  BAandCAcoref.deepDelete();
  BAandCAnocoref.deepDelete();
  BAandCA.deepDelete();
  delete &BAandCAcoref;
  delete &BAandCAnocoref;
  delete &BAandCA;
}

void testCoRef2()
{
  std::cout << "------- Tests with chronicle (A->x B)-[A->y ! C] coreferencing the As" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& ABmACcoref = ($$($(A),x)+$(B)) - ($$($(A),y)+=$(C))  ;
  ChronicleAbsence& ABmACnocoref = ($$($(A),u)+$(B)) - ($$($(A),v)+=$(C)) ;
  ChronicleAbsence& ABmAC = ($(A)+$(B)) - ($(A)+=$(C)) ;
  ChronicleAbsence& ABmC = ($(A)+$(B)) - $(C)  ;
  ABmACcoref.setPredicateFunction(testCoRef1_pred);
  ABmACnocoref.setPredicateFunction(testCoRef2_pred);
  engine.addChronicle(ABmACcoref);
  engine.addChronicle(ABmACnocoref);
  engine.addChronicle(ABmAC);
  engine.addChronicle(ABmC);

  engine << 0.0 << "A" << "A" << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABmACnocoref.getRecognitionSet().size(),    1, false);
  CRL::testInteger((long)ABmAC.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABmC.getRecognitionSet().size(),    0, false);

  engine << "A" << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABmACnocoref.getRecognitionSet().size(),    2, false);
  CRL::testInteger((long)ABmAC.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABmC.getRecognitionSet().size(),    0, false);

  std::cout << std::endl;

  ABmACcoref.deepDelete();
  ABmAC.deepDelete();
  ABmC.deepDelete();
  ABmACnocoref.deepDelete();
  delete &ABmACcoref;
  delete &ABmAC;
  delete &ABmC;
  delete &ABmACnocoref;
}

void testCoRef2bis()
{
  std::cout << "------- Tests with chronicle (A B)-[0 ! C]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& ABmACcoref = ($(A)+$(B)) - ( T(0.0) += $(C))  ;
  ChronicleAbsence& ABmC = ($(A)+$(B)) - $(C)  ;
  engine.addChronicle(ABmACcoref);
  engine.addChronicle(ABmC);

  engine << "A" << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    0, false);
  CRL::testInteger((long)ABmC.getRecognitionSet().size(),    0, false);

  engine << "A" << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    1, false);
  CRL::testInteger((long)ABmC.getRecognitionSet().size(),    0, false);

  std::cout << std::endl;

  ABmACcoref.deepDelete();
  ABmC.deepDelete();
  delete &ABmACcoref;
  delete &ABmC;
}

void testCoRef2ter()
{
  std::cout << "------- Tests with chronicle ( (0 ! A->x) B )-[A->y ! C] coreferencing the As" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleAbsence& ABmACcoref = ( (T(0.0) += $$($(A),x)) + $(B) ) - ($$($(A),y)+=$(C))  ;
  ABmACcoref.setPredicateFunction(testCoRef1_pred);
  engine.addChronicle(ABmACcoref);

  engine << 0.0 << "A" << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    0, false);

  engine << "A" << "C" << "B" << flush;
  CRL::testInteger((long)ABmACcoref.getRecognitionSet().size(),    0, false);

  std::cout << std::endl;

  ABmACcoref.deepDelete();
  delete &ABmACcoref;
}



void testChronicleCoRef()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleCoRef");
  std::cout << "##### ------- Co-referencing tests"
              << std::endl << std::endl;
  testCoRef();
  testCoRef1();
  testCoRef2();
  testCoRef2bis();
  testCoRef2ter();
  std::cout << std::endl;
}