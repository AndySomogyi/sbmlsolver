/*
 * Variant.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: andy
 */

#include "Variant.h"
#include "rrLogger.h"

#include <exception>
#include <iostream>
#include <Poco/Dynamic/Var.h>

using namespace std;
using Poco::Dynamic::Var;

namespace rr
{

struct VariantImpl
{
    Var var;
};

Variant::Variant()
{
    alloc();
}

Variant::Variant(const Variant& other)
{
    alloc();
    self->var = other.self->var;
}

Variant& Variant::operator =(const Variant& other)
{
    self->var = other.self->var;
    return *this;
}

Variant::~Variant()
{
    delete self;
}

const std::type_info& Variant::type() const
{
    return self->var.type();
}

void Variant::alloc()
{
    self = new VariantImpl();
}

#define TRY_ASSIGN(type)                                    \
        if (info == typeid(type)) {                         \
            const type *val = static_cast<const type*>(p);  \
            self->var = *val;                               \
            return;                                         \
        }

void Variant::assign(const std::type_info& info, const void* p)
{
    std::cout << "name: " << info.name() << std::endl;

    TRY_ASSIGN(std::string);

    TRY_ASSIGN(int);

    TRY_ASSIGN(long);

    TRY_ASSIGN(float);

    TRY_ASSIGN(double);

    TRY_ASSIGN(bool);

    string msg = "could not assign type ";
    msg += info.name();
    msg += " to Variant";

    throw invalid_argument(msg);
}

Variant Variant::parse(const std::string& val)
{
    Variant var;
    var.self->var = Var::parse(val);
    return var;
}

std::string Variant::toString() const
{
    return Var::toString(self->var);
}

bool Variant::isString() const
{
    return self->var.isString();
}

bool Variant::isInteger() const
{
    return self->var.isInteger();
}

bool Variant::isNumeric() const
{
    return self->var.isNumeric();
}

bool Variant::isBool() const
{
    return self->var.type() == typeid(bool);
}

#define TRY_CONVERT_TO(type)                       \
        if (info == typeid(type)) {                \
            type* out = static_cast<type*>(p);     \
            *out = self->var.convert<type>();      \
            return;                                \
        }

void Variant::convert_to(const std::type_info& info, void* p) const
{
    TRY_CONVERT_TO(std::string);

    TRY_CONVERT_TO(long);

    TRY_CONVERT_TO(int);

    TRY_CONVERT_TO(bool);

    TRY_CONVERT_TO(float);

    TRY_CONVERT_TO(double);


    string msg = "Could not convert variant with typeid ";
    msg += self->var.type().name();
    msg += " to type";
    msg += info.name();

    throw invalid_argument(msg);
}

} /* namespace rr */
