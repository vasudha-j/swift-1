/*
 * Copyright (c) 2010-2016 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

//...
#include <iostream>
#include <memory>

#include <boost/bind.hpp>

#include <Swiften/Swiften.h>

using namespace Swift;
using namespace boost;
//...
#include "EchoPayload.h"
#include "EchoPayloadParserFactory.h"
#include "EchoPayloadSerializer.h"

class EchoBot {
    public:
        EchoBot(NetworkFactories* networkFactories) {
            //...
            client = new Client("echobot@wonderland.lit", "mypass", networkFactories);
            client->setAlwaysTrustCertificates();
            client->onConnected.connect(bind(&EchoBot::handleConnected, this));
            client->onMessageReceived.connect(
                    bind(&EchoBot::handleMessageReceived, this, _1));
            client->onPresenceReceived.connect(
                    bind(&EchoBot::handlePresenceReceived, this, _1));
            tracer = new ClientXMLTracer(client);

            softwareVersionResponder = new SoftwareVersionResponder(client->getIQRouter());
            softwareVersionResponder->setVersion("EchoBot", "1.0");
            softwareVersionResponder->start();
            //...
            client->addPayloadParserFactory(&echoPayloadParserFactory);
            client->addPayloadSerializer(&echoPayloadSerializer);
            //...
            client->connect();
            //...
        }

        ~EchoBot() {
            client->removePayloadSerializer(&echoPayloadSerializer);
            client->removePayloadParserFactory(&echoPayloadParserFactory);
            //...
            softwareVersionResponder->stop();
            delete softwareVersionResponder;
            delete tracer;
            delete client;
            //...
        }
        //...

    private:
        void handlePresenceReceived(Presence::ref presence) {
            // Automatically approve subscription requests
            if (presence->getType() == Presence::Subscribe) {
                Presence::ref response = Presence::create();
                response->setTo(presence->getFrom());
                response->setType(Presence::Subscribed);
                client->sendPresence(response);
            }
        }

        void handleConnected() {
            // Request the roster
            GetRosterRequest::ref rosterRequest =
                    GetRosterRequest::create(client->getIQRouter());
            rosterRequest->onResponse.connect(
                    bind(&EchoBot::handleRosterReceived, this, _2));
            rosterRequest->send();
        }

        void handleRosterReceived(ErrorPayload::ref error) {
            if (error) {
                std::cerr << "Error receiving roster. Continuing anyway.";
            }
            // Send initial available presence
            client->sendPresence(Presence::create("Send me a message"));
        }

        //...
        void handleMessageReceived(Message::ref message) {
            //...
            // Echo back the incoming message
            message->setTo(message->getFrom());
            message->setFrom(JID());
            //...
            if (!message->getPayload<EchoPayload>()) {
                std::shared_ptr<EchoPayload> echoPayload = std::make_shared<EchoPayload>();
                echoPayload->setMessage("This is an echoed message");
                message->addPayload(echoPayload);
                client->sendMessage(message);
            }
        }
        //...

        //...
    private:
        //...
        Client* client;
        ClientXMLTracer* tracer;
        SoftwareVersionResponder* softwareVersionResponder;
        //...
        EchoPayloadParserFactory echoPayloadParserFactory;
        EchoPayloadSerializer echoPayloadSerializer;
};
//...

int main(int, char**) {
    SimpleEventLoop eventLoop;
    BoostNetworkFactories networkFactories(&eventLoop);

    EchoBot bot(&networkFactories);

    eventLoop.run();
    return 0;
}
//...
