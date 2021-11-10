# Add any resources to JUCE here
juce_add_binary_data(BinaryData SOURCES
	OrganGUI.xml
)

set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)