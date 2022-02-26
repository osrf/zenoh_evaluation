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
 * @file SetBool_Request.cpp
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

#include "SetBool_Request.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

std_srvs::srv::SetBool_Request::SetBool_Request()
{
    // m_data com.eprosima.idl.parser.typecode.PrimitiveTypeCode@37374a5e
    m_data = false;

}

std_srvs::srv::SetBool_Request::~SetBool_Request()
{
}

std_srvs::srv::SetBool_Request::SetBool_Request(
        const SetBool_Request& x)
{
    m_data = x.m_data;
}

std_srvs::srv::SetBool_Request::SetBool_Request(
        SetBool_Request&& x)
{
    m_data = x.m_data;
}

std_srvs::srv::SetBool_Request& std_srvs::srv::SetBool_Request::operator =(
        const SetBool_Request& x)
{

    m_data = x.m_data;

    return *this;
}

std_srvs::srv::SetBool_Request& std_srvs::srv::SetBool_Request::operator =(
        SetBool_Request&& x)
{

    m_data = x.m_data;

    return *this;
}

bool std_srvs::srv::SetBool_Request::operator ==(
        const SetBool_Request& x)
{

    return (m_data == x.m_data);
}

bool std_srvs::srv::SetBool_Request::operator !=(
        const SetBool_Request& x)
{
    return !(*this == x);
}

size_t std_srvs::srv::SetBool_Request::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    return current_alignment - initial_alignment;
}

size_t std_srvs::srv::SetBool_Request::getCdrSerializedSize(
        const std_srvs::srv::SetBool_Request& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    return current_alignment - initial_alignment;
}

void std_srvs::srv::SetBool_Request::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_data;

}

void std_srvs::srv::SetBool_Request::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_data;
}

/*!
 * @brief This function sets a value in member data
 * @param _data New value for member data
 */
void std_srvs::srv::SetBool_Request::data(
        bool _data)
{
    m_data = _data;
}

/*!
 * @brief This function returns the value of member data
 * @return Value of member data
 */
bool std_srvs::srv::SetBool_Request::data() const
{
    return m_data;
}

/*!
 * @brief This function returns a reference to member data
 * @return Reference to member data
 */
bool& std_srvs::srv::SetBool_Request::data()
{
    return m_data;
}


size_t std_srvs::srv::SetBool_Request::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;



    return current_align;
}

bool std_srvs::srv::SetBool_Request::isKeyDefined()
{
    return false;
}

void std_srvs::srv::SetBool_Request::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
     
}


