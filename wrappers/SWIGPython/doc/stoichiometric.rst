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

If there are conserved sums, then the row rank, :math:`m_0` of :math:`N` is :math:`< m`, and 
the stochiometry matrix :math:`N` may first be re-ordered such that the first :math:`m_0` are linearly
independent, and the remaining :math:`m-m_0` rows are linear combinations of the first :math:`m_0`
rows. The *reduced stochiometry matrix* :math:`\mathbf{N_R}` is then formed from the first
:math:`m_0` rows of :math:`N`. Finally, :math:`N` may be expressed as a product of the :math:`m \times m_0`
*link matrix* :math:`\mathbf{L}` and the :math:`m_0 \times n` :math:`\mathbf{N_R}` matrix: 

.. math::

   \mathbf{N} = \mathbf{L}\mathbf{N_R}.

The link matrix :math:`\mathbf{L}` has the form

.. math::

   \mathbf{L} = \left[ \begin{array}{c}
                       \mathbf{I}_{n0} \\
                       \mathbf{L}_0    \end{array} \right],

where :math:`\mathbf{I}_{m0}` is the :math:`m_0 \times m_0` identity matrix. 


The following methods are related to the analysis of the stoichiometric matrix.

.. autosummary::

   RoadRunner.getStoichiometryMatrix
   RoadRunner.getLinkMatrix
   RoadRunner.getNrMatrix
   RoadRunner.getConservationMatrix
   RoadRunner.getL0Matrix
   ExecutableModel.getConservedSums
   ExecutableModel.getNumConservedSums
   ExecutableModel.getConservedSumChanged

