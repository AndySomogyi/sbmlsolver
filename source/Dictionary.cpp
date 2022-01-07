/*
 * Dictionary.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: andy
 */
#include "Dictionary.h"
#include <stdexcept>

namespace rr {

BasicDictionary::BasicDictionary(std::initializer_list<item> initializerList)
{
    for (auto [key, val] : initializerList){
        BasicDictionary::setItem(key, val);
    }
}


void BasicDictionary::setItem(const std::string& key,
        const rr::Setting& value)
{
    items[key] = value;
}

Setting BasicDictionary::getItem(const std::string& key) const
{
    auto i = items.find(key);

    if (i != items.end()) {
        return i->second;
    }
    throw std::invalid_argument("invalid key: " + key);
}

bool BasicDictionary::hasKey(const std::string& key) const
{
    return items.find(key) != items.end();
}

size_t  BasicDictionary::deleteItem(const std::string& key)
{
    return items.erase(key);
}

std::vector<std::string> rr::BasicDictionary::getKeys() const
{
    std::vector<std::string> keys(items.size());
    int j = 0;
    for(auto i = items.begin(); i != items.end(); ++i, ++j) {
        keys[j] = i->first;
    }
    return keys;
}


}




