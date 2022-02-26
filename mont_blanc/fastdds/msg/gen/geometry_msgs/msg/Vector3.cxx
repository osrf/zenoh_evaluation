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
 * @file Vector3.cpp
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

#include "Vector3.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

geometry_msgs::msg::Vector3::Vector3()
{
    // m_x com.eprosima.idl.parser.typecode.PrimitiveTypeCode@7714e963
    m_x = 0.0;
    // m_y com.eprosima.idl.parser.typecode.PrimitiveTypeCode@20ce78ec
    m_y = 0.0;
    // m_z com.eprosima.idl.parser.typecode.PrimitiveTypeCode@6eda5c9
    m_z = 0.0;

}

geometry_msgs::msg::Vector3::~Vector3()
{



}

geometry_msgs::msg::Vector3::Vector3(
        const Vector3& x)
{
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
}

geometry_msgs::msg::Vector3::Vector3(
        Vector3&& x)
{
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
}

geometry_msgs::msg::Vector3& geometry_msgs::msg::Vector3::operator =(
        const Vector3& x)
{

    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;

    return *this;
}

geometry_msgs::msg::Vector3& geometry_msgs::msg::Vector3::operator =(
        Vector3&& x)
{

    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;

    return *this;
}

bool geometry_msgs::msg::Vector3::operator ==(
        const Vector3& x)
{

    return (m_x == x.m_x && m_y == x.m_y && m_z == x.m_z);
}

bool geometry_msgs::msg::Vector3::operator !=(
        const Vector3& x)
{
    return !(*this == x);
}

size_t geometry_msgs::msg::Vector3::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

size_t geometry_msgs::msg::Vector3::getCdrSerializedSize(
        const geometry_msgs::msg::Vector3& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

void geometry_msgs::msg::Vector3::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_x;
    scdr << m_y;
    scdr << m_z;

}

void geometry_msgs::msg::Vector3::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_x;
    dcdr >> m_y;
    dcdr >> m_z;
}

/*!
 * @brief This function sets a value in member x
 * @param _x New value for member x
 */
void geometry_msgs::msg::Vector3::x(
        double _x)
{
    m_x = _x;
}

/*!
 * @brief This function returns the value of member x
 * @return Value of member x
 */
double geometry_msgs::msg::Vector3::x() const
{
    return m_x;
}

/*!
 * @brief This function returns a reference to member x
 * @return Reference to member x
 */
double& geometry_msgs::msg::Vector3::x()
{
    return m_x;
}

/*!
 * @brief This function sets a value in member y
 * @param _y New value for member y
 */
void geometry_msgs::msg::Vector3::y(
        double _y)
{
    m_y = _y;
}

/*!
 * @brief This function returns the value of member y
 * @return Value of member y
 */
double geometry_msgs::msg::Vector3::y() const
{
    return m_y;
}

/*!
 * @brief This function returns a reference to member y
 * @return Reference to member y
 */
double& geometry_msgs::msg::Vector3::y()
{
    return m_y;
}

/*!
 * @brief This function sets a value in member z
 * @param _z New value for member z
 */
void geometry_msgs::msg::Vector3::z(
        double _z)
{
    m_z = _z;
}

/*!
 * @brief This function returns the value of member z
 * @return Value of member z
 */
double geometry_msgs::msg::Vector3::z() const
{
    return m_z;
}

/*!
 * @brief This function returns a reference to member z
 * @return Reference to member z
 */
double& geometry_msgs::msg::Vector3::z()
{
    return m_z;
}


size_t geometry_msgs::msg::Vector3::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t current_align = current_alignment;






    return current_align;
}

bool geometry_msgs::msg::Vector3::isKeyDefined()
{
    return false;
}

void geometry_msgs::msg::Vector3::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
       
}


