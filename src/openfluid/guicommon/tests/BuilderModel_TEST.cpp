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
 \file BuilderModel_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_BuilderModel
#include <boost/test/unit_test.hpp>

#include <openfluid/guicommon/BuilderModel.hpp>
#include "tests-config.hpp"
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/WareDescriptor.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.BuilderDescriptors/singlefile");

  openfluid::guicommon::BuilderModel Model;

  Model.setModelDescriptor(FXDesc.getModelDescriptor());

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const std::list<openfluid::fluidx::ModelItemDescriptor*> Items =
      Model.getItems();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Model.getItemAt(0))->getGeneratedID(),
      "GENERATOR__SCALAR__tests.generator.interp__TU");
  BOOST_CHECK_EQUAL(
      dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(Model.getItemAt(4))->getFileID(),
      "tests.functionB");
  BOOST_CHECK_THROW(Model.getItemAt(5), openfluid::base::OFException);

  BOOST_CHECK_EQUAL(
      Model.getFirstItemIndex("GENERATOR__SCALAR__tests.generator.interp__TU"),
      0);
  BOOST_CHECK_EQUAL(Model.getFirstItemIndex("tests.functionB"), 4);
  BOOST_CHECK_EQUAL(Model.getFirstItemIndex("tests.wrongfunction"), -1);

  std::vector<std::string> IDs = Model.getOrderedIDs();

  BOOST_CHECK_EQUAL(IDs.size(), 5);
  BOOST_CHECK_EQUAL(IDs.at(0), "GENERATOR__SCALAR__tests.generator.interp__TU");
  BOOST_CHECK_EQUAL(IDs.at(4), "tests.functionB");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.BuilderDescriptors/singlefile");

  openfluid::guicommon::BuilderModel Model;

  Model.setModelDescriptor(FXDesc.getModelDescriptor());

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 5);

  const std::list<openfluid::fluidx::ModelItemDescriptor*>* Items =
      &(Model.getItems());

  // appendItem
  openfluid::fluidx::FunctionDescriptor AppItem("appended.item");
  Model.appendItem(&AppItem);

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 6);

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items->begin();

  std::advance(it, 5);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*it))->getFileID(),
      "appended.item");

  //insertItem
  openfluid::fluidx::FunctionDescriptor InsItem("inserted.item");

  BOOST_CHECK_THROW(Model.insertItem(&InsItem, 6),
                    openfluid::base::OFException);

  Model.insertItem(&InsItem, 3);

  BOOST_CHECK_EQUAL(Model.getItemsCount(), 7);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "appended.item");

  // moveItem

  BOOST_CHECK_THROW(Model.moveItem(2, 7), openfluid::base::OFException);
  BOOST_CHECK_THROW(Model.moveItem(7, 2), openfluid::base::OFException);

  Model.moveItem(5, 2);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "appended.item");

  Model.moveItem(3, 0);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "appended.item");

  Model.moveItem(5, 6);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "inserted.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "appended.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);

  //removeItem

  BOOST_CHECK_THROW(Model.removeItem(7), openfluid::base::OFException);

  Model.removeItem(4);

  it = Items->begin();

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionA");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "tests.functionB");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*(++it)))->getFileID(),
      "appended.item");

  BOOST_CHECK_EQUAL(
      (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*(++it)))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);
}

// =====================================================================
// =====================================================================

