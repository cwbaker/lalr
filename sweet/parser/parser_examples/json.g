
json {
    %whitespace "[ \t\r\n]*";
    document: '{' element '}' [document];
    element: name ':' '{' contents '}' [element];
    contents: contents ',' content [add_to_element] | content [create_element];
    content: attribute [content] | element [content];
    attribute: name ':' value [attribute];
    value: null [value] | boolean [value] | integer [value] | real [value] | string [value];
    name: "[\"']:string:";
    null: 'null';
    boolean: 'true' | 'false';
    integer: "(\+|\-)?[0-9]+";
    real: "(\+|\-)?[0-9]+(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?";
    string: "[\"']:string:";
}
