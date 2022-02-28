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

/**
 * @file FastddsTesterPublisher.cpp
 *
 */

#include "FastddsTesterPublisher.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/Domain.h>
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicTypeBuilder.h>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicType.h>

#include <thread>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using namespace eprosima::fastrtps::types;

FastddsTesterPublisher::FastddsTesterPublisher()
    : mp_participant(nullptr)
    , mp_publisher(nullptr)
    , m_DynType(DynamicType_ptr(nullptr))
{
}

uint32_t txmsgs=1;

bool FastddsTesterPublisher::init(uint32_t size)
{
    // Create basic builders
    DynamicTypeBuilder_ptr struct_type_builder(DynamicTypeBuilderFactory::get_instance()->create_struct_builder());
    
    // each member string payload is 128 characters, i.e. 128 bytes
    // size is passed in here as KB, create multiple string members per input size
    txmsgs = 8*size; 
        
    // Add members to the struct.
    struct_type_builder->add_member(0, "index", DynamicTypeBuilderFactory::get_instance()->create_uint32_type());
    struct_type_builder->add_member(1, "timestamp", DynamicTypeBuilderFactory::get_instance()->create_uint64_type());
    std::string member;
    for(uint32_t ii=2; ii<(2+txmsgs); ii++)
    {
    	member="message";
    	member+=std::to_string(ii);
    	struct_type_builder->add_member(ii, member, DynamicTypeBuilderFactory::get_instance()->create_string_type());
    }
    	
    struct_type_builder->set_name("FastddsTester");

    DynamicType_ptr dynType = struct_type_builder->build();
    m_DynType.SetDynamicType(dynType);
    m_DynHello = DynamicDataFactory::get_instance()->create_data(dynType);
    
    // Set values in our payload
    m_DynHello->set_uint32_value(0, 0);
    m_DynHello->set_uint64_value(0, 1);
    std::string tx_message;
    for(uint32_t ii=2; ii<(2+txmsgs); ii++)
    {
    	tx_message="abcdefghijklmnopqrstuvwxyz012345abcdefghijklmnopqrstuvwxyz012345abcdefghijklmnopqrstuvwxyz012345abcdefghijklmnopqrstuvwxyz012345";
    	m_DynHello->set_string_value(tx_message, ii);
    }

    ParticipantAttributes PParam;
    PParam.rtps.setName("DynFastddsTester_pub");
    mp_participant = Domain::createParticipant(PParam, (ParticipantListener*)&m_part_list);

    if (mp_participant == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    Domain::registerDynamicType(mp_participant, &m_DynType);

    //CREATE THE PUBLISHER
    PublisherAttributes Wparam;
    Wparam.topic.topicKind = NO_KEY;
    Wparam.topic.topicDataType = "FastddsTester";
    Wparam.topic.topicName = "FastddsTesterTopic";
    mp_publisher = Domain::createPublisher(mp_participant, Wparam, (PublisherListener*)&m_listener);
    if (mp_publisher == nullptr)
    {
        return false;
    }

    return true;

}

FastddsTesterPublisher::~FastddsTesterPublisher()
{
    Domain::removeParticipant(mp_participant);

    DynamicDataFactory::get_instance()->delete_data(m_DynHello);

    Domain::stopAll();
}

void FastddsTesterPublisher::PubListener::onPublicationMatched(
        Publisher* /*pub*/,
        MatchingInfo& info)
{
    if (info.status == MATCHED_MATCHING)
    {
        n_matched++;
        firstConnected = true;
        std::cout << "Publisher matched" << std::endl;
    }
    else
    {
        n_matched--;
        std::cout << "Publisher unmatched" << std::endl;
    }
}

void FastddsTesterPublisher::PartListener::onParticipantDiscovery(
        Participant*,
        ParticipantDiscoveryInfo&& info)
{
    if (info.status == ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
    {
        std::cout << "Participant " << info.info.m_participantName << " discovered" << std::endl;
    }
    else if (info.status == ParticipantDiscoveryInfo::REMOVED_PARTICIPANT)
    {
        std::cout << "Participant " << info.info.m_participantName << " removed" << std::endl;
    }
    else if (info.status == ParticipantDiscoveryInfo::DROPPED_PARTICIPANT)
    {
        std::cout << "Participant " << info.info.m_participantName << " dropped" << std::endl;
    }
}

void FastddsTesterPublisher::runThread(
        uint32_t samples,
        uint32_t sleep)
{
    uint32_t i = 0;

    while (!stop && (i < samples || samples == 0))
    {
    	auto now = std::chrono::high_resolution_clock::now();
    	auto epoch = now.time_since_epoch();
    	uint64_t tx_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
    	m_DynHello->set_uint64_value(tx_timestamp, 1);
            
        if (publish(samples != 0))
        {
            uint32_t index;
            m_DynHello->get_uint32_value(index, 0);
            
            std::cout << "Index: " << index << " SENT at: " << tx_timestamp << "[ns]" << " with " << txmsgs << " members --->" << std::endl;
            /*
            std::cout << "Message(s) ----->" << std::endl;
            std::string tx_message;
            for(uint32_t ii=2; ii<(2+txmsgs); ii++)
            {
            	m_DynHello->get_string_value(tx_message, ii);
            	std::cout << tx_message << std::endl;
            }
            std::cout << "<-----" << std::endl;
            */
            
            ++i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
    }
}

void FastddsTesterPublisher::run(
        uint32_t samples,
        uint32_t sleep)
{
    stop = false;
    std::thread thread(&FastddsTesterPublisher::runThread, this, samples, sleep);
    if (samples == 0)
    {
        std::cout << "Publisher running. Please press enter to stop the Publisher at any time." << std::endl;
        std::cin.ignore();
        stop = true;
    }
    else
    {
        std::cout << "Publisher running " << samples << " samples." << std::endl;
    }
    thread.join();
}

bool FastddsTesterPublisher::publish(
        bool waitForListener)
{
    if (m_listener.firstConnected || !waitForListener || m_listener.n_matched > 0)
    {
        uint32_t index;
        m_DynHello->get_uint32_value(index, 0);
        m_DynHello->set_uint32_value(index + 1, 0);
        mp_publisher->write((void*)m_DynHello);
        return true;
    }
    return false;
}
