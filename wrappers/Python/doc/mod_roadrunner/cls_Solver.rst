Solver Class
_________________________

.. class:: roadrunner.Solver

   Following functions are supported for given solver instance.
   Keep in mind some of the settings are only supported for certain types of integrators.


.. method:: Solver.getName()

   Returns the name of this solver
   
   
.. method:: Solver.getDescription(key)

   If key = None,  returns description of this solver.
   Else, returns the description associated with a given key.
   
   :param str key: settings name
   
   
.. method:: Solver.getHint(key)

   If key = None,  returns a (user-readable) hint for this solver.
   Else, Returns the hint associated with a given key.
   
   :param str key: settings name
   
   
.. method:: Solver.getSettings()

   Returns a list of all settings for this solver.

   
.. method:: Solver.resetSettings()

   Reset all settings to their respective default values.

   
.. method:: Solver.getValue(key)

   Get the value of an integrator setting.
   
   :param str key: settings name

   
.. method:: Solver.hasValue(key)

   Return true if this setting is supported by the integrator.
   
   :param str key: settings name
   
.. method:: Solver.getNumParams()

   Get the number of parameters.

   
.. method:: Solver.getParamName(n)

   Get the display name of the parameter at index n.
   
   :param int n: index of parameter
   
   
.. method:: Solver.getParamHint(n)

   Get the hint of the parameter at index n.
   
   :param int n: index of parameter
   
   
.. method:: Solver.getParamDesc(n)

   Get the description of the parameter at index n

   :param int n: index of parameter
   
   
.. method:: Solver.getValueAsInt(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name
   
   
.. method:: Solver.getValueAsUInt(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   
   
   
.. method:: Solver.getValueAsLong(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name

   
.. method:: Solver.getValueAsULong(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name
   
   
.. method:: Solver.getValueAsFloat(key)
   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   
   

.. method:: Solver.getValueAsDouble(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   


.. method:: Solver.getValueAsChar(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   
   
   
.. method:: Solver.getValueAsUChar(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   
  
   
.. method:: Solver.getValueAsString(key)

   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name      
   
   
.. method:: Solver.getValueAsBool(key)
   Wrapper for :meth:`Solver.getValue()` which converts output to a specific type
   
   :param str key: settings name   
   
   
.. method:: Solver.setValue(key, value)

   Sets value of a specific setting.
   
   :param str key: settings name      
   :param const value: value of the setting
   
   
.. method:: Solver.getDisplayName(key)

   Returns display name of given key.
   
   :param str key: settings name   
   

.. method:: Solver.getType(key)

   Returns the type associated with a given key.
   
   :param str key: settings name 
   

.. method:: Solver.getSettingsRepr()

   Returns the solver settings as a string.


.. method:: Solver.settingsPyDictRepr()

   Returns Python dictionary-style string representation of settings.
   
   
.. method:: Solver.toString()

   Return a string representation of the solver.
   
   
.. method:: Solver.toRepr()

   Return string representation a la Python __repr__ method.
   
   