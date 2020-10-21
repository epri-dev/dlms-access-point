(function() {
    
    var myEPRIDatasource = function(settings, updateCallback) {

        var self = this;
        var currentSettings = settings;
        var ws;

        var onOpen=function() {
            console.info("Opened websocket %s", currentSettings.url);
            ws.send(currentSettings.request);
        }

        var onClose=function() {
            console.info("Closed websocket");
        }

        var onMessage=function(event) {
            var message = event.data;
            console.info("Websocket received %s", message);
            var objdata = JSON.parse(message);
            if (typeof objdata == "object") {
                updateCallback(objdata);
            } else {
                updateCallback(data);
            }
        }

        var refreshTimer;

        function createRefreshTimer(interval) {
            if (refreshTimer) {
                clearInterval(refreshTimer);
            }
            refreshTimer = setInterval(function() {
                ws.send(currentSettings.request);
            }, interval);
        }

        function initializeDataSource() {
            if (ws) ws.close();
            ws = new WebSocket(currentSettings.url);
            ws.onopen=onOpen;
            ws.onclose=onClose;
            ws.onmessage=onMessage;
            createRefreshTimer(currentSettings.refresh_time);
        }

        this.updateNow = function() {
            initializeDataSource();
        }

        this.onDispose = function() {
            clearInterval(refreshTimer);
            refreshTimer = undefined;
            ws.close();
            ws = undefined;
        }

        this.onSettingsChanged = function(newSettings) {
            currentSettings = newSettings;
            initializeDataSource();
        };
        
        initializeDataSource();
    };

    freeboard
            .loadDatasourcePlugin({
                type_name : "epri_netstat",
                display_name : "EPRI netstat",
                description : "Fetch network stats from EPRI Access Point",
                settings : [
                        {
                            name : "url",
                            display_name : "Access Point URL",
                            description : "The URL of the Access Point, including port if required.",
                            type : "text",
                            default_value : "ws://localhost:8888"
                        },
                        {
                            name : "request",
                            display_name : "Request",
                            description : "The name of the request to issue to the Access Point",
                            type : "text",
                            default_value : "{netstat}"
                        }, 
                        {
				"name"         : "refresh_time",
				"display_name" : "Refresh Time",
				"type"         : "text",
				"description"  : "In milliseconds",
				"default_value": 5000
                        } ],
                newInstance : function(settings, newInstanceCallback,
                        updateCallback) {
                    newInstanceCallback(new myEPRIDatasource(settings,
                            updateCallback));
                }
            });
}());
