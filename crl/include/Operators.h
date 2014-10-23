/** ***********************************************************************************
 * \file Operators.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Shortcut operators for chronicles assembly
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

#ifndef OPERATORS_H_
#define OPERATORS_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleSingleEvent.h"
#include "ChronicleNamed.h"
#include "ChronicleSingleDate.h"
#include "ChronicleConjunction.h"
#include "ChronicleDisjunction.h"
#include "ChronicleAbsence.h"
#include "ChronicleSequence.h"
#include "ChronicleEquals.h"
#include "ChronicleDuring.h"
#include "ChronicleStarts.h"
#include "ChronicleFinishes.h"
#include "ChronicleMeets.h"
#include "ChronicleOverlaps.h"
#include "ChronicleDelayThen.h"
#include "ChronicleDelayLasts.h"
#include "ChronicleDelayAtLeast.h"
#include "ChronicleDelayAtMost.h"
#include "ChronicleAt.h"
#include "ChronicleCut.h"
#include "ChronicleStateChange.h"

#include "Property.h"


// ----------------------------------------------------------------------------
// MACROS ET OPERATEURS DE RACCOURCIS
// ----------------------------------------------------------------------------

namespace CRL {

  //! Single event chronicle
  #define $(s) (*(new ChronicleSingleEvent(#s)))

  //! Chronicle (C->x)
  #define $$(c, name) (*(new ChronicleNamed(c, #name)))

  //! "Simple" date chronicle
  #define T(s) (*(new ChronicleSingleDate(s)))

  //! Chronicle At
  #define AT(c) (*(new ChronicleAt(c)))

  //! Conjunction operator (C1 AND C2)
  ChronicleConjunction& operator&&(Chronicle& c1, Chronicle& c2);

  //! Disjunction operator (C1 OR C2)
  ChronicleDisjunction& operator||(Chronicle& c1, Chronicle& c2);

  //! Sequence operator (C1 THEN C2)
  ChronicleSequence& operator+(Chronicle& c1, Chronicle& c2);

  //! Absence operator (C1 WITHOUT C2)
  ChronicleAbsence& operator-(Chronicle& c1, Chronicle& c2);

  //! Conjunction operator with equal intervals (C1 EQUALS C2)
  ChronicleEquals& operator==(Chronicle& c1, Chronicle& c2);

  //! Conjunction operator with inclusion of intervals (C1 DURING C2)
  ChronicleDuring& operator&=(Chronicle& c1, Chronicle& c2);

  //! Conjunction operator with synchronised beginnings (C1 STARTS C2)
  ChronicleStarts& operator|=(Chronicle& c1, Chronicle& c2);

  //! Conjunction operator with synchronised endings (C1 FINISHES C2)
  ChronicleFinishes& operator>=(Chronicle& c1, Chronicle& c2);

  //! Sequence operator with adjacent intervals (C1 MEETS C2)
  ChronicleMeets& operator^(Chronicle& c1, Chronicle& c2);

  //! Conjunction operator with overlapping intervals (C1 OVERLAPS C2)
  ChronicleOverlaps& operator/(Chronicle& c1, Chronicle& c2);

  //! Delay operator (C1 THEN A DELAY D)
  ChronicleDelayThen& operator+(Chronicle& c1, const DurationType& d);

  //! Operator of exact duration (C1 LASTS EXACLTY D)
  ChronicleDelayLasts& operator==(Chronicle& c1, const DurationType& d);

  //! Operator of minimal duration (C1 LASTS AT LEAST D)
  ChronicleDelayAtLeast& operator>(Chronicle& c1, const DurationType& d);

  //! Operator of maximal duration (C1 LASTS AT MOST D)
  ChronicleDelayAtMost& operator<(Chronicle& c1, const DurationType& d);

  //! Cut operator (C1 THEN THE FIRST C2)
  ChronicleCut& operator+=(Chronicle& c1, Chronicle& c2);

  //! Double cut operator (state change) (THE LAST C1 THEN THE FIRST C2)
  ChronicleStateChange& operator!=(Chronicle& c1, Chronicle& c2);


} /* namespace CRL */

#endif /* OPERATORS_H_ */
