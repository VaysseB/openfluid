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
 \file BuilderModuleFactory.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __BUILDERMODULEFACTORY_HPP__
#define __BUILDERMODULEFACTORY_HPP__

#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/base/RunDescriptor.hpp>
#include <openfluid/base/OutputDescriptor.hpp>

#include "BuilderModule.hpp"


class BuilderListToolBoxFactory;
class BuilderAppActions;
//class BuilderModelModuleSub;
//class BuilderDomainModuleSub;
//class BuilderSimulationModuleSub;
//class BuilderResultsModuleSub;

class BuilderModuleFactory
{
  private:
    // cf. Idiome de construction à la première utilisation : http://cpp.developpez.com/faq/cpp/?page=static
    static BuilderListToolBoxFactory& ListToolBoxFactory();
  public:
    static BuilderModule* createHomeModule(BuilderAppActions& Actions);
    static BuilderModule* createModelModule(
        openfluid::machine::ModelInstance& ModelInstance);
//    static BuilderModelModuleSub* createModelModuleSub();
    static BuilderModule* createDomainModule(
        openfluid::core::CoreRepository& CoreRepos);
//    static BuilderDomainModuleSub* createDomainModuleSub();
    static BuilderModule* createSimulationModule(
        openfluid::base::RunDescriptor& RunDesc,
        openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance);
//    static BuilderSimulationModuleSub* createSimulationModuleSub();
    static BuilderModule* createResultsModule(
        openfluid::base::RunDescriptor& RunDesc,
        openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance);
//    static BuilderResultsModuleSub* createResultsModuleSub();
};

#endif /* __BUILDERMODULEFACTORY_HPP__ */