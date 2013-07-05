/*
 * rrLLVMASTNodeFactory.cpp
 *
 *  Created on: Jul 4, 2013
 *      Author: andy
 */

#include "rrLLVMASTNodeFactory.h"

using namespace std;
using namespace libsbml;

namespace rr
{

LLVMASTNodeFactory::LLVMASTNodeFactory()
{
}

LLVMASTNodeFactory::~LLVMASTNodeFactory()
{
    for (list<ASTNode*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        ASTNode *node = *i;
        delete node;
    }
}

libsbml::ASTNode* LLVMASTNodeFactory::create(libsbml::ASTNodeType_t type)
{
    ASTNode *node = new ASTNode(type);
    nodes.push_back(node);
    return node;
}

libsbml::ASTNode* LLVMASTNodeFactory::create(libsbml::Token_t* token)
{
    ASTNode *node = new ASTNode(token);
    nodes.push_back(node);
    return node;
}

libsbml::ASTNode* LLVMASTNodeFactory::create(const libsbml::ASTNode& orig)
{
    ASTNode *node = new ASTNode(orig);
    nodes.push_back(node);
    return node;
}

} /* namespace rr */
