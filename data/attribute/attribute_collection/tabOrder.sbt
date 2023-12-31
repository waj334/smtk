<?xml version="1.0"?>
<SMTK_AttributeResource Version="4">

  <ItemBlocks>
    <Block Name="DiscreteChildren">
      <ItemDefinitions>
        <Double Name="Discrete">
          <ChildrenDefinitions>
            <Int Name="Child1"><DefaultValue>1</DefaultValue></Int>
            <Int Name="Child2"><DefaultValue>2</DefaultValue></Int>
          </ChildrenDefinitions>
          <DiscreteInfo DefaultIndex="0">
            <Structure>
              <Value Enum="e">2.71828</Value>
              <Items>
                <Item>Child1</Item>
              </Items>
            </Structure>
            <Structure>
              <Value Enum="pi">3.14159</Value>
              <Items>
                <Item>Child1</Item>
                <Item>Child2</Item>
              </Items>
            </Structure>
          </DiscreteInfo>
        </Double>
      </ItemDefinitions>
    </Block>
  </ItemBlocks>

  <Definitions>
    <AttDef Type="InputsItems">
      <ItemDefinitions>
        <Int Name="Before1"><DefaultValue>1</DefaultValue></Int>
        <Double Name="Before2"><DefaultValue>3.14159</DefaultValue></Double>
        <Group Name="InputsItems" Extensible="true">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Int Name="Second">
              <DiscreteInfo DefaultIndex="1">
                <Value Enum="One">1</Value>
                <Value Enum="Two">2</Value>
                <Value Enum="Three">3</Value>
              </DiscreteInfo>
            </Int>
            <Double Name="Third" NumberOfRequiredValues="2">
              <ComponentLabels>
                <Label>Label1</Label>
                <Label>Label2</Label>
              </ComponentLabels>
              <DefaultValue>3.33,6.66</DefaultValue>
            </Double>
            <String Name="text"><DefaultValue>Fourth</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <String Name="After1"><DefaultValue>String data</DefaultValue></String>
        <Int Name="After2"><DefaultValue>2</DefaultValue></Int>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="ReferenceItems">
      <ItemDefinitions>
        <Int Name="Before1"><DefaultValue>1</DefaultValue></Int>
        <Group Name="ReferenceItems" Extensible="true">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Component Name="Component">
              <Accepts>
                <Resource Name="smtk::attribute::Resource" Filter="attribute[type='Example']" />
                <Resource Name="smtk::attribute::Resource" Filter="attribute[type='InputsItems']" />
                <Resource Name="smtk::attribute::Resource" Filter="attribute[type='ReferenceItems']" />
              </Accepts>
            </Component>
            <Resource Name="Resource">
              <Accepts>
                <Resource Name="smtk::attribute::Resource" />
              </Accepts>
            </Resource>
            <String Name="text"><DefaultValue>string item</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <String Name="After1"><DefaultValue>String data</DefaultValue></String>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="ExpressionItems">
      <ItemDefinitions>
        <Int Name="Before1"><DefaultValue>1</DefaultValue></Int>
        <Group Name="FixedGroup">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Double Name="Expression">
              <DefaultValue>2.71828</DefaultValue>
              <ExpressionType>Example</ExpressionType>
            </Double>
            <Double Name="ExpressionOnly">
              <ExpressionType>Example</ExpressionType>
            </Double>
            <String Name="text"><DefaultValue>string item</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <Group Name="ExtensibleGroup" Extensible="true">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Double Name="Expression" NumberOfRequiredValues="2">
              <DefaultValue>2.71828</DefaultValue>
              <ExpressionType>Example</ExpressionType>
            </Double>
            <Double Name="ExpressionOnly">
              <ExpressionType>Example</ExpressionType>
            </Double>
            <String Name="text"><DefaultValue>string item</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <String Name="After1"><DefaultValue>String data</DefaultValue></String>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="OptionalItems">
      <ItemDefinitions>
        <Void Name="TAB ORDERING *NOT* IMPLEMENTED FOR THIS CASE" Optional="false" />
        <Int Name="Before1"><DefaultValue>1</DefaultValue></Int>
        <Int Name="Optional1" Optional="true"><DefaultValue>99</DefaultValue></Int>
        <Double Name="Before2"><DefaultValue>3.14159</DefaultValue></Double>
        <Group Name="ExtensibleGroup" Extensible="true">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Double Name="Second" Optional="true"><DefaultValue>2.71828</DefaultValue></Double>
            <String Name="text"><DefaultValue>string item</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <String Name="After1"><DefaultValue>String data</DefaultValue></String>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="ConditionalItems">
      <ItemDefinitions>
        <Void Name="TAB ORDERING *NOT* IMPLEMENTED FOR THIS CASE" Optional="false" />
        <Int Name="Before1"><DefaultValue>1</DefaultValue></Int>
        <Block Name="DiscreteChildren" />
        <Int Name="Before2"><DefaultValue>2</DefaultValue></Int>
        <Group Name="ExtensibleGroup" Extensible="true">
          <ItemDefinitions>
            <Int Name="First"><DefaultValue>1</DefaultValue></Int>
            <Block Name="DiscreteChildren" />
            <String Name="text"><DefaultValue>string item</DefaultValue></String>
          </ItemDefinitions>
        </Group>
        <String Name="After1"><DefaultValue>String data</DefaultValue></String>
      </ItemDefinitions>
    </AttDef>

    <AttDef Type="Example" />
   </Definitions>

  <Attributes>
    <Att Type="Example" Name="Example" />
  </Attributes>

  <Views>
    <View Type="Group" Title="Tab Order Examples" TabPosition="North" TopLevel="true"
          FilterByAdvanceLevel="false" FilterByCategory="false"
          UseScrollingContainer="false">
      <Views>
        <View Title="InputsItems" />
        <View Title="ReferenceItems" />
        <View Title="ExpressionItems" />
        <View Title="OptionalItems" />
        <View Title="ConditionalItems" />
        <!-- <View Title="Debug" /> -->
      </Views>
    </View>

    <View Type="Instanced" Title="InputsItems" Label="Inputs Items">
      <InstancedAttributes>
        <Att Type="InputsItems" Name="InputsItems" />
      </InstancedAttributes>
    </View>

    <View Type="Instanced" Title="ReferenceItems" Label="Reference Items">
      <InstancedAttributes>
        <Att Type="ReferenceItems" Name="ReferenceItems" />
      </InstancedAttributes>
    </View>

    <View Type="Instanced" Title="ExpressionItems" Label="Expression Items">
      <InstancedAttributes>
        <Att Type="ExpressionItems" Name="ExpressionItems">
        <ItemViews>
          <View Path="/FixedGroup/ExpressionOnly" ExpressionOnly="true" />
          <View Path="/ExtensibleGroup/ExpressionOnly" ExpressionOnly="true" />
        </ItemViews>
      </Att>
      </InstancedAttributes>
    </View>

    <View Type="Instanced" Title="OptionalItems" Label="Optional Items">
      <InstancedAttributes>
        <Att Type="OptionalItems" Name="OptionalItems" />
      </InstancedAttributes>
    </View>

    <View Type="Instanced" Title="ConditionalItems" Label="Conditional Items">
      <InstancedAttributes>
        <Att Type="ConditionalItems" Name="ConditionalItems" />
      </InstancedAttributes>
    </View>

    <View Type="Instanced" Title="Debug">
      <InstancedAttributes>
        <Att Type="ExpressionItems" Name="ExpressionItems">
        <ItemViews>
          <View Path="/FixedGroup/ExpressionOnly" ExpressionOnly="true" />
          <View Path="/ExtensibleGroup/ExpressionOnly" ExpressionOnly="true" />
        </ItemViews>
      </Att>
      </InstancedAttributes>
    </View>

  </Views>
</SMTK_AttributeResource>
