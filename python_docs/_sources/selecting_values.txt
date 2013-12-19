.. _selecting-values:

****************
Selecting Values
****************

.. currentmodule:: roadrunner

RoadRunner support a wide range of options for selecting SBML and derivative values. Selections can
be used to either retrive or in many cases, store values. We define SBML values as any element
specified in the original SBML doucment, whereas derivative values are a function of one or more
SBML values. As these functions typically have no inverse, these are read only selections.

Selections are used a number of RoadRunner methods. They are used to determine what columns are
selected in a simulation result and can also be used to directly select model values. 

Selection syntax
________________
The selection syntax is case sensitive (SBML elements are case sensitive), however it ignores all
white space. 

Selection Types:

Time
    The string ``time`` is interpreted as the current model time. 

Identifier 
    Any continious sequeuence of printing characters is treated as an SBML identifier with
    the exception of ``time``. Some examples are: ``S1``, ``P1``, ``J0_Keq1``,
    ``compartment``. These have to be a valid SBML identifer. If no SBML identifer is found the
    requested an exception is raises. Note, if the identifier is a chemical species, it is
    interpreted as the species amount. 

Concentration 
    Following generally accepted chemical nomenclature, any continous seqeuce of
    characters inside a pair of squared brackets, ``[]`` is interpreded as a concentration. Some
    examples are ``[S1]``, ``[S2 ]`` and so forth. The identifer must be a valid sbml species,
    othersiwe an exception is raised. 

Rates of Change
    These represent the rate of change, :math:`\frac{dy}{dt}` of a species amount. Rates of change
    selections follow generally accepted mathematical convention of using the single quote, ``'`` 
    to represent a time derivateve. A rate of change selection is any identifier followed by a
    single quote. Some examples are ``S1'``, ``S2'``, ``MyParameter'``. A rates of change selection
    is also valid for sbml elementes which are defined by rate rules. 

Elasticity
    The form of ``ee(ReactionId, ParameterId)`` where ``ReactionId`` is a reaction name,
    and ``ParameterId`` is a global parameter id. This selection intentionaly has a function form, it
    selects the elasticity control coefficient, :meth:`RoadRunner.getEE`

Unscaled Elasticity
    The form of ``uee(ReactionId, ParameterId)`` where ``ReactionId`` is a reaction name,
    and ``ParameterId`` is a global parameter id. This selection intentionaly has a function form, it
    selects the elasticity control coefficient, :meth:`RoadRunner.getEE`

Control
    Control coefficient selections have the form of ``cc(Id, ParameterId)``. The string ``Id``
    and ``ParameterId`` must be valid arguments for :meth:`RoadRunner.getCC` This selection
    intentionaly has a function form, it selects the elasticity control coefficient,
    :meth:`RoadRunner.getCC`

Unscaled Control
    A string with the form of ``ucc(Id, ParameterId)``. The string ``Id`` and ``ParameterId`` must be 
    valid arguments for :meth:``RoadRunner.getuCC``
    This selection intentionaly has a function form, it
    selects the unscaled elasticity control coefficient, :meth:`RoadRunner.getuCC`

Eigenvalue
    The form of ``eigen(identifier)``. The eigenvalue of a floating species.

Initial Concentration
    An initial concentration specified in the sbml document

Stoichiometry
    The stoichiometric coefficient for a given species and reaction.

Experimenting With Selections
-----------------------------
One my try individual selection string using the :meth:`RoadRunner.getSelectionValue` method. This
takes a selection string, determines if it is valid, and returns the value it selects. For example::


    >>> rr.getSelectionValue("cc(S1, J4_KS4)")
        -0.42955738179207886

Even though they are almost always specified by a string, RoadRunner selections are actually
object. One can create a selection object directly using :meth:`RoadRunner.createSelection`, and in
order to see extended information about, simply display it. For example, using ipython::

    >>> sel = rr.createSelection("cc(S1, J4_KS4)")
    >>> sel
        SelectionRecord({'index' : -1, 'p1' : 'S1', 'p2' : 'J4_KS4', 'selectionType' : CONTROL})

We can see that this is a CONTROL record. 

It is also possible to modify the simulation selection list by deleting existing items and adding
new once created with :meth:`RoadRunner.createSelection`. If someone has does not want to display
the concentration of species ``S2`` and instead wants to display the rate of change of species
``S``::

    >>> rr.selections
        ["time", "[S1]", "[S2]", "[S3]", "[S4]"]

    >>> sel = rr.createSelection("S1'")
    >>> rr.selections[2] = sel
    >>> rr.selections
        ["time", "[S1]", "S1'", "[S3]", "[S4]"]

Even though the selection list intentionally appears as a list of string, it is actually a list of
selection objects. So, elements that are inserted or appended to this list must be selection objects
created by :meth:`RoadRunner.createSelection`.



Selecting Simulation Results
----------------------------
The colums of the RoadRunner simulation result are determined by selections, these are accessed by
the :attr:`RoadRunner.selections` property. This is a list of what values will be displayed in the
result, and can be set simply by::

    rr.selections = ['time', '[S1]', 'S1', "S1'"]

This example selects the columns time, concentration of ``S1``, amount of ``S1`` and rate of change
of ``S1``. One may also have derivative values in the simulation selection, for example, if one
wanted to plot the elasticity of a particular reaction::

    rr.selections = ['time', '[S1]', "ee(J1, P1)"]



Methods and properties which accept selections
______________________________________________

.. autosummary::

   ExecutableModel[]
   ExecutableModel.__setitem__



