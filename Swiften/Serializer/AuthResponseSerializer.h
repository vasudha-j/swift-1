/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>

#include <Swiften/Base/API.h>
#include <Swiften/Elements/AuthResponse.h>
#include <Swiften/Serializer/GenericElementSerializer.h>

namespace Swift {
	class SWIFTEN_API AuthResponseSerializer : public GenericElementSerializer<AuthResponse> {
		public:
			AuthResponseSerializer();

			virtual SafeByteArray serialize(boost::shared_ptr<Element> element)  const;
	};
}
