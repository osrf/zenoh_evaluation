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
 * @file Empty.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _STD_MSGS_MSG_EMPTY_H_
#define _STD_MSGS_MSG_EMPTY_H_


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
#if defined(Empty_SOURCE)
#define Empty_DllAPI __declspec( dllexport )
#else
#define Empty_DllAPI __declspec( dllimport )
#endif // Empty_SOURCE
#else
#define Empty_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define Empty_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


namespace std_msgs {
    namespace msg {
        /*!
         * @brief This class represents the structure Empty defined by the user in the IDL file.
         * @ingroup EMPTY
         */
        class Empty
        {
        public:

            /*!
             * @brief Default constructor.
             */
            eProsima_user_DllExport Empty();

            /*!
             * @brief Default destructor.
             */
            eProsima_user_DllExport ~Empty();

            /*!
             * @brief Copy constructor.
             * @param x Reference to the object std_msgs::msg::Empty that will be copied.
             */
            eProsima_user_DllExport Empty(
                    const Empty& x);

            /*!
             * @brief Move constructor.
             * @param x Reference to the object std_msgs::msg::Empty that will be copied.
             */
            eProsima_user_DllExport Empty(
                    Empty&& x);

            /*!
             * @brief Copy assignment.
             * @param x Reference to the object std_msgs::msg::Empty that will be copied.
             */
            eProsima_user_DllExport Empty& operator =(
                    const Empty& x);

            /*!
             * @brief Move assignment.
             * @param x Reference to the object std_msgs::msg::Empty that will be copied.
             */
            eProsima_user_DllExport Empty& operator =(
                    Empty&& x);

            /*!
             * @brief Comparison operator.
             * @param x std_msgs::msg::Empty object to compare.
             */
            eProsima_user_DllExport bool operator ==(
                    const Empty& x);

            /*!
             * @brief Comparison operator.
             * @param x std_msgs::msg::Empty object to compare.
             */
            eProsima_user_DllExport bool operator !=(
                    const Empty& x);

            /*!
             * @brief This function sets a value in member dummy
             * @param _dummy New value for member dummy
             */
            eProsima_user_DllExport void dummy(
                    bool _dummy);

            /*!
             * @brief This function returns the value of member dummy
             * @return Value of member dummy
             */
            eProsima_user_DllExport bool dummy() const;

            /*!
             * @brief This function returns a reference to member dummy
             * @return Reference to member dummy
             */
            eProsima_user_DllExport bool& dummy();


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
                    const std_msgs::msg::Empty& data,
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

            bool m_dummy;
        };
    } // namespace msg
} // namespace std_msgs

#endif // _STD_MSGS_MSG_EMPTY_H_