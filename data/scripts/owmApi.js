//Open Weather Map API
function saveOpenWeatherStats(data, callback) {
    //Weather Object
    weather = {};    
    localStorage.weather_code = yahooCodeFromOwm(data.weather[0].id);
    localStorage.cumulus_country = data.sys.country;
    localStorage.cumulus_link = "http://openweathermap.org/city/" + data.id;
    localStorage.weather_temp = getTemperature(data.main.temp, "k");
    localStorage.weather_wind_speed = getSpeed(data.wind.speed, "ms");
    localStorage.weather_humidity = data.main.humidity;    
    localStorage.weather_desc = data.weather[0].description;
    openWeatherForecast(data, function() {
        callback();
    });
}

function openWeatherForecast(data, callback) {
    $.ajax({
        type : "GET",
        dataType : "json",
        url : "http://api.openweathermap.org/data/2.5/forecast/daily?id=" + data.id + "&appid=6cfbd805297a2ab8fe60cfc1fbcf8278&lang=" + getLanguage(),
        success : function(data) {
            weekArr = data.list;
            var j = 0;
            for (var i = 0; i < 5; i++) {    
                localStorage.setItem('forecast' + i + '_day', getEnglishDay(weekArr[i].dt));
                localStorage.setItem('forecast' + i + '_code', yahooCodeFromOwm(weekArr[i].weather[0].id));
                localStorage.setItem('forecast' + i + '_high', getTemperature(weekArr[i].temp.max, "k"));
                localStorage.setItem('forecast' + i + '_low', getTemperature(weekArr[i].temp.min, "k"));
            } 
            
            if (localStorage.weather_code == "3200") {
                localStorage.weather_code = $(weekArr[0]).attr("code");
            }
            
            if (callback) {
                callback();
            } 
        }
    });
}

function getEnglishDay(date) {
    return ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'][new Date(date * 1000).getDay()];
}

//Converts OpenWeatherMap Code to Yahoo code
function yahooCodeFromOwm(owmCode){    
  var yCode= {200: "37", 201: "38", 202: "45", 210: "45", 211: "4", 212: "3", 221: "39", 230: "47", 231: "47", 232: "47", 300: "9", 301: "9", 302: "9", 310: "9", 311: "9", 312: "9", 313: "40", 314: "40", 321: "40", 500: "11",
          501: "11", 502: "11", 503: "11", 504: "11", 511: "10", 520: "12", 521: "12", 522: "12", 531: "12", 600: "14", 601: "16", 602: "41", 611: "18", 612: "7", 615: "5", 616: "5", 620: "13", 621: "15", 622: "46", 701: "20", 711: "22",
          721: "21", 731: "19", 741: "20", 751: "19", 761: "19", 762: "3200", 771: "24", 781: "0", 800: "34", 900: "0", 901: "1", 902: "2", 903: "25", 904: "36", 905: "24", 906: "17", 951: "34", 952: "30", 953: "30", 954: "30", 955: "30",
          956: "30", 957: "24", 958: "24", 959: "24", 960: "23", 961: "23", 962: "2", 3200: "3200", 801: "30", 802: "30", 803: "28", 804: "26",};
  return yCode[owmCode];
}