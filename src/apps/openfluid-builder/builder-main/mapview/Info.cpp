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
 \file Info.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <iostream>

#include "EngineHelper.hpp"
#include "ToolBox.hpp"

#include "Info.hpp"

Info::Info(Gtk::Window& ParentWindow, const Glib::ustring& Title,
    openfluid::core::CoreRepository& CoreRepos) :
  mp_CoreRepos(&CoreRepos)
{
  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->set_transient_for(ParentWindow);
  mp_Dialog->set_title(Title);
  mp_Dialog->set_size_request(500, 600);

  mref_ListStoreIDs = Gtk::ListStore::create(m_ModelColumnIDs);
  mref_ListStoreFrom = Gtk::ListStore::create(m_ModelColumnsNameClassIDsFrom);
  mref_ListStoreTo = Gtk::ListStore::create(m_ModelColumnsNameClassIDsTo);
  mref_ListStoreParent = Gtk::ListStore::create(
      m_ModelColumnsNameClassIDsParent);
  mref_ListStoreChildren = Gtk::ListStore::create(
      m_ModelColumnsNameClassIDsChildren);

  mp_TreeViewIDs = Gtk::manage(new Gtk::TreeView(mref_ListStoreIDs));
  mp_TreeViewFrom = Gtk::manage(new Gtk::TreeView(mref_ListStoreFrom));
  mp_TreeViewTo = Gtk::manage(new Gtk::TreeView(mref_ListStoreTo));
  mp_TreeViewParent = Gtk::manage(new Gtk::TreeView(mref_ListStoreParent));
  mp_TreeViewChildren = Gtk::manage(new Gtk::TreeView(mref_ListStoreChildren));

  mp_TreeViewIDs->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mp_NameClassLabel = Gtk::manage(new Gtk::Label(""));

  mp_MainHBox = Gtk::manage(new Gtk::HBox());
  mp_InfoIntputEventsVBox = Gtk::manage(new Gtk::VBox());
  mp_ButtonInfoIntputEventsVBox = Gtk::manage(new Gtk::VBox());

  mp_InfoIntputEventsScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_IDScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());

  mp_FromScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ToScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ParentScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ChildrenScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());

  mp_InfoIntputEventsScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_IDScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  mp_FromScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_ToScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ParentScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_ChildrenScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);

  mp_EventExpander = Gtk::manage(new Gtk::Expander(_("Events"), false));
  mp_InfoExpander = Gtk::manage(new Gtk::Expander(_("Info"), false));
  mp_InputDataExpander = Gtk::manage(new Gtk::Expander(_("Input Data"), false));

  mp_EventTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable = Gtk::manage(new Gtk::Table(11, 2, false));
  mp_InputDataTable = Gtk::manage(new Gtk::Table());

  mp_FromExpander = Gtk::manage(new Gtk::Expander(_("From :"), false));
  mp_ToExpander = Gtk::manage(new Gtk::Expander(_("To :"), false));
  mp_ParentExpander = Gtk::manage(new Gtk::Expander(_("Parents :"), false));
  mp_ChildrenExpander = Gtk::manage(new Gtk::Expander(_("Children :"), false));

  mp_RestoreDefaultButton = Gtk::manage(
      new Gtk::Button(_("Restore Default"), false));

  mp_IDLabel = Gtk::manage(new Gtk::Label(""));
  mp_ProcessOrderLabel = Gtk::manage(new Gtk::Label(""));

  m_IDLabel.set_label(_("ID :"));
  m_ProcessOrderLabel.set_label(_("Process Order :"));

  m_IDLabel.set_alignment(.02, 0);
  m_ProcessOrderLabel.set_alignment(.02, 0);

  mp_InfoTable->set_spacings(5);
  mp_EventTable->set_spacings(5);

  mp_TreeViewIDs->append_column(_("ID"), m_ModelColumnIDs.m_ID);
  mp_TreeViewIDs->set_size_request(40,-1);
  mp_TreeViewIDs->get_column(0)->set_sort_column(m_ModelColumnIDs.m_ID);
  mp_TreeViewIDs->get_selection()->signal_changed().connect(
      sigc::mem_fun(*this, &Info::onIDViewSelectionChanged));

  mp_TreeViewFrom->append_column(_("Unit classes"),
      m_ModelColumnsNameClassIDsFrom.m_NameClass);
  mp_TreeViewFrom->append_column(_("ID"), m_ModelColumnsNameClassIDsFrom.m_ID);
  mp_TreeViewTo->append_column(_("Unit classes"),
      m_ModelColumnsNameClassIDsTo.m_NameClass);
  mp_TreeViewTo->append_column(_("ID"), m_ModelColumnsNameClassIDsTo.m_ID);
  mp_TreeViewParent->append_column(_("Unit classes"),
      m_ModelColumnsNameClassIDsParent.m_NameClass);
  mp_TreeViewParent->append_column(_("ID"),
      m_ModelColumnsNameClassIDsParent.m_ID);
  mp_TreeViewChildren->append_column(_("Unit classes"),
      m_ModelColumnsNameClassIDsChildren.m_NameClass);
  mp_TreeViewChildren->append_column(_("ID"),
      m_ModelColumnsNameClassIDsChildren.m_ID);

  mp_TreeViewFrom->get_column(0)->set_sort_column(
      m_ModelColumnsNameClassIDsFrom.m_ID);
  mp_TreeViewFrom->get_column(1)->set_sort_column(
      m_ModelColumnsNameClassIDsFrom.m_NameClass);
  mp_TreeViewTo->get_column(0)->set_sort_column(
      m_ModelColumnsNameClassIDsTo.m_ID);
  mp_TreeViewTo->get_column(1)->set_sort_column(
      m_ModelColumnsNameClassIDsTo.m_NameClass);
  mp_TreeViewParent->get_column(0)->set_sort_column(
      m_ModelColumnsNameClassIDsParent.m_ID);
  mp_TreeViewParent->get_column(1)->set_sort_column(
      m_ModelColumnsNameClassIDsParent.m_NameClass);
  mp_TreeViewChildren->get_column(0)->set_sort_column(
      m_ModelColumnsNameClassIDsChildren.m_ID);
  mp_TreeViewChildren->get_column(1)->set_sort_column(
      m_ModelColumnsNameClassIDsChildren.m_NameClass);

  // =====================================================================
  //  Table Info
  mp_InfoTable->attach(m_IDLabel, 0, 1, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(m_ProcessOrderLabel, 0, 1, 2, 3,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_InfoTable->attach(*mp_FromExpander, 0, 2, 4, 5, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ToExpander, 0, 2, 6, 7, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ParentExpander, 0, 2, 8, 9, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ChildrenExpander, 0, 2, 10, 11,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_InfoTable->attach(*mp_IDLabel, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
      Gtk::SHRINK);
  mp_InfoTable->attach(*mp_ProcessOrderLabel, 1, 2, 2, 3,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

  mp_InfoTable->attach(*ToolBox::setHSeparator(), 0, 2, 3, 4,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_InfoTable->attach(*ToolBox::setHSeparator(), 0, 2, 5, 6,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_InfoTable->attach(*ToolBox::setHSeparator(), 0, 2, 7, 8,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
  mp_InfoTable->attach(*ToolBox::setHSeparator(), 0, 2, 9, 10,
      Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

  // =====================================================================

  mp_FromScrolledWindow->add(*mp_TreeViewFrom);
  mp_FromScrolledWindow->set_size_request(-1,100);

  mp_ToScrolledWindow->add(*mp_TreeViewTo);
  mp_ToScrolledWindow->set_size_request(-1,100);

  mp_ParentScrolledWindow->add(*mp_TreeViewParent);
  mp_ParentScrolledWindow->set_size_request(-1,100);

  mp_ChildrenScrolledWindow->add(*mp_TreeViewChildren);
  mp_ChildrenScrolledWindow->set_size_request(-1,100);


  mp_FromExpander->add(*mp_FromScrolledWindow);
  mp_ToExpander->add(*mp_ToScrolledWindow);
  mp_ParentExpander->add(*mp_ParentScrolledWindow);
  mp_ChildrenExpander->add(*mp_ChildrenScrolledWindow);

  mp_InfoExpander->add(*mp_InfoTable);
  mp_InputDataExpander->add(*mp_InputDataTable);

  mp_InfoIntputEventsVBox->pack_start(*mp_InfoExpander, Gtk::PACK_SHRINK, 5);
  mp_InfoIntputEventsVBox->pack_start(*mp_InputDataExpander, Gtk::PACK_SHRINK,
      5);
  mp_InfoIntputEventsVBox->pack_start(*mp_EventExpander, Gtk::PACK_SHRINK, 5);

  mp_InfoIntputEventsScrolledWindow->add(*mp_InfoIntputEventsVBox);
  mp_ButtonInfoIntputEventsVBox->pack_start(*mp_InfoIntputEventsScrolledWindow,
      Gtk::PACK_EXPAND_WIDGET, 5);
  mp_ButtonInfoIntputEventsVBox->pack_end(*mp_RestoreDefaultButton,
      Gtk::PACK_SHRINK, 5);

  mp_IDScrolledWindow->add(*mp_TreeViewIDs);

  mp_MainHBox->pack_start(*mp_IDScrolledWindow, Gtk::PACK_SHRINK, 5);
  mp_MainHBox->pack_start(*mp_ButtonInfoIntputEventsVBox,
      Gtk::PACK_EXPAND_WIDGET, 5);

  mp_Dialog->get_vbox()->pack_start(*mp_NameClassLabel, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*mp_MainHBox, Gtk::PACK_EXPAND_WIDGET, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_OkButton = mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

void Info::onIDViewSelectionChanged()
{
  m_SelectedUnitId.clear();
  mp_TreeViewIDs->get_selection()->selected_foreach_iter(
      sigc::mem_fun(*this, &Info::loadSelectedID));
  std::set<int>::iterator it;
  for (it = m_SelectedUnitId.begin(); it != m_SelectedUnitId.end(); it++)
  {
    loadInfo(*it, m_SelectedUnitId.size());
    loadInputData(*it, m_SelectedUnitId.size());
    //TODO les evenements à faire
    loadEvent(*it, m_SelectedUnitId.size());
  }
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Info::loadSelectedID(const Gtk::TreeModel::iterator& iter)
{
  Gtk::TreeModel::Row row = *iter;
  m_SelectedUnitId.insert(row.get_value(m_ModelColumnIDs.m_ID));

}

// =====================================================================
// =====================================================================

void Info::loadInfo(int ID, int SelectionSize)
{
  mp_IDLabel->set_label("");
  mp_ProcessOrderLabel->set_label("");
  mref_ListStoreFrom->clear();
  mref_ListStoreTo->clear();
  mref_ListStoreParent->clear();
  mref_ListStoreChildren->clear();

  if (SelectionSize == 1)
  {
    openfluid::core::Unit* TempUnit = mp_CoreRepos->getUnit(
        mp_NameClassLabel->get_label(), ID);
    std::stringstream streamID;
    std::stringstream streamProcessOrder;
    streamID << ID;
    streamProcessOrder << TempUnit->getProcessOrder();
    std::string IDstr = streamID.str();
    std::string ProcessOrderstr = streamProcessOrder.str();
    mp_IDLabel->set_label(IDstr);
    mp_ProcessOrderLabel->set_label(ProcessOrderstr);

    std::set<std::string> ClassNames =
        EngineHelper::getClassNames(mp_CoreRepos);
    std::set<std::string>::iterator iter;
    for (iter = ClassNames.begin(); iter != ClassNames.end(); iter++)
    {
      const openfluid::core::UnitsPtrList_t* TempListUnit =
          TempUnit->getFromUnits(*iter);
      fillNameClassIDListStore(mref_ListStoreFrom,
          m_ModelColumnsNameClassIDsFrom, TempListUnit, *iter);
      TempListUnit = TempUnit->getToUnits(*iter);
      fillNameClassIDListStore(mref_ListStoreTo, m_ModelColumnsNameClassIDsTo,
          TempListUnit, *iter);
      TempListUnit = TempUnit->getParentUnits(*iter);
      fillNameClassIDListStore(mref_ListStoreParent,
          m_ModelColumnsNameClassIDsParent, TempListUnit, *iter);
      TempListUnit = TempUnit->getChildrenUnits(*iter);
      fillNameClassIDListStore(mref_ListStoreChildren,
          m_ModelColumnsNameClassIDsChildren, TempListUnit, *iter);
    }
  }
}

// =====================================================================
// =====================================================================

void Info::loadInputData(int ID, int SelectionSize)
{
  if (SelectionSize == 1)
  {
    std::map<Gtk::Label*, std::pair<Gtk::Entry*, Gtk::Button*> >::iterator it;
    for (it = m_InputDataLineTable.begin(); it != m_InputDataLineTable.end(); ++it)
    {
      openfluid::core::Unit* TempUnit = mp_CoreRepos->getUnit(
          mp_NameClassLabel->get_label(), ID);
      std::string Val;
      TempUnit->getInputData()->getValue(((*it).first)->get_label(), Val);
      (*it).second.first->set_text(Val);
    }
  }
}

// =====================================================================
// =====================================================================
//TODO les evenements à faire
void Info::loadEvent(int /*ID*/, int /*SelectionSize*/)
{

}

// =====================================================================
// =====================================================================

void Info::fillNameClassIDListStore(Glib::RefPtr<Gtk::ListStore>& ListStore,
    ModelColumnsNameClassIDs& ModelColumnsNameClassIDs,
    const openfluid::core::UnitsPtrList_t* TempListUnit, std::string ClassName)
{
  openfluid::core::UnitsPtrList_t::const_iterator itF;

  if (TempListUnit != NULL)
  {
    for (itF = TempListUnit->begin(); itF != TempListUnit->end(); ++itF)
    {
      Gtk::TreeRow Row = *ListStore->append();
      Row[ModelColumnsNameClassIDs.m_ID] = (*itF)->getID();
      Row[ModelColumnsNameClassIDs.m_NameClass] = ClassName;
    }
  }
}

// =====================================================================
// =====================================================================

void Info::onEntryInputDataChanged(std::string InputDataName)
{
  if (m_SelectedUnitId.size() == 1)
  {
    Gtk::Label* TempLabel;
    std::set<int>::iterator it;
    for (it = m_SelectedUnitId.begin(); it != m_SelectedUnitId.end(); ++it)
    {
      std::map<Gtk::Label*, std::pair<Gtk::Entry*, Gtk::Button*> >::iterator
          iter;
      for (iter = m_InputDataLineTable.begin(); iter
          != m_InputDataLineTable.end(); ++iter)
      {
        if ((*iter).first->get_label() == InputDataName)
        {
          TempLabel = (*iter).first;
          if (!m_ChangeValue.empty())
          {
            bool addInputDataNameValue = true;
            std::map<int, std::map<std::string, std::string> >::iterator
                ChangeMapIt;
            ChangeMapIt = m_ChangeValue.find(*it);
            std::map<std::string, std::string>::iterator ChangeSubMapIt;
            for (ChangeSubMapIt = (*ChangeMapIt).second.begin(); ChangeSubMapIt
                != (*ChangeMapIt).second.end(); ++ChangeSubMapIt)
            {
              if ((*ChangeSubMapIt).first == InputDataName)
              {
                if ((*ChangeSubMapIt).second
                    != (*iter).second.first->get_text())
                {
                  (*ChangeSubMapIt).second = (*iter).second.first->get_text();
                  (*iter).second.second->set_sensitive(true);
                }
                addInputDataNameValue = false;
              }
            }
            if (addInputDataNameValue)
            {
              (*ChangeMapIt).second.insert(
                  std::pair<std::string, std::string>(
                      (*iter).first->get_label(),
                      (*iter).second.first->get_text()));
              (*iter).second.second->set_sensitive(true);
            }

          } else
          {
            std::map<std::string, std::string> TempMapStringString;
            TempMapStringString.insert(
                std::pair<std::string, std::string>((*iter).first->get_label(),
                    (*iter).second.first->get_text()));
            m_ChangeValue.insert(
                std::pair<int, std::map<std::string, std::string> >(*it,
                    TempMapStringString));
            (*iter).second.second->set_sensitive(true);
          }
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================


bool Info::on_focus_out_event(GdkEventFocus* /*event*/,
    std::string InputDataName)
{
  onEntryInputDataChanged(InputDataName);
  return false;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Info::show(std::string ClassName, std::set<int> UnitIDs)
{
  mp_IDLabel->set_label("");
  mp_ProcessOrderLabel->set_label("");
  mref_ListStoreFrom->clear();
  mref_ListStoreTo->clear();
  mref_ListStoreParent->clear();
  mref_ListStoreChildren->clear();
  mref_ListStoreIDs->clear();
  m_InputDataNames.clear();
  if (!m_InputDataLineTable.empty())
  {
    std::map<Gtk::Label*, std::pair<Gtk::Entry*, Gtk::Button*> >::iterator iter;
    for (iter = m_InputDataLineTable.begin(); iter
    != m_InputDataLineTable.end(); ++iter)
    {
      delete((*iter).first);
      delete((*iter).second.second);
      delete((*iter).second.first);
    }

  }
  m_InputDataLineTable.clear();

  mp_NameClassLabel->set_label(ClassName);
  mp_TreeViewIDs->set_name("");

  std::set<int>::iterator it;
  for (it = UnitIDs.begin(); it != UnitIDs.end(); it++)
  {
    Gtk::TreeRow Row = *mref_ListStoreIDs->append();
    Row[m_ModelColumnIDs.m_ID] = *it;

    m_InputDataNames
    = mp_CoreRepos->getUnit(ClassName, *it)->getInputData()->getInputDataNames();

    if(m_InputDataNames.empty())
      mp_InputDataTable->set_visible(false);
    else
    {
      mp_InputDataTable->resize((m_InputDataNames.size() * 2) - 1, 3);
      mp_InputDataTable->set_homogeneous(false);
      mp_InputDataTable->set_spacings(5);

      int compt = 0;
      for (unsigned int i = 0; i < (m_InputDataNames.size() * 2) - 1; i++)
      {
        if ((i % 2) == 0)
        {
          Gtk::Label* InputDataLabel = Gtk::manage(
              new Gtk::Label(m_InputDataNames[compt]));

          mp_InputDataTable->attach(*InputDataLabel, 0, 1, i, i + 1,
              Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

          Gtk::Entry* InputDataValueEntry = Gtk::manage(new Gtk::Entry());
          InputDataValueEntry->set_name(InputDataLabel->get_label());
          mp_InputDataTable->attach(*InputDataValueEntry, 1, 2, i, i + 1,
              Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
          InputDataValueEntry->signal_activate().connect(
              sigc::bind<std::string>(
                  sigc::mem_fun(*this, &Info::onEntryInputDataChanged),
                  InputDataValueEntry->get_name()));
          InputDataValueEntry->signal_focus_out_event().connect(
              sigc::bind<std::string>(
                  sigc::mem_fun(*this, &Info::on_focus_out_event),
                  InputDataValueEntry->get_name()));
          InputDataValueEntry->set_activates_default(true);

          Gtk::Button* RestoreDefaultButton = Gtk::manage(
              new Gtk::Button(_("Restore"), false));
          mp_InputDataTable->attach(*RestoreDefaultButton, 2, 3, i, i + 1,
              Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

          std::pair<Gtk::Entry*, Gtk::Button*> TempPairEntryButton =
              std::make_pair(InputDataValueEntry, RestoreDefaultButton);
          m_InputDataLineTable.insert(
              std::pair<Gtk::Label*, std::pair<Gtk::Entry*, Gtk::Button*> >(
                  InputDataLabel, TempPairEntryButton));

          InputDataLabel->set_visible(true);
          InputDataValueEntry->set_visible(true);
          RestoreDefaultButton->set_visible(true);
          RestoreDefaultButton->set_sensitive(false);
          compt++;
        } else
        {
          mp_InputDataTable->attach(*ToolBox::setHSeparator(), 0, 3, i, i + 1,
              Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
        }
      }
      mp_InputDataTable->set_visible(true);
    }
  }

  if(! mref_ListStoreIDs->children().empty())
      mp_TreeViewIDs->get_selection()->select(mref_ListStoreIDs->children()[0]);


  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {

  }

  mp_Dialog->hide();
}
