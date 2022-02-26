// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file Empty.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "Empty.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

std_msgs::msg::Empty::Empty()
{
    // m_dummy com.eprosima.idl.parser.typecode.PrimitiveTypeCode@37374a5e
    m_dummy = false;

}

std_msgs::msg::Empty::~Empty()
{
}

std_msgs::msg::Empty::Empty(
        const Empty& x)
{
    m_dummy = x.m_dummy;
}

std_msgs::msg::Empty::Empty(
        Empty&& x)
{
    m_dummy = x.m_dummy;
}

std_msgs::msg::Empty& std_msgs::msg::Empty::operator =(
        const Empty& x)
{

    m_dummy = x.m_dummy;

    return *this;
}

std_msgs::msg::Empty& std_msgs::msg::Empty::operator =(
        Empty&& x)
{

    m_dummy = x.m_dummy;

    return *this;
}

bool std_msgs::msg::Empty::operator ==(
        const Empty& x)
{

    return (m_dummy == x.m_dummy);
}

bool std_msgs::msg::Empty::operator !=(
        const Empty& x)
{
    return !(*this == x);
}

size_t std_msgs::msg::Empty::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    return current_alignment - initial_alignment;
}

size_t std_msgs::msg::Empty::getCdrSerializedSize(
        const std_msgs::msg::Empty& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    return current_alignment - initial_alignment;
}

void std_msgs::msg::Empty::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_dummy;

}

void std_msgs::msg::Empty::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_dummy;
}

/*!
 * @brief This function sets a value in member dummy
 * @param _dummy New value for member dummy
 */
void std_msgs::msg::Empty::dummy(
        bool _dummy)
{
    m_dummy = _dummy;
}

/*!
 * @brief This function returns the value of member dummy
 * @return Value of member dummy
 */
bool std_msgs::msg::Empty::dummy() const
{
    return m_dummy;
}

/*!
 * @brief This function returns a reference to member dummy
 * @return Reference to member dummy
 */
bool& std_msgs::msg::Empty::dummy()
{
    return m_dummy;
}


size_t std_msgs::msg::Empty::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;



    return current_align;
}

bool std_msgs::msg::Empty::isKeyDefined()
{
    return false;
}

void std_msgs::msg::Empty::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
     
}


