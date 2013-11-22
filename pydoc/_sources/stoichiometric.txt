.. currentmodule:: roadrunner

***********************
Stoichiometric Analysis
***********************

Preliminaries
-------------

A network of :math:`m` chemical species and :math:`n` reactions can be described by the
:math:`m` by :math:`n` stochiometry matrix :math:`\mathbf{N}`. :math:`\mathbf{N}_{i,j}` is the net number of  
species :math:`i` produced or consumed in reaction :math:`j`.  The dynamics of the network are
described by 

.. math::

   \frac{d}{dt}\mathbf{s}(t) = \mathbf{N} \mathbf{v}(\mathbf{s}(t),\mathbf{p},t), 

where :math:`\mathbf{s}` is the vector of species concentrations, :math:`\mathbf{p}` 
is a vector of time independent parameters, and :math:`t` is time.

Each structural conservation, or interchangably, *conserved sum* (e.g. conserved moiety) in 
the network coresponds to a lineraly dependent row in the stoichiometry matrix :math:`\mathbf{N}`.

If there are conserved sums, then the row rank, :math:`r` of :math:`N` is :math:`< m`, and 
the stochiometry matrix :math:`N` may first be re-ordered such that the first :math:`r` are linearly
independent, and the remaining :math:`m-r` rows are linear combinations of the first :math:`r`
rows. The *reduced stochiometry matrix* :math:`\mathbf{N_R}` is then formed from the first
:math:`r` rows of :math:`N`. Finally, :math:`N` may be expressed as a product of the :math:`m \times r`
*link matrix* :math:`\mathbf{L}` and the :math:`r \times n` :math:`\mathbf{N_R}` matrix: 

.. math::

   \mathbf{N} = \mathbf{L}\mathbf{N_R}.

The link matrix :math:`\mathbf{L}` has the form

.. math::

   \mathbf{L} = \left[ \begin{array}{c}
                       \mathbf{I}_{r} \\
                       \mathbf{L}_0    \end{array} \right],

where :math:`\mathbf{I}_{r}` is the :math:`r \times r` identity matrix and :math:`\mathbf{L}_0` is a :math:`(m-r) \times r`
matrix.

Methods
-------

The following methods are related to the analysis of the stoichiometric matrix.

.. autosummary::

   ExecutableModel.getStoichiometryMatrix
   RoadRunner.getLinkMatrix
   RoadRunner.getNrMatrix
   RoadRunner.getConservationMatrix
   RoadRunner.getL0Matrix
   ExecutableModel.getNumConservedMoieties
   ExecutableModel.getConservedMoietyIds
   ExecutableModel.getConservedMoietyValues
   ExecutableModel.setConservedMoietyValues



