#pragma once

#include <optional>
#include <string>

#include "../engine/OrderRequest.hpp"

namespace novax::parser
{

class CommandParser
{
public:

    static std::optional<novax::engine::OrderRequest>
    parse(const std::string& command);
};

}