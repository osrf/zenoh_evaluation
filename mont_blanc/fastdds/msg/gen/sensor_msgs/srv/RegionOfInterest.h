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
 * @file RegionOfInterest.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _SENSOR_MSGS_MSG_REGIONOFINTEREST_H_
#define _SENSOR_MSGS_MSG_REGIONOFINTEREST_H_


#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(RegionOfInterest_SOURCE)
#define RegionOfInterest_DllAPI __declspec( dllexport )
#else
#define RegionOfInterest_DllAPI __declspec( dllimport )
#endif // RegionOfInterest_SOURCE
#else
#define RegionOfInterest_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define RegionOfInterest_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


namespace sensor_msgs {
    namespace msg {
        /*!
         * @brief This class represents the structure RegionOfInterest defined by the user in the IDL file.
         * @ingroup REGIONOFINTEREST
         */
        class RegionOfInterest
        {
        public:

            /*!
             * @brief Default constructor.
             */
            eProsima_user_DllExport RegionOfInterest();

            /*!
             * @brief Default destructor.
             */
            eProsima_user_DllExport ~RegionOfInterest();

            /*!
             * @brief Copy constructor.
             * @param x Reference to the object sensor_msgs::msg::RegionOfInterest that will be copied.
             */
            eProsima_user_DllExport RegionOfInterest(
                    const RegionOfInterest& x);

            /*!
             * @brief Move constructor.
             * @param x Reference to the object sensor_msgs::msg::RegionOfInterest that will be copied.
             */
            eProsima_user_DllExport RegionOfInterest(
                    RegionOfInterest&& x);

            /*!
             * @brief Copy assignment.
             * @param x Reference to the object sensor_msgs::msg::RegionOfInterest that will be copied.
             */
            eProsima_user_DllExport RegionOfInterest& operator =(
                    const RegionOfInterest& x);

            /*!
             * @brief Move assignment.
             * @param x Reference to the object sensor_msgs::msg::RegionOfInterest that will be copied.
             */
            eProsima_user_DllExport RegionOfInterest& operator =(
                    RegionOfInterest&& x);

            /*!
             * @brief Comparison operator.
             * @param x sensor_msgs::msg::RegionOfInterest object to compare.
             */
            eProsima_user_DllExport bool operator ==(
                    const RegionOfInterest& x);

            /*!
             * @brief Comparison operator.
             * @param x sensor_msgs::msg::RegionOfInterest object to compare.
             */
            eProsima_user_DllExport bool operator !=(
                    const RegionOfInterest& x);

            /*!
             * @brief This function sets a value in member x_offset
             * @param _x_offset New value for member x_offset
             */
            eProsima_user_DllExport void x_offset(
                    uint32_t _x_offset);

            /*!
             * @brief This function returns the value of member x_offset
             * @return Value of member x_offset
             */
            eProsima_user_DllExport uint32_t x_offset() const;

            /*!
             * @brief This function returns a reference to member x_offset
             * @return Reference to member x_offset
             */
            eProsima_user_DllExport uint32_t& x_offset();

            /*!
             * @brief This function sets a value in member y_offset
             * @param _y_offset New value for member y_offset
             */
            eProsima_user_DllExport void y_offset(
                    uint32_t _y_offset);

            /*!
             * @brief This function returns the value of member y_offset
             * @return Value of member y_offset
             */
            eProsima_user_DllExport uint32_t y_offset() const;

            /*!
             * @brief This function returns a reference to member y_offset
             * @return Reference to member y_offset
             */
            eProsima_user_DllExport uint32_t& y_offset();

            /*!
             * @brief This function sets a value in member height
             * @param _height New value for member height
             */
            eProsima_user_DllExport void height(
                    uint32_t _height);

            /*!
             * @brief This function returns the value of member height
             * @return Value of member height
             */
            eProsima_user_DllExport uint32_t height() const;

            /*!
             * @brief This function returns a reference to member height
             * @return Reference to member height
             */
            eProsima_user_DllExport uint32_t& height();

            /*!
             * @brief This function sets a value in member width
             * @param _width New value for member width
             */
            eProsima_user_DllExport void width(
                    uint32_t _width);

            /*!
             * @brief This function returns the value of member width
             * @return Value of member width
             */
            eProsima_user_DllExport uint32_t width() const;

            /*!
             * @brief This function returns a reference to member width
             * @return Reference to member width
             */
            eProsima_user_DllExport uint32_t& width();

            /*!
             * @brief This function sets a value in member do_rectify
             * @param _do_rectify New value for member do_rectify
             */
            eProsima_user_DllExport void do_rectify(
                    bool _do_rectify);

            /*!
             * @brief This function returns the value of member do_rectify
             * @return Value of member do_rectify
             */
            eProsima_user_DllExport bool do_rectify() const;

            /*!
             * @brief This function returns a reference to member do_rectify
             * @return Reference to member do_rectify
             */
            eProsima_user_DllExport bool& do_rectify();


            /*!
             * @brief This function returns the maximum serialized size of an object
             * depending on the buffer alignment.
             * @param current_alignment Buffer alignment.
             * @return Maximum serialized size.
             */
            eProsima_user_DllExport static size_t getMaxCdrSerializedSize(
                    size_t current_alignment = 0);

            /*!
             * @brief This function returns the serialized size of a data depending on the buffer alignment.
             * @param data Data which is calculated its serialized size.
             * @param current_alignment Buffer alignment.
             * @return Serialized size.
             */
            eProsima_user_DllExport static size_t getCdrSerializedSize(
                    const sensor_msgs::msg::RegionOfInterest& data,
                    size_t current_alignment = 0);


            /*!
             * @brief This function serializes an object using CDR serialization.
             * @param cdr CDR serialization object.
             */
            eProsima_user_DllExport void serialize(
                    eprosima::fastcdr::Cdr& cdr) const;

            /*!
             * @brief This function deserializes an object using CDR serialization.
             * @param cdr CDR serialization object.
             */
            eProsima_user_DllExport void deserialize(
                    eprosima::fastcdr::Cdr& cdr);



            /*!
             * @brief This function returns the maximum serialized size of the Key of an object
             * depending on the buffer alignment.
             * @param current_alignment Buffer alignment.
             * @return Maximum serialized size.
             */
            eProsima_user_DllExport static size_t getKeyMaxCdrSerializedSize(
                    size_t current_alignment = 0);

            /*!
             * @brief This function tells you if the Key has been defined for this type
             */
            eProsima_user_DllExport static bool isKeyDefined();

            /*!
             * @brief This function serializes the key members of an object using CDR serialization.
             * @param cdr CDR serialization object.
             */
            eProsima_user_DllExport void serializeKey(
                    eprosima::fastcdr::Cdr& cdr) const;

        private:

            uint32_t m_x_offset;
            uint32_t m_y_offset;
            uint32_t m_height;
            uint32_t m_width;
            bool m_do_rectify;
        };
    } // namespace msg
} // namespace sensor_msgs

#endif // _SENSOR_MSGS_MSG_REGIONOFINTEREST_H_