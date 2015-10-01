.. class:: SelectionRecord(str)
   :module: RoadRunner

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

.. attribute:: SelectionRecord.CONCENTRATION

.. attribute:: SelectionRecord.AMOUNT

   species must have either a CONCENTRATION or AMOUNT
   modifer to distinguish it.
   
.. attribute:: SelectionRecord.RATE

.. attribute:: SelectionRecord.BOUNDARY

.. attribute:: SelectionRecord.FLOATING

.. attribute:: SelectionRecord.REACTION

.. attribute:: SelectionRecord.INITIAL

.. attribute:: SelectionRecord.CURRENT

.. attribute:: SelectionRecord.UNSCALED

.. attribute:: SelectionRecord.ELASTICITY

.. attribute:: SelectionRecord.CONTROL

.. attribute:: SelectionRecord.EIGENVALUE

.. attribute:: SelectionRecord.ELEMENT

.. attribute:: SelectionRecord.STOICHIOMETRY

.. attribute:: SelectionRecord.UNKNOWN

.. attribute:: SelectionRecord.DEPENDENT

.. attribute:: SelectionRecord.INDEPENDENT

.. attribute:: SelectionRecord.CONSREVED_MOIETY

.. attribute:: SelectionRecord.UNKNOWN_CONCENTRATION

.. attribute:: SelectionRecord.COMPARTMENT

.. attribute:: SelectionRecord.GLOBAL_PARAMETER

.. attribute:: SelectionRecord.FLOATING_AMOUNT

.. attribute:: SelectionRecord.BOUNDARY_AMOUNT

.. attribute:: SelectionRecord.BOUNDARY_CONCENTRATION

.. attribute:: SelectionRecord.FLOATING_CONCENTRATION

.. attribute:: SelectionRecord.FLOATING_AMOUNT_RATE

.. attribute:: SelectionRecord.FLOATING_CONCENTRATION_RATE

.. attribute:: SelectionRecord.REACTION_RATE

.. attribute:: SelectionRecord.INITIAL_FLOATING_AMOUNT

.. attribute:: SelectionRecord.INITIAL_FLOATING_CONCENTRATION

.. attribute:: SelectionRecord.UNSCALED_ELASTICITY

.. attribute:: SelectionRecord.UNSCALED_CONTROL

.. attribute:: SelectionRecord.UNKNOWN_ELEMENT

.. attribute:: SelectionRecord.ALL

.. attribute:: SelectionRecord.ALL_INDEPENDENT

.. attribute:: SelectionRecord.ALL_DEPENDENT

.. attribute:: SelectionRecord.ALL_INDEPENDENT_AMOUNT

.. attribute:: SelectionRecord.ALL_DEPENDENT_AMOUNT

.. attribute:: SelectionRecord.ALL_INDEPENDENT_CONCENTRATION

.. attribute:: SelectionRecord.ALL_DEPENDENT_CONCENTRATION


.. attribute:: SelectionRecord.index
   :module: RoadRunner
   :annotation: int


.. attribute:: SelectionRecord.p1
   :module: RoadRunner
   :annotation: str


.. attribute:: SelectionRecord.p2
   :module: RoadRunner
   :annotation: str


.. attribute:: SelectionRecord.selectionType
   :module: RoadRunner
   :annotation: int

