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
  \file BooleanValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __BOOLEANVALUE_HPP___
#define __BOOLEANVALUE_HPP___

#include <openfluid/core/SimpleValue.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

/**
BooleanValue is a container for a true/false value.\n

\see Value

\n

<I>Example : declaration</I>
@code
  // declaration of a BooleanValue, initialized to false by default
  openfluid::core::BooleanValue Val1;

  // declaration of a BooleanValue, initialized to true
  openfluid::core::BooleanValue Val2(true);
@endcode


<I>Example : getting the contained value</I>
@code
  bool Tmp1;

  // using the get method
  Tmp1 = Val1.get();

  // or using the cast operator
  Tmp1 = Val1;
@endcode


<I>Example : setting the contained value</I>
@code
  // using the set method
  Val1.set(true);
@endcode


<I>Example : conversion from string</I>
@code
  openfluid::core::StringValue StringVal("true");

  // to BooleanValue
  Val1 = StringVal.toBooleanValue();

  // to bool
  bool BoolVal = StringVal.toBoolean();
@endcode


<I>Example : conversion to string</I>
@code
  std::string StdStrVal = Val1.toString();
@endcode
*/
class DLLEXPORT BooleanValue : public SimpleValue
{
  private:

    bool m_Value;

  public:

    /**
      Default constructor
    */
    BooleanValue() : m_Value(false) {};

    /**
      Copy constructor
    */
    BooleanValue(const BooleanValue& Val) : SimpleValue(Val), m_Value(Val.m_Value) {};

    /**
      Constructor from plain old type
    */
    BooleanValue(const bool& POD) : SimpleValue(), m_Value(POD) {};

    Value& operator =(const Value& Other);

    /**
    * Cast operator
    */
    operator bool() const { return m_Value; };

    virtual ~BooleanValue() {};

    inline Type getType() const { return Value::BOOLEAN; };

    Value* clone() const { return new BooleanValue(*this); };

    /**
      Returns the boolean value as plain old type
      @return the boolean value
    */
    inline bool& get() { return m_Value; };

    /**
      Returns the boolean value as a const plain old type
      @return the boolean value
    */
    inline const bool& get() const { return m_Value; };

    /**
      Sets the plain old type boolean value
      @param[in] Val the boolean value
    */
    inline void set(const bool& Val) { m_Value = Val; };

    void writeToStream(std::ostream& OutStm) const;

};


} }  // namespaces



#endif /* __BOOLEANVALUE_HPP___ */
