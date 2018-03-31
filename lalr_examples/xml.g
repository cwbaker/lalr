xml {
   %whitespace "[ \t\r\n]*";
   %left '<' '>';
   %left name;
   document: prolog element [document];
   prolog: "<\?xml" attributes "\?>" | ;
   elements: elements element [add_element] | element [create_element] | %precedence '<';
   element: '<' name attributes '/>' [short_element] | '<' name attributes '>' elements '</' name '>' [long_element];
   attributes: attributes attribute [add_attribute] | attribute [create_attribute] | %precedence name;
   attribute: name '=' value [attribute];
   name: "[A-Za-z_:][A-Za-z0-9_:\.-]*";
   value: "[\"']:string:";
}
