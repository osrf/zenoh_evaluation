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
 * @file ColorRGBA.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _STD_MSGS_MSG_COLORRGBA_H_
#define _STD_MSGS_MSG_COLORRGBA_H_


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
#if defined(ColorRGBA_SOURCE)
#define ColorRGBA_DllAPI __declspec( dllexport )
#else
#define ColorRGBA_DllAPI __declspec( dllimport )
#endif // ColorRGBA_SOURCE
#else
#define ColorRGBA_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ColorRGBA_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


namespace std_msgs {
    namespace msg {
        /*!
         * @brief This class represents the structure ColorRGBA defined by the user in the IDL file.
         * @ingroup COLORRGBA
         */
        class ColorRGBA
        {
        public:

            /*!
             * @brief Default constructor.
             */
            eProsima_user_DllExport ColorRGBA();

            /*!
             * @brief Default destructor.
             */
            eProsima_user_DllExport ~ColorRGBA();

            /*!
             * @brief Copy constructor.
             * @param x Reference to the object std_msgs::msg::ColorRGBA that will be copied.
             */
            eProsima_user_DllExport ColorRGBA(
                    const ColorRGBA& x);

            /*!
             * @brief Move constructor.
             * @param x Reference to the object std_msgs::msg::ColorRGBA that will be copied.
             */
            eProsima_user_DllExport ColorRGBA(
                    ColorRGBA&& x);

            /*!
             * @brief Copy assignment.
             * @param x Reference to the object std_msgs::msg::ColorRGBA that will be copied.
             */
            eProsima_user_DllExport ColorRGBA& operator =(
                    const ColorRGBA& x);

            /*!
             * @brief Move assignment.
             * @param x Reference to the object std_msgs::msg::ColorRGBA that will be copied.
             */
            eProsima_user_DllExport ColorRGBA& operator =(
                    ColorRGBA&& x);

            /*!
             * @brief Comparison operator.
             * @param x std_msgs::msg::ColorRGBA object to compare.
             */
            eProsima_user_DllExport bool operator ==(
                    const ColorRGBA& x);

            /*!
             * @brief Comparison operator.
             * @param x std_msgs::msg::ColorRGBA object to compare.
             */
            eProsima_user_DllExport bool operator !=(
                    const ColorRGBA& x);

            /*!
             * @brief This function sets a value in member r
             * @param _r New value for member r
             */
            eProsima_user_DllExport void r(
                    float _r);

            /*!
             * @brief This function returns the value of member r
             * @return Value of member r
             */
            eProsima_user_DllExport float r() const;

            /*!
             * @brief This function returns a reference to member r
             * @return Reference to member r
             */
            eProsima_user_DllExport float& r();

            /*!
             * @brief This function sets a value in member g
             * @param _g New value for member g
             */
            eProsima_user_DllExport void g(
                    float _g);

            /*!
             * @brief This function returns the value of member g
             * @return Value of member g
             */
            eProsima_user_DllExport float g() const;

            /*!
             * @brief This function returns a reference to member g
             * @return Reference to member g
             */
            eProsima_user_DllExport float& g();

            /*!
             * @brief This function sets a value in member b
             * @param _b New value for member b
             */
            eProsima_user_DllExport void b(
                    float _b);

            /*!
             * @brief This function returns the value of member b
             * @return Value of member b
             */
            eProsima_user_DllExport float b() const;

            /*!
             * @brief This function returns a reference to member b
             * @return Reference to member b
             */
            eProsima_user_DllExport float& b();

            /*!
             * @brief This function sets a value in member a
             * @param _a New value for member a
             */
            eProsima_user_DllExport void a(
                    float _a);

            /*!
             * @brief This function returns the value of member a
             * @return Value of member a
             */
            eProsima_user_DllExport float a() const;

            /*!
             * @brief This function returns a reference to member a
             * @return Reference to member a
             */
            eProsima_user_DllExport float& a();


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
                    const std_msgs::msg::ColorRGBA& data,
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

            float m_r;
            float m_g;
            float m_b;
            float m_a;
        };
    } // namespace msg
} // namespace std_msgs

#endif // _STD_MSGS_MSG_COLORRGBA_H_