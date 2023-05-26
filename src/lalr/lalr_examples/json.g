json {
   %whitespace "[ \t\r\n]*";
   document: '{' attributes '}' [document] | ;
   attributes: attributes ',' attribute [add_to_object] | attribute [create_object] | ;
   attribute: name ':' value [attribute];
   elements: elements ',' value [add_to_array] | value [create_array] | ;
   value:
      null [null] |
      boolean [value] |
      integer [value] |
      real [value] |
      string [value] |
      '{' attributes '}' [object] |
      '[' elements ']' [array]
   ;
   name: "[\"']:string:";
   null: 'null';
   boolean: "true|false";
   integer: "(\+|\-)?[0-9]+";
   real: "(\+|\-)?[0-9]+(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?";
   string: "[\"']:string:";
}
