<?xml version="1.0"?>
<SMTK_AttributeResource Version="7">
  <Properties>
    <!-- Display when loaded -->
    <Property Name="smtk.attribute_panel.display_hint" Type="bool">true</Property>
  </Properties>

  <Definitions>
    <AttDef Type="UnitsExample">
      <ItemDefinitions>
        <Double Name="No Units [0-100)">
          <BriefDescription>No Units, Default Value</BriefDescription>
          <DefaultValue>3.14159</DefaultValue>
          <RangeInfo>
            <Min Inclusive="true">0.0</Min>
            <Max Inclusive="false">100.0</Max>
          </RangeInfo>
        </Double>

        <Double Name="Length [0-100) m" Units="meters">
          <DefaultValue>5.5</DefaultValue>
          <RangeInfo>
            <Min Inclusive="true">0.0</Min>
            <Max Inclusive="false">100.0</Max>
          </RangeInfo>
        </Double>

        <Double Name="Length (-inf, inf) km" Units="km">
          <DefaultValue>2 m</DefaultValue>
        </Double>

        <Double Name="Capacity" Units="gallon">
          <RangeInfo>
            <Min Inclusive="true">0.0</Min>
          </RangeInfo>
        </Double>

        <Double Name="Infix Option" Units="meter">
          <DefaultValue>2.8128</DefaultValue>
          <ExpressionType>infixExpression</ExpressionType>
        </Double>

        <Double Name="Made Up Units" Units="some-units">
          <BriefDescription>Unrecognized Units</BriefDescription>
          <DefaultValue>1.1</DefaultValue>
          <ExpressionType>infixExpression</ExpressionType>
        </Double>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="infixExpression">
      <ItemDefinitions>
        <String Extensible="true" Name="expression">
        </String>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="Other">
      <ItemDefinitions>
        <Double Name="Force" Units="Newton" NumberOfRequiredValues="3" />

        <Double Name="Timeouts" Units="minute" NumberOfRequiredValues="3">
          <ComponentLabels>
            <Label>cmake</Label>
            <Label>gcc</Label>
            <Label>ctest</Label>
          </ComponentLabels>
          <DefaultValue>5.5 min</DefaultValue>
        </Double>

        <Double Name="Race" Units="yards">
          <DiscreteInfo DefaultIndex="0">
            <Value Enum="Quarter Mile">440</Value>
            <Value Enum="Half Mile">880</Value>
            <Value Enum="Mile">1760</Value>
          </DiscreteInfo>
        </Double>
      </ItemDefinitions>
    </AttDef>
  </Definitions>

  <Evaluators>
    <Evaluator Name="InfixExpressionEvaluator">
      <Definition Type="infixExpression">
      </Definition>
    </Evaluator>
  </Evaluators>

  <Views>
    <View Type="Group" Title="Units Example" TopLevel="true" TabPosition="North"
          FilterByAdvanceLevel="false" FilterByCategory="false">
      <Views>
        <View Title="Tab 1" />
        <View Title="Tab 2" />
        <View Title="Tab 3" />
      </Views>
    </View>

    <View Type="Instanced" Title="Tab 1">
      <InstancedAttributes>
        <Att Type="UnitsExample" Name="UnitsExample" />
      </InstancedAttributes>
    </View>

    <View Type="Attribute" Title="Tab 2">
      <AttributeTypes>
        <Att Type="infixExpression" />
      </AttributeTypes>
    </View>

    <View Type="Instanced" Title="Tab 3">
      <InstancedAttributes>
        <Att Type="Other" Name="Other" />
      </InstancedAttributes>
    </View>

  </Views>
</SMTK_AttributeResource>
