function savelocationFromInput(inputLocation, callback) {
    $.ajax({
        type : "GET",
        dataType : typeOfJson(),
        url : locationRequestUrl(inputLocation),
        success : function(data) {
        	hideError();
            saveLocationLocally(data);
            callback(localStorage.cumulus);
        }
    });
}

function getWeatherData(callback) {	
    $.ajax({
        type : "GET",
        dataType : typeOfJson(),
        url : dataRequestUrl(),
        success: function(data) {
        	hideError();
            saveWeather(data, function() {
                callback();
            });            
        }
    });    
}

function saveWeather(data, callback) {
    if ( localStorage.api == "owm" ) {
        saveOpenWeatherStats(data, function(weather) {
            callback();
        });
    }
    else {
        saveYahooStats(data, function(weather) {
            callback();
        });
    }    
}

function saveLocationLocally(data) {
    if ( localStorage.api == "owm" ) {
        localStorage.cumulus = data.id;
        localStorage.cumulus_location = data.name;
    }
    else{
        localStorage.cumulus = data.query.results.place.woeid;
        localStorage.cumulus_location = data.query.results.place.name;
    }
}

//Get url for location id request
function locationRequestUrl(inputLocation) {
    var api = localStorage.api;
    if ( localStorage.api == "owm" ) {
        return "http://api.openweathermap.org/data/2.5/weather?q=" + inputLocation + "&appid=6cfbd805297a2ab8fe60cfc1fbcf8278&lang=" + getLanguage();
    }
    else {
        return "https://query.yahooapis.com/v1/public/yql?q=select woeid, name from geo.places(1) where text='" + inputLocation + "'&format=json";
    }
}

//Get url for location id request
function dataRequestUrl() {
    var api = localStorage.api;
    if ( localStorage.api == "owm" ) {
        return "http://api.openweathermap.org/data/2.5/weather?id=" + localStorage.cumulus + "&appid=6cfbd805297a2ab8fe60cfc1fbcf8278&lang=" + getLanguage();
    }
    else {
        return "https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid=" + localStorage.cumulus + "&format=json";
    }
}