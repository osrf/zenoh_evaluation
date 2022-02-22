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
 * @file FastddsTesterSubscriber.cpp
 *
 */

#include "FastddsTesterSubscriber.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/Domain.h>
#include <fastrtps/types/DynamicTypeBuilderFactory.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicTypeBuilder.h>
#include <fastrtps/types/DynamicTypeBuilderPtr.h>
#include <fastrtps/types/DynamicType.h>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;
using namespace eprosima::fastrtps::types;

FastddsTesterSubscriber::FastddsTesterSubscriber()
    : mp_participant(nullptr)
    , mp_subscriber(nullptr)
    , m_DynType(DynamicType_ptr(nullptr))
{
}

uint32_t rxmsgs=1;

bool FastddsTesterSubscriber::init(uint32_t size)
{
    ParticipantAttributes PParam;
    PParam.rtps.setName("DynFastddsTester_sub");
    mp_participant = Domain::createParticipant(PParam, (ParticipantListener*)&m_part_list);
    if (mp_participant == nullptr)
    {
        return false;
    }
    
    // each member string payload is 128 characters, i.e. 128 bytes
    // size is passed in here as KB, create multiple string members per input size
    rxmsgs = 8*size; 
    
    //  Create basic types and add members to the struct.
    DynamicTypeBuilder_ptr created_type_ulong = DynamicTypeBuilderFactory::get_instance()->create_uint32_builder();
    DynamicTypeBuilder_ptr created_type_uint64 = DynamicTypeBuilderFactory::get_instance()->create_uint64_builder();
    DynamicTypeBuilder_ptr created_type_string = DynamicTypeBuilderFactory::get_instance()->create_string_builder();
    DynamicTypeBuilder_ptr struct_type_builder = DynamicTypeBuilderFactory::get_instance()->create_struct_builder();
    struct_type_builder->add_member(0, "index", created_type_ulong.get());
    struct_type_builder->add_member(1, "timestamp", created_type_uint64.get());
    std::string member;
    for(uint32_t ii=2; ii<(2+rxmsgs); ii++)
    {
    	member="message";
    	member+=std::to_string(ii);
    	struct_type_builder->add_member(ii, member, DynamicTypeBuilderFactory::get_instance()->create_string_type());
    }
    
    struct_type_builder->set_name("FastddsTester");
    DynamicType_ptr dynType = struct_type_builder->build();
    m_DynType.SetDynamicType(dynType);
    m_listener.m_DynHello = DynamicDataFactory::get_instance()->create_data(dynType);

    //REGISTER THE TYPE
    Domain::registerDynamicType(mp_participant, &m_DynType);

    //CREATE THE SUBSCRIBER
    SubscriberAttributes Rparam;
    Rparam.topic.topicKind = NO_KEY;
    Rparam.topic.topicDataType = "FastddsTester";
    Rparam.topic.topicName = "FastddsTesterTopic";

    mp_subscriber = Domain::createSubscriber(mp_participant, Rparam, (SubscriberListener*)&m_listener);

    if (mp_subscriber == nullptr)
    {
        return false;
    }


    return true;
}

FastddsTesterSubscriber::~FastddsTesterSubscriber()
{
    Domain::removeParticipant(mp_participant);

    DynamicDataFactory::get_instance()->delete_data(m_listener.m_DynHello);

    Domain::stopAll();
}

void FastddsTesterSubscriber::SubListener::onSubscriptionMatched(
        Subscriber* /*sub*/,
        MatchingInfo& info)
{
    if (info.status == MATCHED_MATCHING)
    {
        n_matched++;
        std::cout << "Subscriber matched" << std::endl;
        std::cout << "Rx at (ns), Tx time (ns), Transmitted (Bytes), Rate (B/s), Bandwidth (bps), Bandwidth (Mbps)" << std::endl;
    }
    else
    {
        n_matched--;
        std::cout << "Subscriber unmatched" << std::endl;
    }
}

void FastddsTesterSubscriber::PartListener::onParticipantDiscovery(
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

void FastddsTesterSubscriber::SubListener::onNewDataMessage(
        Subscriber* sub)
{

	auto now = std::chrono::high_resolution_clock::now();
        auto epoch = now.time_since_epoch();
        uint64_t rx_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
        
    if (sub->takeNextData((void*)m_DynHello, &m_info))
    {           
        if (m_info.sampleKind == ALIVE)
        {
            this->n_samples++;
            // Print your structure data here.
            uint32_t index;
            m_DynHello->get_uint32_value(index, 0);
            uint64_t tx_timestamp;
            m_DynHello->get_uint64_value(tx_timestamp, 1);
            
            //std::cout << "----" << std::endl;            
            //std::cout << "New message with index: " << index << " received " << rxmsgs << " members" << std::endl;
            
            std::string rx_message;
            m_DynHello->get_string_value(rx_message, 2);
            uint64_t message_size = rx_message.size() * rxmsgs;
            
            uint64_t tdiff = rx_timestamp - tx_timestamp;
            uint64_t data_rate = message_size*((1000*1000*1000)/tdiff);
            /*
            std::cout << "Tx at: " << tx_timestamp << "[ns], Rx at: " << rx_timestamp << "[ns]" << " Diff: "<< tdiff << std::endl;
            std::cout << "Payload size: " << message_size << " [B], " << (float) message_size/1024 << " [KB]" << std::endl;
            std::cout << "Datarate: " << data_rate << "[B/s], " << (float) data_rate/(1024*1024) << "[MB/s]" << std::endl;
            
            std::cout << "Message(s) ----->" << std::endl;
            for(uint32_t ii=2; ii<(2+rxmsgs); ii++)
            {
            	m_DynHello->get_string_value(rx_message, ii);
            	std::cout << rx_message << std::endl;
            }
            std::cout << "<-----" << std::endl;
            */

            std::cout << rx_timestamp << "," << tdiff << "," << message_size << "," << data_rate << "," << data_rate*8 << "," << (float) (data_rate*8)/(1024*1024) << std::endl;
            
            
        }
    }
}

void FastddsTesterSubscriber::run()
{
    std::cout << "Subscriber running. Please press enter to stop the Subscriber" << std::endl;
    std::cin.ignore();
}

void FastddsTesterSubscriber::run(
        uint32_t number)
{
    std::cout << "Subscriber running until " << number << "samples have been received" << std::endl;
    while (number > this->m_listener.n_samples)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
