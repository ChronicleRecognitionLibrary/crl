/** ***********************************************************************************
 * \file TestRecognitionEngine.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Recognition Engine
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

#include <fstream>

#include "ChronicleAbsence.h"
#include "ChronicleConjunction.h"
#include "ChronicleDisjunction.h"
#include "ChronicleSequence.h"
#include "ChronicleSingleEvent.h"
#include "ChronicleSingleDate.h"
#include "ChronicleDelayAtLeast.h"
#include "ChronicleDelayLasts.h"
#include "ChronicleOverlaps.h"
#include "ChronicleDuring.h"

#include "RecognitionEngine.h"
#include "TestUtils.h"
#include "Operators.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

void testRecognitionEngine_addEvent()
{
  RecognitionEngine r1(&std::cout, RecognitionEngine::DETAILED);

  // Adding a non-dated event to an empty buffer t=0, order=0
  r1.setCurrentTime(0.0);
  Event a("a");
  r1.addEvent(&a);
  CRL::testDouble((double)a.getDate(), 0.0, 1e-10, false);
  CRL::testInteger(a.getOrder(), 0L, false);
  CRL::testInteger((long)r1.getEventBuffer().size(), 1L, false);

  // Adding a dated event to an empty buffer t=0, order=0
  r1.clearEventBuffer();
  Event b("b", 2.5);
  r1 << b;
  CRL::testDouble((double)b.getDate(), 2.5, 1e-10, false);
  CRL::testInteger(b.getOrder(), 1L, false);
  CRL::testInteger((long)r1.getEventBuffer().size(), 1, false);

  // Adding a second non-dated event in LAST_EVENT mode
  Event c("c");
  r1 << &c;
  CRL::testDouble((double)c.getDate(), 2.5, 1e-10, false);
  CRL::testInteger(c.getOrder(), 2L, false);
  CRL::testInteger((long)r1.getEventBuffer().size(), 2, false);

  // Adding a third non-dated event in CURRENT_TIME mode
  r1.setPolicyCurrentTime();
  Event d("d");
  r1 << d;
  CRL::testDouble((double)d.getDate(), 0.0, 1e-10, false);
  CRL::testInteger(d.getOrder(), 1L, false);
  CRL::testInteger(b.getOrder(), 2L, false);
  CRL::testInteger(c.getOrder(), 3L, false);
  CRL::testInteger((long)r1.getEventBuffer().size(), 3, false);

  // Returning to LAST_EVENT mode
  CRL::testBoolean(r1.isInsertionPolicyCurrentTime(), true, false);
  CRL::testBoolean(r1.isInsertionPolicyLastEvent(),  false, false);
  r1.setPolicyLastEvent();
  CRL::testBoolean(r1.isInsertionPolicyCurrentTime(), false, false);
  CRL::testBoolean(r1.isInsertionPolicyLastEvent(),    true, false);

  // Adding a fourth dated event in the middle of the buffer
  Event e("e", 1.5);
  r1 << e;
  CRL::testDouble((double)e.getDate(), 1.5, 1e-10, false);
  CRL::testInteger(e.getOrder(), 2L, false);
  CRL::testInteger(d.getOrder(), 1L, false);
  CRL::testInteger(b.getOrder(), 3L, false);
  CRL::testInteger(c.getOrder(), 4L, false);

  // Adding a fifth and a sixth dated events at the end of the buffer
  Event f("f", 2.5);
  r1 << f;
  CRL::testDouble((double)f.getDate(), 2.5, 1e-10, false);
  CRL::testInteger(f.getOrder(), 5L, false);
  Event g("g", 3.5);
  r1 << g;
  CRL::testDouble((double)g.getDate(), 3.5, 1e-10, false);
  CRL::testInteger(g.getOrder(), 6L, false);

  // Tests of shortened operators
  r1 << "h";
  CRL::testInteger((long)r1.getEventBuffer().size(), 7, false);
  r1 << std::string("i");
  CRL::testInteger((long)r1.getEventBuffer().size(), 8, false);

  // Test of clearEventBuffer
  //r1.clearEventBuffer();
  r1 << clear;
  CRL::testInteger((long)r1.getEventBuffer().size(), 0, false);

  std::cout << std::endl;
}


void testRecognitionEngine_process()
{
  RecognitionEngine r1(&std::cout, RecognitionEngine::DETAILED);

  CRL::testInteger(r1.process(0.0), 0, true);
  r1 << "a";
  CRL::testInteger(r1.process(0.0), 1, true);
  Event b("b", 1.5);
  r1 << b;
  Event c("c", 2.5);
  r1 << c;
  CRL::testInteger(r1.process(2.0), 1, true);
  CRL::testInteger(r1.process(2.0), 0, true);
  CRL::testInteger(r1.process(2.5), 1, true);
  CRL::testInteger(r1.process(2.5), 0, true);
  CRL::testInteger(r1.process(3.5), 0, true);


  std::cout << std::endl;
}


void testRecognitionEngine_lookAhead()
{
  RecognitionEngine r1(&std::cout, RecognitionEngine::DETAILED);

  CRL::testDouble((double)r1.lookAhead(), INFTY_DATE, 1e10, false);
  ChronicleSingleEvent a("a");
  r1.addChronicle(a);
  CRL::testDouble((double)r1.lookAhead(), INFTY_DATE, 1e10, false);
  ChronicleSingleDate b(4.0);
  ChronicleSingleDate c(2.0);
  r1.addChronicle(b);
  r1.addChronicle(c);
  CRL::testDouble((double)r1.lookAhead(), 2.0, 1e-10, false);
  r1.clearChronicleList();
  CRL::testDouble((double)r1.lookAhead(), INFTY_DATE, 1e10, false);
}


void testRecognitionEngine()
{
  CRL::CRL_ErrReport::START("CRL", "RecognitionEngine");
  std::cout << "##### ------- Tests of RecognitionEngine class" << std::endl;
  std::cout << std::endl;

  RecognitionEngine r1;

  std::cout << "------- addChronicle/clearChronicleList functions" << std::endl << std::endl;
  Chronicle* a = new ChronicleSingleEvent("a");
  r1.addChronicle(a);
  CRL::testInteger((long)r1.getRootChronicles().size(), 1);
  ChronicleSingleEvent b("b");
  r1.addChronicle(b);
  CRL::testInteger((long)r1.getRootChronicles().size(), 2);
  r1.clearChronicleList();
  CRL::testInteger((long)r1.getRootChronicles().size(), 0);
  delete a;
  std::cout << std::endl;

  std::cout << "------- Accessors" << std::endl << std::endl;

  CRL::testDouble((double)r1.getCurrentTime(), NO_DATE);
  CRL::testInteger(r1.getCurrentOrder(), 0L);
  r1.setCurrentTime(2.5);
  r1.setCurrentOrder(10L);
  CRL::testDouble((double)r1.getCurrentTime(), 2.5);
  CRL::testInteger(r1.getCurrentOrder(), 10L);
  std::cout << std::endl;

  std::cout << "------- addEvent/clearEventBuffer functions" << std::endl << std::endl;

  testRecognitionEngine_addEvent();

  std::cout << "------- process/processEvent functions" << std::endl << std::endl;

  testRecognitionEngine_process();

  std::cout << "------- lookAhead function" << std::endl << std::endl;

  testRecognitionEngine_lookAhead();

  std::cout << std::endl;

}
