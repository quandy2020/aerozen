/*
 * Private data for NodeOptions.
 */

#ifndef AEROZEN_NODE_OPTIONS_PRIVATE_HPP_
#define AEROZEN_NODE_OPTIONS_PRIVATE_HPP_

#include <map>
#include <string>

namespace aerozen {

class NodeOptionsPrivate
{
public:
    std::string ns;
    std::string partition;
    std::map<std::string, std::string> topicsRemap;
};

}  // namespace aerozen

#endif  // AEROZEN_NODE_OPTIONS_PRIVATE_HPP_
