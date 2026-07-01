#pragma once

#include <vector>

#include "../common/Trade.hpp"

namespace novax::engine
{

struct OrderResult
{
    bool accepted = false;

    bool executed = false;

    bool partialFill = false;

    int remainingQuantity = 0;

    std::vector<novax::common::Trade> trades;
};

}