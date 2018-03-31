json {
   %whitespace "[ \t\r\n]*";
   document: '{' element '}' [document];
   element: name ':' '{' contents '}' [element];
   contents: contents ',' content [add_to_element] | content [create_element];
   content: attribute [content] | element [content];
   attribute: name ':' value [attribute];
   value: 'null' [value] | 'true' [value] | 'false' [value] | integer [value] | real [value] | string [value];
   name: "[\"']:string:";
   integer: "(\+|\-)?[0-9]+";
   real: "(\+|\-)?[0-9]+(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?";
   string: "[\"']:string:";
}
