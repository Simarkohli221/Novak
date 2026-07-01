#include "../../include/parser/CommandParser.hpp"

#include <sstream>

namespace novax::parser
{

std::optional<novax::engine::OrderRequest>
CommandParser::parse(const std::string& command)
{
    std::stringstream ss(command);

    std::string type;
    ss >> type;

    // ---------------- BUY / SELL ----------------

    if (type == "BUY" || type == "SELL")
    {
        novax::engine::OrderRequest request;

        request.type =
            novax::engine::RequestType::NEW_ORDER;

        request.order.side =
            (type == "BUY")
                ? novax::common::Side::BUY
                : novax::common::Side::SELL;

        ss >> request.order.symbol;
        ss >> request.order.price;
        ss >> request.order.quantity;

        request.order.remainingQuantity =
            request.order.quantity;

        return request;
    }

    // ---------------- MODIFY ----------------

    if (type == "MODIFY")
    {
        novax::engine::OrderRequest request;

        request.type =
            novax::engine::RequestType::MODIFY_ORDER;

        ss >> request.orderId;
        ss >> request.newPrice;
        ss >> request.newQuantity;

        return request;
    }
    if (type == "CANCEL")
{
    novax::engine::OrderRequest request;

    request.type =
        novax::engine::RequestType::CANCEL_ORDER;

    ss >> request.orderId;

    return request;
}
if (type == "BOOK")
{
    novax::engine::OrderRequest request;

    request.type =
        novax::engine::RequestType::BOOK;

    return request;
}
    // ---------------- INVALID ----------------

    return std::nullopt;
}

} // namespace novax::parser