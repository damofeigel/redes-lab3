#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>

#include "FeedBackPacket_m.h"

using namespace omnetpp;

class TransportRx: public cSimpleModule {
private:
    cQueue buffer;
    cQueue bufferFeedPacket;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
public:
    TransportRx();
    virtual ~TransportRx();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
}

void TransportRx::initialize() {
    buffer.setName("buffer");
    bufferFeedPacket.setName("bufferFeedPacket");
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("Buffer Size");
    packetDropVector.setName("Packet drops");
}

void TransportRx::finish() {
}

void TransportRx::handleMessage(cMessage *msg) {

    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toOut$o");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);

            // Send FeedBackPacket with ACK False to queue2
            FeedBackPacket *feedBackPkt = new FeedBackPacket("packet");
            feedBackPkt->setWaitingAck(false);
            feedBackPkt->setKind(2);
            send(feedBackPkt, "toApp");
        }

    // check buffer limit
    } else if (buffer.getLength() >= par("bufferSize").intValue()) {
        // drop the packet
        delete msg;
        this->bubble("packet dropped");
        packetDropVector.record(1);
    } else {
        // enqueue the packet
        buffer.insert(msg);
        bufferSizeVector.record(buffer.getLength());

        // if the server is idle
        if (!endServiceEvent->isScheduled()) {
            // start the service now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }

}

#endif /* TransportRx */
