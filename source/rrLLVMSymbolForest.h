/*
 * rrLLVMSymbolForest.h
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */

#ifndef LLVMSymbolForestH
#define LLVMSymbolForestH

#include <sbml/math/ASTNode.h>
#include <map>

namespace rr
{
using std::string;
using std::map;
using libsbml::ASTNode;



/**
 * Similar to a symbol table, except instead of a name / value,
 * we have a name / tree, where the tree is the root of an ASTNode
 * tree.
 *
 * The rationality behind this will be covered in the paper, but
 * briefly its required because we are dealing with a declarative
 * language in which the rules can be defined in any order.
 *
 * This class does not own anything, it just has const pointers
 * to a set of ASTNodes which are owned either by the sbml model,
 * or some containing class.
 */
class LLVMSymbolForest
{
public:
    map<string, const ASTNode*> floatingSpecies;
    map<string, const ASTNode*> boundarySpecies;
    map<string, const ASTNode*> compartments;
    map<string, const ASTNode*> globalParameters;
};

} /* namespace rr */
#endif /* LLVMSymbolForestH
 */
