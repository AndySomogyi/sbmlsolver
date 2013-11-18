.. class:: SelectionRecord(str)
   :module: roadrunner

   RoadRunner provides a range of flexible ways of selecting values from
   a simulation. These values can not only be calculated directly via
   RoadRunner.getSelectionValue, but any of these selections can be
   used as columns in the simulate result matrix.

   The SectionRecord.selectionType should be one of the constants listed
   here.

   Most selection types only require the first symbol id, p1 to be set,
   however certain ones such as [???] require both p1 and p2.

.. method::SelectionRecord.__init__(str)
   Create a new selection record. This constructor really should not be called,
   SelectionRecords should be created by the RoadRunner.createSelection


.. attribute:: SelectionRecord.TIME

   Model Time
               
.. attribute:: SelectionRecord.BOUNDARY_CONCENTRATION

   Boundary Concentration

.. attribute:: SelectionRecord.FLOATING_CONCENTRATION

   Floating Species Concentration

.. attribute:: SelectionRecord.REACTION_RATE

   Reaction Rate

.. attribute:: SelectionRecord.FLOATING_AMOUNT_RATE

   Rate of change of floating species amount

.. attribute:: SelectionRecord.COMPARTMENT

   Compartment Volume

.. attribute:: SelectionRecord.GLOBAL_PARAMETER

   Global Parameter

.. attribute:: SelectionRecord.FLOATING_AMOUNT

   Floating Species Amount

.. attribute:: SelectionRecord.BOUNDARY_AMOUNT

   Boundary species amount

.. attribute:: SelectionRecord.ELASTICITY

   Elasticity control coefficient

.. attribute:: SelectionRecord.UNSCALED_ELASTICITY

   Unscaled elasticity control coefficient

.. attribute:: SelectionRecord.CONTROL

   Control coefficient

.. attribute:: SelectionRecord.UNSCALED_CONTROL

   Unscaled control coefficient

.. attribute:: SelectionRecord.EIGENVALUE

   Eigenvalue 

.. attribute:: SelectionRecord.INITIAL_CONCENTRATION

   Floating species intial concentration

.. attribute:: SelectionRecord.STOICHIOMETRY

   Stochiometry matrix entry

.. attribute:: SelectionRecord.UNKNOWN_ELEMENT

   A syntactically valid element but it has not been determined
   what sbml element ths id refers to. 

.. attribute:: SelectionRecord.UNKNOWN_CONCENTRATION

   A syntactially valid but unknown concentration.

.. attribute:: SelectionRecord.UNKNOWN
   
   An error condition.




.. attribute:: SelectionRecord.index
   :module: roadrunner
   :annotation: int


.. attribute:: SelectionRecord.p1
   :module: roadrunner
   :annotation: str


.. attribute:: SelectionRecord.p2
   :module: roadrunner
   :annotation: str


.. attribute:: SelectionRecord.selectionType
   :module: roadrunner
   :annotation: int

