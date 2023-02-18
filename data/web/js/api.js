
//API stadnard errors
const EEER_SUCCESS   = 0x00; //No error
const EEER_INVALID   = 0x01; //Invalid input
const EEER_UNHANDLED = 0x02; //Unhandled command
const EEER_DELAYED   = 0x03; //Delayed response


//Delayed call
function later(delay) {
  return new Promise(function(resolve) {
      setTimeout(resolve, delay);
  });
}


function apiCall(url){   
  //Send api call
  return fetch(url)
    .then(result=>{
      //Check if request was sent
      if(!result.ok){
        throw new Error("Network error");
      }      
      return result.json();
    })
    .then(json=>{
      //Check if json returns an error
      if(json.hasOwnProperty("error")){
        //Pending respins
        if(json.error == EEER_DELAYED){
          //recall in 2 seconds
          return later(2000).then( function(){
            return apiCall(url);
          })
        }
        else{
          throw new Error("Bad request");
        }
      }
      
      return json.data;                    
    })
}

//API prefix
function apiPrefix(){
  var prefix = "http://" + (location.host == "" ? "172.16.0.128" : location.host) + "/api";
  
  return prefix;
}

//Wifi status
function apiWifiStatus(){
  return apiCall(apiPrefix() + "/wifi");
}

//Scan networks
function apiWifiScan(){
  return apiCall(apiPrefix() + "/wifi/scan");
}

//Connect to wifi
function apiWifiConnect(ssid, pwd){
  return apiCall(apiPrefix() + "/wifi/connect?ssid=" + ssid + "&pwd=" + pwd);
}
