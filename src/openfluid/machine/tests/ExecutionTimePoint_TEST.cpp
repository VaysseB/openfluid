/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ExecutionTimePoint_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_executiontimepoint
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/machine/ExecutionTimePoint.hpp>
#include <openfluid/base/SimulationStatus.hpp>


// =====================================================================
// =====================================================================


class FuncA : openfluid::ware::PluggableFunction
{
  public:

    FuncA() : openfluid::ware::PluggableFunction() {};

    ~FuncA() {};

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)  { }

    void prepareData() { }

    void checkConsistency() { }

    openfluid::core::Duration_t initializeRun() { return DefaultDeltaT(); }

    openfluid::core::Duration_t runStep() { std::cout << "func.a" << std::endl; return DefaultDeltaT(); }

    void finalizeRun() { }

};


class FuncB : openfluid::ware::PluggableFunction
{
  public:

    FuncB() : openfluid::ware::PluggableFunction() {};

    ~FuncB() {};

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)  { }

    void prepareData() { }

    void checkConsistency() { }

    openfluid::core::Duration_t initializeRun() { return DefaultDeltaT(); }

    openfluid::core::Duration_t runStep() { std::cout << "func.b" << std::endl; return DefaultDeltaT(); }

    void finalizeRun() { }

};


class FuncC : openfluid::ware::PluggableFunction
{
  public:

    FuncC() : openfluid::ware::PluggableFunction() {};

    ~FuncC() {};

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)  { }

    void prepareData() { }

    void checkConsistency() { }

    openfluid::core::Duration_t initializeRun() { return DefaultDeltaT(); }

    openfluid::core::Duration_t runStep() { std::cout << "func.c" << std::endl; return DefaultDeltaT(); }

    void finalizeRun() { }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),openfluid::core::DateTime(2012,1,15,14,46,39),60);
  openfluid::machine::ExecutionTimePoint TP(17);

  BOOST_REQUIRE_EQUAL(TP.getTimeIndex(),17);
  BOOST_REQUIRE(!TP.hasItemsToProcess());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
	openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),openfluid::core::DateTime(2012,1,15,14,46,39),60);
	openfluid::machine::ExecutionTimePoint TP(17);

	BOOST_REQUIRE_EQUAL(TP.getTimeIndex(),17);

	openfluid::machine::ModelItemInstance* MII;

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableFunction*)(new FuncA());
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::FunctionSignature();
  MII->Signature->ID = "func.a";
  MII->OriginalPosition = 1;
  TP.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableFunction*)(new FuncC());
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::FunctionSignature();
  MII->Signature->ID = "func.c";
  MII->OriginalPosition = 3;
  TP.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body = (openfluid::ware::PluggableFunction*)(new FuncB());
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::FunctionSignature();
  MII->Signature->ID = "func.b";
  MII->OriginalPosition = 2;
  TP.appendItem(MII);



	BOOST_REQUIRE(TP.hasItemsToProcess());

	TP.sortByOriginalPosition();

	while (TP.hasItemsToProcess()) TP.processNextItem();

  BOOST_REQUIRE(!TP.hasItemsToProcess());

}

// =====================================================================
// =====================================================================
