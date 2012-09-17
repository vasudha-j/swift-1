/*
 * Copyright (c) 2011 Tobias Markmann
 * Licensed under the BSD license.
 * See http://www.opensource.org/licenses/bsd-license.php for more information.
 */

#pragma once

#include <Swiften/Serializer/GenericPayloadSerializer.h>
#include <Swiften/Elements/DeliveryReceiptRequest.h>
#include <Swiften/Base/API.h>

namespace Swift {
	class SWIFTEN_API DeliveryReceiptRequestSerializer : public GenericPayloadSerializer<DeliveryReceiptRequest> {
		public:
			DeliveryReceiptRequestSerializer();

			virtual std::string serializePayload(boost::shared_ptr<DeliveryReceiptRequest> request) const;
	};
}
