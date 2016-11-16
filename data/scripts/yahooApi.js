//Yahoo! Weather API

function saveYahooStats(data, callback) {
    //Weather Object
    weather = {};
    
    var channel = data.query.results.channel;
    var location = data.query.results.channel.location;
    var units = data.query.results.channel.units;
    var item = data.query.results.channel.item;
    var wind = data.query.results.channel.wind;
    var atmosphere = data.query.results.channel.atmosphere;
    
    localStorage.cumulus_location = location.city;
    localStorage.cumulus_country = location.country;
    localStorage.cumulus_link = channel.link;
    localStorage.weather_temp = getTemperature(item.condition.temp, units.temperature.toLowerCase());
    localStorage.weather_wind_speed = getSpeed(wind.speed, units.speed.toLowerCase());
    localStorage.weather_humidity = atmosphere.humidity;
    localStorage.weather_desc = item.condition.text;
    
    //Weekly Weather
    weekArr = item.forecast;    
    for (var i = 0; i < 5; i++) {
        localStorage.setItem('forecast' + i + '_day', $(weekArr[i]).attr("day"));
        localStorage.setItem('forecast' + i + '_code', $(weekArr[i]).attr("code"));
        localStorage.setItem('forecast' + i + '_high', getTemperature($(weekArr[i]).attr("high"), units.temperature.toLowerCase()));
        localStorage.setItem('forecast' + i + '_low', getTemperature($(weekArr[i]).attr("low"), units.temperature.toLowerCase()));
    }

    //Current Weather 
    localStorage.weather_code = item.condition.code;
    if (localStorage.weather_code == "3200") {
        localStorage.weather_code = weather.week[0].code;
    }
    
    callback();
}

//Converts Yahoo weather to icon font
function weather_code(a){
  var b={0:"(",1:"z",2:"(",3:"z",4:"z",5:"e",6:"e",7:"o",8:"3",9:"3",10:"9",11:"9",12:"9",13:"o",14:"o",15:"o",16:"o",17:"e",18:"e",19:"s",20:"s",21:"s",22:"s",23:"l",24:"l",25:"`",26:"`",27:"2",28:"1",29:"2",30:"1",31:"/",32:"v",33:"/",34:"v",35:"e",36:"v",37:"z",38:"z",39:"z",40:"3",41:"o",42:"o",43:"o",44:"`",45:"z",46:"o",47:"z",3200:"`"};
  return b[a];
}
