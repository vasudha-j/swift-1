/*
 * Copyright (c) 2010-2013 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include <Swiften/Client/MemoryStorages.h>
#include <Swiften/VCards/VCardMemoryStorage.h>
#include <Swiften/Avatars/AvatarMemoryStorage.h>
#include <Swiften/Disco/CapsMemoryStorage.h>
#include <Swiften/Roster/RosterMemoryStorage.h>
#include <Swiften/History/SQLiteHistoryStorage.h>

namespace Swift {

MemoryStorages::MemoryStorages(CryptoProvider* crypto) {
	vcardStorage = new VCardMemoryStorage(crypto);
	capsStorage = new CapsMemoryStorage();
	avatarStorage = new AvatarMemoryStorage();
	rosterStorage = new RosterMemoryStorage();
#ifdef SWIFT_EXPERIMENTAL_HISTORY
	historyStorage = new SQLiteHistoryStorage(":memory:");
#else
	historyStorage = NULL;
#endif
}

MemoryStorages::~MemoryStorages() {
	delete rosterStorage;
	delete avatarStorage;
	delete capsStorage;
	delete vcardStorage;
	delete historyStorage;
}

VCardStorage* MemoryStorages::getVCardStorage() const {
	return vcardStorage;
}

CapsStorage* MemoryStorages::getCapsStorage() const {
	return capsStorage;
}

AvatarStorage* MemoryStorages::getAvatarStorage() const {
	return avatarStorage;
}

RosterStorage* MemoryStorages::getRosterStorage() const {
	return rosterStorage;
}

HistoryStorage* MemoryStorages::getHistoryStorage() const {
#ifdef SWIFT_EXPERIMENTAL_HISTORY
	return historyStorage;
#else
	return NULL;
#endif
}


}
