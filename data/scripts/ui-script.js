window.oncontextmenu = function () {
    return false;
}

$(document).ready(function() {
	background(null);
    var updateInteval = 3600000;
    var updateTimer;
    $("a:link").on("click",function(event) {
        console.log("url" + $(this).attr("href"));
        event.preventDefault();
        event.stopPropagation();
    });
    $("#city").on("click",function(event) {
        console.log("url" + $(this).find("a").attr("href"));
        event.preventDefault();
        event.stopPropagation();
    });

    //Filters Proprietary RSS Tags
    jQuery.fn.filterNode = function(name){
        return this.filter(function(){
            return this.nodeName === name;
        });
    };
    
    //APP START.    
    if (!localStorage.cumulus) {
        //if there is no data initialise default data in localStorage for UI
        $("#decoratorBar .settings, #decoratorBar .sync").hide();
        init_storage();
        show_settings("location");
    } else {
        //Has been run before
        getWeather(null);
        updateTimer = setInterval(getPeriodicWeather, updateInteval);
    }
    init_settings();
    
    //Update weather data based on location stored
    $("#decoratorBar .sync").on("click", function() {
        getWeather(null);
        clearInterval(updateTimer);
        updateTimer = setInterval(getPeriodicWeather, updateInteval);
    }); 
    
    //Sets temperature measurement
    $('.measurement td').on("click", function() {
        $(this).parent().children().removeClass('selected');
        $(this).addClass('selected');
        localStorage.cumulus_measurement = $(this).find(":input").val();
        $("#decoratorBar .settings").hide();
    });
    
    //Sets temperature measurement
    $('.speed td').on("click", function() {
    	$(this).parent().children().removeClass('selected');
        $(this).addClass('selected');
        localStorage.cumulus_speed = $(this).find(":input").val();
        $("#decoratorBar .settings").hide();
    });
    
    //Sets up background color switches
    $('.color span').on("click", function() {
        localStorage.cumulus_color = $(this).attr("data-color");
        background(null);
    });
    
    //First Run
    var locationInput = $("#locationModal .input :input");
    var typingTimer = 0;
    var doneTypingInterval = 1500;

    //on keyup, start the countdown
    locationInput.keyup(function(){
        typingTimer = setTimeout(doneTyping, doneTypingInterval);
        $("#decoratorBar .settings, #decoratorBar .sync").hide();
    }).keydown(function(){
    //on keydown, clear the countdown
        clearTimeout(typingTimer);        
    });

    function doneTyping() {
        $("#locationModal .loader").attr("class", "loading loader").html("|");
        savelocationFromInput(locationInput.val(), function(cityCode) {
            if ( cityCode ) {
                $("#locationModal .loader").attr("class", "tick loader").html("&#10003;").attr("data-code", cityCode);
            } else {
                $("#locationModal .loader").attr("class", "loader").html("&#10005;");
            }
        }); 
    }
    
    //This can only be run if there is a tick.
    $("#locationModal .loader").on("click", function() {        
        if ($(this).hasClass("tick")) {
            getWeather(null);            
            $("#locationModal .input :input").val("");
            $(this).removeClass("tick");
            $(this).removeAttr("data-code");
            $(this).html("");       
            clearInterval(updateTimer);
            updateTimer = setInterval(getPeriodicWeather, updateInteval);
        }
    });
    
    //Show/hide api options when button is clicked
    $(".dropbtn:button").on("click", function() {       
        $(".dropdown-content").toggle();
    });

    //Hide api options when option is clicked
    $(".dropdown-content div").on("click", function() {  
        //Do not toggle if weather is syncing
        if (!$('#decoratorBar .sync').hasClass('busy') && $("#locationModal .input :input").val().length == 0) {
            $(".dropdown-content").toggle();
            $("#decoratorBar .settings, #decoratorBar .sync").hide();
            localStorage.api = $(this).find(":input:hidden:eq(0)").val();      
            var searchLocation = localStorage.cumulus_location + ", " + localStorage.cumulus_country;
            savelocationFromInput(searchLocation, function(cityCode) {
                $("#decoratorBar .sync").show();
                setApiSelected();
                clearInterval(updateTimer);
                updateTimer = setInterval(getPeriodicWeather, updateInteval);
            });
        }            
    });

    //Hide api options when hovers out of api select button or options
    $(".dropdown-content, .dropdown").on("mouseleave", function() {
        if ( $(".dropdown-content").css("display") == "block" ) {
            $(".dropdown-content").css("display", "none");
        }
    });
});

function getWeather(inputLocation, callback) {
    $('#decoratorBar .sync').addClass('busy');
    $("#decoratorBar .settings, #decoratorBar .sync").show();    
    if ( inputLocation != null ) {
        //Saves location name and id in localStorage for future updates
        savelocationFromInput(inputLocation, function(cityCode) {
            callback(cityCode);
        });
    }
    else {
        getWeatherData(function() {            
            console.log("Updating Data...");
            //Updates the weather in UI with data from localStorage 
            setWeather();       
            //Sets opacity
            opacity();
        });
    }
}

function init_settings() {
    $(getDragExceptions()).mouseover(function() {
        document.title = "disabledrag";
    }).mouseout(function() {
        document.title = "enabledrag";
    }).click(function() {
        if ( $(this).hasClass("close") ) {
            document.title = 'close';
        } else if ( $(this).hasClass("minimize") ) {
            document.title = 'minimize';
        } else if ( $(this).hasClass("settings") ) {
            show_settings("all");
        } else if ( $(this).hasClass("sync") ) {
            setWeather();
        }
    });
    
    $('#locationModal .measurement #' + localStorage.cumulus_measurement).addClass('selected');
    $('#locationModal .speed #' + localStorage.cumulus_speed).addClass('selected');
    
    //Control CSS.
    $("span[data-color]:not([data-color=gradient])").map(function() { 
        $(this).css('background', '#' + $(this).attr("data-color")); 
    });

    /* Error Message Retry Button */
    $('#errorMessage .btn').click(function() {
        setWeather();
        $("#decoratorBar .settings, #decoratorBar .sync").show();
    });
    
    if (localStorage.cumulus_launcher == "checked") {
        $('#locationModal .launcher input').attr("checked", "checked");
        document.title = "enable_launcher";
    }
    $('#locationModal .launcher input').click(function() {
        localStorage.cumulus_launcher = $('#locationModal .launcher input').attr("checked");
        if (localStorage.cumulus_launcher == "checked") {
            document.title = "enable_launcher";
        } else {
            document.title = "disable_launcher";
        }
    });

    if (localStorage.cumulus_tray == "checked") {
        $('#locationModal .launcher-to-tray input').attr("checked", "checked");
        document.title = "enable_tray";
    }
    $('#locationModal .launcher-to-tray input').click(function() {
        localStorage.cumulus_tray = $('#locationModal .launcher-to-tray input').attr("checked");
        if (localStorage.cumulus_tray == "checked") {
            document.title = "enable_tray";
        } else {
            document.title = "disable_tray";
        }
    });
}

function setWeather() {     
    //Weather Object
    weather = {};
    
    //Location
    weather.city = localStorage.cumulus_location;
    weather.country = localStorage.cumulus_country;
    weather.code = localStorage.weather_code || "3200";
    weather.link = localStorage.cumulus_link || "www.google.com";
    weather.temperature = localStorage.weather_temp || "NaN";
    weather.windSpeed = localStorage.weather_wind_speed || "NaN";
    weather.humidity = localStorage.weather_humidity;
    
    if(localStorage.weather_desc.length > 15) {
        $("#weatherDesc").css("display", "none");
        $("#longWeatherDesc").css("display", "block");
        $(".week").css("margin-top", "15px");
    }
    else {
        $("#weatherDesc").css("display", "block");
        $("#longWeatherDesc").css("display", "none");
        $(".week").css("margin-top", "30px");
    }

    $('#city span').html("<a href='" + localStorage.cumulus_link + "'>" + localStorage.cumulus_location + "</a>");
    $("#code").text(weather_code(weather.code)).attr("class", "w" + weather.code);
    $("#temperature").html(weather.temperature + getTemperatureSymbol(localStorage.cumulus_measurement.toLowerCase()));
    $("#windSpeed").html(weather.windSpeed + " " + getSpeedUnitSymbol(localStorage.cumulus_speed.toLowerCase()));
    $("#humidity").html(weather.humidity + " %");
    $("#weatherDesc").html("<span style='text-transform:capitalize;font-family:UbuntuCondensed;'>" + localStorage.weather_desc + "</span>");
    $("#longWeatherDesc").html("<span style='text-transform:capitalize;font-family:UbuntuCondensed;'>" + localStorage.weather_desc + "</span>");
    //Weekly Weather
    for (var i = 0; i < 5; i++) {
        $('#' + i + ' .day').text(localStorage.getItem('forecast' + i + '_day'));
        $('#' + i + ' .code').text(weather_code(localStorage.getItem('forecast' + i + '_code')));
        $('#' + i + ' .temp').html(localStorage.getItem('forecast' + i + '_high') + "°<span>" + localStorage.getItem('forecast' + i + '_low') + "°</span>");
    }
    
    setApiSelected();       
    //Background Color
    background(weather.temperature);
    // spin the thing for 500ms longer than it actually takes, because
    // most of the time refreshing is actually instant :)
    setTimeout(function() {
        $('#decoratorBar .sync').removeClass('busy');
        $('#actualWeather').fadeIn(500);
        $("#locationModal").fadeOut(500);        
        document.title = "t" + weather.temperature;
        }, 500);
 }

function init_storage() {
    localStorage.cumulus_measurement = localStorage.cumulus_measurement || "c";
    localStorage.cumulus_speed = localStorage.cumulus_speed || "kph";
    localStorage.cumulus_color =  localStorage.cumulus_color || "gradient";
    localStorage.cumulus_launcher = localStorage.cumulus_launcher || "unchecked";
    localStorage.cumulus_tray = localStorage.cumulus_tray || "unchecked";
    localStorage.api = localStorage.api || "owm";
    localStorage.app_opacity = localStorage.app_opacity || "1.0";
    localStorage.weather_desc = localStorage.weather_desc || ""; 
}

function show_settings(amount) {
    if (amount == 'all') {      
        $("#locationModal .full").show();
        $("#locationModal .credits").hide();
    } else if (amount == 'location') {
        $("#locationModal .full").hide();
        $("#locationModal .credits").hide();
    }
    $('.btn[tag="credits"]').click(function() {
        $("#locationModal .input, #locationModal .full, .settings, .sync").hide();
        $("#locationModal .credits").fadeIn(500);
    });
    $('#locationModal .credits img').click(function() {
        $("#locationModal .credits").fadeOut(350);
        $("#locationModal .input, #locationModal .full, .settings, .sync").fadeIn(350);
    }); 
    //Show the Modal
    $("#locationModal").fadeToggle(350);
    if (amount != "noweather") {
        $("#actualWeather").fadeToggle(350);
    }
}

function setApiSelected() {
    $(".dropdown-content div").removeClass("apiSelected");
    $("#source-" + localStorage.api).addClass("apiSelected");
    $(".dropbtn").html("API Select: " + $("#source-" + localStorage.api).html());
}

function opacity() {
    $('input[type=range]').val(localStorage.app_opacity);
    document.title = "o" + localStorage.app_opacity;
    document.title = "enabledrag";
}

function updateTitle(val) {
	rounded_val = Math.round(val*100)/100;
	document.title = "o" + rounded_val;
	localStorage.app_opacity = rounded_val;
}

function showNotFinished() {
    $("#notFinishedPlaceholder").parent().css("display", "block");
}

function hideNotFinished() {
    $("#notFinishedPlaceholder").parent().css("display", "none");
}

function showError() {
    $('#actualWeather, .week').fadeOut(350);    
    $('#errorMessage').fadeIn(350);
    $("#decoratorBar .settings, #decoratorBar .sync").hide();
}

function hideError() {
    $('#errorMessage').fadeOut(350);
    $('#actualWeather, .week').fadeIn(350);    
}

function background(temp) {
    // Convert RGB array to CSS
    var convert = function(i) {
        // Array to RGB
        if (typeof(i) == 'object') {
            return 'rgb(' + i.join(', ') + ')';

        // Hex to array
        } else if (typeof(i) == 'string') {
            var output = [];
            if (i[0] == '#') i = i.slice(1);
            if (i.length == 3)  i = i[0] + i[0] + i[1] + i[1] + i[2] + i[2];
            output.push(parseInt(i.slice(0,2), 16));
            output.push(parseInt(i.slice(2,4), 16));
            output.push(parseInt(i.slice(4,6), 16));
            return output;
        }
    };

    // Get color at position
    var blend = function(x) {
        x = Number(x);
        var gradient = [{
            pos: 0,
            color: convert('#0081d3')
        }, {
            pos: 10,
            color: convert('#007bc2')
        }, {
            pos: 20,
            color: convert('#0071b2')
        }, {
            pos: 30,
            color: convert('#2766a2')
        }, {
            pos: 40,
            color: convert('#575591')
        }, {
            pos: 50,
            color: convert('#94556b')
        }, {
            pos: 60,
            color: convert('#af4744')
        }, {
            pos: 70,
            color: convert('#bb4434')
        }, {
            pos: 80,
            color: convert('#c94126')
        }, {
            pos: 90,
            color: convert('#d6411b')
        }, {
            pos: 100,
            color: convert('#e44211')
        }];

        var left = {
            pos: -1,
            color: false,
            percent: 0
        };
        var right = {
            pos: 101,
            color:  false,
            percent: 0
        };

        // Get the 2 closest stops to the specified position
        for (var i = 0, l = gradient.length; i < l ; i++) {
            var stop = gradient[i];
            if ( stop.pos <= x && stop.pos > left.pos ) {
                left.pos = stop.pos;
                left.color = stop.color;
            } else if ( stop.pos >= x && stop.pos < right.pos ) {
                right.pos = stop.pos;
                right.color = stop.color;
            }
        }

        // If there is no stop to the left or right
        if (!left.color) {
            return convert(right.color);
        } else if (!right.color) {
            return convert(left.color);
        }

        // Calculate percentages
        right.percent = Math.abs(1 / ((right.pos - left.pos) / (x - left.pos)));
        left.percent = 1 - right.percent;

        // Blend colors!
        var blend = [
            Math.round((left.color[0] * left.percent) + (right.color[0] * right.percent)),
            Math.round((left.color[1] * left.percent) + (right.color[1] * right.percent)),
            Math.round((left.color[2] * left.percent) + (right.color[2] * right.percent)),
        ];
        return convert(blend);
    };

    //Sets Background Color
    if (localStorage.cumulus_color == "gradient") {
        var percentage = Math.round((temp - 45) *  2.2);
        $("#container").css("background-color", blend(percentage));
    } else {
        $("#container").css("background-color", "#" + localStorage.cumulus_color);
    }
}

function getSpeedUnitSymbol(unit) { 
    if (unit == "ms" || unit == "m/s" ) {
        return " m/s";
    } 
    else if (unit == "kph") {
        return " kph";
    } 
    else {
        return " mph";
    }
}

function getTemperatureSymbol(unit) {
    if (unit == "c") {
        return " °C";
    } 
    else if (unit == "k") {
        return " °K";
    } 
    else {
        return " °F";
    }
}

function getDragExceptions() {
    //Prevents Dragging on certain elements
    var dragExceptions = '#decoratorBar .settings, #decoratorBar .sync, #decoratorBar .close, #decoratorBar .minimize, #locationModal input, ' + 
    '#locationModal .measurement td, #locationModal .speed td, #locationModal .loader, #locationModal a, #locationModal .color, ' +
    '#locationModal .btn, #city span, #locationModal img, #notFinishedPlaceholder, .dropdown-content, .dropbtn, #errorMessage .btn';
    
    return dragExceptions;
}

$(document).ajaxError(function() {
	showError();
});

function getPeriodicWeather() {
    getWeather(null)
}
