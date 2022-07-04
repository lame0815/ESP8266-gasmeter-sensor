/* render HTML Head */
String generateHTMLHead(String title) {
  String text = "<!doctype html>\r\n<html lang=\"en\">\r\n<head>\r\n<title>"+title+"ESP8266 Gasmeter Sensor</title>\r\n";
  text = text + "<meta charset=\"utf-8\">\r\n";
  text = text + "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n";
  text = text + "<link rel=\"icon\" type=\"image/png\" sizes=\"64x64\" href=\"";
  text = text + returnFavicon();
  text = text + "\" >";
  text = text + "<script>\r\n";
  text = text + loadJSON();
  text = text + "</script>\r\n";
  text = text + "<link rel=\"stylesheet\" href=\"/style.css\">\r\n";
  text = text + "</head>\r\n<body>\r\n";
  text = text + "<div class=\"row\">";
  text = text + "<div class=\"row\"><div class=\"column\">";
  text = text + "<img src=\"";
  text = text + returnFavicon();
  text = text + "\" alt=\"Gasmeter Sensor\" >";
  text = text + "<h1 style=\"display: inline\">Gasmeter Sensor "+deviceId+" </h1>";
  return(text);
}

/* render HTML Footer */
String generateHTMLFooter() {
  String text = "<div class=\"credits\"><small>"+VERSION + " " + AUTHOR+"</small></div>";
  text = text + "</body></html>";
  return(text);
}


String returnFavicon() {
  return "data:image/png;base64,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";
}

/* render HTML Menu */
String generateHTMLMenu() {
  String response =
  "<div class=\"pure-button-group\" role=\"group\" aria-label=\"...\"><button class=\"pure-button\">Wifi-Settings</button><button class=\"pure-button\">Menu2</button><button class=\"pure-button\">Menu3</button></div>";
return response;
}
