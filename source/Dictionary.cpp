/*
 * Dictionary.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: andy
 */
#include "Dictionary.h"
#include <stdexcept>

namespace rr {

void DictionaryImpl::setItem(const std::string& key,
        const rr::Variant& value)
{
    items[key] = value;
}

Variant DictionaryImpl::getItem(const std::string& key) const
{
    VariantMap::const_iterator i = items.find(key);

    if (i != items.end()) {
        return i->second;
    }
    throw std::invalid_argument("invalid key: " + key);
}

bool DictionaryImpl::hasKey(const std::string& key) const
{
    return items.find(key) != items.end();
}

int  DictionaryImpl::deleteItem(const std::string& key)
{
    return items.erase(key);
}

std::vector<std::string> rr::DictionaryImpl::getKeys() const
{
    std::vector<std::string> keys(items.size());
    int j = 0;
    for(VariantMap::const_iterator i = items.begin(); i != items.end(); ++i, ++j) {
        keys[j] = i->first;
    }
    return keys;
}


}




