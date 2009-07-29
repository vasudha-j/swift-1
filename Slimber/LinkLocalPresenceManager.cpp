#include "Slimber/LinkLocalPresenceManager.h"

#include <boost/bind.hpp>

#include "Swiften/Base/foreach.h"
#include "Swiften/LinkLocal/LinkLocalServiceBrowser.h"
#include "Swiften/Elements/RosterPayload.h"
#include "Swiften/Elements/Presence.h"

namespace Swift {

LinkLocalPresenceManager::LinkLocalPresenceManager(LinkLocalServiceBrowser* browser) : browser(browser) {
	browser->onServiceAdded.connect(
			boost::bind(&LinkLocalPresenceManager::handleServiceAdded, this, _1));
	browser->onServiceChanged.connect(
			boost::bind(&LinkLocalPresenceManager::handleServiceChanged, this, _1));
	browser->onServiceRemoved.connect(
			boost::bind(&LinkLocalPresenceManager::handleServiceRemoved, this, _1));
}

void LinkLocalPresenceManager::handleServiceAdded(const LinkLocalService& service) {
	boost::shared_ptr<RosterPayload> roster(new RosterPayload());
	roster->addItem(getRosterItem(service));
	onRosterChanged(roster);
	onPresenceChanged(getPresence(service));
}

void LinkLocalPresenceManager::handleServiceChanged(const LinkLocalService& service) {
	onPresenceChanged(getPresence(service));
}

void LinkLocalPresenceManager::handleServiceRemoved(const LinkLocalService& service) {
	boost::shared_ptr<RosterPayload> roster(new RosterPayload());
	roster->addItem(RosterItemPayload(getJIDForService(service), "", RosterItemPayload::Remove));
	onRosterChanged(roster);
}

boost::shared_ptr<RosterPayload> LinkLocalPresenceManager::getRoster() const {
	boost::shared_ptr<RosterPayload> roster(new RosterPayload());
	foreach(const LinkLocalService& service, browser->getServices()) {
		roster->addItem(getRosterItem(service));
	}
	return roster;
}

std::vector<boost::shared_ptr<Presence> > LinkLocalPresenceManager::getAllPresence() const {
	std::vector<boost::shared_ptr<Presence> > result;
	foreach(const LinkLocalService& service, browser->getServices()) {
		result.push_back(getPresence(service));
	}
	return result;
}

RosterItemPayload LinkLocalPresenceManager::getRosterItem(const LinkLocalService& service) const {
 return RosterItemPayload(getJIDForService(service), getRosterName(service), RosterItemPayload::Both);
}

String LinkLocalPresenceManager::getRosterName(const LinkLocalService& service) const {
	LinkLocalServiceInfo info = service.getInfo();
	if (!info.getNick().isEmpty()) {
		return info.getNick();
	}
	else if (!info.getFirstName().isEmpty()) {
		String result = info.getFirstName();
		if (!info.getLastName().isEmpty()) {
			result += " " + info.getLastName();
		}
		return result;
	}
	else if (!info.getLastName().isEmpty()) {
		return info.getLastName();
	}
	return "";
}

JID LinkLocalPresenceManager::getJIDForService(const LinkLocalService& service) const {
	return JID(service.getName());
}

boost::shared_ptr<Presence> LinkLocalPresenceManager::getPresence(const LinkLocalService& service) const {
	boost::shared_ptr<Presence> presence(new Presence());
	presence->setFrom(getJIDForService(service));
	switch (service.getInfo().getStatus()) {
		case LinkLocalServiceInfo::Available:
			presence->setShow(StatusShow::Online);
			break;
		case LinkLocalServiceInfo::Away:
			presence->setShow(StatusShow::Away);
			break;
		case LinkLocalServiceInfo::DND:
			presence->setShow(StatusShow::DND);
			break;
	}
	presence->setStatus(service.getInfo().getMessage());
	return presence;
}

}
