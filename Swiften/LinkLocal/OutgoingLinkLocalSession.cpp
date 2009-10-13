#include "Swiften/LinkLocal/OutgoingLinkLocalSession.h"

#include <boost/bind.hpp>

#include "Swiften/StreamStack/XMPPLayer.h"
#include "Swiften/Elements/ProtocolHeader.h"
#include "Swiften/Elements/StreamFeatures.h"
#include "Swiften/Elements/IQ.h"

namespace Swift {

OutgoingLinkLocalSession::OutgoingLinkLocalSession(
		const JID& localJID,
		const JID& remoteJID,
		boost::shared_ptr<Connection> connection,
		PayloadParserFactoryCollection* payloadParserFactories, 
		PayloadSerializerCollection* payloadSerializers) :
			Session(connection, payloadParserFactories, payloadSerializers) {
	setLocalJID(localJID);
	setRemoteJID(remoteJID);
}

void OutgoingLinkLocalSession::handleSessionStarted() {
	ProtocolHeader header;
	header.setFrom(getLocalJID());
	getXMPPLayer()->writeHeader(header);
}

void OutgoingLinkLocalSession::handleStreamStart(const ProtocolHeader&) {
	foreach(const boost::shared_ptr<Element>& stanza, queuedElements_) {
		sendElement(stanza);
	}
	queuedElements_.clear();
}

void OutgoingLinkLocalSession::handleElement(boost::shared_ptr<Element> element) {
	onElementReceived(element);
}

void OutgoingLinkLocalSession::queueElement(boost::shared_ptr<Element> element) {
	queuedElements_.push_back(element);
}


}
