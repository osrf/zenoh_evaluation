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
 * @file datatypesPubSubTypes.h
 * This header file contains the declaration of the serialization functions.
 *
 * This file was generated by the tool fastcdrgen.
 */


#ifndef _DATATYPES_PUBSUBTYPES_H_
#define _DATATYPES_PUBSUBTYPES_H_

#include <fastdds/dds/topic/TopicDataType.hpp>
#include <fastrtps/utils/md5.h>

#include "datatypes.h"

#if !defined(GEN_API_VER) || (GEN_API_VER != 1)
#error Generated datatypes is not compatible with current installed Fast DDS. Please, regenerate it with fastddsgen.
#endif  // GEN_API_VER

/*!
 * @brief This class represents the TopicDataType of the type Int32 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Int32PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Int32 type;

    eProsima_user_DllExport Int32PubSubType();

    eProsima_user_DllExport virtual ~Int32PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Int32();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Int64 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Int64PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Int64 type;

    eProsima_user_DllExport Int64PubSubType();

    eProsima_user_DllExport virtual ~Int64PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Int64();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Float32 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Float32PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Float32 type;

    eProsima_user_DllExport Float32PubSubType();

    eProsima_user_DllExport virtual ~Float32PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Float32();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Float64 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Float64PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Float64 type;

    eProsima_user_DllExport Float64PubSubType();

    eProsima_user_DllExport virtual ~Float64PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Float64();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type String defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class StringPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef String type;

    eProsima_user_DllExport StringPubSubType();

    eProsima_user_DllExport virtual ~StringPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Timestamp defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class TimestampPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Timestamp type;

    eProsima_user_DllExport TimestampPubSubType();

    eProsima_user_DllExport virtual ~TimestampPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Timestamp();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Header defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class HeaderPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Header type;

    eProsima_user_DllExport HeaderPubSubType();

    eProsima_user_DllExport virtual ~HeaderPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Point defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class PointPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Point type;

    eProsima_user_DllExport PointPubSubType();

    eProsima_user_DllExport virtual ~PointPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Point();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Quaternion defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class QuaternionPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Quaternion type;

    eProsima_user_DllExport QuaternionPubSubType();

    eProsima_user_DllExport virtual ~QuaternionPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Quaternion();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Vector3 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Vector3PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Vector3 type;

    eProsima_user_DllExport Vector3PubSubType();

    eProsima_user_DllExport virtual ~Vector3PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Vector3();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Vector3Stamped defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class Vector3StampedPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Vector3Stamped type;

    eProsima_user_DllExport Vector3StampedPubSubType();

    eProsima_user_DllExport virtual ~Vector3StampedPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Pose defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class PosePubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Pose type;

    eProsima_user_DllExport PosePubSubType();

    eProsima_user_DllExport virtual ~PosePubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Pose();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Twist defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class TwistPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Twist type;

    eProsima_user_DllExport TwistPubSubType();

    eProsima_user_DllExport virtual ~TwistPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Twist();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type TwistWithCovariance defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class TwistWithCovariancePubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef TwistWithCovariance type;

    eProsima_user_DllExport TwistWithCovariancePubSubType();

    eProsima_user_DllExport virtual ~TwistWithCovariancePubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type TwistWithCovarianceStamped defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class TwistWithCovarianceStampedPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef TwistWithCovarianceStamped type;

    eProsima_user_DllExport TwistWithCovarianceStampedPubSubType();

    eProsima_user_DllExport virtual ~TwistWithCovarianceStampedPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Wrench defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class WrenchPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Wrench type;

    eProsima_user_DllExport WrenchPubSubType();

    eProsima_user_DllExport virtual ~WrenchPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        new (memory) Wrench();
        return true;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type WrenchStamped defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class WrenchStampedPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef WrenchStamped type;

    eProsima_user_DllExport WrenchStampedPubSubType();

    eProsima_user_DllExport virtual ~WrenchStampedPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type Image defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class ImagePubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef Image type;

    eProsima_user_DllExport ImagePubSubType();

    eProsima_user_DllExport virtual ~ImagePubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};








/*!
 * @brief This class represents the TopicDataType of the type PointField defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class PointFieldPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef PointField type;

    eProsima_user_DllExport PointFieldPubSubType();

    eProsima_user_DllExport virtual ~PointFieldPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type PointCloud2 defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class PointCloud2PubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef PointCloud2 type;

    eProsima_user_DllExport PointCloud2PubSubType();

    eProsima_user_DllExport virtual ~PointCloud2PubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};
/*!
 * @brief This class represents the TopicDataType of the type LaserScan defined by the user in the IDL file.
 * @ingroup DATATYPES
 */
class LaserScanPubSubType : public eprosima::fastdds::dds::TopicDataType
{
public:

    typedef LaserScan type;

    eProsima_user_DllExport LaserScanPubSubType();

    eProsima_user_DllExport virtual ~LaserScanPubSubType();

    eProsima_user_DllExport virtual bool serialize(
            void* data,
            eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;

    eProsima_user_DllExport virtual bool deserialize(
            eprosima::fastrtps::rtps::SerializedPayload_t* payload,
            void* data) override;

    eProsima_user_DllExport virtual std::function<uint32_t()> getSerializedSizeProvider(
            void* data) override;

    eProsima_user_DllExport virtual bool getKey(
            void* data,
            eprosima::fastrtps::rtps::InstanceHandle_t* ihandle,
            bool force_md5 = false) override;

    eProsima_user_DllExport virtual void* createData() override;

    eProsima_user_DllExport virtual void deleteData(
            void* data) override;

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED
    eProsima_user_DllExport inline bool is_bounded() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_BOUNDED

#ifdef TOPIC_DATA_TYPE_API_HAS_IS_PLAIN
    eProsima_user_DllExport inline bool is_plain() const override
    {
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_IS_PLAIN

#ifdef TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE
    eProsima_user_DllExport inline bool construct_sample(
            void* memory) const override
    {
        (void)memory;
        return false;
    }

#endif  // TOPIC_DATA_TYPE_API_HAS_CONSTRUCT_SAMPLE

    MD5 m_md5;
    unsigned char* m_keyBuffer;
};

#endif // _DATATYPES_PUBSUBTYPES_H_