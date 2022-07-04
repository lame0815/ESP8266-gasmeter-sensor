


/* Java Script to load JSON from REST Service and print it on Website */
String loadJSON() {
  String text = "function loadJSON() {\n";
  text = text + "var d_file = \"/api/QueryCounter\";\n";
  text = text + "var http_request = new XMLHttpRequest();\n";
  text = text + "http_request.onreadystatechange = function() {\n";
  text = text +  "if (http_request.readyState == 4 && document.getElementById(\"counter\")) {\n";
  text = text +  "var newjsonobj = JSON.parse(http_request.responseText);\n";
  text = text + "document.getElementById(\"counter\").innerHTML = newjsonobj.count;\n";
  text = text + "document.getElementById(\"cpm\").innerHTML = newjsonobj.cpm;\n";
  text = text + "}\n}\n";
  text = text + "http_request.open(\"GET\", d_file, true);\n";
  text = text + "http_request.send();\n}";
  text = text + "var intervalId = window.setInterval(function(){";
  text = text + "loadJSON()";/// call your function here
  text = text + "}, 500);";  
  return text;
 }
