/*
 * Private data for SubscribeOptions.
 */

#ifndef AEROZEN_SUBSCRIBE_OPTIONS_PRIVATE_HPP_
#define AEROZEN_SUBSCRIBE_OPTIONS_PRIVATE_HPP_

#include <cstdint>

#include "aerozen/helpers.hpp"

namespace aerozen {

class SubscribeOptionsPrivate
{
public:
    uint64_t msgsPerSec = kUnthrottled;
    bool ignoreLocalMessages = false;
};

}  // namespace aerozen

#endif  // AEROZEN_SUBSCRIBE_OPTIONS_PRIVATE_HPP_
