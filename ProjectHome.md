**CRL (Chronicle Recognition Library)** has been developed by the Department for the Conception and evaluation of System Performances (DCPS) of the French Aerospace Lab (Office National d'Etudes et Recherches Aérospatiales -- [ONERA](http://www.onera.fr/en)). In the framework of **Complex Event Processing**, this C++ library allows for the analysis of complex event flows to **recognise predetermined searched-for behaviours**. These behaviours are defined as specific arrangements of events using a behaviour description language called the **Chronicle Language**. The behaviour recognition process relies upon a solid mathematical framework which has been developed during a Ph.D. thesis available [here](https://hal.archives-ouvertes.fr/tel-01093015/document). The recognition process has been completely formalised through a set semantics and the algorithms of CRL directly correspond to the mathematical definitions.



## Features ##

  * analysis of complex event flows;
  * temporal language, the chronicle language, supporting classic and complex event operators including the transcription of all of Allen's interval algebra;
  * support for the recognition of the absence of a certain behaviour during another behaviour;
  * possibility to reason and express constraints over complex event properties;
  * all the power of the C++ language available to define these constraints over event properties;
  * support for continuous time;
  * multiplicity of recognitions: for a given behaviour, _all_ possible recognitions are computed and stored;
  * historisation of recognitions: for each recognition, exactly all the events which lead to the recognition are stored, allowing for further reasoning;
  * possibility to set an expiry duration specifying when certain recognitions may be deleted since considered too old (for space saving reasons, if a certain behaviour is recognised very frequently in the observed event flow);
  * action trigger after each recognition;
  * facilitated use, in particular for the definition of the searched-for behaviours, thanks to the definition of C++ operators and macros;
  * algorithms directly mapped onto a mathematical framework, thus ensuring the validity of the program;
  * high adaptability to different application domains;
  * easy integration into very different coding environments.





## Installation ##

CRL is available online [here](http://crl.googlecode.com/svn/crl/).

In order to use the library, it is sufficient to have the "include" files along with the compiled library "lib". One may also directly work with the source files (".cpp").



## Examples ##

Simple examples used as unit tests and non-regression tests are available [here](http://crl.googlecode.com/svn/crl/tests).

CRL has been used for two case studies:
  * The first application oversees the consistency of an unmanned aircraft system (unmanned aircraft, pilot, air traffic control) inside air traffic in case of communication link breakdowns. This application allows, on the one hand, to check the consistency of the procedures currently followed in case of failures; and, on the other hand, to complete these procedures with alarms in case of unavoidable situations caused by human errors. Descriptions and analysis of this application may be found [here](http://drops.dagstuhl.de/opus/volltexte/2013/4093/pdf/13.pdf) and [here](https://hal.inria.fr/hal-01076941).

  * The second application oversees that the security procedures of an unmanned aircraft flying through controlled or uncontrolled airspace are correctly followed. Descriptions and analysis of this application may be found [here](https://hal.archives-ouvertes.fr/tel-01093015/document).


## Contacts ##

  * Ariane Piel  : Ariane.Piel@onera.fr
  * Jean Bourrely   : Jean.Bourrely@onera.fr