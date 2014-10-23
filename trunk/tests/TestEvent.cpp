/** ***********************************************************************************
 * \file TestEvent.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Event
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

#include "Event.h"
#include "TestUtils.h"

using namespace CRL;


  void testEvent()
  {
    CRL::CRL_ErrReport::START("CRL", "Event");
    std::cout << "##### ------- Tests of Event class" << std::endl;

    // Various constructors
    Event   a("A");
    Event   b("B", 2.5);

    CRL::testString(a.getName().c_str(), "A");

    CRL::testInteger(a.getOrder(), -1L);

    CRL::testDouble((double)b.getDate(), 2.5);

    a.setDate(1.5);
    CRL::testDouble((double)a.getDate(), 1.5);

    a.setOrder(5L);
    CRL::testInteger(a.getOrder(), 5L);

    a.setName("AA");
    CRL::testString(a.getName().c_str(), "AA");

    std::cout << std::endl;
  }




