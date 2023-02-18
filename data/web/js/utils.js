

function isValidIP(ipaddress){  
  var regexp = /^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;  

  return regexp.test(ipaddress);
}
