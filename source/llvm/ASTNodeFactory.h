/*
 * rrLLVMASTNodeFactory.h
 *
 *  Created on: Jul 4, 2013
 *      Author: andy
 */

#ifndef LLVMASTNodeFactoryH
#define LLVMASTNodeFactoryH

#include <sbml/math/ASTNode.h>
#include <list>

namespace rrllvm
{

/**
 * Manages a set of ASTNodes.
 *
 * This class provides a createASTNode which creates a new ASTNode,
 * but it retains a reference to it. When this class is destroyed,
 * all the nodes that it created are destroyed as well.
 *
 * TODO: There might be a standard way of doing this, look it up
 * and replace if there exists such a thing.
 */
class ASTNodeFactory
{
public:
    ASTNodeFactory();
    ~ASTNodeFactory();

    /**
     * Creates and returns a new ASTNode.
     *
     * Unless the argument @p type is given, the returned node will by
     * default have a type of @link ASTNodeType_t#AST_UNKNOWN
     * AST_UNKNOWN@endlink.  If the type isn't supplied when caling this
     * constructor, the caller should set the node type to something else as
     * soon as possible using
     * @if clike setType()@else ASTNode::setType(int)@endif.
     *
     * @param type an optional
     * @if clike @link #ASTNodeType_t ASTNodeType_t@endlink@else type@endif@~
     * code indicating the type of node to create.
     *
     * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
     */
    libsbml::ASTNode *create(libsbml::ASTNodeType_t type = libsbml::AST_UNKNOWN);


    /**
     * Creates a new ASTNode from the given Token.  The resulting ASTNode
     * will contain the same data as the Token.
     *
     * @param token the Token to add.
     */
    libsbml::ASTNode *create(libsbml::Token_t *token);


    /**
     * Copy constructor; creates a deep copy of the given ASTNode.
     *
     * @param orig the ASTNode to be copied.
     */
    libsbml::ASTNode *create(const libsbml::ASTNode& orig);

private:
    /**
     * no copy or asigment
     */
    ASTNodeFactory(const ASTNodeFactory&) {};
    ASTNodeFactory& operator=( const ASTNodeFactory& rhs ) {return *this;};

    /**
     * the list of nodes this factory created.
     */
    std::list<libsbml::ASTNode*> nodes;
};

} /* namespace rr */
#endif /* LLVMASTNodeFactoryH */
