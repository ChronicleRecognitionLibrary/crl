/** ***********************************************************************************
 * \file Operators.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Operators.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL {


  ChronicleConjunction& operator&&(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleConjunction(&c1, &c2));
  }


  ChronicleDisjunction& operator||(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleDisjunction(&c1, &c2));
  }


  ChronicleAbsence& operator-(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleAbsence(&c1, &c2));
  }


  ChronicleSequence& operator+(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleSequence(&c1, &c2));
  }


  ChronicleEquals& operator==(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleEquals(&c1, &c2));
  }


  ChronicleDuring& operator&=(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleDuring(&c1, &c2));
  }


  ChronicleStarts& operator|=(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleStarts(&c1, &c2));
  }


  ChronicleFinishes& operator>=(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleFinishes(&c1, &c2));
  }


  ChronicleMeets& operator^(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleMeets(&c1, &c2));
  }


  ChronicleOverlaps& operator/(Chronicle& c1, Chronicle& c2) {
    return *(new ChronicleOverlaps(&c1, &c2));
  }


  ChronicleDelayThen& operator+(Chronicle& c1, const DurationType& d) {
    return *(new ChronicleDelayThen(&c1,d));
  }


  ChronicleDelayLasts& operator==(Chronicle& c1, const DurationType& d) {
    return *(new ChronicleDelayLasts(&c1,d));
  }


  ChronicleDelayAtLeast& operator>(Chronicle& c1, const DurationType& d) {
    return *(new ChronicleDelayAtLeast(&c1,d));
  }


  ChronicleDelayAtMost& operator<(Chronicle& c1, const DurationType& d) {
    return *(new ChronicleDelayAtMost(&c1,d));
  }


  ChronicleCut& operator+=(Chronicle& c1, Chronicle& c2){
    return *(new ChronicleCut(&c1, &c2));
  }

  ChronicleStateChange& operator!=(Chronicle& c1, Chronicle& c2){
    return *(new ChronicleStateChange(&c1, &c2));
  }

} /* namespace CRL */
