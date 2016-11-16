//Get language from url
function getLanguage() {
	var url = decodeURIComponent(window.location.search.substring(1));
	if (url.split("=")[1] == "undefined")
		return "en";
	else
		return url.split("=")[1];
}

//Calculate temperature based on local selection of measurement and api provided
function getTemperature(temperature, unit) {
    var localUnit = localStorage.cumulus_measurement;
    if ( localUnit == "c" && unit == "f" ) {
        return fahrenheitToCelsius(temperature);
    }
    else if ( localUnit == "c" && unit == "k" ) {
        return kelvinToCelsius(temperature);
    }
    else if ( localUnit == "k" && unit == "f" ) {
        return fahrenheitToKelvin(temperature);
    }
    else if ( localUnit == "k" && unit == "c" ) {
        return celsiusToKelvin(temperature);
    }
    else if ( localUnit == "f" && unit == "c" ) {
        return celsiusToFahrenheit(temperature);
    }
    else if ( localUnit == "f" && unit == "k" ) {
        return kelvinToFahrenheit(temperature);
    }
    else {
        return Math.round(temperature);
    }
}

//Calculate speed based on local selection of speed unit and api provided
function getSpeed(speed, unit) {
    var localUnit = localStorage.cumulus_speed;
    if ( localUnit == "ms" && unit == "kph" ) {
        return kphToMs(speed);
    }
    else if ( localUnit == "ms" && unit == "mph" ) {
        return mphToMs(speed);
    }
    else if ( localUnit == "kph" && unit == "ms" ) {
        return msToKph(speed);
    }
    else if ( localUnit == "kph" && unit == "mph" ) {
        return mphToKph(speed);
    }
    else if ( localUnit == "mph" && unit == "ms" ) {
        return msToMph(speed);
    }
    else if ( localUnit == "mph" && unit == "kph" ) {
        return kphToMph(speed);
    }
    else {
        return Math.round(speed);
    }
}

function kelvinToCelsius(temperature) {
    return Math.round(temperature - 272.15);
}

function fahrenheitToCelsius(temperature) {
    return Math.round((temperature - 32) * 5 / 9);
}

function celsiusToKelvin(temperature) {
    return Math.round(temperature + 273.15);
}

function celsiusToFahrenheit(temperature) {
    return Math.round((temperature * 9 / 5) + 32);
}

function kelvinToFahrenheit(temperature) {
    return Math.round(celsiusToFahrenheit(kelvinToCelsius(temperature)));
}

function fahrenheitToKelvin(temperature) {
    return Math.round(celsiusToKelvin(fahrenheitToCelsius(temperature)));
}

function msToMph(speed) {
    return Math.round(speed / 0.44704);
}

function msToKph(speed) {
    return Math.round(speed * 3.6);
}

function mphToKph(speed) {
    return Math.round(speed * 1.609344);
}

function mphToMs(speed) {
    return Math.round(kphToMs(mphToKph(speed)));
}

function kphToMph(speed) {
    return Math.round(msToMph(kphToMs(speed)));
}

function kphToMs(speed) {
    return Math.round(speed * 0.27);
}

function typeOfJson() {
    if ( localStorage.api == "wund" ) {
        return "jsonp";
    }
    else {
        return "json";
    }
}