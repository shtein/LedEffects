
//API stadnard errors
const EEER_SUCCESS   = 0x00; //No error
const EEER_INVALID   = 0x01; //Invalid input
const EEER_UNHANDLED = 0x02; //Unhandled command
const EEER_DELAYED   = 0x03; //Delayed response

//Progress call back
function CallProgress(){

  this.counter = 0;
  this.callback = null;
  
  this.beginCall = function(){
    if(this.counter == 0){
      if(this.callback)
        this.callback(true);
    }

    this.counter = this.counter + 1;
  }

  this.endCall = function(){
    this.counter =  this.counter - 1;

    if(this.counter == 0){
      if(this.callback)
        this.callback(false);
    }
  }
}

let apiCallProgress = new CallProgress();

//Delayed call
function later(delay) {
  return new Promise(function(resolve) {
      setTimeout(resolve, delay);
  });
}



function apiCall(url){
  //Show progress
  apiCallProgress.beginCall();

  //Send api call
  return fetch(url)
    .then(result=>{
      //Check if request was sent
      if(!result.ok){                
        throw new Error("Network error");
      }      
      return result.json();
    }).then(json=>{              
      //Check if json returns an error
      if(json.hasOwnProperty("error")){     
        switch(json.error){
          
          case EEER_SUCCESS:   
            apiCallProgress.endCall(); 
            return EEER_SUCCESS;
          break;

          case EEER_DELAYED:                       
            apiCallProgress.endCall(); 
            return EEER_DELAYED;
          break;

          case EEER_UNHANDLED: 
            throw new Error("Unhandled request");
          break;

          case EEER_INVALID:   
            throw new Error("Bad request");
          break;
        }
      }
      else{        
        //Return data
        apiCallProgress.endCall(); 
        return json.data;                    
      }
  }, error=>{
    //Stop progress
    apiCallProgress.endCall();
    //Pass error to next handler
    throw error;
  })

}

//API prefix
function host(){
  return location.host == "" ? "192.168.101.138" : location.host;
}

function apiPrefix(){
  var prefix = "http://" + host() + "/api";  
  return prefix;
}

function paramString(params){
  const result = new URLSearchParams();  
  for (const [key, value] of Object.entries(params)) {
    if(value != null && value != ""){
      result.append( key, value);
    }        
  }

  return result.toString();
}

//WiFi modes
const WIFI_MODE_NONE = 0x00;
const WIFI_MODE_ST   = 0x01;
const WIFI_MODE_AP   = 0x02;

//Station WiFi status - other statuses are disconnected
const WIFI_STATUS_NO_SSID   = 1;
const WIFI_STATUS_CONNECTED = 3;
const WIFI_STATUS_PASSWORD  = 6;

function wifiStatusToStr(status){
  switch(status){
    case WIFI_STATUS_NO_SSID: return "No SSID";
    case WIFI_STATUS_CONNECTED: return "Connected"
    case WIFI_STATUS_PASSWORD: return "Wrong password";
  }

  return "Disconnected (" + status + ")";
}
  
//Wifi status
function apiWifiStatus(){
  return apiCall(apiPrefix() + "/wifi/status");
}

//Scan networks
function apiWifiScan(){
  return apiCall(apiPrefix() + "/wifi/scan")
    .then(result =>{
      if(result == EEER_DELAYED){
        apiCallProgress.beginCall();
        return later(2000).then(
          function(){
          apiCallProgress.endCall();
          return apiWifiScan();
        });
      }
      else{
        return result;
      }
    });
}

//Connect to wifi
function apiWifiConnect(ssid, pwd, ipaddress = "", gateway = "", subnet = "", dns1 = "", dns2 = ""){
  var params = {"ssid":ssid, "pwd":pwd, "ipaddress":ipaddress, "gateway":gateway, "subnetmask":subnet, "dns1":dns1, "dns2":dns2 };
  return apiCall(apiPrefix() + "/wifi/connect?" + paramString(params));
}

function apiWifiDisconnect(){
  return apiCall(apiPrefix() + "/wifi/disconnect");
}


function apiWifiConfig(){
  return apiCall(apiPrefix() + "/wifi/config");
}

function apiWifiAPOn(ssid){
  return apiCall(apiPrefix() + "/wifi/ap/on");
}

function apiWifiAPOff(ssid){
  return apiCall(apiPrefix() + "/wifi/ap/off");
}
