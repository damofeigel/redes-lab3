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
    bool bufferFull;
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
    bufferFull = false;
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
        // if packet in bufferFeed
        if (!bufferFeedPacket.isEmpty()) {
            // dequeue packet
            FeedBackPacket *feedBackPkt = (FeedBackPacket*) bufferFeedPacket.pop();
            // send packet to queue2
            send(feedBackPkt, "toApp");
            // Start new service
            serviceTime = feedBackPkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);

        // if packet in buffer, send next one
        } else if (!buffer.isEmpty()) {
            // dequeue packet
            cPacket *pkt = (cPacket*) buffer.pop();
            // send packet
            send(pkt, "toOut$o");
            // start new service
            serviceTime = pkt->getDuration();
            scheduleAt(simTime() + serviceTime, endServiceEvent);
        }

    // check if is a FeedBackPacket from queue
    } else if (msg->getKind() == 2) {
        FeedBackPacket *feedBackPkt = (FeedBackPacket*) msg;

        bufferFeedPacket.insert(feedBackPkt);

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

        // If buffer is half full
        if (!bufferFull && buffer.getLength() >= par("bufferSize").intValue() / 2) {
            bufferFull = true;
            // Create FeedBackPacket to send a TraTx
            FeedBackPacket *feedBackPkt = new FeedBackPacket("packet");
            feedBackPkt->setBufferFull(true);
            feedBackPkt->setKind(2);
            bufferFeedPacket.insert(feedBackPkt);

        // If buffer is half empty and was sent a FeedBackPacket
        } else if (bufferFull && buffer.getLength() < par("bufferSize").intValue() / 2) {
            bufferFull = false;
            FeedBackPacket *feedBackPkt = new FeedBackPacket("packet");
            feedBackPkt->setBufferFull(false);
            feedBackPkt->setKind(2);
            bufferFeedPacket.insert(feedBackPkt);
        }

        // if the server is idle
        if (!endServiceEvent->isScheduled()) {
            // start the service now
            scheduleAt(simTime() + 0, endServiceEvent);
        }
    }

}

#endif /* TransportRx */
