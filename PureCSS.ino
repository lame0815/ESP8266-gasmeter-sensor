
/* Custom CSS */
void handleCustCSS() {
  String text = "body { background-color: #000000ba;";
  text = text +"color: white;}";
  text = text + "a {\r\n";
  text = text + "color: white; }";
  text = text + "div.counter {";
  text = text + "font-size: 4em;";
  text = text + "color: yellow;";
  text = text + "font-weight: bold;";
  text = text + "}";
  text = text + "div.cpm {";
  text = text + "font-size: 2em;";
  text = text + "color: orange;";
  text = text + "font-weight: bold;";
  text = text + "}";
  text = text + "div.credits {";
  text = text + "width: max-content;";
  text = text + "margin-top: 100px;";
  text = text + "}";
  text = text + "input {";
  text = text + "line-height: 30px;";
  text = text + "width: 100%";
  text = text + "}";
  text = text + "ul {";
  text = text + "list-style-type: none;";
  text = text + "width: 100%";
  text = text + "margin: 5px";
  text = text + "}";
  text = text + "li {";
  text = text + "float: left;";
  text = text + "text-align: center;";
  text = text + "padding: 16px;";
  text = text + "margin: 2px;";
  text = text + "background-color: #666666;";  
  text = text + "border-radius: 10px;";
  text = text + "}";
  text = text + "li.primary {";
  text = text + "background-color: #17b8ea";
  text = text + "}";
  text = text + "li a:hover {";  
  text = text + "background-color: #111111;";
  text = text + "}";
  text = text + "button {";
  text = text + "background: none!important;";
  text = text + "border: none;";
  text = text + "color: white;";
  text = text + "text-decoration: underline;";
  text = text + "cursor: pointer;";
  text = text + "}";
  text = text + "div.status {";
  text = text + "display:table;";
  text = text + "}";
  text = text + ".tablecell {";
  text = text + "display:table-cell;";
  text = text + "padding-left: 10px;";
  text = text + "padding-right: 10px;";
  text = text + "border: 1px solid white;";
  text = text + "}";
  text = text + ".tablerow {";
  text = text + "display:table-row;";
  text = text + "}";
  
server.send(200, "text/css", text);
}