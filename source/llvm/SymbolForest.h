/*
 * SymbolForest.h
 *
 *  Created on: Jul 3, 2013
 *      Author: andy
 */

#ifndef SymbolForestH
#define SymbolForestH

#include <sbml/math/ASTNode.h>
#include <map>

namespace rrllvm
{
using std::string;
using std::map;

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
 *
 * This class basically stores the name / value of all the re-write rules
 * (assigment or initial assigment).
 */
class SymbolForest
{
public:
    typedef map<string, const libsbml::ASTNode*> Map;
    typedef map<string, const libsbml::ASTNode*>::const_iterator _const_iterator;

    /**
     * syntatically the same as a map<string, const libsbml::ASTNode*>::const_iterator
     *
     * Designed so that the SymbolForest can behave like a std::map.
     */
    class ConstIterator
    {
    public:

        const libsbml::ASTNode* second;

        ConstIterator(_const_iterator i) : second(i->second), p(this)
        {
        }

        ConstIterator() : second(0), p(this)
        {
        }

        const ConstIterator* operator->() const
        {
            return this;
        }

        bool operator !=(const ConstIterator& other) const
        {
            return p != other.p;
        }

        ConstIterator & operator= (const ConstIterator & o)
        {
            this->second = o.second;
            this->p = o.p;
            // by convention, always return *this
            return *this;
        }

        ConstIterator(const ConstIterator &o) : second(o.second), p(o.p)
        {
        }


    private:
        // address of where it was created.
        const ConstIterator* p;

    };

    map<string, const libsbml::ASTNode*> floatingSpecies;
    map<string, const libsbml::ASTNode*> boundarySpecies;
    map<string, const libsbml::ASTNode*> compartments;
    map<string, const libsbml::ASTNode*> globalParameters;
    map<string, const libsbml::ASTNode*> speciesReferences;

    /**
     * find and end are designed to work identically to the map::find.
     *
     * Note that the resulting iterator is not iteratable, it is only suitable for
     * comparing with end() and getting the value out of it.
     */
    const ConstIterator find(const map<string, const libsbml::ASTNode*>::key_type& x) const
    {
        _const_iterator result;

        if ((result = floatingSpecies.find(x)) != floatingSpecies.end())
        {
            return ConstIterator(result);
        }
        else if ((result = boundarySpecies.find(x)) != boundarySpecies.end())
        {
            return ConstIterator(result);
        }
        else if ((result = compartments.find(x)) != compartments.end())
        {
            return ConstIterator(result);
        }
        else if ((result = globalParameters.find(x)) != globalParameters.end())
        {
            return ConstIterator(result);
        }
        else if ((result = speciesReferences.find(x)) != speciesReferences.end())
        {
            return ConstIterator(result);
        }
        else
        {
            return _end;
        }
    }

    const ConstIterator& end() const
    {
        return _end;
    }

private:
    ConstIterator _end;

};


} /* namespace rr */
#endif /* LLVMSymbolForestH */
