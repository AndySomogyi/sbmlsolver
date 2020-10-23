(*

category:        Test
synopsis:        Multiple species references to the same species with assigned stoichiometries
componentTags:   Compartment, InitialAssignment, Reaction, Species
testTags:        Amount, AssignedConstantStoichiometry, InitialValueReassigned, NonUnityStoichiometry
testType:        TimeCourse
levels:          3.1, 3.2
generatedBy:     Numeric
packagesPresent: 

 In this test, a single reaction has multiple reactants that all point to the same species, each of which has an assigned value.  This is equivalent to a reaction where all of the species references that point to the same species are collapsed into a single species reference with a stoichiometry equal to the sum of the stoichiometries of the original species references.

The model contains:
* 1 species (A)
* 1 compartment (C)
* 2 species references (A1_sr, A1_sr2)

There is one reaction:

[{width:30em,margin: 1em auto}|  *Reaction*  |  *Rate*  |
| J0: A1_sr A + A1_sr2 A -> | $-1$ |]
Note:  the following stoichiometries are set separately:  A1_sr, A1_sr2

The initial conditions are as follows:

[{width:35em,margin: 1em auto}|       | *Value* | *Constant* |
| Initial concentration of species A | $1$ | variable |
| Initial volume of compartment 'C' | $1$ | constant |
| Initial value of species reference 'A1_sr' | $2$ | constant |
| Initial value of species reference 'A1_sr2' | $1$ | constant |]

*)
