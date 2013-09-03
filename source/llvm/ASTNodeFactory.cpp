/*
 * ASTNodeCodeGen.cpp
 *
 *  Created on: Jul 4, 2013
 *      Author: andy
 */

#include "ASTNodeFactory.h"

using namespace std;
using namespace libsbml;

namespace rrllvm
{

ASTNodeFactory::ASTNodeFactory()
{
}

ASTNodeFactory::~ASTNodeFactory()
{
    for (list<ASTNode*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        ASTNode *node = *i;
        delete node;
    }
}

libsbml::ASTNode* ASTNodeFactory::create(libsbml::ASTNodeType_t type)
{
    ASTNode *node = new ASTNode(type);
    nodes.push_back(node);
    return node;
}

libsbml::ASTNode* ASTNodeFactory::create(libsbml::Token_t* token)
{
    ASTNode *node = new ASTNode(token);
    nodes.push_back(node);
    return node;
}

libsbml::ASTNode* ASTNodeFactory::create(const libsbml::ASTNode& orig)
{
    ASTNode *node = new ASTNode(orig);
    nodes.push_back(node);
    return node;
}

} /* namespace rr */
