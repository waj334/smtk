Expanding Units Support in Attribute Resource
--------------------------------------------

SMTK's Attribute DoubleItems and DoubleItemDefinitions can now support units specified as Defaults
as well as values.  When a default's or value's units differ from those defined by the Item's Definition,
they are converted into the Definition's units.  If no conversion is possible then the method assigning
the default or value will fail.

The Item's value(...) methods will always return the value in the units specified in its Definition.
The Item's valueAsString(...) methods will always return a string based on the unconverted value

When specifying Default Values (for DoubleItemDefinitions) and Values (for DoubleItems) the following is the expected behavior:

DoubleItemDefinition::setDefaultValueAsString and DoubleItemDefinition::setDefaultValue

	1. Will only append units to its default string values iff its units are supported by its units system
	2. Will remove the units from a default value string iff its units are not supported by its units system
	3. Its static splitStringStartingDouble method now trims both the value and units strings it returns.
	4. Added a hasSupportedUnits method that returns true if the Definition's units are set and are supported by its units system.

 DoubleItem::setValue and DoubleItem::setValueFromString
	1. Will only append units if its definition's units are supported by its units system and the input value string does not contains units
    2. Will remove units from an input value string if its definition's units are not supported by its units system

Developer changes
~~~~~~~~~~~~~~~~~~

SMTK Resources can now hold a units::System.  In the case of an Attribute Resource, it will have a
default units::System associated with it at construction time; however, this can be replaced as long
as there are no Definitions defined within the Resource.

New Resource Methods:
* setUnitsSystem(const shared_ptr<units::System> & unitsSystem)
* const shared_ptr<units::System> & unitsSystem() const;

All ItemDefintions now hold onto a units::System.  The methods are protected and are identical to the ones added to Resource.

DoubleItemDefinition has the following new methods:

* bool setDefaultValue(const double& val, const std::string& units);
* bool setDefaultValue(const std::vector<double>& vals, const std::string& units);
* bool setDefaultValueAsString(const std::string& val);
* bool setDefaultValueAsString(std::size_t element, const std::string& val);
* bool setDefaultValueAsString(const std::vector<std::string>& vals);
* const std::string defaultValueAsString(std::size_t element = 0) const;
* const std::vector<std::string> defaultValuesAsStrings() const;
* bool hasSupportedUnits() const;

DoubleItem has the following new methods:

* bool setValue(std::size_t element, const double& val, const std::string& units);

In addition, DoubleItem::setValueFromString method can now handle strings that include a double
followed by an option units.  For example "20 m/s".
